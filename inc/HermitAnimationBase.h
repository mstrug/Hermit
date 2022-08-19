/*
 ============================================================================
  Name        : HermitAnimationBase.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONBASE_H_
#define HERMITANIMATIONBASE_H_

#include <e32def.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include <gles/gl.h>


class CHermitAnimationBase;


class MHermitAnimationObserver
	{
public:

	virtual void AnimationFinished( CHermitAnimationBase* aAnimation ) = 0;

	};


/**
 * Animation consists of animation sequences which has duration time.
 * Simple animation can have only one sequence.
 */
class CHermitAnimationBase : public CBase, public MSGEUpdatableObject
	{
public:

	enum TSequencesDirection
	{
		EDirForward,
		EDirBackward
	};

public:

	~CHermitAnimationBase();

	void SetObserver( MHermitAnimationObserver* aObserver );

	void SetSequenceTime( TInt aSequenceNumber, TInt aMicrosecondsTime );

	TBool IsLastSequence() const;

	TBool IsAnimationStarted() const;

	TBool IsAnimationFinished() const;

	TInt CurrentSequence() const;

	TInt SequencesCount() const;

	virtual void RestartAnimation();

	void ReverseAnimation();

	// time of scale animation is same as animation in derived class
	void EnableScaleAnimation( TInt aStepTime, GLfloat aScaleStep );

	void DisableScaleAnimation();

	void ResetScale();

protected:

	CHermitAnimationBase( MHermitAnimationObserver* aObserver );

	void BaseConstructL( TInt aSequenceCount );

	// should be called by child class
	void DoStartAnimation();

	// should be called by child class
	void DoFinishAnimation();

	// if scale animation is enabled, call this before draw
	void ApplyScaleAnimation();

public: // from MSGEUpdatableObject

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

protected: // to be implemented in child class

    virtual void AnimationSequenceChanged() = 0;

    virtual void AnimationSequenceLast() = 0;

    virtual TBool AnimationUpdate() = 0;

protected: // data

    MHermitAnimationObserver* iObserver;

    RArray< TInt > iSequencesTimes;

    TInt iCurrentSequence;

    TBool iLastSequence;

    TSequencesDirection iSequencesDirection;

    TInt iSequenceTimeCounter; // used only to handle sequence switch

    TInt iTimeCoutner; // used by child class as elapsed time counter

    TBool iAnimationStarted;

    // scale animation
    TInt iScaleStepTime;

    TInt iScaleStepTimeCounter;

    GLfloat iScale;

    GLfloat iScaleStep;

	};


#endif /* HERMITANIMATIONBASE_H_ */
