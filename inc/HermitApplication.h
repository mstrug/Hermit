/*
 ============================================================================
  Name        : HermitApplication.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef __HERMITAPPLICATION_H__
#define __HERMITAPPLICATION_H__

#include <SGEApplication/SGEGameApplicationGL.h>
#include "HermitEngineController.h"
#include "HermitCompilationSettings.h"




class CHermitApplication : public CSGEGameApplicationGL, MHermitEngineControllerObserver
	{
public:

	CHermitApplication();

	virtual ~CHermitApplication();
	
	TUid ApplicationUid();
	
    static CApaApplication* NewApplication();
    
    TBool FullScreen() const;
    
    TBool GoBackgroundOnEndKey() const;

    TSGEApplicationOrientation Orientation() const;

    TInt SampleBuffers();

    TInt SampleSize();
    
protected: // from base class abstract functions

    void DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval );

protected: // from base class (optional)

    void InitL();

    void HandleForegroundEventL( TBool aForeground );

    TKeyResponse HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

    TBool HandleWsEventL( const TWsEvent& aEvent, CCoeControl *aDestination );

    void SizeChanged();

    TBool HandlePointerEventL( const TPointerEvent& aPointerEvent );
    
    TEglSurfaceType SurfaceType();

private: // from MHermitEngineControllerObserver

    void EngineControllerClose( TInt aError );

    void EngineCreated();

private: // data

    CHermitEngineController *iEngineCntrl;

    TBool iForeground;

#ifdef DUMP_FRAMES
	TAny* iBuf;
#endif

	};


#endif // __HERMITAPPLICATION_H__
