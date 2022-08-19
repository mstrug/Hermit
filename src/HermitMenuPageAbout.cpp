/*
 ============================================================================
  Name        : HermitMenuPageAbout.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageAbout.h"
#include "HermitCommonObjects.h"
#include "HermitMenuTitle.h"
#include "HermitConst.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitCompilationSettings.h"
#include "GlFont.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <e32def.h>


const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KLogoSize( 256, 64 );
const TSize KLogoTexSize( 256, 64 );

const TSize KTextSize( 512, 256 );
const TSize KTextTexSize( 512, 256 );

const TInt KBtnBackIdx = 0;

const TInt KDrawOffset = 32;



CHermitMenuPageAbout::CHermitMenuPageAbout( MHermitPageObserver& aObserver,
											CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
    {
    }

CHermitMenuPageAbout::~CHermitMenuPageAbout()
    {
    }

CHermitMenuPageAbout* CHermitMenuPageAbout::NewL( MHermitPageObserver& aObserver,
													    CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageAbout* self = new (ELeave) CHermitMenuPageAbout( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageAbout::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnBackIdx ].iActionId = KAboutActionBtnBack;

	SizeChanged( iSize );

	iState = EIdle;
	}

void CHermitMenuPageAbout::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );

	iTextRect = TRect( TPoint( ( iSize.iWidth - KTextSize.iWidth ) / 2, iSize.iHeight / 2 - KTextSize.iHeight ), KTextSize );

	iLogoRect = TRect( TPoint( ( iSize.iWidth - KLogoSize.iWidth ) / 2, iSize.iHeight / 2 + KLogoSize.iHeight - KDrawOffset ), KLogoSize );

    CHermitMenuTitle* title = iCommonObjects.MenuTitle();
    TRect r = title->Rect();

    CGlFont *f1 = iCommonObjects.Font1();
	TInt w = f1->GetTextWidth( KHermitVersion );
	iVersionRect = TRect( TPoint( ( iSize.iWidth - w ) / 2, iLogoRect.iBr.iY + ( r.iTl.iY - iLogoRect.iBr.iY ) / 2 - f1->GetMaxHeight() / 2 ), TSize( w, f1->LetterSize().iHeight ) );
	}

void CHermitMenuPageAbout::DrawAbout()
	{
	CHermitMenuTitle* title = iCommonObjects.MenuTitle();
	TRect r = title->Rect();

    iCommonObjects.MenuTitle()->Draw( TRect() );

	//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
    // version 
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );
    glColor4ub( 0, 0, 0, 255 );

	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, r.iTl.iY ) );
	GlUtils::Scale2D( TSize( r.iTl.iX, r.iBr.iY - r.iTl.iY ) );
	iRect->Draw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( r.iBr.iX, r.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - r.iBr.iX, r.iBr.iY - r.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, r.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iSize.iHeight - r.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();
	}

void CHermitMenuPageAbout::Draw( const TRect aDrawRect )
	{
	// fills texture
//	iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );
    glColor4ub( 0, 0, 0, 255 );

	// title to version fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iVersionRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iCommonObjects.MenuTitle()->Rect().iTl.iY - iVersionRect.iBr.iY ) );
	iRect->Draw();
	glPopMatrix();

	// version to text fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iTextRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iVersionRect.iTl.iY - iTextRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// version left fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iVersionRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iVersionRect.iTl.iX, iVersionRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// version right fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iVersionRect.iBr.iX, iVersionRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iVersionRect.iBr.iX, iVersionRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// text to back button fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iTextRect.iTl.iY - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// back button left fill
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();
	
	
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

    glColor4ub( 255, 255, 255, 255 );
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );


	// logo
	glPushMatrix();
	GlUtils::Translate2D( iLogoRect.iTl );
	GlUtils::Scale2D( KLogoTexSize );
	iCommonObjects.TextureManager()->Texture( KHermitTexAboutLogoId )->Use();
	iRect->Draw();
	glPopMatrix();

	// text
	glPushMatrix();
	GlUtils::Translate2D( iTextRect.iTl );
	GlUtils::Scale2D( KTextTexSize );
	iCommonObjects.TextureManager()->Texture( KHermitTexAboutTextId )->Use();
	iRect->Redraw();
	glPopMatrix();

    // buttons
    for ( TInt i = 0; i < iButtons.Count(); i++ )
        {
        if ( iButtons[ i ].iSelected )
            {
            glPushMatrix();
            GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
            GlUtils::Scale2D( KButtonSize );
            iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
            iRect->Redraw();
            glPopMatrix();
            }
        glPushMatrix();
        GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
        GlUtils::Scale2D( KButtonTexSize );
        iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
        iRect->Redraw();
        glPopMatrix();
        }
		
    // version
    CGlFont *f1 = iCommonObjects.Font1();
    f1->DrawText( iVersionRect.iTl, KHermitVersion );

    // title
    DrawAbout();
  
	// button pressed animation
	if ( iState == EAnimation )
		{
	    GlUtils::EnableTexturing();
	    GlUtils::EnableTransparency();
    glColor4ub( 255, 255, 255, 255 );
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );
	    
		iAnimationButtonPressed->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );
	}

TBool CHermitMenuPageAbout::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}
	return ETrue;
	}

TBool CHermitMenuPageAbout::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState != EIdle && iState != EEntering )
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
			if ( iButtons[ i ].iSelected && iButtons[ i ].iActionId &&
				 iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
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

void CHermitMenuPageAbout::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	//iState = EIdle;

	CHermitPageBase::AnimationFinished( aAnimation );

	if ( aAnimation == iAnimationButtonPressed )
		{
		THermitGuiButton* btn = iAnimationButtonPressed->AnimatedButton();
		if ( btn )
			{
			iObserver->PageAction( this, btn->iActionId );
			}
		}
	}

TBool CHermitMenuPageAbout::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished();
	}

