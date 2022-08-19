/*
 ============================================================================
  Name        : HermitMenuPageTutorial.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUPAGETUTORIAL_H_
#define HERMITMENUPAGETUTORIAL_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"


class CHermitGameLevel;
class CHermitAnimationFade;


const TInt KTutorialActionBtnBack = 0x0301;

const TInt KMenuPageTutorialBtnCount = 1;



class CHermitMenuPageTutorial : public CHermitPageBase
	{

	enum TTutorialLevelState
		{
		ELevelLoading,
		ELevelIntro,
		ELevelIndicator,
		ELevelHintAndIndicator,
		ELevelHint,
		ELevelAfterHint,
		ELevelOutro,
		ELevelUnloading,
		ELevelStateMax
		};

public:

	virtual ~CHermitMenuPageTutorial();

	static CHermitMenuPageTutorial* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize );

	void ResetState();

private:

	CHermitMenuPageTutorial( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

	void CalculateLevelRect();

	void SetLevelL( TInt aLevel );

	void SetIndicator( TPoint aPos );

	void DrawFills();

public: // from base class

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

private: // data

	CHermitGameLevel *iGameLevel; // owned

	CHermitAnimationFade *iAnimationFadeLevel; // owned

	CHermitAnimationFade *iAnimationFadeIndicator; // owned

	CHermitAnimationFade *iAnimationFadeIndicator2; // owned

	TFixedArray< THermitGuiButton, KMenuPageTutorialBtnCount > iButtons;

	TInt iCurrentLevel;

	TRect iLevelRect;

	// level rect - margins
	TRect iBoardRect;

	TInt iCurrentMove;

	TBool iDrawIndicator;

	TRect iIndicatorRect;

	TTutorialLevelState iLevelState;

	TInt iLevelStateTimeCounter;

	TBool iNextLevelAnimation;

	};

#endif /* HERMITMENUPAGETUTORIAL_H_ */
