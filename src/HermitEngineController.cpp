/*
 ============================================================================
  Name        : HermitEngineController.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#include "HermitEngineController.h"
#include "HermitEngineGame.h"
#include "HermitEngineMenu.h"
#include "HermitCommonObjects.h"
#include "HermitLevelProvider.h"
#include "HermitBackground.h"
#include "HermitSettings.h"
#include "HermitGamesStore.h"
#include "HermitGamePageGame.h"    // for KHermitGameStateFile
#include "HermitCompilationSettings.h"
#include "HermitConst.h"
#include "HermitMenuPageSettings.h" // volume calculation
#include <SGEEngine/SGEEngineControllerObserver.h>
#include <SGEAudio/SGEAudioConfig.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEGl/GlUtils.h>
#include <BAUTILS.H>
#include <SYSUTIL.H>
#ifdef _SGE_BADA_
#include <FIo.h>
using namespace Osp::Io;
#endif



CHermitEngineController::CHermitEngineController( MHermitEngineControllerObserver& aObserver, TSize aSize )
        : CSGEEngineController( aObserver, aSize )
    {
#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
	iIndicatorButtonUp = ETrue;
#endif
    }

CHermitEngineController::~CHermitEngineController()
    {
#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
	iAnimationFadeIndicators.ResetAndDestroy();
#endif
    }

CHermitEngineController* CHermitEngineController::NewL( MHermitEngineControllerObserver& aObserver, TSize aSize )
    {
	CHermitEngineController *self = new (ELeave) CHermitEngineController( aObserver, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CHermitEngineController::ConstructL()
    {
    BaseConstructL();

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
    glEnable( GL_CULL_FACE );
    glEnable( GL_SCISSOR_TEST );
    
    glDisable( GL_DEPTH_TEST );
    glDisable( GL_LIGHTING );

    CSGEEngine* engine = CHermitEngineMenu::NewL( *this, iCommonObjects, iSize );
    TInt err = AddEngine( engine );
    if ( err != KErrNone )
        {
        delete engine;
        iObserver.EngineControllerClose( err );
        return;
        }
    SetCurrentEngine( engine->Id() );

    // temp:
    //EngineSwitch( KHermitEngineMenuId );
    }

CSGEEngineCommonObjects* CHermitEngineController::CreateCommonObjectsL()
	{
    CHermitCommonObjects *objects = new ( ELeave ) CHermitCommonObjects();
    CleanupStack::PushL( objects );

    CSGEFileProvider *fp = CSGEFileProvider::NewL();
    objects->SetFileProvider( fp );
#ifdef USE_SGEFILE
    TFileName fn = fp->AppendToPrivateFolder( KHermitSGEFileName() );
    fp->AddSGEFileL( fn, KHermitSGEFileId );
#endif

//    objects->SetBitmapProvider( CSGEBitmapProvider::NewL( CurrentEngine() ) );
//    objects->SetFontProvider( CSGEFontProvider::NewL() );
//    objects->SetStringProvider( CSGEStringProvider::NewL() );

#ifdef _SGE_BADA_
    objects->SetTextureManager( CGlTextureManager::NewL( CurrentEngine() ) );
#else
    objects->SetTextureManager( CGlTextureManager::NewL( CurrentEngine()/*, objects->BitmapProvider()*/ ) );
#endif

    CHermitLevelProvider* levelProvider = CHermitLevelProvider::NewL( *objects );
    objects->SetLevelProvider( levelProvider );

    CHermitSettings* settings = CHermitSettings::NewL( *objects );
    objects->SetSettings( settings );

    objects->SetBackground( CHermitBackground::NewL( *objects->Settings() ) );

    objects->SetGamesStore( CHermitGamesStore::NewL( *objects ) );


    TSGEAudioConfig defaultConfig;
    defaultConfig.iSubsystem = ESGEAudioSubsystemMMF;
    defaultConfig.iMaxVolumePercent = 90;
    defaultConfig.iSampleRate = ESGEAudioSampleRate32000Hz;
    //defaultConfig.SetBufferSizeByLatency( 1000 ); // 1 ms latency
    //defaultConfig.iBufferSize = 128;

    TBuf<KSysUtilVersionTextLength> versionBuf;
    if ( SysUtil::GetSWVersion( versionBuf ) == KErrNone )
        {
        TInt version = KErrNotFound;
	TInt dotIdx = versionBuf.Locate( '.' );
	if ( dotIdx > KErrNotFound )
	    {
            TInt err = KErrNotFound;
            TLex lex( versionBuf.Left( dotIdx ) );
            for ( TInt i = 0; i < dotIdx; i++ )
                {
                version = KErrNotFound;
                err = lex.Val( version );
                if ( err == KErrNone )
                    {
                    break;
                    }
                lex.Inc();
                }

            if ( version != KErrNotFound && version < 20 )
                {
                defaultConfig.SetBufferSizeByLatency( 1000 );
                }
            else
                {
                defaultConfig.SetBufferSizeByLatency( 30000 );
//                defaultConfig.iMultithreeded = ETrue;
                }
	    }
        }
    else
        {
        defaultConfig.SetBufferSizeByLatency( 30000 );
        }
	
    CSGEAudioProvider *audio = CSGEAudioProvider::NewL( CurrentEngine(), defaultConfig );
    objects->SetAudioProvider( audio );
#ifdef USE_AUDIO
    audio->CreateEmptyTracksL( 4 );
    audio->SetVolumeDefault( ESGEAudioGroupMusic, CHermitMenuPageSettings::CalculateMusicVolumeShift( settings->MusicVolume() ) );
    audio->SetVolumeDefault( ESGEAudioGroupSound, CHermitMenuPageSettings::CalculateSoundVolumeShift( settings->SoundVolume() ) );
#endif

    CleanupStack::Pop( objects );
    return objects;
	}

void CHermitEngineController::EngineSwitch( TUint aId )
	{
    if ( aId == KHermitEngineMenuId )
    	{
    	if ( !EngineAdded( KHermitEngineGameId ) )
			{
			CSGEEngine* engine = NULL;
			MSGEEngineObserver* obs = (CSGEEngineController*)this;
			TRAPD( err, engine = CHermitEngineGame::NewL( *obs, iCommonObjects, iSize ) );
			if ( err != KErrNone )
				{
				iObserver.EngineControllerClose( err );
				return;
				}
			err = AddEngine( engine );
			if ( err != KErrNone )
				{
				delete engine;
				iObserver.EngineControllerClose( err );
				return;
				}
			}
        SetCurrentEngine( KHermitEngineGameId );
    	}
    else if ( aId == KHermitEngineGameId )
		{
        SetCurrentEngine( KHermitEngineMenuId );
		}
	}

void CHermitEngineController::EngineClose( TUint aId, TInt aError )
	{
	iCommonObjects->AudioProvider()->StopAll();

#ifdef STORE_LEVEL_PROVIDER_STATE
	reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Settings()->SaveToFile();
#endif

	CSGEEngineController::EngineClose( aId, aError );
	}

void CHermitEngineController::EngineMenuTitleAnimationFinished()
	{
    // restore saved state
#ifdef _SGE_BADA_
     if ( Osp::Io::File::IsFileExist( KHermitGameStateFile ) )
#else
     if ( BaflUtils::FileExists( iCommonObjects->FileProvider()->Fs(), KHermitGameStateFile() ) )
#endif
         {
    	 CHermitEngineMenu* engine = reinterpret_cast< CHermitEngineMenu* >( CurrentEngine() );
    	 EngineSwitch( KHermitEngineMenuId );
    	 if ( RestoreState() )
			 {
    		 engine->SwitchToGamePage();
			 }
    	 else
			 {
    		 EngineSwitch( KHermitEngineGameId );
			 }
         }
	}

void CHermitEngineController::EngineMenuCreated()
    {
    iEngineCreated = ETrue;
    
    reinterpret_cast< MHermitEngineControllerObserver& >( iObserver ).EngineCreated();
    }

void CHermitEngineController::HandleForegroundEventL( TBool aForeground )
	{
	CSGEEngineController::HandleForegroundEventL( aForeground );

	if ( !aForeground )
		{
		iCommonObjects->AudioProvider()->PauseAll();
		}
	else
		{
		iCommonObjects->AudioProvider()->ResumeAll();
		}
	}

TBool CHermitEngineController::EngineCreated() const
    {
    return iEngineCreated;
    }

TBool CHermitEngineController::Update( CBitmapContext* aGc, TRect aDrawRect, TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	TBool ret = CSGEEngineController::Update( aGc, aDrawRect, aTimeInterval );

#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
	glLoadIdentity();

	for ( TInt i = 0; i < iAnimationFadeIndicators.Count(); i++ )
		{
		iAnimationFadeIndicators[ i ]->Update( aTimeInterval );
		}

	((CHermitCommonObjects*)iCommonObjects)->TextureManager()->Texture( KHermitTexTutorialIndicator2Id )->Use();
	for ( TInt i = 0; i < iAnimationFadeIndicators.Count(); i++ )
		{
		iAnimationFadeIndicators[ i ]->Draw( TRect() );
		}
#endif

	return ret;
	}

TBool CHermitEngineController::HandlePointerEventL( const TPointerEvent &aPointerEvent )
	{
	TBool ret = CSGEEngineController::HandlePointerEventL( aPointerEvent );

#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
	if ( aPointerEvent.iType == TPointerEvent::EButton1Down && iIndicatorButtonUp )
		{
		TInt m = 16;
		TRect r( TPoint( aPointerEvent.iPosition.iX - m, aPointerEvent.iPosition.iY - m ),
				 TSize( 2 * m, 2 * m ) );
		CHermitAnimationFade* animationFadeIndicator = CHermitAnimationFade::NewL( this, (CHermitCommonObjects&) *iCommonObjects );
		animationFadeIndicator->SetStartIncrement( 40 );
		animationFadeIndicator->SetAnimationStepTime( 10000 );
		animationFadeIndicator->SetFadeColor( TRgb( 255, 255, 255 ) );
		animationFadeIndicator->UseExternalTextures( ETrue );
		animationFadeIndicator->EnableScaleAnimation( 10000, 0.3 );
		animationFadeIndicator->SetFadeRect( r );
		animationFadeIndicator->RestartAnimation();
		animationFadeIndicator->ResetScale();
		animationFadeIndicator->StartAnimation( CHermitAnimationFade::EFadeOut );
		iAnimationFadeIndicators.Append( animationFadeIndicator );

		iIndicatorButtonUp = EFalse;
		}
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		iIndicatorButtonUp = ETrue;
		}
#endif

	return ret;
	}

void CHermitEngineController::SaveStateOnClose()
	{
	iCommonObjects->AudioProvider()->StopAll();

#ifdef STORE_LEVEL_PROVIDER_STATE
	reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Settings()->SaveToFile();
#endif

	if ( CurrentEngine() && CurrentEngine()->Id() == KHermitEngineGameId )
		{
		reinterpret_cast< CHermitEngineGame* >( CurrentEngine() )->SaveStateOnClose();
		}
	}

TBool CHermitEngineController::RestoreState()
	{
	if ( CurrentEngine() && CurrentEngine()->Id() == KHermitEngineGameId )
		{
		return reinterpret_cast< CHermitEngineGame* >( CurrentEngine() )->RestoreState();
		}
	return EFalse;
	}

void CHermitEngineController::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
	for ( TInt i = 0; i < iAnimationFadeIndicators.Count(); i++ )
		{
		if ( aAnimation == iAnimationFadeIndicators[ i ] )
			{
			if ( iAnimationFadeIndicators[ i ]->FadeType() == CHermitAnimationFade::EFadeOut )
				{
				iAnimationFadeIndicators[ i ]->StartAnimation( CHermitAnimationFade::EFadeIn );
				}
			else
				{
				delete iAnimationFadeIndicators[ i ];
				iAnimationFadeIndicators.Remove( i );
				}
			break;
			}
		}
#endif
	}

