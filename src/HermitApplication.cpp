/*
 ============================================================================
  Name        : HermitApplication.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitApplication.h"
#include "HermitEngineController.h"
#include "HermitUid.hrh"
#include <SGEGl/GlUtils.h>
#ifdef _SGE_BADA_
#include <FGraphicsOpengl.h>
using namespace Osp::Graphics::Opengl;
#include <FMediaImage.h>
using namespace Osp::Media;
#include <FGrpBitmap.h>
using namespace Osp::Graphics;
#endif


CHermitApplication::CHermitApplication()
	{
	}

CHermitApplication::~CHermitApplication()
	{
#ifdef DUMP_FRAMES
	User::Free( iBuf );
#endif
	delete iEngineCntrl;
	}

TUid CHermitApplication::ApplicationUid()
    {
    return TUid::Uid(_UID3);
    }

CApaApplication* CHermitApplication::NewApplication()
	{
	return CSGEGameApplication::NewApplication( new CHermitApplication );
	}

TBool CHermitApplication::FullScreen() const
    {
    return ETrue;
    }

TBool CHermitApplication::GoBackgroundOnEndKey() const
    {
    return EFalse;
    }

TSGEApplicationOrientation CHermitApplication::Orientation() const
    {
    return ESGEApplicationOrientationPortrait;
    }

TInt CHermitApplication::SampleBuffers()
	{
	return 0;
	}

TInt CHermitApplication::SampleSize()
	{
	return 0;
	}

void CHermitApplication::DoGameLoopEvent( TTimeIntervalMicroSeconds32 aTimeInterval )
	{
#ifdef ALWAYS_CLEAR
	glClearColor( 1, 0, 0, 1 );
	glClear( GL_COLOR_BUFFER_BIT );
#endif

	glLoadIdentity();

	//glScalef( 0.75, 0.8, 1 );

#ifndef DUMP_FRAMES
    
	iEngineCntrl->Update( NULL, Size(), aTimeInterval );

	User::ResetInactivityTime();
	//Thread::Sleep( 300 );

#else // DUMP_FRAMES

	iEngineCntrl->Update( NULL, Size(), 40000 );

	static int tmp = 0;
	//if ( tmp++ < 10 )
		{
		AppLog( "glReadPixels start" );
		glReadPixels( 0, 0, 480, 800, GL_RGBA, GL_UNSIGNED_BYTE, iBuf );
		GLenum err = glGetError();
#ifdef _SGE_BADA_
		if ( err != GL_NO_ERROR )
			{
			AppLog( "glReadPixels error: %d", err );
			}
		else
			{
			ByteBuffer* buf = new ByteBuffer();
			buf->Construct( 480 * 800 * 4 );
			buf->SetArray( (const byte*) iBuf, 0, 480 * 800 * 4 );
			buf->SetPosition(0);
			AppLog( "ByteBuffer created" );

			Bitmap* bmp = new Bitmap();
			result r = bmp->Construct( *buf, Dimension( 480, 800 ), BITMAP_PIXEL_FORMAT_R8G8B8A8 );
			if ( r != E_SUCCESS )
			{
				AppLog( "bmp->Construct error: %d", r );
				delete bmp;
				delete buf;
				return;
			}
			else
			{
				AppLog( "Bmp ok" );
			}

			Image* img = new Image();
			img->Construct();

			String str( L"/Media/Others/hermit_scr" );
#ifdef DUMP_FRAMES_BMP
			str.Format( 35, L"/Media/Others/hermit_scr_%04d.bmp", tmp++ );
#elif DUMP_FRAMES_PNG
			str.Format( 35, L"/Media/Others/hermit_scr_%04d.png", tmp++ );
#else
			str.Format( 35, L"/Media/Others/hermit_scr_%04d.jpg", tmp++ );
#endif

			AppLog( "%S", str.GetPointer() );

#ifdef DUMP_FRAMES_BMP
			r = img->EncodeToFile( *bmp, IMG_FORMAT_BMP, str, true );
#elif DUMP_FRAMES_PNG
			r = img->EncodeToFile( *bmp, IMG_FORMAT_PNG, str, true );
#else
			r = img->EncodeToFile( *bmp, IMG_FORMAT_JPG, str, true );
#endif
			if ( r != E_SUCCESS )
			{
				AppLog( "img->EncodeToFile error: %d", r );
			}
			else
			{
				AppLog( "file ok" );
			}

			delete img;
			delete bmp;
			delete buf;
			}
#endif // _SGE_BADA_
		AppLog( "glReadPixels end" );
		}
#endif
	// dunp to avi -> http://msdn.microsoft.com/en-us/library/ms779636(VS.85).aspx
	}

void CHermitApplication::InitL()
	{
	CSGEGameApplicationGL::InitL();

	SizeChanged();

    iEngineCntrl = CHermitEngineController::NewL( *this, Size() );

#ifdef DUMP_FRAMES
    iBuf = User::Alloc( 480 * 800 * 4 * sizeof( TUint8 ) );
#endif

    SetGameLoopMaxFPS( 1 );
    StartGameLoop();
	}

void CHermitApplication::HandleForegroundEventL( TBool aForeground )
	{
	if ( iForeground == aForeground )
		{
		return;
		}

	iForeground = aForeground;

    if ( aForeground )
        {
        StartGameLoop();
        }
    else
        {
        StopGameLoop();
        }

    iEngineCntrl->HandleForegroundEventL( aForeground );
	}

TKeyResponse CHermitApplication::HandleKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType )
	{
	return iEngineCntrl->HandleKeyEventL( aKeyEvent, aType );
	}

TBool CHermitApplication::HandleWsEventL( const TWsEvent& aEvent, CCoeControl* /*aDestination*/ )
	{
	if ( aEvent.Type() == KAknUidValueEndKeyCloseEvent || aEvent.Type() == KAknShutOrHideApp )
		{
		iEngineCntrl->SaveStateOnClose();
		return EFalse;
		}
	else if ( aEvent.Type() == EEventUser )
	    {
        TApaSystemEvent* eventData = reinterpret_cast<TApaSystemEvent*>( aEvent.EventData() );
        if ( (*eventData) == EApaSystemEventShutdown )
            {
            /* taken from goommonitorsession.h (plugin) */
            const TInt KGoomMemoryLowEvent = 0x10282DBF;
            //const TInt KGoomMemoryGoodEvent = 0x20026790;

            eventData++;
            if ((*eventData) == KGoomMemoryLowEvent)
                {
                //return ETrue; // stop the event from killing us (not working unless using RGOomSession)
                }

            iEngineCntrl->SaveStateOnClose();
            }
	    }

	return EFalse;
	}

void CHermitApplication::SizeChanged()
	{
	TRect r = Rect();
	//iEngineCntrl->SizeChanged( Size() );

//	GlUtils::SetViewportAsScreen( Size() );
//	GlUtils::SetSceneSizeAsScreen( Size() );

	TSize s = Size();
    GlUtils::SetViewportAsScreen( s );
    GlUtils::SetSceneSizeAsScreen( s );
	}

TBool CHermitApplication::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{
	//AppLog( "HandlePointerEventL  %d (%d,%d) %d", aPointerEvent.iType, aPointerEvent.iPosition.iX, aPointerEvent.iPosition.iY, aPointerEvent.iModifiers );
	TPointerEvent pointerEvent( aPointerEvent );
	pointerEvent.iPosition.iY = Size().iHeight - pointerEvent.iPosition.iY; // OpenGL uses different coord system
	TBool ret = iEngineCntrl->HandlePointerEventL( pointerEvent );
	if ( ret && aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		GameLoopEvent();
		}
	return ret;
	}

CSGEGameApplicationGL::TEglSurfaceType CHermitApplication::SurfaceType()
	{
	return EEglSurfaceWindow;
	}

void CHermitApplication::EngineControllerClose( TInt aError )
    {
    if ( aError != KErrNone )
        {
        AlertWin( _L("Error"), _L("Try restart or reinstall application.") );
        }
    Exit();
    }

void CHermitApplication::EngineCreated()
    {
    SetGameLoopMaxFPS( 60 );
    StartGameLoop();    
    }


