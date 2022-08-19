/*
 ============================================================================
  Name        : HermitPageBase.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitPageBase.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitCommonObjects.h"
#include "HermitAnimationFade.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlObjectRect.h>
#include <SGEAudio/SGEAudioProvider.h>


CHermitPageBase::CHermitPageBase( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
								  TSize aSize )
	: iObserver( &aObserver ), iCommonObjects( aCommonObjects ), iSize( aSize )
	{
	}

CHermitPageBase::~CHermitPageBase()
	{
	delete iRect;
	delete iAnimationButtonPressed;
	delete iAnimationFade;
	}

void CHermitPageBase::BaseConstructL()
	{
	iRect = CGlObjectRect::NewL();

	iAnimationButtonPressed = CHermitAnimationButtonPressed::NewL( this, iSize, *iRect, iCommonObjects );

	iAnimationFade = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationFade->SetFadeRect( iSize );
	iAnimationFade->SetStartIncrement( 20 );
	iAnimationFade->SetAnimationStepTime( 30000 );
	}

TBool CHermitPageBase::AllAnimationsFinished()
	{
	return ETrue;
	}

void CHermitPageBase::PlaySound( TUint aSampleId )
	{
#ifdef USE_AUDIO
	iCommonObjects.AudioProvider()->Play( aSampleId, ESGEAudioRepeatNone );
#endif
	}

void CHermitPageBase::PageEnter()
	{
#ifdef USE_ANIMATIONS
	iState = EEntering;
	iAnimationFade->RestartAnimation();
	iAnimationFade->StartAnimation( CHermitAnimationFade::EFadeIn );
#else
	iState = EIdle;
#endif
	}

void CHermitPageBase::PageLeave()
	{
#ifdef USE_ANIMATIONS
	iState = ELeaving;
	iAnimationFade->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
	iState = EIdle;
#endif
	}

TBool CHermitPageBase::IsEntering() const
	{
	return ( iState == EEntering );
	}

void CHermitPageBase::Draw( const TRect aDrawRect )
	{
#ifdef USE_ANIMATIONS
	if ( iState == EEntering || iState == EAnimation || iState == ELeaving )
		{
		iAnimationFade->Draw( aDrawRect );
		}
#endif
	}

TBool CHermitPageBase::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
#ifdef USE_ANIMATIONS
	if ( iState == EEntering || iState == EAnimation || iState == ELeaving )
		{
		return iAnimationFade->Update( aTimeInterval );
		}
#endif
	return ETrue;
	}

TBool CHermitPageBase::PointerEvent( const TPointerEvent& aPointerEvent )
	{
	return EFalse;
	}

void CHermitPageBase::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	if ( aAnimation == iAnimationFade && AllAnimationsFinished() )
		{
		iState = EIdle;
		}
	}




