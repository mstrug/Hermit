/*
 ============================================================================
  Name        : HermitAnimationTexZoom.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#include "HermitAnimationTexZoom.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>


const TInt KAnimationStepTime = 50000; // 50 ms



CHermitAnimationTexZoom::CHermitAnimationTexZoom( MHermitAnimationObserver* aObserver,
												  CHermitCommonObjects& aCommonObjects )
	: CHermitAnimationBase( aObserver ), iCommonObjects( aCommonObjects ), iAnimationStepTime( KAnimationStepTime ),
	  iScale( 1 ), iScaleMin( 0.5 ), iScaleMax( 1.5 ), iScaleStep( 0.1 )
	{
	}

CHermitAnimationTexZoom::~CHermitAnimationTexZoom()
	{
	delete iObjectRect;
	}
CHermitAnimationTexZoom* CHermitAnimationTexZoom::NewL( MHermitAnimationObserver* aObserver,
													    CHermitCommonObjects& aCommonObjects )
	{
	CHermitAnimationTexZoom* self = new (ELeave) CHermitAnimationTexZoom( aObserver, aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitAnimationTexZoom::ConstructL()
	{
	CHermitAnimationBase::BaseConstructL( 1 );

	iObjectRect = CGlObjectRect::NewL();
	}

void CHermitAnimationTexZoom::StartAnimation( TZoomType aType )
	{
	if ( iAnimationStarted )
		{
		if ( aType == iZoomType )
			{
			}
		else
			{
			iZoomType = aType;
			}

		if ( aType == EZoomIn )
			{
			iScale = iScaleMax;
			}
		else // EZoomOut
			{
			iScale = iScaleMin;
			}
		return;
		}

	DoStartAnimation();
	iZoomType = aType;

	if ( aType == EZoomIn )
		{
		iScale = iScaleMax;
		}
	else // EZoomOut
		{
		iScale = iScaleMin;
		}

	iTimeCoutner = 0;
	}

CHermitAnimationTexZoom::TZoomType CHermitAnimationTexZoom::ZoomType() const
	{
	return iZoomType;
	}

void CHermitAnimationTexZoom::SetAnimationStepTime( TInt aTimeMS )
	{
	iAnimationStepTime = aTimeMS;
	}

void CHermitAnimationTexZoom::SetParameters( GLfloat aScaleMin, GLfloat aScaleMax, GLfloat aScaleStep )
	{
	iScaleMin = aScaleMin;
	iScaleMax = aScaleMax;
	iScaleStep = aScaleStep;

	if ( iScaleStep < 0 )
		{
		iScaleStep *= -1;
		}
	if ( iScaleMin > iScaleMax )
		{
		GLfloat tmp = iScaleMin;
		iScaleMin = iScaleMax;
		iScaleMax = tmp;
		}
	}

void CHermitAnimationTexZoom::SetScale( GLfloat aScale )
	{
	iScale = aScale;
	}

void CHermitAnimationTexZoom::SetBouncing( TBool aBouncing )
	{
	iBouncing = aBouncing;
	}

void CHermitAnimationTexZoom::RestartAnimation()
	{
	CHermitAnimationBase::RestartAnimation();

	iZoomType = EZoomIn;
	}

void CHermitAnimationTexZoom::DoAnimationStep()
	{
	if ( iZoomType == EZoomIn )
		{
		iScale -= iScaleStep;

		if ( iScale <= iScaleMin )
			{
			iScale = iScaleMin;
			if ( iBouncing )
				{
				iZoomType = EZoomOut;
				}
			else
				{
				DoFinishAnimation();
				}
			}
		}
	else // EZoomOut
		{
		iScale += iScaleStep;

		if ( iScale >= iScaleMax )
			{
			iScale = iScaleMax;
			if ( iBouncing )
				{
				iZoomType = EZoomIn;
				}
			else
				{
				DoFinishAnimation();
				}
			}
		}
	}

void CHermitAnimationTexZoom::Draw( const TRect /*aDrawRect*/ )
	{
	if ( !iAnimationStarted )
		{
		return;
		}

	glMatrixMode( GL_TEXTURE );
	glPushMatrix();
	glLoadIdentity();
	glTranslatef( ( 1 - iScale ) / 2, ( 1 - iScale ) / 2, 0 );
	glScalef( iScale, iScale, 0 );
	glMatrixMode( GL_MODELVIEW );
	iObjectRect->Draw();
	glMatrixMode( GL_TEXTURE );
	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	}

void CHermitAnimationTexZoom::AnimationSequenceChanged()
	{
	}

void CHermitAnimationTexZoom::AnimationSequenceLast()
	{
	}

TBool CHermitAnimationTexZoom::AnimationUpdate()
	{
	if ( iTimeCoutner > iAnimationStepTime )
		{
		iTimeCoutner = 0;

		DoAnimationStep();

		return ETrue;
		}

	return EFalse;
	}


