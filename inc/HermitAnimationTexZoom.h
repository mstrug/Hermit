/*
 ============================================================================
  Name        : HermitAnimationTexZoom.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONTEXZOOM_H_
#define HERMITANIMATIONTEXZOOM_H_

#include "HermitAnimationBase.h"

class CGlObjectRect;
class CHermitCommonObjects;


class CHermitAnimationTexZoom : public CHermitAnimationBase, public MSGEGlDrawableObject
	{
public:

	enum TZoomType
	{
		EZoomIn,
		EZoomOut
	};

public:

	~CHermitAnimationTexZoom();

	static CHermitAnimationTexZoom* NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects );

	void StartAnimation( TZoomType aType );

	void RestartAnimation();

	TZoomType ZoomType() const;

	void SetAnimationStepTime( TInt aTimeMS );

	void SetParameters( GLfloat aScaleMin, GLfloat aScaleMax, GLfloat aScaleStep );

	void SetScale( GLfloat aScale );

	void SetBouncing( TBool aBouncing );

private:

	CHermitAnimationTexZoom( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects );

	void ConstructL();

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

    TZoomType iZoomType;

    TInt iAnimationStepTime;

    GLfloat iScale;

    GLfloat iScaleMin;

    GLfloat iScaleMax;

    GLfloat iScaleStep;

    TBool iBouncing;

	};


#endif /* HERMITANIMATIONTEXZOOM_H_ */
