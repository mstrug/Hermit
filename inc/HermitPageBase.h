/*
 ============================================================================
  Name        : HermitPageBase.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITPAGEBASE_H_
#define HERMITPAGEBASE_H_

#include <e32def.h>
#include <e32base.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include "HermitAnimationBase.h"

class CHermitCommonObjects;
class CHermitPageBase;
class CHermitAnimationButtonPressed;
class CHermitAnimationFade;
class CGlObjectRect;


class MHermitPageObserver
	{
public:

	virtual void PageAction( CHermitPageBase* aPage, TInt aAction ) = 0;

	};


class CHermitPageBase : public CBase, public MSGEGlDrawableObject,
						public MSGEUpdatableObject, public MSGEPointerEventableObject,
						public MHermitAnimationObserver
	{
protected:

	enum TPageState
		{
		ELoading,
		EEntering,
		EIdle,
		ESelecting,
		EUnselecting,
		EUnselectingSelecting,
		EAnimation,
		ELeaving
		};

public:

	virtual ~CHermitPageBase();

	virtual void PageEnter();

	virtual void PageLeave();

	TBool IsEntering() const;

protected:

	CHermitPageBase( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void BaseConstructL();

	// empty implementation
	virtual TBool AllAnimationsFinished();

	void PlaySound( TUint aSampleId );

public: // from MSGEGlDrawableObject

	// empty implementation
	void Draw( const TRect aDrawRect );

public: // from MSGEUpdatableObject

	// empty implementation
	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

public: // from MSGEPointerEventableObject

	// empty implementation
    TBool PointerEvent( const TPointerEvent &aPointerEvent );

public: // from MHermitAnimationObserver

    // empty implementation
    void AnimationFinished( CHermitAnimationBase* aAnimation );

protected: // data

	MHermitPageObserver* iObserver;

	CHermitCommonObjects& iCommonObjects;

	CGlObjectRect *iRect; // owned

	CHermitAnimationButtonPressed* iAnimationButtonPressed; // owned

	CHermitAnimationFade* iAnimationFade; // owned

	TInt iState;

	TSize iSize;

	TPoint iButtonDownPos;

	};

#endif /* HERMITPAGEBASE_H_ */
