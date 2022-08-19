/*
 ============================================================================
  Name        : HermitGamePageGame.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITGAMEPAGEGAME_H_
#define HERMITGAMEPAGEGAME_H_

#include "HermitPageBase.h"
#include "HermitGuiButton.h"
#include "HermitGuiImage.h"
#include "HermitGameLevel.h"
#include "HermitCompilationSettings.h"


class CHermitLevel;
class CHermitAnimationFade;


const TInt KGameActionBtnBack = 0x0601;
const TInt KGameActionBtnSettings = 0x0602;

const TInt KGamePageGameBtnCount = 3;

#ifndef _SGE_BADA_
_LIT( KHermitGameStateFile, "game.sav" );
#else
_LIT( KHermitGameStateFile, "/Home/game.sav" );
#endif



class CHermitGamePageGame : public CHermitPageBase, MHermitGameLevelObserver
	{
public:

	virtual ~CHermitGamePageGame();

	static CHermitGamePageGame* NewL( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects,
										  TSize aSize );

	void UpdateCurrentLevel();

	void HideMenu();

	void GameTimeSecondsInc();

	void GameTimeSecondsReset();

	TBool IsGameStarted();

	TBool IsGameFinished();

	void ResetState();

	void SaveGameLevelOnClose();

	TBool RestoreGameLevelOnStart(); // returns ETrue when level has been restored

private:

	CHermitGamePageGame( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize );

	void ConstructL();

	void SizeChanged( TSize aSize );

	void CalculateLevelRect();

	void AnimateNextLevel();

	void DrawFills();

	void DrawStatus();

	void StartHidingMenu( TBool aAlways = EFalse );

	void StartShowingMenu();

	void ResetCurrentLevel();

	void StartSwitchingLevels();

public: // from base class

	void PageEnter();

	void Draw( const TRect aDrawRect );

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

	TBool PointerEvent( const TPointerEvent &aPointerEvent );

	void AnimationFinished( CHermitAnimationBase* aAnimation );

	TBool AllAnimationsFinished();

private: // from MHermitGameLevelObserver

	void GameFinished( TBool aWin );

	void GameNextLevelAnimationFinished();

private: // data

	enum TGamePageState
		{
		ESwitchingLevel = ELeaving + 1
		};

	TFixedArray< THermitGuiButton, KGamePageGameBtnCount > iButtons;

	CHermitGameLevel* iGameLevel; // owned

	CHermitAnimationFade* iAnimationStatusFade; // owned

	CHermitAnimationFade* iAnimationMenuFade; // owned

#ifdef PROTO_TOUCH_ANIMATIONS
	RPointerArray< CHermitAnimationFade > iAnimationFadeIndicators; // owned
#endif

	TRect iLevelRect;

	TPoint iButtonDownPos;

	TBool iStatusVisible;

	TBool iMenuVisible;

	TInt iGameTimeSeconds;

	TInt iSwitchingLevelCounter;

	TInt iSwitchingLevelTime;

	TInt iSwitchingLevelPhase;

	TInt iSwitchingLevelSavedTime;

	TInt iSwitchingLevelSavedMovesCounter;

	TBool iGameLose;

	};

#endif /* HERMITGAMEPAGEGAME_H_ */
