/*
 ============================================================================
  Name        : HermitAnimationFade.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITANIMATIONFADE_H_
#define HERMITANIMATIONFADE_H_

#include "HermitAnimationBase.h"
#include "HermitGuiButton.h"

class CGlObjectRect;
class CHermitCommonObjects;


class CHermitAnimationFade : public CHermitAnimationBase, public MSGEGlDrawableObject
	{
public:

	enum TFadeType
	{
		EFadeIn, 	// black -> transparent
		EFadeOut	// transparent -> black
	};

public:

	~CHermitAnimationFade();

	static CHermitAnimationFade* NewL( MHermitAnimationObserver* aObserver, CGlObjectRect& aObjectRect, CHermitCommonObjects& aCommonObjects );

	static CHermitAnimationFade* NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects );

	void StartAnimation( TFadeType aType );

	void StartAnimationDifferential( TFadeType aFadeRect1Type );

	void SetFadeRect( TRect aFadeRect );

	void SetFadeRect2( TRect aFadeRect );

	TRect FadeRect() const;

	TRect FadeRect2() const;

	void EnableFadeRects( TBool aEnableFadeRect1, TBool aEnableFadeRect2 );

	TFadeType FadeType() const;

	void RestartAnimation();

	void SetStartIncrement( TInt aValue );

	void SetAnimationStepTime( TInt aValue );

	CGlObjectRect* ObjectRect() const;

	void UseExternalTextures( TBool aUse );

	void SetFadeColor( TRgb aColor );

private:

	CHermitAnimationFade( MHermitAnimationObserver* aObserver, CGlObjectRect& aObjectRect, CHermitCommonObjects& aCommonObjects );

	CHermitAnimationFade( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects );

	void ConstructL();

	void DoAnimationStep();

	void UpdateColor();

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

private: // from base class

    void AnimationSequenceChanged();

    void AnimationSequenceLast();

    TBool AnimationUpdate();

private: // data

	TRect iFadeRect;

	TRect iFadeRect2;

	TFadeType iFadeType;

	CGlObjectRect* iObjectRect; // owned

    CHermitCommonObjects& iCommonObjects; // not owned

    TReal iIncrement;

    TInt iTransparencyValue; // 0 - 0xff

    TBool iOgjectRectOwned;

    TInt iStartIncrement;

    TInt iAnimationStepTime;

    TBool iEnabledFadeRect;

    TBool iEnabledFadeRect2;

    TBool iDifferentialAnimation;

    TBool iExternalTextures;

    TRgb iColor;

	};


#endif /* HERMITANIMATIONFADE_H_ */
