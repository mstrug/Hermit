/*
 ============================================================================
  Name        : HermitMenuPageMain.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageMain.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitMenuTitle.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitAnimationTitle.h"
#include "HermitAnimationFade.h"
#include "HermitSettings.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <e32def.h>


const TSize KButtonSize( 128, 128 );
const TSize KButtonTexSize( 128, 128 );
const TSize KButtonAboutSize( 64, 64 );

const TInt KBtnGameIdx = 0;
const TInt KBtnTutorialIdx = 1;
const TInt KBtnSettingsIdx = 2;
const TInt KBtnExitIdx = 3;
const TInt KBtnAboutIdx = 4;

const TInt KTitleAniamtionTime = 1000000;	// 1 sec



CHermitMenuPageMain::CHermitMenuPageMain( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
	{
	}

CHermitMenuPageMain::~CHermitMenuPageMain()
	{
	delete iTitle;
	delete iAnimationTitle;
	}

CHermitMenuPageMain* CHermitMenuPageMain::NewL( MHermitPageObserver& aObserver,
												CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageMain* self = new (ELeave) CHermitMenuPageMain( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageMain::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnGameIdx ].iTextureId = KHermitTexMenuBtnGameId;
	iButtons[ KBtnTutorialIdx ].iTextureId = KHermitTexMenuBtnTutorialId;
	iButtons[ KBtnSettingsIdx ].iTextureId = KHermitTexMenuBtnSettingsId;
	iButtons[ KBtnExitIdx ].iTextureId = KHermitTexMenuBtnExitId;
	iButtons[ KBtnAboutIdx ].iTextureId = KHermitTexMenuBtnAboutId;

	iButtons[ KBtnGameIdx ].iActionId = KMenuActionBtnGame;
	iButtons[ KBtnTutorialIdx ].iActionId = KMenuActionBtnTutorial;
	iButtons[ KBtnSettingsIdx ].iActionId = KMenuActionBtnSettings;
	iButtons[ KBtnExitIdx ].iActionId = KMenuActionBtnExit;
	iButtons[ KBtnAboutIdx ].iActionId = KMenuActionBtnAbout;

	iButtons[ KBtnGameIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnTutorialIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnSettingsIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnExitIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnAboutIdx ].iTextureSize = KButtonAboutSize;

	iTitle = CHermitMenuTitle::NewL( iCommonObjects );
	iCommonObjects.SetMenuTitle( iTitle );

	iAnimationTitle = CHermitAnimationTitle::NewL( this, iCommonObjects, iSize );

	SizeChanged( iSize );

	iState = ELoading;
	}

void CHermitMenuPageMain::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	iMarginX = ( iSize.iWidth - 2 * KButtonSize.iWidth ) / 2;
	iMarginY = ( iSize.iHeight - 2 * KButtonSize.iHeight ) / 2 - KButtonSize.iHeight / 5;

	iButtons[ KBtnGameIdx ].iRect = TRect( TPoint( iMarginX, iMarginY + KButtonSize.iHeight ), KButtonSize );
	iButtons[ KBtnTutorialIdx ].iRect = TRect( TPoint( iMarginX + KButtonSize.iWidth, iMarginY + KButtonSize.iHeight ), KButtonSize );
	iButtons[ KBtnSettingsIdx ].iRect = TRect( TPoint( iMarginX, iMarginY ), KButtonSize );
	iButtons[ KBtnExitIdx ].iRect = TRect( TPoint( iMarginX + KButtonSize.iWidth, iMarginY ), KButtonSize );

	iButtons[ KBtnAboutIdx ].iRect = TRect( TPoint( 0, iMarginX / 4 ), KButtonAboutSize );

	TSize elements = iTitle->ElementsCount();
	TSize es = iTitle->ElementsSize();
	TSize titleSize( elements.iWidth * es.iWidth, elements.iHeight * es.iHeight );
	iTitleRect = TRect( TPoint( ( iSize.iWidth - titleSize.iWidth ) / 2, iSize.iHeight - 3 * titleSize.iHeight / 2 ), titleSize );

	iTitle->SetRect( iTitleRect );
	}

TBool CHermitMenuPageMain::IsLeaving() const
	{
	return ( iState == ELeaving );
	}

void CHermitMenuPageMain::PageEnter()
	{
	if ( iState != ELoading )
		{
		CHermitPageBase::PageEnter();
		}
	}

void CHermitMenuPageMain::Draw( const TRect aDrawRect )
	{
	if ( iState == ELoading )
		{
#ifdef LANDSCAPE_TITLE
	glRotatef( 90, 0, 0, 1 );
	glTranslatef( ( iSize.iHeight - iSize.iWidth ) / 2, -iSize.iWidth / 2 - iSize.iHeight / 2, 0 );
#endif

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

		iAnimationTitle->Draw( aDrawRect );
		iAnimationFade->Draw( aDrawRect );
		return;
		}
	if ( iState == ELeaving )
		{ // black
        GlUtils::DisableTexturing();
        GlUtils::DisableTransparency();
        glClearColor( 0, 0, 0, 1 );
        glClear( GL_COLOR_BUFFER_BIT );
		return;
		}

    iTitle->Draw( TRect() );

	// title
	//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );

	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iSize.iHeight - iMarginY ) );
	GlUtils::Scale2D( TSize( iTitleRect.iTl.iX, iMarginY ) );
	iRect->Draw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iTitleRect.iBr.iX, iSize.iHeight - iMarginY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iTitleRect.iBr.iX, iMarginY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnGameIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iTitleRect.iTl.iY - iButtons[ KBtnGameIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iTitleRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iSize.iHeight - iTitleRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom
	glPushMatrix();
	GlUtils::Scale2D( TSize( iSize.iWidth, iButtons[ KBtnAboutIdx ].iRect.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnAboutIdx ].iRect.iTl.iX, iMarginY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnAboutIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iMarginY - iButtons[ KBtnAboutIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnAboutIdx ].iRect.iBr.iX, 0 ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnAboutIdx ].iRect.iBr.iX, iMarginY ) );
	iRect->Redraw();
	glPopMatrix();

	// left
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iMarginY ) );
	GlUtils::Scale2D( TSize( iMarginX, iSize.iHeight - 2 * iMarginY ) );
	iRect->Redraw();
	glPopMatrix();

	// right
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iSize.iWidth - iMarginX, iMarginY ) );
	GlUtils::Scale2D( TSize( iMarginX, iSize.iHeight - 2 * iMarginY ) );
	iRect->Redraw();
	glPopMatrix();
    
    glColor4ub( 255, 255, 255, 255 );
	
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

	iTitle->Draw( TRect() );

	// buttons
	for ( TInt i = 0; i < iButtons.Count(); i++ )
		{
		if ( iButtons[ i ].iSelected ) //&& i != KBtnAboutIdx )
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			//glColor4f( 1, 1, 1, 1 );
			iRect->Redraw();
			glPopMatrix();
			}
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( iButtons[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
		iRect->Redraw();
		glPopMatrix();
		}

	// button pressed animation
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );

	iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSettingsId )->Discard();
	}

TBool CHermitMenuPageMain::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	if ( iState == ELoading )
		{
#ifdef USE_ANIMATIONS
		if ( !iAnimationTitle->IsAnimationStarted() )
			{
			iAnimationTitle->StartAnimation();

			iAnimationFade->RestartAnimation();
			iAnimationFade->StartAnimation( CHermitAnimationFade::EFadeIn );
			}
		else
			{
			iAnimationTitle->Update( aTimeInterval );
			iAnimationFade->Update( aTimeInterval );
			}
#else
		CHermitPageBase::PageEnter();
#endif
		}
	else if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}

	return ETrue;
	}

TBool CHermitMenuPageMain::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState == ELoading )
		{
		iAnimationTitle->StopAnimation();
		CHermitPageBase::PageEnter();
		iObserver->PageAction( this, KMenuActionAnimationTitleFinished );
		return ETrue;
		}
	else if ( iState != EIdle && iState != EEntering )
		{
		return EFalse;
		}

	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
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
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iSelected && iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = EFalse;
				// button pressed -> start animation
#ifdef USE_ANIMATIONS
				iState = EAnimation;
				iAnimationButtonPressed->StartAnimation( &iButtons[ i ] );
#else
				iObserver->PageAction( this, iButtons[ i ].iActionId );
#endif

				PlaySound( KHermitSoundButtonPressId );

				return ETrue;
				}
			}
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			iButtons[ i ].iSelected = EFalse;
			}
		}

	return EFalse;
	}

void CHermitMenuPageMain::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	//iState = EIdle;

	CHermitPageBase::AnimationFinished( aAnimation );

	if ( aAnimation == iAnimationButtonPressed )
		{
		THermitGuiButton* btn = iAnimationButtonPressed->AnimatedButton();
		if ( btn )
			{
			if ( btn->iActionId == KMenuActionBtnExit )
				{
				iState = ELeaving;
				}
			iObserver->PageAction( this, btn->iActionId );
			}
		}
	else if ( aAnimation == iAnimationTitle )
		{
		CHermitPageBase::PageEnter();
		iObserver->PageAction( this, KMenuActionAnimationTitleFinished );
		}
	}

TBool CHermitMenuPageMain::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished() && iAnimationTitle->IsAnimationFinished();
	}

