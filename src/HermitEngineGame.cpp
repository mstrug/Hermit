/*
 ============================================================================
  Name        : HermitEngineGame.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitEngineGame.h"
#include "HermitBackground.h"
#include "HermitCommonObjects.h"
#include "HermitGamePageGame.h"
#include "HermitGamePageSettings.h"
#include "HermitMenuPageAck.h"
#include "HermitGamesStore.h"
#include "HermitSettings.h"
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEEngine/SGEEngineGPTimer.h>
#include <SGEAudio/SGEAudioProvider.h>




CHermitEngineGame* CHermitEngineGame::NewL( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
											TSize aSize )
    {
	CHermitEngineGame *self = new (ELeave) CHermitEngineGame( aObserver, aCommonObjects, aSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CHermitEngineGame::ConstructL()
    {
    BaseConstructL();

	iPageGame = CHermitGamePageGame::NewL( *this, *reinterpret_cast< CHermitCommonObjects* >( iCommonObjects ), iSize );
	iPageSettings = reinterpret_cast< CHermitGamePageSettings* >( reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->PageSettings() );
	iPageAck = CHermitMenuPageAck::NewL( *this, *reinterpret_cast< CHermitCommonObjects* >( iCommonObjects ), iSize );

	iGameTimer = CSGEEngineGPTimer::NewL( *this );
	iGameTimer->Start( 1000000, ETrue );

    iState = EPageGame;
    }

CHermitEngineGame::CHermitEngineGame( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
									  TSize aSize )
        : CSGEEngine( aObserver, aCommonObjects, aSize, EColor16MA )
    {
    }

CHermitEngineGame::~CHermitEngineGame()
	{
	delete iPageGame;
	delete iGameTimer;
	delete iPageAck;
	}

TUint CHermitEngineGame::Id() const
    {
    return KHermitEngineGameId;
    }

void CHermitEngineGame::SaveStateOnClose()
	{
	if ( !iPageGame->IsGameFinished() )
		{
		iPageGame->SaveGameLevelOnClose();
		}
	}

TBool CHermitEngineGame::RestoreState()
	{
	return iPageGame->RestoreGameLevelOnStart();
	}

void CHermitEngineGame::SetPause( TBool aPause )
    {
    iPause = aPause;
    }

void CHermitEngineGame::Activate()
	{
	iPageGame->ResetState();
	iPageSettings->SetObserver( this );
	iGameTimer->Start( 1000000, ETrue );

	iButtonDownPage = ELoading;

	switch ( iState )
		{
		case ELoading:
			break;
		case EPageGame:
				iPageGame->PageEnter();
			break;
		case EPageSettings:
				iPageSettings->PageEnter();
			break;
		case EPageAck:
				iPageAck->PageEnter();
			break;
		}
	}

void CHermitEngineGame::Draw( CBitmapContext* /*aGc*/, const TRect aDrawRect )
    {
	// background
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();
	glPushMatrix();
	glScalef( iSize.iWidth, iSize.iHeight, 0 );
	reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Background()->Draw();
	glPopMatrix();
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

	switch ( iState )
		{
		case ELoading:
			break;
		case EPageGame:
				iPageGame->Draw( aDrawRect );
			break;
		case EPageSettings:
				iPageSettings->Draw( aDrawRect );
			break;
		case EPageAck:
				iPageAck->Draw( aDrawRect );
			break;
		}
    }

TBool CHermitEngineGame::Update( const TTimeIntervalMicroSeconds32 aTimeIntercal )
    {
	TBool ret = reinterpret_cast< CHermitCommonObjects* >( iCommonObjects )->Background()->Update( aTimeIntercal );

	switch ( iState )
		{
		case ELoading:
			break;
		case EPageGame:
				ret |= iPageGame->Update( aTimeIntercal );
			break;
		case EPageSettings:
				ret |= iPageSettings->Update( aTimeIntercal );
			break;
		case EPageAck:
				ret |= iPageAck->Update( aTimeIntercal );
			break;
		}

    return ret;
    }

TBool CHermitEngineGame::KeyDownEvent( const TKeyEvent& aKeyEvent )
    {
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

TBool CHermitEngineGame::KeyEvent( const TKeyEvent& aKeyEvent )
    {
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

TBool CHermitEngineGame::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	//AppLog("CHermitEngineGame::PointerEvent");
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
		case EPageGame:
				page = iPageGame;
			break;
		case EPageSettings:
				page = iPageSettings;
			break;
		case EPageAck:
				page = iPageAck;
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

void CHermitEngineGame::AudioSoundTrackPlayStarted( TUint /*aTrackId*/ )
	{
	}

void CHermitEngineGame::AudioSoundTrackPlayFinished( TUint /*aTrackId*/ )
	{
	}

void CHermitEngineGame::AudioStreamError( TInt /*aError*/ )
	{
	}

void CHermitEngineGame::PageAction( CHermitPageBase* aPage, TInt aAction )
	{
	if ( aPage == iPageSettings )
		{
		if ( aAction == KSettingsActionBtnBack )
			{
            aPage->PageLeave();
			iGameTimer->Start( 1000000, ETrue );
			iState = EPageGame;
			iPageGame->PageEnter();
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
			iGameTimer->Stop();
			iObserver.EngineSwitch( KHermitEngineGameId );
			}
		else if ( aAction == KGameActionBtnSettings )
			{
            aPage->PageLeave();
			iGameTimer->Stop();
			iPageSettings->ResetState();
			iState = EPageSettings;
			iPageSettings->PageEnter();
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
			iPageSettings->ResetState();
			iState = EPageSettings;
			iPageSettings->PageEnter();
			}
		}
	}

void CHermitEngineGame::GPTimerEvent( CSGEEngineGPTimer* aTimer )
	{
	if ( aTimer == iGameTimer )
		{
		iPageGame->GameTimeSecondsInc();
		}
	}




