/*
 ============================================================================
  Name        : HermitAnimationArrowButton.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONARROWBUTTON_H_
#define HERMITANIMATIONARROWBUTTON_H_

#include "HermitAnimationBase.h"
#include "HermitGuiButton.h"

class CGlObjectRect;
class CHermitCommonObjects;



class CHermitAnimationArrowButton : public CHermitAnimationBase, public MSGEGlDrawableObject
	{
public:

	enum TArrowDirection
	{
		EDirUp,
		EDirDown,
		EDirLeft,
		EDirRight
	};

public:

	~CHermitAnimationArrowButton();

	static CHermitAnimationArrowButton* NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects,
											  THermitGuiButton& aAnimatedButton, TArrowDirection aButtonDirection );

	// button is always animated
	//void StartAnimation();

	void RestartAnimation();

private:

	CHermitAnimationArrowButton( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects,
								 THermitGuiButton& aAnimatedButton, TArrowDirection aButtonDirection );

	void ConstructL();

	void DoAnimationStep();

	void UpdateTexCoords();

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

private: // from base class

    void AnimationSequenceChanged();

    void AnimationSequenceLast();

    TBool AnimationUpdate();

private: // data

	CGlObjectRect* iObjectRect; // owned

    CHermitCommonObjects& iCommonObjects; // not owned

    THermitGuiButton& iAnimatedButton; // not owned

    TArrowDirection iButtonDirection;

    GLfixed iPosition;

    GLfixed iPositionDirection;

	};

#endif /* HERMITANIMATIONARROWBUTTON_H_ */
