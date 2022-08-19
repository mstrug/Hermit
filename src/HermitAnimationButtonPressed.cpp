/*
 ============================================================================
  Name        : HermitAnimationButtonPressed.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitAnimationButtonPressed.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>


const TInt KAnimationStepTime = 10000; // 10 ms

const TReal KAnimationOffsetStep = 20;
const TReal KAnimationOffsetStepIncrement = 7;
const TReal KAnimationOffsetStepMax = 91;

const TInt KAnimationStedyTime = 20000; // 20 ms


CHermitAnimationButtonPressed::CHermitAnimationButtonPressed( MHermitAnimationObserver* aObserver,
															  TSize aSize,
														      CGlObjectRect& aObjectRect,
														      CHermitCommonObjects& aCommonObjects )
	: CHermitAnimationBase( aObserver ), iObjectRect( aObjectRect ), iCommonObjects( aCommonObjects ), iSize( aSize )
	{
	}

CHermitAnimationButtonPressed::~CHermitAnimationButtonPressed()
	{
	}

CHermitAnimationButtonPressed* CHermitAnimationButtonPressed::NewL( MHermitAnimationObserver* aObserver,
																    TSize aSize,
																	CGlObjectRect& aObjectRect,
																    CHermitCommonObjects& aCommonObjects )
	{
	CHermitAnimationButtonPressed* self = new (ELeave) CHermitAnimationButtonPressed( aObserver, aSize, aObjectRect, aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitAnimationButtonPressed::ConstructL()
	{
	CHermitAnimationBase::BaseConstructL( 1 );
	}

void CHermitAnimationButtonPressed::StartAnimation( THermitGuiButton* aAnimatedButton )
	{
	DoStartAnimation();
	iAnimatedButton = aAnimatedButton;

	iIncrement = KAnimationOffsetStep;
	iAnimationState = EOffsets;
	iTimeCoutner = 0;

	iOffsets[ ELeft ] = 0;
	iOffsets[ ETop ] = 0;
	iOffsets[ ERight ] = 0;
	iOffsets[ EBottom ] = 0;
	}

THermitGuiButton* CHermitAnimationButtonPressed::AnimatedButton() const
	{
	return iAnimatedButton;
	}

void CHermitAnimationButtonPressed::DoAnimationStep()
	{
	if ( !iAnimatedButton )
		{
		return;
		}

	if ( iAnimatedButton->iRect.iTl.iX - iOffsets[ ELeft ] / 2 <= 0 &&
		 iAnimatedButton->iRect.iBr.iX + iOffsets[ ERight ] / 2 >= iSize.iWidth &&
		 iAnimatedButton->iRect.iTl.iY - iOffsets[ EBottom ] / 2 <= 0 &&
		 iAnimatedButton->iRect.iBr.iY + iOffsets[ ETop ] / 2 >= iSize.iHeight )
		{
		//iAnimationState = EStady;
		DoFinishAnimation();
		}

	iOffsets[ ELeft ] += (TInt)iIncrement;
	iOffsets[ ETop ] += (TInt)iIncrement;
	iOffsets[ ERight ] += (TInt)iIncrement;
	iOffsets[ EBottom ] += (TInt)iIncrement;

	if ( iIncrement <= KAnimationOffsetStepMax )
		{
		iIncrement += KAnimationOffsetStepIncrement;
		}
	}

void CHermitAnimationButtonPressed::Draw( const TRect /*aDrawRect*/ )
	{
	if ( !iAnimatedButton || !iAnimationStarted )
		{
		return;
		}

	TRect r = iAnimatedButton->iRect;

	for ( TInt i = 1; i <= 2; i++ )
		{
		if ( i == 1 )
			{ // first draw transparented image
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnPressAnimId )->Use();
			}
		else
			{ // second draw black image
			iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
			}

		// left
		glPushMatrix();
		GlUtils::Translate2D( TPoint( r.iTl.iX - iOffsets[ ELeft ] / i, r.iTl.iY - iOffsets[ EBottom ] / i ) );
		GlUtils::Scale2D( TSize( iOffsets[ ELeft ] / i, r.Height() + iOffsets[ EBottom ] / i + iOffsets[ ETop ] / i ) );
		iObjectRect.Draw();
		glPopMatrix();

		// right
		glPushMatrix();
		GlUtils::Translate2D( TPoint( r.iBr.iX, r.iTl.iY - iOffsets[ EBottom ] / i ) );
		GlUtils::Scale2D( TSize( iOffsets[ ERight ] / i, r.Height() + iOffsets[ EBottom ] / i + iOffsets[ ETop ] / i ) );
		iObjectRect.Draw();
		glPopMatrix();

		// top
		glPushMatrix();
		GlUtils::Translate2D( TPoint( r.iTl.iX - iOffsets[ ELeft ] / i, r.iBr.iY ) );
		GlUtils::Scale2D( TSize( r.Width() + iOffsets[ ELeft ] / i + iOffsets[ ERight ] / i, iOffsets[ ETop ] / i ) );
		iObjectRect.Draw();
		glPopMatrix();

		// bottom
		glPushMatrix();
		GlUtils::Translate2D( TPoint( r.iTl.iX - iOffsets[ ELeft ] / i, r.iTl.iY - iOffsets[ EBottom ] / i ) );
		GlUtils::Scale2D( TSize( r.Width() + iOffsets[ ELeft ] / i + iOffsets[ ERight ] / i, iOffsets[ EBottom ] / i ) );
		iObjectRect.Draw();
		glPopMatrix();
		}

	}

void CHermitAnimationButtonPressed::AnimationSequenceChanged()
	{

	}

void CHermitAnimationButtonPressed::AnimationSequenceLast()
	{

	}

TBool CHermitAnimationButtonPressed::AnimationUpdate()
	{
	if ( iAnimationState == EOffsets )
		{
		if ( iTimeCoutner > KAnimationStepTime )
			{
			iTimeCoutner -= KAnimationStepTime;

			DoAnimationStep();

			return ETrue;
			}
		}
	else // EStady
		{
		if ( iTimeCoutner > KAnimationStedyTime )
			{
			iTimeCoutner -= KAnimationStedyTime;

			DoFinishAnimation();

			return ETrue;
			}
		}

	return EFalse;
	}


