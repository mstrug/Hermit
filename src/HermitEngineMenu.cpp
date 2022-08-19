/*
 ============================================================================
  Name        : HermitEngineMenu.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitEngineMenu.h"
#include "HermitCompilationSettings.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitBackground.h"
#include "HermitGamesStore.h"
#include "HermitMenuPageMain.h"
#include "HermitMenuPageSettings.h"
#include "HermitMenuPageGame.h"
#include "HermitMenuPageTutorial.h"
#include "HermitMenuPageAbout.h"
#include "HermitMenuPageAck.h"
#include "HermitSettings.h"
#include "HermitLevelProvider.h"
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlTexture.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEFile/SGEFile.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <SGEAudio/SGEAudioFile.h>

const TSize KLogoSize( 256, 64 );
const TSize KLogoTexSize( 256, 64 );

const TInt KLogoTime = 1000000;


CHermitEngineMenu::CHermitEngineMenu( MHermitEngineMenuObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
									  TSize aSize )
        : CSGEEngine( aObserver, aCommonObjects, aSize, EColor16MA ), iMusicTrackId( KErrNotFound )
    {
    }

CHermitEngineMenu::~CHermitEngineMenu()
	{
	delete iRect;
	delete iPageMain;
	delete iPageSettings;
	delete iPageGame;
	delete iPageTutorial;
	delete iPageAbout;
	delete iPageAck;
	}

CHermitEngineMenu* CHermitEngineMenu::NewL( MHermitEngineMenuObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
										    TSize aSize )
    {
	CHermitEngineMenu *self = new (ELeave) CHermitEngineMenu( aObserver, aCommonObjects, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CHermitEngineMenu::ConstructL()
    {
    BaseConstructL();

    GlUtils::EnableTexturing();

	iRect = CGlObjectRect::NewL();

	LoadTexturesL();
    }

void CHermitEngineMenu::ConstructPagesL()
	{
    CHermitCommonObjects *obj = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects );

	iPageMain = CHermitMenuPageMain::NewL( *this, *obj, iSize );
	iPageSettings = CHermitMenuPageSettings::NewL( *this, *obj, iSize );
	iPageGame = CHermitMenuPageGame::NewL( *this, *obj, iSize );
	iPageTutorial = CHermitMenuPageTutorial::NewL( *this, *obj, iSize );
	iPageAbout = CHermitMenuPageAbout::NewL( *this, *obj, iSize );
	iPageAck = CHermitMenuPageAck::NewL( *this, *obj, iSize );

	obj->SetPageSettings( iPageSettings );

	iState = EPageMain;
	Activate();

	iPageSettings->SetMusicTrackId( iMusicTrackId );

    reinterpret_cast< MHermitEngineMenuObserver& >( iObserver ).EngineMenuCreated();
	}

void CHermitEngineMenu::LoadTexturesL()
	{
    CHermitCommonObjects *obj = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects );

    // load textures
    obj->TextureManager()->SetObserver( this );

#ifdef USE_SGEFILE
    CSGEFile& sgeFile = obj->FileProvider()->GetSGEFileL( KHermitSGEFileId );
    
    if ( iLoadingTexturesPhase == 0 )
        {
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuAboutLogoName, KHermitTexAboutLogoId );
        }
    else
        {    
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldEmptyName, KHermitTexFieldEmptyId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldEmptyNameS, KHermitTexFieldEmptyId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldItemName, KHermitTexFieldItemId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldItemNameS, KHermitTexFieldItemId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldNoneName, KHermitTexFieldNoneId );
    
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldSelectedName, KHermitTexFieldSelectedId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldSelectedNameS, KHermitTexFieldSelectedId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldHintName, KHermitTexFieldHintId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldHintNameS, KHermitTexFieldHintId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldEndName, KHermitTexFieldEndId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldEndNameS, KHermitTexFieldEndId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFieldEndIndName, KHermitTexFieldEndIndId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexFieldEndIndNameS, KHermitTexFieldEndIndId, 1 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnGameName, KHermitTexMenuBtnGameId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnSettingsName, KHermitTexMenuBtnSettingsId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnExitName, KHermitTexMenuBtnExitId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnTutorialName, KHermitTexMenuBtnTutorialId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnAboutName, KHermitTexMenuBtnAboutId );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTitleName, KHermitTexImageTitleId, ETrue );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexMenuTitleNameS, KHermitTexImageTitleId, 1 );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexMenuTitleNameSS, KHermitTexImageTitleId, 2 );
        obj->TextureManager()->AddImageToLoadAsMipMapL( sgeFile, KHermitTexMenuTitleNameSSS, KHermitTexImageTitleId, 3 );
        
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnSelectedName, KHermitTexMenuBtnSelectedId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnBackName, KHermitTexMenuBtnBackId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnYesName, KHermitTexMenuBtnYesId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnPressAnimName, KHermitTexMenuBtnPressAnimId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnMinusName, KHermitTexMenuBtnMinusId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnPlusName, KHermitTexMenuBtnPlusId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnResetName, KHermitTexMenuBtnResetId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsSoundName, KHermitTexSettingsSoundId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsMusicName, KHermitTexSettingsMusicId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsLevelName, KHermitTexSettingsLevelId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsTitleName, KHermitTexSettingsTitleId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsColorsName, KHermitTexSettingsColorsId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsColorsSelName, KHermitTexSettingsColorsSelId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuSettingsSelName, KHermitTexSettingsSelId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnPlayName, KHermitTexMenuBtnPlayId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnRightName, KHermitTexMenuBtnRightId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuGameTitleName, KHermitTexGameTitleId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuBtnUndoName, KHermitTexMenuBtnUndoId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialTitleName, KHermitTexTutorialTitleId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialIndicator1Name, KHermitTexTutorialIndicator1Id );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialIndicator2Name, KHermitTexTutorialIndicator2Id );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialText1Name, KHermitTexTutorialText1Id );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialText2Name, KHermitTexTutorialText2Id );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuTutorialText3Name, KHermitTexTutorialText3Id );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuAboutTextName, KHermitTexAboutTextId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexMenuAckTitleName, KHermitTexAckTitleId );
        obj->TextureManager()->AddImageToLoadL( sgeFile, KHermitTexFont1Name, KHermitTexFont1Id );
        }

#else
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuAboutLogoName, KHermitTexAboutLogoId );

    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldEmptyName, KHermitTexFieldEmptyId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldEmptyNameS, KHermitTexFieldEmptyId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldItemName, KHermitTexFieldItemId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldItemNameS, KHermitTexFieldItemId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldNoneName, KHermitTexFieldNoneId );

    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldSelectedName, KHermitTexFieldSelectedId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldSelectedNameS, KHermitTexFieldSelectedId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldHintName, KHermitTexFieldHintId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldHintNameS, KHermitTexFieldHintId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldEndName, KHermitTexFieldEndId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldEndNameS, KHermitTexFieldEndId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexFieldEndIndName, KHermitTexFieldEndIndId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexFieldEndIndNameS, KHermitTexFieldEndIndId, 1 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnGameName, KHermitTexMenuBtnGameId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnSettingsName, KHermitTexMenuBtnSettingsId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnExitName, KHermitTexMenuBtnExitId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnTutorialName, KHermitTexMenuBtnTutorialId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnAboutName, KHermitTexMenuBtnAboutId );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTitleName, KHermitTexImageTitleId, ETrue );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexMenuTitleNameS, KHermitTexImageTitleId, 1 );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexMenuTitleNameSS, KHermitTexImageTitleId, 2 );
    obj->TextureManager()->AddImageToLoadAsMipMapL( KHermitTexMenuTitleNameSSS, KHermitTexImageTitleId, 3 );
    
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnSelectedName, KHermitTexMenuBtnSelectedId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnBackName, KHermitTexMenuBtnBackId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnYesName, KHermitTexMenuBtnYesId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnPressAnimName, KHermitTexMenuBtnPressAnimId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnMinusName, KHermitTexMenuBtnMinusId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnPlusName, KHermitTexMenuBtnPlusId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnResetName, KHermitTexMenuBtnResetId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsSoundName, KHermitTexSettingsSoundId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsMusicName, KHermitTexSettingsMusicId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsLevelName, KHermitTexSettingsLevelId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsTitleName, KHermitTexSettingsTitleId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsColorsName, KHermitTexSettingsColorsId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsColorsSelName, KHermitTexSettingsColorsSelId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuSettingsSelName, KHermitTexSettingsSelId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnPlayName, KHermitTexMenuBtnPlayId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnRightName, KHermitTexMenuBtnRightId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuGameTitleName, KHermitTexGameTitleId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuBtnUndoName, KHermitTexMenuBtnUndoId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialTitleName, KHermitTexTutorialTitleId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialIndicator1Name, KHermitTexTutorialIndicator1Id );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialIndicator2Name, KHermitTexTutorialIndicator2Id );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialText1Name, KHermitTexTutorialText1Id );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialText2Name, KHermitTexTutorialText2Id );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuTutorialText3Name, KHermitTexTutorialText3Id );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuAboutTextName, KHermitTexAboutTextId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexMenuAckTitleName, KHermitTexAckTitleId );
    obj->TextureManager()->AddImageToLoadL( KHermitTexFont1Name, KHermitTexFont1Id );
#endif
    
    glColor4ub( 255, 255, 255, 255 );
    obj->TextureManager()->StartLoadingImagesL();
    }

void CHermitEngineMenu::LoadAudioL()
    {
	RArray< TSGEAudioFile > files;
	CleanupClosePushL( files );

	iCommonObjects->AudioProvider()->SetObserver( this );

	files.AppendL( TSGEAudioFile( KHermitMusic1FileName, KHermitMusic1Id, ESGEAudioGroupMusic, ESGEAudioFileTypeMP3 ) );
	files.AppendL( TSGEAudioFile( KHermitSoundButtonPressFileName, KHermitSoundButtonPressId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundButton2PressFileName, KHermitSoundButton2PressId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundEndGameWinFileName, KHermitSoundEndGameWinId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundEndGameLoseFileName, KHermitSoundEndGameLoseId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundFieldPressFileName, KHermitSoundFieldPressId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundMenuInFileName, KHermitSoundMenuInId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
	files.AppendL( TSGEAudioFile( KHermitSoundMenuOutFileName, KHermitSoundMenuOutId, ESGEAudioGroupSound, ESGEAudioFileTypeWAV ) );
#ifdef USE_SGEFILE
    CHermitCommonObjects *obj = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects );
    //CSGEFile& sgeFile = obj->FileProvider()->GetSGEFileL( KHermitSGEFileId );
    TFileName fn = obj->FileProvider()->AppendToPrivateFolder( KHermitSGEFileName() );
    iCommonObjects->AudioProvider()->LoadFilesL( fn, files );    
#else
    iCommonObjects->AudioProvider()->LoadFilesL( files );
#endif
    
    CleanupStack::PopAndDestroy( &files );
    }

void CHermitEngineMenu::SwitchToGamePage()
	{
	iState = EPageGame;
	}

TUint CHermitEngineMenu::Id() const
    {
    return KHermitEngineMenuId;
    }

void CHermitEngineMenu::SetPause( TBool aPause )
    {
    iPause = aPause;
    }

void CHermitEngineMenu::Activate()
	{
    if ( iState != ELoading )
        {
        iPageSettings->SetObserver( this );
        }
    
	iButtonDownPage = ELoading;

	switch ( iState )
		{
		case ELoading:
			break;
		case EPageMain:
				iPageMain->PageEnter();
			break;
		case EPageSettings:
				iPageSettings->PageEnter();
			break;
		case EPageGame:
				iPageGame->ResetState();
				iPageGame->PageEnter();
			break;
		case EPageTutorial:
				iPageTutorial->PageEnter();
			break;
		case EPageAbout:
				iPageAbout->PageEnter();
			break;
		case EPageAck:
				iPageAck->PageEnter();
			break;
		case EExit:
		    break;
		}
	}

void CHermitEngineMenu::DrawLoadingLogo()
    {
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    TGlTexture* tex = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->TextureManager()->Texture( KHermitTexAboutLogoId );
    if ( tex )
        {
        GlUtils::EnableTexturing();
        //GlUtils::EnableTransparency();
        GlUtils::EnableAlphaTest();
        
        TPoint p( ( iSize.iWidth - KLogoSize.iWidth ) / 2, ( iSize.iHeight - KLogoSize.iHeight ) / 2 );
    
        glPushMatrix();
        GlUtils::Translate2D( p );
        GlUtils::Scale2D( KLogoTexSize );
        tex->Use();
        iRect->Draw();
        glPopMatrix();
        
        GlUtils::DisableAlphaTest();
        }
    }

void CHermitEngineMenu::Draw( CBitmapContext* /*aGc*/, const TRect aDrawRect )
    {
#ifndef USE_ANIMATIONS
	if ( iLeaving )
		{
		glClearColor( 0, 0, 0, 1 );
		glClear( GL_COLOR_BUFFER_BIT );
		return;
		}
#endif
	
	//glClearColor( 0, 1, 0, 1 );
	//glClear( GL_COLOR_BUFFER_BIT );

	glPushMatrix();

	glScalef( iSize.iWidth, iSize.iHeight, 0 );
        //GlUtils::UnbindTexture();
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();
	reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Background()->Draw();
	//GlUtils::TestDraw();

	glPopMatrix();

	switch ( iState )
		{
		case ELoading:
                DrawLoadingLogo();
			break;
		case EPageMain:
				iPageMain->Draw( aDrawRect );
			break;
		case EPageSettings:
				iPageSettings->Draw( aDrawRect );
			break;
		case EPageGame:
				iPageGame->Draw( aDrawRect );
			break;
		case EPageTutorial:
				iPageTutorial->Draw( aDrawRect );
			break;
		case EPageAbout:
				iPageAbout->Draw( aDrawRect );
			break;
		case EPageAck:
				iPageAck->Draw( aDrawRect );
			break;
		case EExit:
		    break;
		}
    }

TBool CHermitEngineMenu::Update( const TTimeIntervalMicroSeconds32 aTimeIntercal )
    {
	TBool ret = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Background()->Update( aTimeIntercal );

	switch ( iState )
		{
		case ELoading:
                if ( iLoadingTexturesPhase == 1 )
                    {
                    iLoadingTexturesPhase++;
                    }
                else if ( iLoadingTexturesPhase == 2 )
                    {
                    iLogoTime += aTimeIntercal.Int();
                    if ( iLogoTime > KLogoTime )
                        {
                        iLoadingTexturesPhase++;
                        TRAPD( err, LoadTexturesL() );
                        if ( err )
                            {
                            iObserver.EngineClose( KHermitEngineMenuId, err );
                            }
                        }
                    }
			break;
		case EPageMain:
				ret |= iPageMain->Update( aTimeIntercal );
			break;
		case EPageSettings:
				ret |= iPageSettings->Update( aTimeIntercal );
			break;
		case EPageGame:
				ret |= iPageGame->Update( aTimeIntercal );
			break;
		case EPageTutorial:
				ret |= iPageTutorial->Update( aTimeIntercal );
			break;
		case EPageAbout:
				ret |= iPageAbout->Update( aTimeIntercal );
			break;
		case EPageAck:
				ret |= iPageAck->Update( aTimeIntercal );
			break;
		case EExit:
                iObserver.EngineClose( KHermitEngineMenuId, KErrNone );		    
		    break;
		}

    return ret;
    }

TBool CHermitEngineMenu::KeyDownEvent( const TKeyEvent& aKeyEvent )
    {
    if ( iState == ELoading )
        {
        return EFalse;
        }

    if ( aKeyEvent.iCode == EKeyIncVolume )
        {
        iPageSettings->KeyEventVolumeUp();

        return ETrue;
        }
    else if ( aKeyEvent.iCode == EKeyDecVolume )
        {
        iPageSettings->KeyEventVolumeDown();

        return ETrue;
        }
    
    return EFalse;
    }

TBool CHermitEngineMenu::KeyEvent( const TKeyEvent& aKeyEvent )
    {
    if ( iState == ELoading )
        {
        return EFalse;
        }

#ifndef _SGE_BADA_
    if ( aKeyEvent.iCode == EKeyIncVolume )
#else
	if ( aKeyEvent.iCode == Osp::Ui::KEY_SIDE_UP )
#endif
	    {
		iPageSettings->KeyEventVolumeUp();

		return ETrue;
        }
#ifndef _SGE_BADA_
    else if ( aKeyEvent.iCode == EKeyDecVolume )
#else
    else if ( aKeyEvent.iCode == Osp::Ui::KEY_SIDE_DOWN )
#endif
        {
		iPageSettings->KeyEventVolumeDown();

		return ETrue;
        }
    
    return EFalse;
    }

TBool CHermitEngineMenu::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	//AppLog("CHermitEngineMenu::PointerEvent");
	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		iButtonDownPage = iState;
		}
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		if ( iButtonDownPage != iState )
			{
			return EFalse;
			}
		}

	CHermitPageBase* page = NULL;

	switch ( iState )
		{
		case ELoading:
			break;
		case EPageMain:
				page = iPageMain;
			break;
		case EPageSettings:
				page = iPageSettings;
			break;
		case EPageGame:
				page = iPageGame;
			break;
		case EPageTutorial:
				page = iPageTutorial;
			break;
		case EPageAbout:
				page = iPageAbout;
			break;
		case EPageAck:
				page = iPageAck;
			break;
        case EExit:
            break;
		}

	if ( page )
		{
#ifndef POINTER_EVENTS_ON_PAGE_ENTER
		if ( page->IsEntering() )
			{
			iButtonDownPage = ELoading;
			}
		else
#endif
			{
			return page->PointerEvent( aPointerEvent );
			}
		}

	return EFalse;
	}

void CHermitEngineMenu::TextureCreated( TUint aTextureId, TInt aError )
	{
    glColor4ub( 255, 255, 255, 255 );

    if ( aError != KErrNone )
        {
        return;
        }
    
    CHermitCommonObjects *obj = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects );
    TGlTexture* tex = obj->TextureManager()->Texture( aTextureId );

    if ( aTextureId == (TUint)KHermitTexMenuBtnRightId )
		{
    	tex->iTextureWrapS = GL_REPEAT;
    	tex->iTextureWrapT = GL_REPEAT;
		}
    else
		{
    	tex->iTextureWrapS = GL_CLAMP_TO_EDGE;
    	tex->iTextureWrapT = GL_CLAMP_TO_EDGE;
		}

	if ( aTextureId == (TUint)KHermitTexFieldEmptyId || aTextureId == (TUint)KHermitTexFieldItemId ||
		 aTextureId == (TUint)KHermitTexFieldEndId || aTextureId == (TUint)KHermitTexFieldHintId ||
		 aTextureId == (TUint)KHermitTexFieldSelectedId || aTextureId == (TUint)KHermitTexFieldEndIndId ||
		 aTextureId == (TUint)KHermitTexImageTitleId )
		{
		tex->iTextureMinification = GL_LINEAR_MIPMAP_LINEAR; //GL_NEAREST_MIPMAP_NEAREST; //GL_NEAREST_MIPMAP_LINEAR;
		tex->iTextureMagnification = GL_LINEAR_MIPMAP_LINEAR; //GL_NEAREST_MIPMAP_NEAREST; //GL_NEAREST_MIPMAP_LINEAR;
		}
	else if ( aTextureId == (TUint)KHermitTexMenuBtnPressAnimId || aTextureId == (TUint)KHermitTexTutorialIndicator1Id ||
		 aTextureId == (TUint)KHermitTexTutorialIndicator2Id )
		{
		//tex->iTextureMinification = GL_LINEAR;
		tex->iTextureMagnification = GL_LINEAR;
		}
	}

void CHermitEngineMenu::TexturesCreatedAll()
	{
    if ( iLoadingTexturesPhase == 0 )
        {
        iLogoTime = 0;
        iLoadingTexturesPhase++;
        return;
        }
    
#ifdef USE_AUDIO
	TRAPD( err, LoadAudioL() );
	if ( err )
	    {
	    iObserver.EngineClose( KHermitEngineMenuId, err );
	    }
#else
	ConstructPagesL();
#endif
	}

void CHermitEngineMenu::AudioFileLoaded( TInt aSoundSampleId, TInt aError )
	{
#ifdef USE_AUDIO
	if ( aError == KErrNone && (TUint) aSoundSampleId == KHermitMusic1Id )
		{
		//iMusicTrackId = iCommonObjects->AudioProvider()->Play( aSoundSampleId, ESGEAudioRepeatInfinite );
		}
	
	if ( aError != KErrNone )
	    { // resume load
	    RArray<TSGEAudioFile> tab;
	    iCommonObjects->AudioProvider()->LoadFilesL( tab );
	    }
#endif
	}

void CHermitEngineMenu::AudioAllFilesLoaded()
	{
	//ConstructPagesL();
    iCommonObjects->AudioProvider()->CreateAudioStreamL();
	}

void CHermitEngineMenu::AudioSoundTrackPlayStarted( TUint /*aTrackId*/ )
	{
	}

void CHermitEngineMenu::AudioSoundTrackPlayFinished( TUint aTrackId )
	{
	if ( iLeaving )
		{
		TSGEAudioTrackState state = iCommonObjects->AudioProvider()->TrackState( aTrackId );
		if ( (TUint)state.iSampleId == KHermitSoundButtonPressId )
			{
		    iState = EExit;
			//iObserver.EngineClose( KHermitEngineMenuId, KErrNone );
			}
		}
	}

void CHermitEngineMenu::AudioStreamCreated( TInt /*aError*/ )
    {
    iMusicTrackId = iCommonObjects->AudioProvider()->Play( KHermitMusic1Id, ESGEAudioRepeatInfinite );
    
    ConstructPagesL();
    }

void CHermitEngineMenu::AudioStreamError( TInt /*aError*/ )
	{
	if ( iLeaving )
		{
		iObserver.EngineClose( KHermitEngineMenuId, KErrNone );
		}
	}

void CHermitEngineMenu::PageAction( CHermitPageBase* aPage, TInt aAction )
	{
	if ( aPage == iPageMain )
		{
		if ( aAction == KMenuActionBtnGame )
			{
		    aPage->PageLeave();
			iPageGame->ResetState();
			iState = EPageGame;
			iPageGame->PageEnter();
			}
		else if ( aAction == KMenuActionBtnExit )
			{
			iLeaving = ETrue;
#ifdef USE_AUDIO
			if ( reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Settings()->SoundVolume() == 0 )
				{
				iObserver.EngineClose( KHermitEngineMenuId, KErrNone );
				}
			// else -> close in AudioSoundTrackPlayFinished
#else
			iObserver.EngineClose( KHermitEngineMenuId, KErrNone );
#endif
			}
		else if ( aAction == KMenuActionBtnSettings )
			{
            aPage->PageLeave();
			iPageSettings->ResetState();
			iState = EPageSettings;
			iPageSettings->PageEnter();
			}
		else if ( aAction == KMenuActionBtnTutorial )
			{
            aPage->PageLeave();
			iPageTutorial->ResetState();
			iState = EPageTutorial;
			iPageTutorial->PageEnter();
			}
		else if ( aAction == KMenuActionBtnAbout )
			{
            aPage->PageLeave();
			iState = EPageAbout;
			iPageAbout->PageEnter();
			}
		else if ( aAction == KMenuActionAnimationTitleFinished )
			{
			reinterpret_cast< MHermitEngineMenuObserver& >( iObserver ).EngineMenuTitleAnimationFinished();
			}
		}
	else if ( aPage == iPageSettings )
		{
		if ( aAction == KSettingsActionBtnBack )
			{
            aPage->PageLeave();
			iState = EPageMain;
			iPageMain->PageEnter();
			}
		else if ( aAction == KSettingsActionBtnResetLevels )
			{
            aPage->PageLeave();
			iState = EPageAck;
			iPageAck->PageEnter();
			}
		}
	else if ( aPage == iPageGame )
		{
		if ( aAction == KGameActionBtnBack )
			{
            aPage->PageLeave();
			iState = EPageMain;
			iPageMain->PageEnter();
			}
		else if ( aAction == KGameActionBtnPlay )
			{
            aPage->PageLeave();
			iObserver.EngineSwitch( KHermitEngineMenuId );
			}
		}
	else if ( aPage == iPageTutorial )
		{
		if ( aAction == KTutorialActionBtnBack )
			{
            aPage->PageLeave();
			iState = EPageMain;
			iPageMain->PageEnter();
			}
		}
	else if ( aPage == iPageAbout )
		{
		if ( aAction == KAboutActionBtnBack )
			{
            aPage->PageLeave();
			iState = EPageMain;
			iPageMain->PageEnter();
			}
		}
	else if ( aPage == iPageAck )
		{
		if ( aAction == KAckActionBtnBack || aAction == KAckActionBtnNo )
			{
            aPage->PageLeave();
			iPageSettings->ResetState();
			iState = EPageSettings;
			iPageSettings->PageEnter();
			}
		else if ( aAction == KAckActionBtnYes )
			{
            aPage->PageLeave();
			reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->GamesStore()->ResetAllData();
			reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->GamesStore()->SaveToFile();
			reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Settings()->SetLevelIdVisible( ETrue );
			reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->LevelProvider()->ResetLevelPosition();
			iPageSettings->ResetState();
			iState = EPageSettings;
			iPageSettings->PageEnter();
			}
		}

	}





