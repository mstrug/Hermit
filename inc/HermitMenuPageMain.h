/*
 ============================================================================
  Name        : HermitMenuPageMain.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUPAGEMAIN_H_
#define HERMITMENUPAGEMAIN_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"

class CHermitMenuTitle;
class CHermitAnimationTitle;


const TInt KMenuActionBtnGame = 0x0101;
const TInt KMenuActionBtnTutorial = 0x0102;
const TInt KMenuActionBtnSettings = 0x0103;
const TInt KMenuActionBtnExit = 0x0104;
const TInt KMenuActionBtnAbout = 0x0105;
const TInt KMenuActionAnimationTitleFinished = 0x0106;

const TInt KMenuPageMainBtnCount = 5;



class CHermitMenuPageMain : public CHermitPageBase
	{
public:

	virtual ~CHermitMenuPageMain();

	static CHermitMenuPageMain* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
								      TSize aSize );

	TBool IsLeaving() const;

private:

	CHermitMenuPageMain( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

public: // from base class

	void PageEnter();

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

private: // data

	TFixedArray< THermitGuiButton, KMenuPageMainBtnCount > iButtons;

	CHermitMenuTitle* iTitle; // owned

	CHermitAnimationTitle* iAnimationTitle; // owned

	TRect iTitleRect;

	TInt iMarginX;

	TInt iMarginY;

	};

#endif /* HERMITMENUPAGEMAIN_H_ */
