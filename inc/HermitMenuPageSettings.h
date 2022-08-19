/*
 ============================================================================
  Name        : HermitMenuPageSettings.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUPAGESETTINGS_H_
#define HERMITMENUPAGESETTINGS_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"
#include "HermitGuiImage.h"
#include "HermitBackground.h"


class CHermitBackgroundGlObject;

const TInt KSettingsActionBtnBack = 0x0201;
const TInt KSettingsActionBtnResetLevels = 0x0202;

const TInt KMenuPageSettingsBtnCount = 8;
const TInt KMenuPageSettingsColorsBtnCount = KColorSets;
const TInt KMenuPageSettingsImgCount = 6;



class CHermitMenuPageSettings : public CHermitPageBase
	{
public:

	virtual ~CHermitMenuPageSettings();

	static CHermitMenuPageSettings* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize );

	void SetObserver( MHermitPageObserver* aObserver );

	void ResetState();

	void SetMusicTrackId( TInt aTrackId );

	void KeyEventVolumeUp();

	void KeyEventVolumeDown();

    static TInt CalculateSoundVolumeShift( TInt aValue ); // aValue (0..100) is transfered to -3..2

    static TInt CalculateMusicVolumeShift( TInt aValue ); // aValue (0..100) is transfered to -3..2

protected:

	CHermitMenuPageSettings( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

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

	TFixedArray< THermitGuiButton, KMenuPageSettingsBtnCount > iButtons;

	TFixedArray< THermitGuiButton, KMenuPageSettingsColorsBtnCount > iColorsButtons;

	TFixedArray< THermitGuiImage, KMenuPageSettingsImgCount > iImages;

	THermitColorSets iColorSets;

	CHermitBackgroundGlObject* iRectColorSet; // owned

	TInt iMusicTrackId;

	};

#endif /* HERMITMENUPAGESETTINGS_H_ */
