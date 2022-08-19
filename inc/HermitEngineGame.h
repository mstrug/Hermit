/*
 ============================================================================
  Name        : HermitEngineGame.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITENGINEGAME_H_
#define HERMITENGINEGAME_H_

#include <SGEEngine/SGEEngine.h>
#include <SGEEngine/SGEEngineObserver.h>
#include <SGEEngine/SGEEngineGPTimerObserver.h>
#include "HermitPageBase.h"


class CGlObjectRect;
class CHermitGamePageGame;
class CHermitGamePageSettings;
class CSGEEngineGPTimer;
class CHermitMenuPageAck;


const TUint KHermitEngineGameId = 2;



class MHermitEngineGameObserver : public MSGEEngineObserver
    {
public:
    virtual void EngineGameSwitch() = 0;
    };




class CHermitEngineGame : public CSGEEngine, MHermitPageObserver, MSGEEngineGPTimerObserver
    {

	enum TState
	{
		ELoading,
		EPageGame,
		EPageSettings,
		EPageAck
	};

public:

    ~CHermitEngineGame();

    static CHermitEngineGame* NewL( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
									TSize aSize );

    void SaveStateOnClose();

    TBool RestoreState();

private:

    void ConstructL();

    CHermitEngineGame( MSGEEngineObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
					   TSize aSize );

public: // from base class

    TUint Id() const;

    void SetPause( TBool aPause );

    void Activate();

    void Draw( CBitmapContext* aGc, const TRect aDrawRect );

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal );

    TBool KeyDownEvent( const TKeyEvent &aKeyEvent );

    TBool KeyEvent( const TKeyEvent &aKeyEvent );

    TBool PointerEvent( const TPointerEvent &aPointerEvent );

    void AudioSoundTrackPlayStarted( TUint aTrackId );

    void AudioSoundTrackPlayFinished( TUint aTrackId );

    void AudioStreamError( TInt aError );

private: // from MHermitPageObserver

	void PageAction( CHermitPageBase* aPage, TInt aAction );

private: //from MSGEEngineGPTimerObserver

	void GPTimerEvent( CSGEEngineGPTimer* aTimer );

private: // data

    TBool iPause;

    TState iState;

    CGlObjectRect *iRect; // owned

	CHermitGamePageGame* iPageGame; // owned

	CHermitGamePageSettings* iPageSettings; // not owned (passed from menu engine)

	CHermitMenuPageAck* iPageAck; // owned

	CSGEEngineGPTimer* iGameTimer; // owned

	TState iButtonDownPage;

    };


#endif /* HERMITENGINEGAME_H_ */
