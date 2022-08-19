/*
 ============================================================================
  Name        : HermitGamePageGame.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitGamePageGame.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitGameLevel.h"
#include "HermitLevel.h"
#include "HermitLevelProvider.h"
#include "HermitGamesStore.h"
#include "GlFont.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitAnimationFade.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <SGEFile/SGEFileProvider.h>
#include <e32def.h>
#ifdef _SGE_BADA_
#include <FIo.h>
using namespace Osp::Io;
#endif



const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KButtonUndoSize( 128, 128 );
const TSize KButtonUndoTexSize( 128, 128 );

const TSize KButtonBigSize( 128, 128 );
const TSize KButtonBigTexSize( 128, 128 );

const TSize KButtonSmallSize( 64, 64 );
const TSize KButtonSmallTexSize( 64, 64 );


const TInt KBtnBackIdx = 0;
const TInt KBtnSettingsIdx = 1;
const TInt KBtnUndoIdx = 2;

const TInt KBtnSettingsMargin = 20;

const TInt KSwitchingLevelCounter = 2000000;
const TInt KSwitchingLevelCounterGameLose = 50000;

const TInt KAnimationMenuStatusFadeStepTime = 20000; // 20ms


#define ABS( x ) ( x > 0 ? (x) : -(x) )


CHermitGamePageGame::CHermitGamePageGame( MHermitPageObserver& aObserver,
												  CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
    {
    }

CHermitGamePageGame::~CHermitGamePageGame()
    {
#ifdef PROTO_TOUCH_ANIMATIONS
	iAnimationFadeIndicators.ResetAndDestroy();
#endif
	delete iAnimationStatusFade;
	delete iAnimationMenuFade;
	delete iGameLevel;
    }

CHermitGamePageGame* CHermitGamePageGame::NewL( MHermitPageObserver& aObserver,
													    CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitGamePageGame* self = new (ELeave) CHermitGamePageGame( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitGamePageGame::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnBackIdx ].iActionId = KGameActionBtnBack;
	iButtons[ KBtnBackIdx ].iTextureSize = KButtonTexSize;

	iButtons[ KBtnSettingsIdx ].iTextureId = KHermitTexMenuBtnSettingsId;
	iButtons[ KBtnSettingsIdx ].iActionId = KGameActionBtnSettings;
	iButtons[ KBtnSettingsIdx ].iTextureSize = KButtonBigTexSize;

	iButtons[ KBtnUndoIdx ].iTextureId = KHermitTexMenuBtnUndoId;
	iButtons[ KBtnUndoIdx ].iTextureSize = KButtonUndoTexSize;

	iAnimationStatusFade = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationStatusFade->SetAnimationStepTime( KAnimationMenuStatusFadeStepTime );
	iAnimationMenuFade = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationMenuFade->SetAnimationStepTime( KAnimationMenuStatusFadeStepTime );

	iGameLevel = CHermitGameLevel::NewL( iCommonObjects );
	iGameLevel->SetObserver( *this );
	UpdateCurrentLevel();

	SizeChanged( iSize );

	iState = EIdle;
	}

void CHermitGamePageGame::UpdateCurrentLevel()
	{
    CHermitLevel* lvl = iCommonObjects.LevelProvider()->CurrentLevel();
    if ( lvl )
		{
    	iGameLevel->LoadGameLevelL( *lvl );
        CalculateLevelRect();
        GameTimeSecondsReset();
        iState = EIdle;
		}
	}

void CHermitGamePageGame::HideMenu()
	{
	if ( iGameLose )
		{
		return;
		}
	iMenuVisible = EFalse;
	iStatusVisible = EFalse;
	}

void CHermitGamePageGame::ResetState()
	{
	iGameLose = EFalse;
	UpdateCurrentLevel();
	HideMenu();
	//iAnimationStatusFade->EnableFadeRects( ETrue, ETrue );
	iState = EIdle;

#ifdef PROTO_TOUCH_ANIMATIONS
	iAnimationFadeIndicators.ResetAndDestroy();
#endif

#ifdef USE_ANIMATIONS
	iAnimationStatusFade->RestartAnimation();
	iAnimationMenuFade->RestartAnimation();
#endif
	}

void CHermitGamePageGame::ResetCurrentLevel()
	{
    CHermitLevel* lvl = iCommonObjects.LevelProvider()->CurrentLevel();
    if ( lvl )
		{
    	TSize ls = lvl->Size();
        TRect rlvl( TPoint( 0, ( iSize.iHeight - iSize.iWidth ) / 2 ), TSize( iSize.iWidth, iSize.iWidth ) );
        iGameLevel->AnimateNextLevel( lvl, rlvl );
        GameTimeSecondsReset();
        StartHidingMenu( ETrue );
		}
	}

void CHermitGamePageGame::StartHidingMenu( TBool aAlways )
	{
	if ( iGameLose && !aAlways )
		{
		return;
		}

#ifdef USE_ANIMATIONS
	iState = EAnimation;
	iAnimationStatusFade->StartAnimation( CHermitAnimationFade::EFadeOut );
	iAnimationMenuFade->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
	HideMenu();
#endif
	}

void CHermitGamePageGame::StartShowingMenu()
	{
	iAnimationStatusFade->EnableFadeRects( ETrue, ETrue );
	iStatusVisible = ETrue;
	iMenuVisible = ETrue;
#ifdef USE_ANIMATIONS
	iState = EAnimation;
	iAnimationStatusFade->StartAnimation( CHermitAnimationFade::EFadeIn );
	iAnimationMenuFade->StartAnimation( CHermitAnimationFade::EFadeIn );
#endif
	}

void CHermitGamePageGame::GameTimeSecondsInc()
	{
	if ( !iGameLevel->GameFinished() && iGameLevel->MovesCounter() > 0 )
		{
		iGameTimeSeconds++;
		}
	}

void CHermitGamePageGame::GameTimeSecondsReset()
	{
	iGameTimeSeconds = 0;
	}

TBool CHermitGamePageGame::IsGameStarted()
	{
	return ( !iGameLevel->GameFinished() && iGameLevel->MovesCounter() > 0 );
	}

TBool CHermitGamePageGame::IsGameFinished()
	{
	return iGameLevel->GameFinished();
	}

void CHermitGamePageGame::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );
	iButtons[ KBtnSettingsIdx ].iRect = TRect( TPoint( -KBtnSettingsMargin, -KBtnSettingsMargin ), KButtonBigSize );
	iButtons[ KBtnUndoIdx ].iRect = TRect( TPoint( iButtons[ KBtnSettingsIdx ].iRect.iBr.iX, 0 ), KButtonUndoSize );

	iAnimationMenuFade->SetFadeRect( TRect( TPoint( 0, 0 ), TSize( iSize.iWidth, iLevelRect.iTl.iY ) ) );
	iAnimationStatusFade->SetFadeRect( TRect( TPoint( 0, iLevelRect.iBr.iY ), TSize( iSize.iWidth, iSize.iHeight - iLevelRect.iBr.iY ) ) );

	CalculateLevelRect();
	}

void CHermitGamePageGame::CalculateLevelRect()
	{
    TSize ls = iGameLevel->Size();
    iLevelRect = TRect( TPoint( 0, ( iSize.iHeight - iSize.iWidth ) / 2 ), TSize( iSize.iWidth, iSize.iWidth ) );
    iGameLevel->SetLevelRect( iLevelRect, ls );
	}

void CHermitGamePageGame::AnimateNextLevel()
	{
    CHermitLevel* lvl = NULL;
    if ( iGameLose )
    	{
    	lvl = iCommonObjects.LevelProvider()->CurrentLevel();
    	}
    else
		{
    	lvl = iCommonObjects.LevelProvider()->NextLevel();
		}

    if ( lvl )
		{
    	TSize ls = lvl->Size();
        TRect rlvl( TPoint( 0, ( iSize.iHeight - iSize.iWidth ) / 2 ), TSize( iSize.iWidth, iSize.iWidth ) );
        iGameLevel->AnimateNextLevel( lvl, rlvl );
        GameTimeSecondsReset();
		}
	}

void CHermitGamePageGame::PageEnter()
	{
	CHermitPageBase::PageEnter();

	HideMenu();

#ifdef USE_ANIMATIONS
	iAnimationStatusFade->RestartAnimation();
	iAnimationMenuFade->RestartAnimation();
#endif

	}

void CHermitGamePageGame::DrawFills()
	{
	//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();

    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );
    
	if ( !iStatusVisible )
		{
		// top
		glPushMatrix();
		GlUtils::Translate2D( TPoint( 0, iLevelRect.iBr.iY ) );
		GlUtils::Scale2D( TSize( iSize.iWidth, iSize.iHeight - iLevelRect.iBr.iY ) );
		iRect->Draw();
		glPopMatrix();
		}
	// top fills are drawed in draw status

	// bottom
	if ( !iMenuVisible /*|| ( iState == ESwitchingLevel && iSwitchingLevelPhase > 1 )*/ )
		{
		glPushMatrix();
		GlUtils::Scale2D( TSize( iSize.iWidth, iLevelRect.iTl.iY ) );
		iRect->Draw();
		glPopMatrix();
		}
	else
		{
		// bottom
		TInt tmp = iButtons[ KBtnSettingsIdx ].iRect.iBr.iX - KBtnSettingsMargin;

		if ( iGameLevel->UndoPossible() || iGameLose || iState == ESwitchingLevel /*&& iAnimationStatusFade->IsAnimationFinished()*/ )
			{ // drawing undo/reset button
			// between buttons (settings and undo)
			glPushMatrix();
			GlUtils::Translate2D( TPoint( tmp, 0 ) );
			GlUtils::Scale2D( TSize( iButtons[ KBtnUndoIdx ].iRect.iTl.iX - tmp, iButtons[ KBtnUndoIdx ].iRect.iBr.iY ) );
			iRect->Draw();
			glPopMatrix();

			// between buttons (undo and back)
			glPushMatrix();
			GlUtils::Translate2D( TPoint( iButtons[ KBtnUndoIdx ].iRect.iBr.iX, 0 ) );
			GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX - iButtons[ KBtnUndoIdx ].iRect.iBr.iX, iButtons[ KBtnUndoIdx ].iRect.iBr.iY ) );
			iRect->Redraw();
			glPopMatrix();

			// between buttons (top of settings button)
			glPushMatrix();
			GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnSettingsIdx ].iRect.iBr.iY - KBtnSettingsMargin ) );
			GlUtils::Scale2D( TSize( iSize.iWidth, iLevelRect.iTl.iY - iButtons[ KBtnSettingsIdx ].iRect.iBr.iY + KBtnSettingsMargin ) );
			iRect->Redraw();
			glPopMatrix();

			// top of back button
			glPushMatrix();
			GlUtils::Translate2D( TPoint( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
			GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.Width(), iLevelRect.iTl.iY - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
			iRect->Redraw();
			glPopMatrix();
			}
		else
			{
			// between buttons
			glPushMatrix();
			GlUtils::Translate2D( TPoint( tmp, 0 ) );
			GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX - tmp, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
			iRect->Draw();
			glPopMatrix();

			// between buttons (top of back button)
			glPushMatrix();
			GlUtils::Translate2D( TPoint( tmp, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
			GlUtils::Scale2D( TSize( iSize.iWidth - tmp, iButtons[ KBtnSettingsIdx ].iRect.iBr.iY - KBtnSettingsMargin - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
			iRect->Redraw();
			glPopMatrix();

			// between buttons (top of settings button)
			glPushMatrix();
			GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnSettingsIdx ].iRect.iBr.iY - KBtnSettingsMargin ) );
			GlUtils::Scale2D( TSize( iSize.iWidth, iLevelRect.iTl.iY - iButtons[ KBtnSettingsIdx ].iRect.iBr.iY + KBtnSettingsMargin ) );
			iRect->Redraw();
			glPopMatrix();
			}
		}
    glColor4ub( 255, 255, 255, 255 );
    
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();	
	}

void CHermitGamePageGame::DrawStatus()
	{
	CGlFont *f1 = iCommonObjects.Font1();

#ifdef _SGE_BADA_
	String time; //( _L("00:12") );
	String moves;
	if ( iState == ESwitchingLevel )
		{
		time.Format( 7, L"%02d:%02d", iSwitchingLevelSavedTime / 60, iSwitchingLevelSavedTime % 60 );
		}
	else if ( iGameTimeSeconds > 0 )
		{
		time.Format( 7, L"%02d:%02d", iGameTimeSeconds / 60, iGameTimeSeconds % 60 );
		}
#else
    TBuf<15> time;
    TBuf<10> moves;
    if ( iState == ESwitchingLevel )
        {
        time.Format( _L("%02d:%02d"), iSwitchingLevelSavedTime / 60, iSwitchingLevelSavedTime % 60 );
        }
    else if ( iGameTimeSeconds > 0 )
        {
        time.Format( _L("%02d:%02d"), iGameTimeSeconds / 60, iGameTimeSeconds % 60 );
        }
#endif
	if ( iState == ESwitchingLevel )
		{
		moves.AppendNum( iSwitchingLevelSavedMovesCounter );
		}
	else if ( iGameLevel->MovesCounter() > 0 )
		{
		moves.AppendNum( iGameLevel->MovesCounter() );
		}

	TInt l1 = f1->GetTextWidth( time );
	TInt l2 = f1->GetTextWidth( moves );
    TInt lH = f1->LetterSize().iHeight;
	TInt marginX = f1->LetterSize().iWidth;
	TInt marginY = marginX + lH;
	TInt stepY = lH / 3;

	TPoint p1( iSize.iWidth - l1 - marginX, iSize.iHeight - marginY );
	TPoint p2( iSize.iWidth - l2 - marginX, iSize.iHeight - ( marginY + lH + stepY ) );

	f1->DrawText( p1, time );
	f1->DrawText( p2, moves );

	iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();

    GlUtils::DisableTransparency();
    
	// left fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iLevelRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( p1.iX, iSize.iHeight - iLevelRect.iBr.iY ) );
	iRect->Draw();
	glPopMatrix();

	// right fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iSize.iWidth - marginX, iLevelRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( marginX, iSize.iHeight - iLevelRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// top fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( p1.iX, p1.iY + lH ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - p1.iX, marginY ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( p1.iX, iLevelRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( l1, p2.iY - iLevelRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// between lines fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( p1.iX, p1.iY - stepY ) );
	GlUtils::Scale2D( TSize( l1, stepY ) );
	iRect->Redraw();
	glPopMatrix();

	// line 2 fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( p1.iX, p2.iY ) );
	GlUtils::Scale2D( TSize( l1 - l2, lH ) );
	iRect->Redraw();
	glPopMatrix();

	GlUtils::EnableTransparency();
	}

void CHermitGamePageGame::Draw( const TRect aDrawRect )
	{
	glPushMatrix();
	iGameLevel->Draw( aDrawRect );
	glPopMatrix();

	if ( iMenuVisible )
		{
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( i == KBtnUndoIdx && ( !iGameLevel->UndoPossible() && !iGameLose && iState != ESwitchingLevel /*|| !iAnimationStatusFade->IsAnimationFinished()*/ ) )
				{
				continue;
				}
			if ( iButtons[ i ].iSelected )
				{
				glPushMatrix();
				GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
				GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
				iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
				iRect->Redraw();
				glPopMatrix();
				}
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			iButtons[ i ].Rotate();
			GlUtils::Scale2D( iButtons[ i ].iTextureSize );
			iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
			iRect->Redraw();
			glPopMatrix();
			}
		}
	if ( iStatusVisible )
		{
		DrawStatus();
		}

	// button pressed animation
#ifdef USE_ANIMATIONS
	if ( iState == EAnimation || iState == ELeaving || iGameLose || iState == ESwitchingLevel )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		iAnimationStatusFade->Draw( aDrawRect );
		iAnimationMenuFade->Draw( aDrawRect );
		}
#endif

    DrawFills();

#ifdef PROTO_TOUCH_ANIMATIONS
	iCommonObjects.TextureManager()->Texture( KHermitTexTutorialIndicator2Id )->Use();
	for ( TInt i = 0; i < iAnimationFadeIndicators.Count(); i++ )
		{
		iAnimationFadeIndicators[ i ]->Draw( TRect() );
		}
#endif

	CHermitPageBase::Draw( aDrawRect );

	iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSettingsId )->Discard();
	}

TBool CHermitGamePageGame::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	iGameLevel->Update( aTimeInterval );

#ifdef PROTO_TOUCH_ANIMATIONS
	for ( TInt i = 0; i < iAnimationFadeIndicators.Count(); i++ )
		{
		iAnimationFadeIndicators[ i ]->Update( aTimeInterval );
		}
#endif

	if ( iState == ESwitchingLevel )
		{
		iSwitchingLevelCounter += aTimeInterval.Int();

		if ( iSwitchingLevelCounter >= iSwitchingLevelTime )
			{
			if ( iSwitchingLevelPhase == 1 )
				{
				iSwitchingLevelCounter = 0;
				iCommonObjects.GamesStore()->SaveToFile();
				StartHidingMenu();
				iState = ESwitchingLevel;
				iSwitchingLevelPhase = 2;
				iSwitchingLevelTime = KSwitchingLevelCounter;

				AnimateNextLevel();
				iGameLose = EFalse;
				}
			else if ( iSwitchingLevelPhase == 2 )
				{
				}
//			else
//				{
//				iSwitchingLevelCounter = 0;
//				iCommonObjects.LevelProvider()->NextLevel();
//				UpdateCurrentLevel();
//				iState = EIdle;
//				AppLog( "switching levels end" );
//				}
			}
		}

#ifdef USE_ANIMATIONS
	if ( iState == EAnimation || iState == ELeaving || iState == ESwitchingLevel || iGameLose )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		iAnimationStatusFade->Update( aTimeInterval );
		iAnimationMenuFade->Update( aTimeInterval );
		}
#endif

	return ETrue;
	}

TBool CHermitGamePageGame::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState != EIdle && iState != EAnimation && iState != EEntering )
		{
		return EFalse;
		}

	TBool menuShow = EFalse;
	TBool menuHide = EFalse;

#ifdef PROTO_TOUCH_ANIMATIONS
	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		TInt m = 16;
		TRect r( TPoint( aPointerEvent.iPosition.iX - m, aPointerEvent.iPosition.iY - m ),
				 TSize( 2 * m, 2 * m ) );
		CHermitAnimationFade* animationFadeIndicator = CHermitAnimationFade::NewL( this, iCommonObjects );
		animationFadeIndicator->SetStartIncrement( 20 );
		animationFadeIndicator->SetAnimationStepTime( 20000 );
		animationFadeIndicator->SetFadeColor( TRgb( 255, 255, 255 ) );
		animationFadeIndicator->UseExternalTextures( ETrue );
		animationFadeIndicator->EnableScaleAnimation( 20000, 0.05 );
		animationFadeIndicator->SetFadeRect( r );
		animationFadeIndicator->RestartAnimation();
		animationFadeIndicator->ResetScale();
		animationFadeIndicator->StartAnimation( CHermitAnimationFade::EFadeOut );
		iAnimationFadeIndicators.Append( animationFadeIndicator );
		}
#endif

	/* Handling buttons actions */
	TBool sndPlay = EFalse;
	if ( iMenuVisible && aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iSelected &&
				 iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				if ( iButtons[ i ].iActionId )
					{
					if ( i == KBtnSettingsIdx )
						{
						if ( TRect( iButtons[ i ].iRect.iTl, KButtonUndoSize ).Contains( aPointerEvent.iPosition ) )
							{
							iButtons[ i ].iSelected = EFalse;
							// button pressed -> start animation
#ifdef USE_ANIMATIONS
							iState = EAnimation;
							iAnimationButtonPressed->StartAnimation( &iButtons[ i ] );
#else
							iObserver->PageAction( this, iButtons[ i ].iActionId );
#endif
							//iMenuVisible = EFalse;

					    	PlaySound( KHermitSoundButtonPressId );

					    	return ETrue;
							}
						}
					else
						{
						iButtons[ i ].iSelected = EFalse;
						// button pressed -> start animation
#ifdef USE_ANIMATIONS
						iState = ELeaving; // because user can do a pointer event on animatedobjects
						iAnimationButtonPressed->StartAnimation( &iButtons[ i ] );
#else
						iObserver->PageAction( this, iButtons[ i ].iActionId );
#endif
						//iMenuVisible = EFalse;

						PlaySound( KHermitSoundButtonPressId );

						return ETrue;
						}
					}
				else
					{
					if ( i == KBtnUndoIdx )
						{
						if ( iGameLose )
							{ /* reset level on game lose */
							//ResetCurrentLevel();
							StartSwitchingLevels();

							PlaySound( KHermitSoundButton2PressId );

							for ( TInt i = 0; i < iButtons.Count(); i++ )
								{
								iButtons[ i ].iSelected = EFalse;
								}

							return ETrue;
							}
						else if ( iGameLevel->UndoPossible() )
							{
							iGameLevel->Undo();

							PlaySound( KHermitSoundButton2PressId );

							sndPlay = ETrue;
							}
						}
					}
				}
			}
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			iButtons[ i ].iSelected = EFalse;
			}
		}

	/* Handling menu animations */
	if ( ( !iMenuVisible ||
			( iAnimationMenuFade->IsAnimationStarted() && iAnimationMenuFade->FadeType() == CHermitAnimationFade::EFadeOut ) )
			&& aPointerEvent.iType == TPointerEvent::EButton1Up &&
			  ( aPointerEvent.iPosition.iY > iLevelRect.iBr.iY ||
				aPointerEvent.iPosition.iY < iLevelRect.iTl.iY ) )
		{
		menuShow = ETrue;

		StartShowingMenu();
		}
	else if ( iMenuVisible && aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		iButtonDownPos = aPointerEvent.iPosition;
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = ETrue;
				return ETrue;
				}
			}
		}
	else if ( iMenuVisible && aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		if ( !sndPlay && !iGameLose )
			{
			menuHide = ETrue;
			}

		StartHidingMenu();
		}

	if ( iState != ESwitchingLevel &&
		 aPointerEvent.iType == TPointerEvent::EButton1Down
#ifdef _SGE_BADA_
		 && aPointerEvent.iModifiers != EModifierLong
#endif
		 )
		{
		iGameLevel->PointerEvent( iLevelRect, aPointerEvent.iPosition );
		}

	if ( menuHide && !iGameLevel->PlaySoundAfterPointerEvent() )
		{
		PlaySound( KHermitSoundMenuOutId );
		}
	else if ( menuShow && !iGameLevel->PlaySoundAfterPointerEvent() )
		{
		PlaySound( KHermitSoundMenuInId );
		}

	return EFalse;
	}

void CHermitGamePageGame::StartSwitchingLevels()
	{
	if ( iGameLose )
		{
		iSwitchingLevelTime = KSwitchingLevelCounterGameLose;
		}
	else
		{
		iSwitchingLevelTime = KSwitchingLevelCounter;
		}
#ifdef USE_ANIMATIONS
	if ( iGameLose )
		{
		if ( iStatusVisible )
			{
			iAnimationStatusFade->StartAnimation( CHermitAnimationFade::EFadeOut );
			}
		}
	else
		{
		iStatusVisible = ETrue;
		iAnimationStatusFade->StartAnimation( CHermitAnimationFade::EFadeIn );
		}

	if ( iMenuVisible )
		{
		iAnimationMenuFade->StartAnimation( CHermitAnimationFade::EFadeOut );
		}
#else
	iStatusVisible = ETrue;
	iMenuVisible = EFalse;
#endif

	iState = ESwitchingLevel;
	iSwitchingLevelPhase = 1;
	iSwitchingLevelCounter = 0;
	iSwitchingLevelSavedTime = iGameTimeSeconds;
	iSwitchingLevelSavedMovesCounter = iGameLevel->MovesCounter();
	//iGameLose = EFalse;
	}

void CHermitGamePageGame::GameFinished( TBool aWin )
	{
    iGameLose = !aWin;

	if ( aWin )
		{
		PlaySound( KHermitSoundEndGameWinId );

		iCommonObjects.GamesStore()->SetLevelCompleted( iGameLevel->Level().LevelId(), iGameTimeSeconds, iGameLevel->MovesCounter() );

		StartSwitchingLevels();
		}
	else
		{
		PlaySound( KHermitSoundEndGameLoseId );

        StartShowingMenu();
		}
	}

void CHermitGamePageGame::GameNextLevelAnimationFinished()
	{
	iState = EIdle;
	}

void CHermitGamePageGame::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	if ( iState == ESwitchingLevel )
		{
		if ( aAnimation == iAnimationStatusFade /*&&
			 iAnimationStatusFade->FadeType() == CHermitAnimationFade::EFadeOut*/ &&
			 iSwitchingLevelPhase == 2 )
			{ // phase 2
			HideMenu();
			iSwitchingLevelCounter = 0;
			//iCommonObjects.LevelProvider()->NextLevel();
			//UpdateCurrentLevel();
			//AnimateNextLevel();
			//iState = EIdle;
			}
		if ( aAnimation == iAnimationMenuFade )
			{
			iMenuVisible = EFalse;
			}
		return;
		}

	CHermitPageBase::AnimationFinished( aAnimation );

	if ( aAnimation == iAnimationButtonPressed )
		{
		//StartHidingMenu();
		HideMenu();
		THermitGuiButton* btn = iAnimationButtonPressed->AnimatedButton();
		if ( btn )
			{
			iObserver->PageAction( this, btn->iActionId );
			}
		}
	else if ( aAnimation == iAnimationStatusFade )
		{
		if ( iAnimationStatusFade->FadeType() == CHermitAnimationFade::EFadeIn )
			{
			}
		else if ( iAnimationStatusFade->FadeType() == CHermitAnimationFade::EFadeOut )
			{
			HideMenu();
			}
		}
#ifdef PROTO_TOUCH_ANIMATIONS
	else
		{
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
		}
#endif
	}

TBool CHermitGamePageGame::AllAnimationsFinished()
	{
	return ( iAnimationButtonPressed->IsAnimationFinished() && iAnimationStatusFade->IsAnimationFinished() );
	}

void CHermitGamePageGame::SaveGameLevelOnClose()
	{
	if ( !iGameLevel->SaveToFile( KHermitGameStateFile, iGameTimeSeconds ) )
		{
#ifdef _SGE_BADA_
		Osp::Io::File::Remove( KHermitGameStateFile );
#else
        iCommonObjects.FileProvider()->Fs().Delete( iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitGameStateFile ) );
#endif
		}
	}

TBool CHermitGamePageGame::RestoreGameLevelOnStart()
	{
	if ( iGameLevel->RestoreFromFile( KHermitGameStateFile, iGameTimeSeconds ) )
		{
		iCommonObjects.LevelProvider()->SetCurrentLevelFromLevelId( iGameLevel->Level().LevelId() );
		CalculateLevelRect();
		PageEnter();
#ifdef _SGE_BADA_
		Osp::Io::File::Remove( KHermitGameStateFile );
#else
		iCommonObjects.FileProvider()->Fs().Delete( iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitGameStateFile ) );
#endif
		return ETrue;
		}

#ifdef _SGE_BADA_
		Osp::Io::File::Remove( KHermitGameStateFile );
#else
        iCommonObjects.FileProvider()->Fs().Delete( iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitGameStateFile ) );
#endif

	return EFalse;
	}

