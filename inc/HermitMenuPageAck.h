/*
 ============================================================================
  Name        : HermitMenuPageAck.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITMENUPAGEACK_H_
#define HERMITMENUPAGEACK_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"


const TInt KAckActionBtnBack = 0x0601;
const TInt KAckActionBtnYes = 0x0602;
const TInt KAckActionBtnNo = 0x0603;

const TInt KMenuPageAckBtnCount = 3;




class CHermitMenuPageAck : public CHermitPageBase
	{
public:

	virtual ~CHermitMenuPageAck();

	static CHermitMenuPageAck* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
									 TSize aSize );

private:

	CHermitMenuPageAck( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

	void DrawFills();

public: // from base class

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

private: // data

	TFixedArray< THermitGuiButton, KMenuPageAckBtnCount > iButtons;

	};

#endif /* HERMITMENUPAGEACK_H_ */
