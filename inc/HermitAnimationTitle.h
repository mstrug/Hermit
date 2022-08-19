/*
 ============================================================================
  Name        : HermitAnimationTitle.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONTITLE_H_
#define HERMITANIMATIONTITLE_H_

#include "HermitAnimationBase.h"
#include "HermitGuiButton.h"
#ifdef _SGE_BADA_
#include <FSecAesSecureRandom.h>
#endif

class CGlObjectRect;
class CHermitCommonObjects;
class CHermitMenuTitle;


class CHermitAnimationTitle : public CHermitAnimationBase, public MSGEGlDrawableObject
	{

	enum TAnimationPhase
	{
		ERotation,
		EScaleOut
	};

public:

	~CHermitAnimationTitle();

	static CHermitAnimationTitle* NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void StartAnimation();

	void StopAnimation();

private:

	CHermitAnimationTitle( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void UpdateTitleRect();

	void DoAnimationStep();

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

private: // from base class

    void AnimationSequenceChanged();

    void AnimationSequenceLast();

    TBool AnimationUpdate();

private: // data

	CGlObjectRect* iObjectRect; // owned

    CHermitCommonObjects& iCommonObjects; // not owned

    CHermitMenuTitle* iTitle; // owned

#ifdef _SGE_BADA_
    Osp::Security::AesSecureRandom* iRandom;
#endif

    TAnimationPhase iPhase;

    TSize iSize;

    TInt iAnimationCounter;

    TInt iAnimationCounterEnd;

    GLfloat iTitleScale;

    GLfloat iTitleScaleStep;

    GLfloat iTitleRotation;

    TRect iTitleRect;

	};

#endif /* HERMITANIMATIONTITLE_H_ */
