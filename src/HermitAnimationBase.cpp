/*
 ============================================================================
  Name        : HermitAnimationBase.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitAnimationBase.h"
#include "HermitCompilationSettings.h"



CHermitAnimationBase::CHermitAnimationBase( MHermitAnimationObserver* aObserver )
	: iObserver( aObserver ), iAnimationStarted( EFalse ), iScale( 1 )
	{
	}

CHermitAnimationBase::~CHermitAnimationBase()
	{
	iSequencesTimes.Close();
	}

void CHermitAnimationBase::BaseConstructL( TInt aSequenceCount )
	{
	for ( TInt i = 0; i < aSequenceCount; i++ )
		{
		iSequencesTimes.AppendL( 0 );
		}
	}

void CHermitAnimationBase::DoStartAnimation()
	{
	RestartAnimation();
	iAnimationStarted = ETrue;
	}

void CHermitAnimationBase::DoFinishAnimation()
	{
	iAnimationStarted = EFalse;
	iObserver->AnimationFinished( this );
	}

void CHermitAnimationBase::ApplyScaleAnimation()
	{
	if ( iScaleStepTime > 0 )
		{
		glTranslatef( 0.5, 0.5, 0 );
		glTranslatef( - iScale / 2, - iScale / 2, 0 );
		glScalef( iScale, iScale, 0 );
		}
	}

void CHermitAnimationBase::SetObserver( MHermitAnimationObserver* aObserver )
	{
	iObserver = aObserver;
	}

void CHermitAnimationBase::SetSequenceTime( TInt aSequenceNumber, TInt aMicrosecondsTime )
	{
	iSequencesTimes[ aSequenceNumber ] = aMicrosecondsTime;
	}

TBool CHermitAnimationBase::IsLastSequence() const
	{
	return iLastSequence;
	}

TBool CHermitAnimationBase::IsAnimationFinished() const
	{
#ifdef USE_ANIMATIONS
	return !iAnimationStarted;
#else
	return ETrue;
#endif
	}

TBool CHermitAnimationBase::IsAnimationStarted() const
	{
#ifdef USE_ANIMATIONS
	return iAnimationStarted;
#else
	return EFalse;
#endif
	}

TInt CHermitAnimationBase::CurrentSequence() const
	{
	return iCurrentSequence;
	}

TInt CHermitAnimationBase::SequencesCount() const
	{
	return iSequencesTimes.Count();
	}

void CHermitAnimationBase::RestartAnimation()
	{
	iLastSequence = EFalse;
	iCurrentSequence = 0;
	iSequenceTimeCounter = 0;
	iTimeCoutner = 0;
	iSequencesDirection = EDirForward;
	iAnimationStarted = EFalse;

//	iScale = 1;
	iScaleStepTimeCounter = 0;
	}

void CHermitAnimationBase::ReverseAnimation()
	{
	iSequencesDirection = ( iSequencesDirection == EDirForward ? EDirBackward : EDirForward );
	}

void CHermitAnimationBase::EnableScaleAnimation( TInt aStepTime, GLfloat aScaleStep )
	{
	iScaleStepTime = aStepTime;
	iScaleStepTimeCounter = 0;
	iScaleStep = aScaleStep;
	//iScale = 1;
	}

void CHermitAnimationBase::DisableScaleAnimation()
	{
	iScaleStepTime = 0;
	iScaleStepTimeCounter = 0;
	iScaleStep = 0;
	iScale = 1;
	}

void CHermitAnimationBase::ResetScale()
	{
	iScale = 1;
	}

TBool CHermitAnimationBase::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	if ( !iAnimationStarted )
		{
		return EFalse;
		}

	iSequenceTimeCounter += aTimeInterval.Int();
	if ( iSequenceTimeCounter >= iSequencesTimes[ iCurrentSequence ] )
		{
		iSequenceTimeCounter -= iSequencesTimes[ iCurrentSequence ];

		if ( iSequencesDirection == EDirForward && iCurrentSequence < iSequencesTimes.Count() - 1 )
			{
			iCurrentSequence++;
			AnimationSequenceChanged();
			}
		else if ( iSequencesDirection == EDirBackward && iCurrentSequence > 0 )
			{
			iCurrentSequence--;
			AnimationSequenceChanged();
			}
		else
			{
			iLastSequence = ETrue;
			AnimationSequenceLast();
			}
		}

	iScaleStepTimeCounter += aTimeInterval.Int();
	if ( iScaleStepTimeCounter >= iScaleStepTime )
		{
		iScaleStepTimeCounter -= iScaleStepTime;
		iScale += iScaleStep;
		}

	iTimeCoutner += aTimeInterval.Int();
	return AnimationUpdate();
	}



