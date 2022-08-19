/*
 ============================================================================
  Name        : HermitMenuPageAck.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageAck.h"
#include "HermitCommonObjects.h"
#include "HermitMenuTitle.h"
#include "HermitConst.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <e32def.h>


const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KButtonBigSize( 128, 128 );
const TSize KButtonBigTexSize( 128, 128 );

const TSize KTitleSize( 64, 64 );
const TSize KTitleTexSize( 64, 64 );

const TInt KBtnYesIdx = 0;
const TInt KBtnNoIdx = 1;
const TInt KBtnBackIdx = 2;



CHermitMenuPageAck::CHermitMenuPageAck( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
	{
	}

CHermitMenuPageAck::~CHermitMenuPageAck()
	{
	}

CHermitMenuPageAck* CHermitMenuPageAck::NewL( MHermitPageObserver& aObserver,
											  CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageAck* self = new (ELeave) CHermitMenuPageAck( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageAck::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnYesIdx ].iTextureId = KHermitTexMenuBtnYesId;
	iButtons[ KBtnYesIdx ].iActionId = KAckActionBtnYes;
	iButtons[ KBtnYesIdx ].iTextureSize = KButtonBigTexSize;

	iButtons[ KBtnNoIdx ].iTextureId = KHermitTexMenuBtnNoId;
	iButtons[ KBtnNoIdx ].iActionId = KAckActionBtnNo;
	iButtons[ KBtnNoIdx ].iTextureSize = KButtonBigTexSize;

	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnBackIdx ].iActionId = KAckActionBtnBack;
	iButtons[ KBtnBackIdx ].iTextureSize = KButtonTexSize;

	SizeChanged( iSize );

	iState = EIdle;
	}

void CHermitMenuPageAck::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	iButtons[ KBtnYesIdx ].iRect = TRect( TPoint( ( iSize.iWidth - 2 * KButtonBigSize.iWidth ) / 2, iSize.iHeight / 2 - KButtonBigSize.iHeight / 2 ), KButtonBigSize );
	iButtons[ KBtnNoIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonBigSize.iWidth - ( iSize.iWidth - 2 * KButtonBigSize.iWidth ) / 2, iSize.iHeight / 2 - KButtonBigSize.iHeight / 2 ), KButtonBigSize );
	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );
	}

void CHermitMenuPageAck::DrawFills()
	{
	// fills texture
//	iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();

    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );


	// right side of title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( KTitleSize.iWidth, iSize.iHeight - KTitleSize.iHeight ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - KTitleSize.iWidth, KTitleSize.iHeight ) );
	iRect->Draw();
	glPopMatrix();

	// title to buttons
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnYesIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iSize.iHeight - KTitleSize.iHeight - iButtons[ KBtnYesIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// left size of buttons
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnYesIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnYesIdx ].iRect.iTl.iX, iButtons[ KBtnYesIdx ].iRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// right size of buttons
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnNoIdx ].iRect.iBr.iX, iButtons[ KBtnYesIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnNoIdx ].iRect.iBr.iX, iButtons[ KBtnYesIdx ].iRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// back button to buttons
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iButtons[ KBtnYesIdx ].iRect.iTl.iY - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// back button left fill
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();
    
    glColor4ub( 255, 255, 255, 255 );
    
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
	}

void CHermitMenuPageAck::Draw( const TRect aDrawRect )
	{
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
    
	// buttons
	for ( TInt i = 0; i < iButtons.Count(); i++ )
		{
		if ( iButtons[ i ].iSelected )
			{
		    GlUtils::DisableTexturing();
		    GlUtils::DisableTransparency();
            glPushMatrix();
            GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
            GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
            iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
            iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );
            //iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
            //glColor4f( 1, 1, 1, 1 );
            glColor4ub( 255, 0, 0, 255 );
            iRect->Draw();
            glPopMatrix();
            glColor4ub( 255, 255, 255, 255 );
            iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
            iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );    
            GlUtils::EnableTexturing();
            GlUtils::EnableTransparency();
			}
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( iButtons[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
		iRect->Draw();
		glPopMatrix();
		}
    
    DrawFills();

	// title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iSize.iHeight - KTitleSize.iHeight ) );
	GlUtils::Scale2D( KTitleTexSize );
	iCommonObjects.TextureManager()->Texture( KHermitTexAckTitleId )->Use();
	iRect->Draw();
	glPopMatrix();

	// button pressed animation
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );
	}

TBool CHermitMenuPageAck::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}

	return ETrue;
	}

TBool CHermitMenuPageAck::PointerEvent( const TPointerEvent &aPointerEvent )
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

void CHermitMenuPageAck::AnimationFinished( CHermitAnimationBase* aAnimation )
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

TBool CHermitMenuPageAck::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished();
	}

