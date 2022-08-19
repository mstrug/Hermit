/*
 ============================================================================
  Name        : HermitMenuPageAbout.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUPAGEABOUT_H_
#define HERMITMENUPAGEABOUT_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"


const TInt KAboutActionBtnBack = 0x0501;

const TInt KMenuPageAboutBtnCount = 1;



class CHermitMenuPageAbout : public CHermitPageBase
	{
public:

	virtual ~CHermitMenuPageAbout();

	static CHermitMenuPageAbout* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize );

private:

	CHermitMenuPageAbout( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

	void DrawAbout();

public: // from base class

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

private: // data

	TFixedArray< THermitGuiButton, KMenuPageAboutBtnCount > iButtons;

	TRect iTextRect;

	TRect iLogoRect;

	TRect iVersionRect;

	};

#endif /* HERMITMENUPAGEABOUT_H_ */
