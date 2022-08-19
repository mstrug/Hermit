/*
 ============================================================================
  Name        : HermitMenuPageGame.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUPAGEGAME_H_
#define HERMITMENUPAGEGAME_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"
#include "HermitGuiImage.h"
#include <SGESensors/SGESensorShake.h>
#ifdef _SGE_BADA_
#include <FUix.h>
#include <FSystem.h>
#endif


class CHermitLevel;
class CHermitAnimationArrowButton;
class CGlFont;


const TInt KGameActionBtnBack = 0x0401;
const TInt KGameActionBtnPlay = 0x0402;

const TInt KMenuPageGameBtnCount = 7;
const TInt KMenuPageGameImgCount = 1;
const TInt KMenuPageGameAnimBtnCount = 4;



class CHermitMenuPageGame : public CHermitPageBase
#ifndef _SGE_BADA_
                            , public MSGESensorShakeObserver
#else
							, public Osp::Uix::IMotionEventListener
#endif
	{
public:

	virtual ~CHermitMenuPageGame();

	static CHermitMenuPageGame* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize );

	void ResetState();

private:

	CHermitMenuPageGame( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

	void CalculateLevelRect();

	void UpdateDirectionButtons();

	void UpdateLevelStoreAnimation();

	void DrawFills();

	void DrawLevel();

	void DrawLevelStore();

	void DrawLevelId();

public: // from base class

	void PageEnter();

	void PageLeave();

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

#ifndef _SGE_BADA_
private: // from MSGESensorShakeObserver
	
    void ShakeStarted();

    void ShakeEnded( TBool aPassDuration );

#else
private: // from IMotionEventListener

    void OnSnapDetected(Osp::Uix::MotionSnapType snapType);

    void OnDoubleTapDetected();

    void OnShakeDetected(Osp::Uix::MotionState motionState);
#endif

private: // data

	TFixedArray< THermitGuiButton, KMenuPageGameBtnCount > iButtons;

	TFixedArray< THermitGuiImage, KMenuPageGameImgCount > iImages;

	TFixedArray< CHermitAnimationArrowButton*, KMenuPageGameAnimBtnCount > iAnimatedButtons;

	CHermitLevel* iHermitLevel; // not owned

	CHermitAnimationFade* iStoreFadeAnimator; // owned

	CHermitAnimationFade* iLevelIdFadeAnimator; // owned

	TInt iStoreTime;

	TInt iStoreMoves;

	TRect iBoardRect; // LevelRect with buttons

	TRect iBoardRectOnly; // board rect without buttons

	TRect iLevelRect; // levle rect on (0,0) start point

	TRect iLevelRectCentered; // centered board with field size of 32 inside iLevelRect

	CGlFont* iFont1; // owned

	TInt iFastScrollCounter;

	TBool iFastScrollStarted;

#ifndef _SGE_BADA_
	CSGESensorShake *iShakeSensor;
#else
    Osp::Uix::Motion *iMotion; // owned
#endif

	};

#endif /* HERMITMENUPAGEGAME_H_ */
