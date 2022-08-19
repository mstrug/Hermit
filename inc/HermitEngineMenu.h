/*
 ============================================================================
  Name        : HermitEngineMenu.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITENGINEMENU_H_
#define HERMITENGINEMENU_H_

#include <SGEEngine/SGEEngine.h>
#include <SGEEngine/SGEEngineObserver.h>
#include "HermitPageBase.h"


class CGlObjectRect;
class CHermitMenuPageMain;
class CHermitMenuPageSettings;
class CHermitMenuPageGame;
class CHermitMenuPageTutorial;
class CHermitMenuPageAbout;
class CHermitMenuPageAck;


const TUint KHermitEngineMenuId = 1;



class MHermitEngineMenuObserver : public MSGEEngineObserver
    {
public:
    virtual void EngineMenuTitleAnimationFinished() = 0;

    virtual void EngineMenuCreated() = 0;
    };




class CHermitEngineMenu : public CSGEEngine, MHermitPageObserver
    {

	enum TState
	{
		ELoading,
		EPageMain,
		EPageSettings,
		EPageGame,
		EPageTutorial,
		EPageAbout,
		EPageAck,
		EExit
	};

public:

    static CHermitEngineMenu* NewL( MHermitEngineMenuObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects,
									TSize aSize );

    ~CHermitEngineMenu();

    // used after game state restored
    void SwitchToGamePage();

private:

    CHermitEngineMenu( MHermitEngineMenuObserver& aObserver, CSGEEngineCommonObjects *aCommonObjects, TSize aSize );

    void ConstructL();

    void ConstructPagesL();

    void LoadTexturesL();

    void LoadAudioL();

    void DrawLoadingLogo();
    
public: // from base class

    TUint Id() const;

    void SetPause( TBool aPause );

    void Activate();

    void Draw( CBitmapContext* aGc, const TRect aDrawRect );

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeIntercal );

    TBool KeyDownEvent( const TKeyEvent &aKeyEvent );

    TBool KeyEvent( const TKeyEvent &aKeyEvent );

    TBool PointerEvent( const TPointerEvent &aPointerEvent );

    void TextureCreated( TUint aTextureId, TInt aError );

    void TexturesCreatedAll();

    void AudioFileLoaded( TInt aSoundSampleId, TInt aError );

    void AudioAllFilesLoaded();

    void AudioSoundTrackPlayStarted( TUint aTrackId );

    void AudioSoundTrackPlayFinished( TUint aTrackId );

    void AudioStreamCreated( TInt aError );

    void AudioStreamError( TInt aError );

private: // from MHermitPageObserver

	void PageAction( CHermitPageBase* aPage, TInt aAction );

private: // data

    TBool iPause;

    TBool iLeaving;

    TState iState;

	CGlObjectRect *iRect; // owned

	CHermitMenuPageMain* iPageMain; // owned

	CHermitMenuPageSettings* iPageSettings; // owned

	CHermitMenuPageGame* iPageGame; // owned

	CHermitMenuPageTutorial* iPageTutorial; // owned

	CHermitMenuPageAbout* iPageAbout; // owned

	CHermitMenuPageAck* iPageAck; // owned

	TState iButtonDownPage;

	TInt iMusicTrackId;

	TInt iLoadingTexturesPhase; // 0 - loading Logo, 1 - starting timer, 2 - delay for showing logo, 3 - loading rest textures
	
	TInt iLogoTime;
	
    };


#endif /* HERMITENGINEMENU_H_ */
