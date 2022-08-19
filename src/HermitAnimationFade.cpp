/*
 ============================================================================
  Name        : HermitAnimationFade.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitAnimationFade.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>


const TInt KAnimationStepTime = 50000; // 50 ms

const TReal KAnimationOffsetStepIncrement = 0;
const TReal KAnimationOffsetStepMin = 15;
const TReal KAnimationOffsetStepMax = 30;




CHermitAnimationFade::CHermitAnimationFade( MHermitAnimationObserver* aObserver, CGlObjectRect& aObjectRect,
											CHermitCommonObjects& aCommonObjects )
	: CHermitAnimationBase( aObserver ), iObjectRect( &aObjectRect ), iCommonObjects( aCommonObjects ),
	  iOgjectRectOwned( EFalse ), iStartIncrement( (TInt)KAnimationOffsetStepMin ),
	  iAnimationStepTime( KAnimationStepTime ), iColor( 0 )
	{
	}

CHermitAnimationFade::CHermitAnimationFade( MHermitAnimationObserver* aObserver,
											CHermitCommonObjects& aCommonObjects )
	: CHermitAnimationBase( aObserver ), iCommonObjects( aCommonObjects ), iOgjectRectOwned( ETrue ),
	  iStartIncrement( (TInt)KAnimationOffsetStepMin ), iAnimationStepTime( KAnimationStepTime ), iColor( 0 )
	{
	}

CHermitAnimationFade::~CHermitAnimationFade()
	{
	if ( iOgjectRectOwned )
		{
		delete iObjectRect;
		}
	}

CHermitAnimationFade* CHermitAnimationFade::NewL( MHermitAnimationObserver* aObserver,
												  CGlObjectRect& aObjectRect,
												  CHermitCommonObjects& aCommonObjects )
	{
	CHermitAnimationFade* self = new (ELeave) CHermitAnimationFade( aObserver, aObjectRect, aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

CHermitAnimationFade* CHermitAnimationFade::NewL( MHermitAnimationObserver* aObserver,
												  CHermitCommonObjects& aCommonObjects )
	{
	CHermitAnimationFade* self = new (ELeave) CHermitAnimationFade( aObserver, aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitAnimationFade::ConstructL()
	{
	CHermitAnimationBase::BaseConstructL( 1 );

	if ( iOgjectRectOwned )
		{
		iObjectRect = CGlObjectRect::NewL();
		iObjectRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
		iObjectRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );
		}
	}

void CHermitAnimationFade::StartAnimation( TFadeType aType )
	{
	if ( iAnimationStarted )
		{
		if ( aType == iFadeType )
			{ // not restarting the same animation
			return;
			}
		else
			{
			iFadeType = aType;
			return;
			}
		}

	DoStartAnimation();
	iFadeType = aType;

	if ( aType == EFadeIn )
		{
		iIncrement = iStartIncrement;
		iTransparencyValue = 0xFF;
		}
	else // EFadeOut
		{
		iIncrement = iStartIncrement;
		iTransparencyValue = 0;
		}

	iTimeCoutner = 0;

	iDifferentialAnimation = EFalse;

	UpdateColor();
	}

void CHermitAnimationFade::StartAnimationDifferential( TFadeType aFadeRect1Type )
	{
	iDifferentialAnimation = ETrue;

	if ( iAnimationStarted )
		{
		if ( aFadeRect1Type == iFadeType )
			{ // not restarting the same animation
			return;
			}
		else
			{
			iFadeType = aFadeRect1Type;
			return;
			}
		}

	DoStartAnimation();
	iFadeType = aFadeRect1Type;

	if ( aFadeRect1Type == EFadeIn )
		{
		iIncrement = iStartIncrement;
		iTransparencyValue = 0xFF;
		}
	else // EFadeOut
		{
		iIncrement = iStartIncrement;
		iTransparencyValue = 0;
		}

	iTimeCoutner = 0;

	UpdateColor();
	}

void CHermitAnimationFade::SetFadeRect( TRect aFadeRect )
	{
	iEnabledFadeRect = ETrue;
	iFadeRect = aFadeRect;
	}

void CHermitAnimationFade::SetFadeRect2( TRect aFadeRect )
	{
	iEnabledFadeRect2 = ETrue;
	iFadeRect2 = aFadeRect;
	}

TRect CHermitAnimationFade::FadeRect() const
	{
	return iFadeRect;
	}

TRect CHermitAnimationFade::FadeRect2() const
	{
	return iFadeRect2;
	}

void CHermitAnimationFade::EnableFadeRects( TBool aEnableFadeRect1, TBool aEnableFadeRect2 )
	{
	iEnabledFadeRect = aEnableFadeRect1;
	iEnabledFadeRect2 = aEnableFadeRect2;
	}

CHermitAnimationFade::TFadeType CHermitAnimationFade::FadeType() const
	{
	return iFadeType;
	}

void CHermitAnimationFade::RestartAnimation()
	{
	CHermitAnimationBase::RestartAnimation();

	iFadeType = EFadeIn;
	iIncrement = iStartIncrement;
	iTransparencyValue = 0xFF;
	UpdateColor();
	}

void CHermitAnimationFade::SetStartIncrement( TInt aValue )
	{
	iStartIncrement = aValue;
	}

void CHermitAnimationFade::SetAnimationStepTime( TInt aValue )
	{
	iAnimationStepTime = aValue;
	}

CGlObjectRect* CHermitAnimationFade::ObjectRect() const
	{
	return iObjectRect;
	}

void CHermitAnimationFade::UseExternalTextures( TBool aUse )
	{
	iExternalTextures = aUse;
	iObjectRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, aUse );
	}

void CHermitAnimationFade::SetFadeColor( TRgb aColor )
	{
	iColor = aColor;
	}

void CHermitAnimationFade::DoAnimationStep()
	{
	if ( iFadeType == EFadeIn )
		{
		if ( iTransparencyValue <= 0 )
			{
			DoFinishAnimation();
			return;
			}

//		if ( iIncrement < KAnimationOffsetStepMax )
//			{
//			iIncrement += KAnimationOffsetStepIncrement;
//			}

		iTransparencyValue -= (TInt)iIncrement;
		}
	else // EFadeOut
		{
		if ( iTransparencyValue >= 0xFF )
			{
			DoFinishAnimation();
			return;
			}

//		if ( iIncrement > KAnimationOffsetStepMin )
//			{
//			iIncrement -= KAnimationOffsetStepIncrement;
//			}

		iTransparencyValue += (TInt)iIncrement;
		}

	UpdateColor();
	}

void CHermitAnimationFade::UpdateColor()
	{
//	const TInt KVertexCount = 4;
//	const TInt KVertexColorSize = 4;

	if ( iTransparencyValue < 0 )
		{
		iTransparencyValue = 0;
		}
	else if ( iTransparencyValue > 0xFF )
		{
		iTransparencyValue = 0xFF;
		}

//	GLubyte r = iColor.Red();
//	GLubyte g = iColor.Green();
//	GLubyte b = iColor.Blue();
//
//	GLubyte colors[ KVertexCount * KVertexColorSize ] =
//	    {
//	    r, g, b, iTransparencyValue,
//	    r, g, b, iTransparencyValue,
//	    r, g, b, iTransparencyValue,
//	    r, g, b, iTransparencyValue
//	    };
//	iObjectRect->Primitive().VertexArray( 0 )->SetColorData( (TAny*) colors, KVertexCount * KVertexColorSize * sizeof( GLubyte ) );

	}

void CHermitAnimationFade::Draw( const TRect /*aDrawRect*/ )
	{
	if ( !iAnimationStarted )
		{
		return;
		}

	if ( !iExternalTextures )
		{
		GlUtils::DisableTexturing();
		}
	GlUtils::EnableTransparency();

	glColor4ub( iColor.Red(), iColor.Green(), iColor.Blue(), iTransparencyValue );

	if ( iEnabledFadeRect )
		{
		glPushMatrix();
		GlUtils::Translate2D( iFadeRect.iTl );
		GlUtils::Scale2D( iFadeRect.Size() );
		ApplyScaleAnimation();
		iObjectRect->Draw();
		glPopMatrix();
		}

	if ( iEnabledFadeRect2 )
		{
		if ( iDifferentialAnimation )
			{
			TInt tmp = iTransparencyValue;
			iTransparencyValue = 0xFF - iTransparencyValue;
			UpdateColor();
			iTransparencyValue = tmp;
			}
		glPushMatrix();
		GlUtils::Translate2D( iFadeRect2.iTl );
		GlUtils::Scale2D( iFadeRect2.Size() );
		ApplyScaleAnimation();
		iObjectRect->Draw();
		glPopMatrix();
		}

	glColor4ub( 255, 255, 255, 255 );
	if ( !iExternalTextures )
		{
		GlUtils::EnableTexturing();
		}
	}

void CHermitAnimationFade::AnimationSequenceChanged()
	{

	}

void CHermitAnimationFade::AnimationSequenceLast()
	{

	}

TBool CHermitAnimationFade::AnimationUpdate()
	{
	if ( iTimeCoutner > iAnimationStepTime )
		{
#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
		iTimeCoutner -= iAnimationStepTime;
#else
		iTimeCoutner = 0;
#endif

		DoAnimationStep();

		return ETrue;
		}

	return EFalse;
	}



