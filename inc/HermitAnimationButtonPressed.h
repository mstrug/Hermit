/*
 ============================================================================
  Name        : HermitAnimationButtonPressed.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONBUTTONPRESSED_H_
#define HERMITANIMATIONBUTTONPRESSED_H_

#include "HermitAnimationBase.h"
#include "HermitGuiButton.h"

class CGlObjectRect;
class CHermitCommonObjects;


class CHermitAnimationButtonPressed : public CHermitAnimationBase, public MSGEGlDrawableObject
	{

	enum TSide
	{
		ELeft,
		ETop,
		ERight,
		EBottom,
		ESideMax
	};

	enum TState
	{
		EOffsets,
		EStady
	};

public:

	~CHermitAnimationButtonPressed();

	static CHermitAnimationButtonPressed* NewL( MHermitAnimationObserver* aObserver, TSize aSize, CGlObjectRect& aObjectRect, CHermitCommonObjects& aCommonObjects );

	void StartAnimation( THermitGuiButton* aAnimatedButton );

	THermitGuiButton* AnimatedButton() const;

private:

	CHermitAnimationButtonPressed( MHermitAnimationObserver* aObserver, TSize aSize, CGlObjectRect& aObjectRect, CHermitCommonObjects& aCommonObjects );

	void ConstructL();

	void DoAnimationStep();

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

private: // from base class

    void AnimationSequenceChanged();

    void AnimationSequenceLast();

    TBool AnimationUpdate();

private: // data

    THermitGuiButton* iAnimatedButton; // not owned

    CGlObjectRect& iObjectRect; // not owned

    CHermitCommonObjects& iCommonObjects; // not owned

    TFixedArray< TInt, ESideMax > iOffsets;

    TReal iIncrement;

    TSize iSize;

    TState iAnimationState;

	};

#endif /* HERMITANIMATIONBUTTONPRESSED_H_ */
