/*
 ============================================================================
  Name        : HermitEngineController.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITENGINECONTROLLER_H_
#define HERMITENGINECONTROLLER_H_

#include <SGEEngine/SGEEngineController.h>
#include <SGEEngine/SGEEngineControllerObserver.h>
#include "HermitEngineMenu.h"
#include "HermitCompilationSettings.h"
#include "HermitAnimationFade.h"
#ifdef _SGE_BADA_
#include <FMedia.h>
#endif


class MHermitEngineControllerObserver : public MSGEEngineControllerObserver
    {
public:
    
    virtual void EngineCreated() = 0;
    
    };


class CHermitEngineController : public CSGEEngineController, public MHermitEngineMenuObserver,
								private MHermitAnimationObserver
	{
public:

    static CHermitEngineController* NewL( MHermitEngineControllerObserver& aObserver, TSize aSize );

    ~CHermitEngineController();

    void HandleForegroundEventL( TBool aForeground );

    void SaveStateOnClose();

    TBool EngineCreated() const;

    TBool Update( CBitmapContext* aGc, TRect aDrawRect, TTimeIntervalMicroSeconds32 aTimeInterval );

    TBool HandlePointerEventL( const TPointerEvent &aPointerEvent );

private:

    void ConstructL();

    CHermitEngineController( MHermitEngineControllerObserver& aObserver, TSize aSize );

    CSGEEngineCommonObjects* CreateCommonObjectsL();

    TBool RestoreState();

public: // from MSGEEngineObserver

    void EngineSwitch( TUint aId );

    void EngineClose( TUint aId, TInt aError );

public: // from MHermitEngineMenuObserver

    void EngineMenuTitleAnimationFinished();
    
    void EngineMenuCreated();

private: // from MHermitAnimationObserver

    void AnimationFinished( CHermitAnimationBase* aAnimation );

private: // data
    
    TBool iEngineCreated;
    
#ifdef PROTO_TOUCH_ANIMATIONS_ALWAYS
    RPointerArray< CHermitAnimationFade > iAnimationFadeIndicators; // owned

    TBool iIndicatorButtonUp;
#endif

	};

#endif /* HERMITENGINECONTROLLER_H_ */
