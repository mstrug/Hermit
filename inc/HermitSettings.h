/*
 ============================================================================
  Name        : HermitSettings.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITSETTINGS_H_
#define HERMITSETTINGS_H_

#include <e32def.h>
#include <e32base.h>

class CHermitCommonObjects;

const TInt KMaxVolume = 100;
const TInt KVolumeStep = KMaxVolume / 5;


class CHermitSettings : public CBase
	{
public:

	virtual ~CHermitSettings();

	static CHermitSettings* NewL( CHermitCommonObjects& aCommonObjects );

    void SaveToFile();

	TInt MusicVolume() const;

	void SetMusicVolume( TInt aMusicVolume );

	TInt StepMusicVolume( TInt aDirection ); // -1 or +1; returns volume after update

	TInt SoundVolume() const;

	void SetSoundVolume( TInt aSoundVolume );

	TInt StepSoundVolume( TInt aDirection ); // -1 or +1; returns volume after update

	TInt ColorSet() const;

	void SetColorSet( TInt aColorSet );

	TBool GreenIndicator() const;

	void SetGreenIndicator( TInt aGreenIndicator );

	void ToggleGreenIndicator();

	TBool OrangeIndicator() const;

	void SetOrangeIndicator( TInt aOrangeIndicator );

	void ToggleOrangeIndicator();

	TBool LevelIdVisible() const;

	void SetLevelIdVisible( TInt aLevelIdVisible );

	void ToggleLevelIdVisible();

private:

	CHermitSettings( CHermitCommonObjects& aCommonObjects );

	void ConstructL();

    void ReadFromFile();

    void CheckSilentMode();

private: // data

    CHermitCommonObjects& iCommonObjects; // not owned

	TInt iSoundVolume;

	TInt iMusicVolume;

	TInt iColorSet;

	TBool iGreenIndicator;

	TBool iOrangeIndicator;

	TBool iLevelIdVisible;

	TBool iSilentMode;

	TInt iSilentModeSoundVolume;

	TInt iSilentModeMusicVolume;

	};

#endif /* HERMITSETTINGS_H_ */
