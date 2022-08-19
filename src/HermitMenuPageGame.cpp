/*
 ============================================================================
  Name        : HermitMenuPageGame.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageGame.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitLevel.h"
#include "HermitLevelProvider.h"
#include "HermitGamesStore.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitAnimationArrowButton.h"
#include "HermitAnimationFade.h"
#include "HermitCompilationSettings.h"
#include "HermitSettings.h"
#include "GlFont.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <e32def.h>

#ifdef _SGE_BADA_
using namespace Osp::Uix;
#endif



const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KButtonBigSize( 128, 128 );
const TSize KButtonBigTexSize( 128, 128 );

const TSize KButtonSmallSize( 64, 64 );
const TSize KButtonSmallTexSize( 64, 64 );


const TSize KImageTitleSize( 64, 64 );
const TSize KImageTitleTexSize( 64, 64 );


const TSize KFieldSizeSmall( 26, 26 );


const TInt KBtnLeftIdx = 0;
const TInt KBtnRightIdx = 1;
const TInt KBtnUpIdx = 2;
const TInt KBtnDownIdx = 3;
const TInt KBtnBackIdx = 4;
const TInt KBtnPlayIdx = 5;
const TInt KBtnLevelIdIdx = 6;

const TInt KImgTitleIdx = 0;


const TInt KFastScrollStartTime = 700000;	// ms
const TInt KFastScrollNextTime = 100000;	// ms


#define ABS( x ) ( x > 0 ? (x) : -(x) )


CHermitMenuPageGame::CHermitMenuPageGame( MHermitPageObserver& aObserver,
												  CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
    {
    }

CHermitMenuPageGame::~CHermitMenuPageGame()
    {
	delete iLevelIdFadeAnimator;
	delete iStoreFadeAnimator;
	delete iFont1;

	for ( TInt i = 0; i < iAnimatedButtons.Count(); i++ )
		{
		delete iAnimatedButtons[ i ];
		}

#ifndef _SGE_BADA_
	delete iShakeSensor;
#else
    delete iMotion;
#endif
    }

CHermitMenuPageGame* CHermitMenuPageGame::NewL( MHermitPageObserver& aObserver,
													    CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageGame* self = new (ELeave) CHermitMenuPageGame( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageGame::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnBackIdx ].iActionId = KGameActionBtnBack;
	iButtons[ KBtnBackIdx ].iTextureSize = KButtonTexSize;

	iButtons[ KBtnPlayIdx ].iTextureId = KHermitTexMenuBtnPlayId;
	iButtons[ KBtnPlayIdx ].iActionId = KGameActionBtnPlay;
	iButtons[ KBtnPlayIdx ].iTextureSize = KButtonBigTexSize;

	iButtons[ KBtnRightIdx ].iTextureId = KHermitTexMenuBtnRightId;
	iButtons[ KBtnRightIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnLeftIdx ].iTextureId = KHermitTexMenuBtnRightId;
	iButtons[ KBtnLeftIdx ].iRotate = 180;
	iButtons[ KBtnLeftIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnUpIdx ].iTextureId = KHermitTexMenuBtnRightId;
	iButtons[ KBtnUpIdx ].iRotate = 90;
	iButtons[ KBtnUpIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnDownIdx ].iTextureId = KHermitTexMenuBtnRightId;
	iButtons[ KBtnDownIdx ].iRotate = 270;
	iButtons[ KBtnDownIdx ].iTextureSize = KButtonTexSize;

	iImages[ KImgTitleIdx ].iTextureId = KHermitTexGameTitleId;
	iImages[ KImgTitleIdx ].iTextureSize = KImageTitleTexSize;

	iHermitLevel = iCommonObjects.LevelProvider()->CurrentLevel();
	UpdateDirectionButtons();

	iFont1 = CGlFont::NewL( *iCommonObjects.TextureManager()->Texture( KHermitTexFont1Id ), TSize( 16, 16 ), '-' );
	iFont1->AddLetterMargins( '-', 4, 4 );
	iFont1->AddLetterMargins( '.', 5, 5 );
	iFont1->AddLetterMargins( ':', 5, 5 );
	for ( TInt i = '0'; i <= '9'; i++ )
	    {
	    iFont1->AddLetterMargins( i, 3, 3 );
	    }
	iCommonObjects.SetFont1( iFont1 );

	iAnimatedButtons[ KBtnLeftIdx ] = CHermitAnimationArrowButton::NewL( this, iCommonObjects, iButtons[ KBtnLeftIdx ], CHermitAnimationArrowButton::EDirLeft );
	iAnimatedButtons[ KBtnRightIdx ] = CHermitAnimationArrowButton::NewL( this, iCommonObjects, iButtons[ KBtnRightIdx ], CHermitAnimationArrowButton::EDirRight );
	iAnimatedButtons[ KBtnUpIdx ] = CHermitAnimationArrowButton::NewL( this, iCommonObjects, iButtons[ KBtnUpIdx ], CHermitAnimationArrowButton::EDirUp );
	iAnimatedButtons[ KBtnDownIdx ] = CHermitAnimationArrowButton::NewL( this, iCommonObjects, iButtons[ KBtnDownIdx ], CHermitAnimationArrowButton::EDirDown );

	iStoreFadeAnimator = CHermitAnimationFade::NewL( this, iCommonObjects );
	iStoreFadeAnimator->SetAnimationStepTime( 15000 );
	iStoreFadeAnimator->SetStartIncrement( 30 );

	iLevelIdFadeAnimator = CHermitAnimationFade::NewL( this, iCommonObjects );
	iLevelIdFadeAnimator->SetAnimationStepTime( 15000 );
	iLevelIdFadeAnimator->SetStartIncrement( 30 );

	SizeChanged( iSize );

#ifndef _SGE_BADA_
	iShakeSensor = CSGESensorShake::NewL( *this );
	iShakeSensor->SetTreshold( 20000 );
	iShakeSensor->SetDuration( 1000000 );
	iShakeSensor->SetReadRepeats( 20 );
#else
    iMotion = new Motion();
    iMotion->Construct( *this );
    iMotion->SetEnabled( MOTION_TYPE_NONE );
#endif

	iState = EIdle;
	}

void CHermitMenuPageGame::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	TInt marginX = iSize.iWidth / 24;
	//TInt marginY = iSize.iHeight / 6 + KButtonSize.iHeight;
	TInt step = KButtonSize.iHeight;

	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );
	iButtons[ KBtnPlayIdx ].iRect = TRect( TPoint( marginX, marginX ), KButtonBigSize );
	iImages[ KImgTitleIdx ].iRect = TRect( TPoint( 0, iSize.iHeight - KImageTitleSize.iHeight ), KImageTitleSize );

	TInt ofs = 4;
	TInt boardW = iSize.iWidth + 2 * ofs;
    iBoardRect = TRect( TPoint( -ofs, iSize.iHeight / 2 - boardW / 2 + step ), TSize( boardW, boardW ) );
//    iBoardRect = TRect( TPoint( -ofs, iButtons[ KBtnPlayIdx ].iRect.iBr.iY ), TSize( iSize.iWidth + 2 * ofs, iImages[ KImgTitleIdx ].iRect.iTl.iY - iButtons[ KBtnPlayIdx ].iRect.iBr.iY ) );
	iBoardRectOnly = iBoardRect;
	iBoardRectOnly.Grow( -KButtonSize.iWidth, -KButtonSize.iHeight );

	iButtons[ KBtnRightIdx ].iRect = TRect( TPoint( iBoardRect.iBr.iX - KButtonSize.iWidth, iBoardRect.iTl.iY + iBoardRect.Height() / 2 - KButtonSize.iHeight / 2 ), KButtonSize );
	iButtons[ KBtnLeftIdx ].iRect = TRect( TPoint( iBoardRect.iTl.iX, iBoardRect.iTl.iY + iBoardRect.Height() / 2 - KButtonSize.iHeight / 2 ), KButtonSize );
	iButtons[ KBtnUpIdx ].iRect = TRect( TPoint( iBoardRect.iTl.iX + iBoardRect.Width() / 2 - KButtonSize.iWidth / 2, iBoardRect.iBr.iY - KButtonSize.iHeight ), KButtonSize );
	iButtons[ KBtnDownIdx ].iRect = TRect( TPoint( iBoardRect.iTl.iX + iBoardRect.Width() / 2 - KButtonSize.iWidth / 2, iBoardRect.iTl.iY ), KButtonSize );

	iStoreFadeAnimator->SetFadeRect( TRect( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY ), TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) ) );

	iLevelIdFadeAnimator->SetFadeRect( TRect( TPoint( 0, iButtons[ KBtnUpIdx ].iRect.iTl.iY ), TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) ) );
	iButtons[ KBtnLevelIdIdx ].iRect = iLevelIdFadeAnimator->FadeRect();

	CalculateLevelRect();
	}

void CHermitMenuPageGame::CalculateLevelRect()
	{
	TSize ls = iHermitLevel->Size();

	iLevelRect = TRect( TPoint( 0, 0 ), TSize( iBoardRect.Width() - 2 * KButtonSize.iWidth, iBoardRect.Height() - 2 * KButtonSize.iHeight ) );
	iLevelRectCentered = TRect( TPoint( ( iLevelRect.Width() - ls.iWidth * KFieldSizeSmall.iWidth ) / 2, ( iLevelRect.Height() - ls.iHeight * KFieldSizeSmall.iHeight ) / 2  ), TSize( ls.iWidth * KFieldSizeSmall.iWidth, ls.iHeight * KFieldSizeSmall.iHeight ) );
	}

void CHermitMenuPageGame::UpdateDirectionButtons()
	{
	iButtons[ KBtnRightIdx ].iVisible = iCommonObjects.LevelProvider()->IsNextLevelGroupAvailable();
	iButtons[ KBtnLeftIdx ].iVisible = iCommonObjects.LevelProvider()->IsNextLevelGroupAvailable( -1 );
	iButtons[ KBtnUpIdx ].iVisible = iCommonObjects.LevelProvider()->IsNextLevelInGroupAvailable( -1 );
	iButtons[ KBtnDownIdx ].iVisible = iCommonObjects.LevelProvider()->IsNextLevelInGroupAvailable();
	}

void CHermitMenuPageGame::UpdateLevelStoreAnimation()
	{
	TBool showed = ( iStoreTime > 0 && iStoreMoves > 0 );

	if ( iCommonObjects.GamesStore()->GetLevelCompleted( iHermitLevel->LevelId(), iStoreTime, iStoreMoves ) )
		{
#ifdef USE_ANIMATIONS
		if ( !showed || iStoreFadeAnimator->IsAnimationStarted() )
			{
			iStoreFadeAnimator->StartAnimation( CHermitAnimationFade::EFadeIn );
			}
#else
#endif
		}
	else if ( showed || iStoreFadeAnimator->IsAnimationStarted() )
		{
#ifdef USE_ANIMATIONS
		iStoreFadeAnimator->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
		iStoreTime = 0;
		iStoreMoves = 0;
#endif
		}
	}

void CHermitMenuPageGame::ResetState()
	{
	iHermitLevel = iCommonObjects.LevelProvider()->CurrentLevel();
	UpdateDirectionButtons();
	CalculateLevelRect();

	iStoreTime = 0;
	iStoreMoves = 0;
    UpdateLevelStoreAnimation();

	for ( TInt i = 0; i < iAnimatedButtons.Count(); i++ )
		{
		iAnimatedButtons[ i ]->RestartAnimation();
		}

	iStoreFadeAnimator->RestartAnimation();
	iLevelIdFadeAnimator->RestartAnimation();
	}


void CHermitMenuPageGame::PageEnter()
	{
	CHermitPageBase::PageEnter();

#ifndef _SGE_BADA_
	iShakeSensor->StartListeningL();
#else
	iMotion->SetEnabled( MOTION_TYPE_SHAKE );
#endif
	}

void CHermitMenuPageGame::PageLeave()
	{
	CHermitPageBase::PageLeave();

#ifndef _SGE_BADA_
    iShakeSensor->StopListening();
#else
	iMotion->SetEnabled( MOTION_TYPE_NONE );
#endif
	}

void CHermitMenuPageGame::DrawFills()
	{
	//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
	
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );
	
	// right side of title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iImages[ KImgTitleIdx ].iRect.iBr.iX, iImages[ KImgTitleIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iImages[ KImgTitleIdx ].iRect.iBr.iX, iImages[ KImgTitleIdx ].iRect.Height() ) );
	iRect->Draw();
	glPopMatrix();

    // bottom of title
    glPushMatrix();
    GlUtils::Translate2D( TPoint( 0, iBoardRect.iBr.iY ) );
    GlUtils::Scale2D( TSize( iSize.iWidth, iImages[ KImgTitleIdx ].iRect.iTl.iY - iBoardRect.iBr.iY ) );
    iRect->Redraw();
    glPopMatrix();

	// left side of play button
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnPlayIdx ].iRect.iTl.iX, iButtons[ KBtnPlayIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

    // top of play button
    glPushMatrix();
    GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnPlayIdx ].iRect.iBr.iY ) );
    GlUtils::Scale2D( TSize( iSize.iWidth, iBoardRect.iTl.iY - iButtons[ KBtnPlayIdx ].iRect.iBr.iY ) );
    iRect->Redraw();
    glPopMatrix();
    
	// bottom
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnPlayIdx ].iRect.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// right side of play button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnPlayIdx ].iRect.iBr.iX, iButtons[ KBtnPlayIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX - iButtons[ KBtnPlayIdx ].iRect.iBr.iX, iButtons[ KBtnPlayIdx ].iRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// top of back button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.Width(), iButtons[ KBtnPlayIdx ].iRect.iBr.iY - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// left side of down button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnDownIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnDownIdx ].iRect.iTl.iX, iButtons[ KBtnDownIdx ].iRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// right side of down button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnDownIdx ].iRect.iBr.iX, iButtons[ KBtnDownIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnDownIdx ].iRect.iBr.iX, iButtons[ KBtnDownIdx ].iRect.Height() ) );
	iRect->Redraw();
	glPopMatrix();

	// left side of up button
//	glPushMatrix();
//	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
//	GlUtils::Scale2D( TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) );
//	iRect->Draw();
//	glPopMatrix();

	// right side of up button
//	glPushMatrix();
//	GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
//	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) );
//	iRect->Draw();
//	glPopMatrix();

	// top of left button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnLeftIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnLeftIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY - iButtons[ KBtnLeftIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom of left button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnLeftIdx ].iRect.iBr.iX, iButtons[ KBtnLeftIdx ].iRect.iTl.iY - iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// top of right button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnRightIdx ].iRect.iTl.iX, iButtons[ KBtnRightIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnRightIdx ].iRect.Width(), iButtons[ KBtnUpIdx ].iRect.iTl.iY - iButtons[ KBtnRightIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom of right button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnRightIdx ].iRect.iTl.iX, iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnRightIdx ].iRect.Width(), iButtons[ KBtnRightIdx ].iRect.iTl.iY - iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// board left
	if ( iLevelRectCentered.iTl.iX > 0 )
		{
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnLeftIdx ].iRect.iBr.iX, iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
		GlUtils::Scale2D( TSize( iLevelRectCentered.iTl.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY - iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
		iRect->Redraw();
		glPopMatrix();
		}

	// board right
	TInt tmp = iButtons[ KBtnRightIdx ].iRect.iTl.iX - iLevelRectCentered.iBr.iX - iButtons[ KBtnLeftIdx ].iRect.iBr.iX;
	if ( tmp > 0 )
		{
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnLeftIdx ].iRect.iBr.iX + iLevelRectCentered.iBr.iX, iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
		GlUtils::Scale2D( TSize( tmp, iButtons[ KBtnUpIdx ].iRect.iTl.iY - iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
		iRect->Redraw();
		glPopMatrix();
		}

	// board bottom
	if ( iLevelRectCentered.iTl.iY > 0 )
		{
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnLeftIdx ].iRect.iBr.iX + iLevelRectCentered.iTl.iX, iButtons[ KBtnDownIdx ].iRect.iBr.iY ) );
		GlUtils::Scale2D( TSize( iLevelRectCentered.Width(), iLevelRectCentered.iTl.iY ) );
		iRect->Redraw();
		glPopMatrix();
		}

	// board top
	if ( iLevelRect.Height() - iLevelRectCentered.iBr.iY > 0 )
		{
		tmp = ( iLevelRect.Height() - iLevelRectCentered.Height() ) / 2;
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnLeftIdx ].iRect.iBr.iX + iLevelRectCentered.iTl.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY - tmp ) );
		GlUtils::Scale2D( TSize( iLevelRectCentered.Width(), tmp ) );
		iRect->Redraw();
		glPopMatrix();
		}
    
    glColor4ub( 255, 255, 255, 255 );
	
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
	}

void CHermitMenuPageGame::DrawLevel()
	{
	TGlTexture* tex[ 3 ];
	tex[ CHermitLevel::EFieldNone ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId );
	tex[ CHermitLevel::EFieldEmpty ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEmptyId );
	tex[ CHermitLevel::EFieldItem ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldItemId );
	TGlTexture* texGray = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEndId );
	//TGlTexture* texGrayInd = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEndIndId );

	GlUtils::Translate2D( iLevelRectCentered.iTl );
	GlUtils::Scale2D( KFieldSizeSmall );

	for ( TInt j = 0; j < iHermitLevel->Size().iHeight; j++ )
		{
		for ( TInt i = 0; i < iHermitLevel->Size().iWidth; i++ )
			{
			//if ( Osp::Base::Utility::Math::Rand() > Osp::Base::Utility::Math::RAND_VALUE_MAX / 2 )

			TInt ftype = iHermitLevel->FieldC( i, j );

			if ( iHermitLevel->IsFieldFinish( TPoint( i, j ) ) )
				{
				texGray->Use();
				iRect->Redraw();
				}

			if ( ftype == CHermitLevel::EFieldNone ||
				 ftype == CHermitLevel::EFieldEmpty ||
				 ftype == CHermitLevel::EFieldItem )
				{
				tex[ ftype ]->Use();
				iRect->Redraw();
				}

			glTranslatef( 1, 0, 0 );
			}
		glTranslatef( -iHermitLevel->Size().iWidth, 1, 0 );
		}
	}

void CHermitMenuPageGame::DrawLevelStore()
	{
	if ( iStoreMoves > 0 && iStoreTime > 0 )
		{
		CGlFont *f1 = iCommonObjects.Font1();
#ifdef _SGE_BADA_
		String time; //( _L("00:12") );
		String moves;
		time.Format( 10, L"%02d:%02d", iStoreTime / 60, iStoreTime % 60 );
		moves.Append( iStoreMoves );
#else
        TBuf<15> time;
        TBuf<10> moves;
        time.Format( _L("%02d:%02d"), iStoreTime / 60, iStoreTime % 60 );
        moves.AppendNum( iStoreMoves );
#endif

		TInt l1 = f1->GetTextWidth( time );
		TInt l2 = f1->GetTextWidth( moves );
		TInt lH = f1->LetterSize().iHeight;
		TInt marginX = f1->LetterSize().iWidth;
		TInt marginY = iButtons[ KBtnUpIdx ].iRect.iBr.iY - lH;
		TInt stepY = lH / 3;

		TPoint p1( iSize.iWidth - l1 - marginX, marginY );
		TPoint p2( iSize.iWidth - l2 - marginX, marginY - lH - stepY );

		f1->DrawText( p1, time );
		f1->DrawText( p2, moves );

		// fils
		//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
	    GlUtils::DisableTexturing();
	    GlUtils::DisableTransparency();
	    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
	    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );   
	    glColor4ub( 0, 0, 0, 255 );

		// top of text
//		glPushMatrix();
//		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, p1.iY + lH ) );
//		GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, lH ) );
//		iRect->Draw();
//		glPopMatrix();

		// left side of time
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, p1.iY ) );
		GlUtils::Scale2D( TSize( p1.iX - iButtons[ KBtnUpIdx ].iRect.iBr.iX, lH ) );
		iRect->Draw();
		glPopMatrix();

		// right side of time
		glPushMatrix();
		GlUtils::Translate2D( TPoint( p1.iX + l1, p1.iY ) );
		GlUtils::Scale2D( TSize( marginX, lH ) );
		iRect->Redraw();
		glPopMatrix();

		// between texts
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, p1.iY - stepY ) );
		GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, stepY ) );
		iRect->Redraw();
		glPopMatrix();

		// left side of moves
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, p2.iY ) );
		GlUtils::Scale2D( TSize( p2.iX - iButtons[ KBtnUpIdx ].iRect.iBr.iX, lH ) );
		iRect->Redraw();
		glPopMatrix();

		// right side of moves
		glPushMatrix();
		GlUtils::Translate2D( TPoint( p2.iX + l2, p2.iY ) );
		GlUtils::Scale2D( TSize( marginX, lH ) );
		iRect->Redraw();
		glPopMatrix();

		// bottom of moves
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, p2.iY - iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		iRect->Redraw();
		glPopMatrix();
		}
	else if ( !iStoreFadeAnimator->IsAnimationStarted() )
		{
		// right side of up button
		iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnUpIdx ].iRect.iBr.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) );
		iRect->Draw();
		glPopMatrix();
		}
    glColor4ub( 255, 255, 255, 255 );
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

#ifdef USE_ANIMATIONS
	iStoreFadeAnimator->Draw( TRect() );
#endif
	}

void CHermitMenuPageGame::DrawLevelId()
	{
	CGlFont *f1 = iCommonObjects.Font1();
	TInt levelId = iHermitLevel->LevelId();
#ifdef _SGE_BADA_
	String id;
	id.Format( 7, L"%d.%d", ( ( levelId & 0xFF00 ) >> 8 ), ( levelId & 0xFF ) );
//	id.Append( ( ( levelId & 0xFF00 ) >> 8 ) );
//	id.Append( "-" );
//	id.Append( ( levelId & 0xFF ) );
#else
    TBuf<15> id;
    id.Format( _L("%d.%d"), ( ( levelId & 0xFF00 ) >> 8 ), ( levelId & 0xFF ) );
#endif

	if ( iCommonObjects.Settings()->LevelIdVisible() )
		{
		TInt l1 = f1->GetTextWidth( id );
		TInt lH = f1->LetterSize().iHeight;
		TInt marginX = f1->LetterSize().iWidth;
		TInt marginY = iButtons[ KBtnUpIdx ].iRect.iBr.iY - 2 * lH;
		//TInt stepY = lH / 3;

		TPoint p1( marginX, marginY );

		f1->DrawText( p1, id );

		// fils
		//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
	    GlUtils::DisableTexturing();
	    GlUtils::DisableTransparency();
	    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
	    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );   
	    glColor4ub( 0, 0, 0, 255 );

		// top of text
		glPushMatrix();
		GlUtils::Translate2D( TPoint( 0, p1.iY + lH ) );
		GlUtils::Scale2D( TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX, lH ) );
		iRect->Draw();
		glPopMatrix();

		// left of text
		glPushMatrix();
		GlUtils::Translate2D( TPoint( 0, p1.iY ) );
		GlUtils::Scale2D( TSize( p1.iX, lH ) );
		iRect->Redraw();
		glPopMatrix();

		// right of text
		glPushMatrix();
		GlUtils::Translate2D( TPoint( p1.iX + l1, p1.iY ) );
		GlUtils::Scale2D( TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX - ( p1.iX + l1 ), lH ) );
		iRect->Redraw();
		glPopMatrix();

		// bottom of text
		glPushMatrix();
		GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX, p1.iY - iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		iRect->Redraw();
		glPopMatrix();
		}
	else
		{
		iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
		glPushMatrix();
		GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnUpIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( iButtons[ KBtnUpIdx ].iRect.iTl.iX, iButtons[ KBtnUpIdx ].iRect.Height() ) );
		iRect->Draw();
		glPopMatrix();
		}
    glColor4ub( 255, 255, 255, 255 );
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
	iLevelIdFadeAnimator->Draw( TRect() );
	}

void CHermitMenuPageGame::Draw( const TRect aDrawRect )
	{
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
    
	DrawLevelStore();

	DrawFills();

	for ( TInt i = 0; i < iButtons.Count(); i++ )
		{
		if ( !iButtons[ i ].iVisible )
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
			iRect->Draw();
			glPopMatrix();
			continue;
			}
		if ( iButtons[ i ].iSelected )
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			iRect->Draw();
			glPopMatrix();
			}
		if ( i == KBtnLevelIdIdx )
			{ // only selection indication is drawing for level id button
			continue;
			}

#ifdef USE_ANIMATIONS
		if ( i < iAnimatedButtons.Count() )
			{
			iAnimatedButtons[ i ]->Draw( aDrawRect );
			}
		else
#endif
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			iButtons[ i ].Rotate();
			GlUtils::Scale2D( iButtons[ i ].iTextureSize );
			iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
			iRect->Draw();
			glPopMatrix();
			}
		}

	for ( TInt i = 0; i < iImages.Count(); i++ )
		{
		glPushMatrix();
		GlUtils::Translate2D( iImages[ i ].iRect.iTl );
		GlUtils::Scale2D( iImages[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iImages[ i ].iTextureId )->Use();
		if ( i == 0 )
		    {
		    iRect->Draw();
		    }
		else
		    {
		    iRect->Redraw();
		    }
		glPopMatrix();
		}

	glPushMatrix();
	GlUtils::Translate2D( TPoint( iBoardRect.iTl.iX + KButtonSize.iWidth, iBoardRect.iTl.iY + KButtonSize.iHeight ) );
	DrawLevel();
	glPopMatrix();

	DrawLevelId();

	//iFont1->DrawText( TPoint( 10, 100 ), _L("0123") );

	// button pressed animation
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );

	iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSettingsId )->Discard();
	}

TBool CHermitMenuPageGame::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	if ( ( iButtons[ KBtnDownIdx ].iSelected && iButtons[ KBtnDownIdx ].iVisible ) ||
	 	 ( iButtons[ KBtnUpIdx ].iSelected && iButtons[ KBtnUpIdx ].iVisible ) )
		{
		iFastScrollCounter += aTimeInterval.Int();
		if ( iFastScrollCounter >= KFastScrollStartTime )
			{
			iFastScrollCounter = KFastScrollStartTime - KFastScrollNextTime;

			TBool fsNow = EFalse;
			if ( !iFastScrollStarted )
				{
				iFastScrollStarted = ETrue;
				if ( iStoreTime > 0 && iStoreMoves > 0 )
					{
#ifdef USE_ANIMATIONS
					iStoreFadeAnimator->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
					iStoreTime = 0;
					iStoreMoves = 0;
#endif
					}
				PlaySound( KHermitSoundButton2PressId );
				fsNow = ETrue;
				}

			CHermitLevel* lvl = NULL;
			if ( iButtons[ KBtnDownIdx ].iSelected )
				{
				lvl = iCommonObjects.LevelProvider()->NextLevelInGroup( +1 );
				if ( !fsNow && lvl && !iCommonObjects.LevelProvider()->IsNextLevelInGroupAvailable( +1 ) )
					{
					PlaySound( KHermitSoundButton2PressId );
					UpdateLevelStoreAnimation();
					}
				}
			else
				{
				lvl = iCommonObjects.LevelProvider()->NextLevelInGroup( -1 );
				if ( !fsNow && lvl && !iCommonObjects.LevelProvider()->IsNextLevelInGroupAvailable( -1 ) )
					{
					PlaySound( KHermitSoundButton2PressId );
					UpdateLevelStoreAnimation();
					}
				}
		    if ( lvl )
				{
		    	iHermitLevel = lvl;
		    	CalculateLevelRect();
		    	UpdateDirectionButtons();
		    	//UpdateLevelStoreAnimation();
				}
			}
		}
	else
		{
		if ( iFastScrollStarted )
			{
			iFastScrollStarted = EFalse;
			UpdateLevelStoreAnimation();
			}
		iFastScrollCounter = 0;
		}


#ifdef USE_ANIMATIONS
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}

	for ( TInt i = 0; i < iAnimatedButtons.Count(); i++ )
		{
		iAnimatedButtons[ i ]->Update( aTimeInterval );
		}

	iStoreFadeAnimator->Update( aTimeInterval );
	iLevelIdFadeAnimator->Update( aTimeInterval );
#endif

	return ETrue;
	}

TBool CHermitMenuPageGame::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState != EIdle && iState != EEntering )
		{
		return EFalse;
		}

	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		iButtonDownPos = aPointerEvent.iPosition;
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = ETrue;
				return ETrue;
				}
			}
		}
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		if ( iBoardRectOnly.Contains( iButtonDownPos ) &&
			 iBoardRectOnly.Contains( aPointerEvent.iPosition ) )
			{
			CHermitLevel* lvl = NULL;
			TInt margin = 60;
			TInt detection = 120;
			if ( ABS( iButtonDownPos.iX - aPointerEvent.iPosition.iX ) < margin )
				{ // ruch w pionie
				if ( ABS( iButtonDownPos.iY - aPointerEvent.iPosition.iY ) >= detection )
					{
					if ( iButtonDownPos.iY > aPointerEvent.iPosition.iY )
						{ // ruch w dol
						lvl = iCommonObjects.LevelProvider()->NextLevelInGroup( -1 );
						}
					else
						{ // ruch w gore
						lvl = iCommonObjects.LevelProvider()->NextLevelInGroup();
						}
					}
				}
			else if ( ABS( iButtonDownPos.iY - aPointerEvent.iPosition.iY ) < margin )
				{ // ruch w poziomie
				if ( ABS( iButtonDownPos.iX - aPointerEvent.iPosition.iX ) >= detection )
					{
					if ( iButtonDownPos.iX > aPointerEvent.iPosition.iX )
						{ // ruch w prawo
						lvl = iCommonObjects.LevelProvider()->NextLevelGroup();
						}
					else
						{ // ruch w lewo
						lvl = iCommonObjects.LevelProvider()->NextLevelGroup( -1 );
						}
					}
				}

		    if ( lvl )
				{
		    	iHermitLevel = lvl;
		    	CalculateLevelRect();
		    	UpdateDirectionButtons();
		    	UpdateLevelStoreAnimation();
		    	// clean buttons
				for ( TInt i = 0; i < iButtons.Count(); i++ )
					{
					iButtons[ i ].iSelected = EFalse;
					}
				return ETrue;
				}
			}

		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iSelected &&
				 iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				if ( iButtons[ i ].iActionId )
					{
					iButtons[ i ].iSelected = EFalse;
					// button pressed -> start animation
#ifdef USE_ANIMATIONS
					iState = EAnimation;
					iAnimationButtonPressed->StartAnimation( &iButtons[ i ] );
#else
					iObserver->PageAction( this, iButtons[ i ].iActionId );
#endif

					PlaySound( KHermitSoundButtonPressId );

					return ETrue;
					}
				else if ( i == KBtnLevelIdIdx )
					{
					if ( iCommonObjects.Settings()->LevelIdVisible() )
						{
#ifdef USE_ANIMATIONS
						iLevelIdFadeAnimator->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
						iCommonObjects.Settings()->SetLevelIdVisible( EFalse );
#endif
						PlaySound( KHermitSoundMenuOutId );
						}
					else
						{
#ifdef USE_ANIMATIONS
						iLevelIdFadeAnimator->StartAnimation( CHermitAnimationFade::EFadeIn );
#else
#endif
						iCommonObjects.Settings()->SetLevelIdVisible( ETrue );
						PlaySound( KHermitSoundMenuInId );
						}
					}
				else
					{
					CHermitLevel* lvl = NULL;
					if ( i == KBtnRightIdx )
						{
					    lvl = iCommonObjects.LevelProvider()->NextLevelGroup();
						}
					else if ( i == KBtnLeftIdx )
						{
					    lvl = iCommonObjects.LevelProvider()->NextLevelGroup( -1 );
						}
					else if ( i == KBtnUpIdx )
						{
					    lvl = iCommonObjects.LevelProvider()->NextLevelInGroup( -1 );
						}
					else if ( i == KBtnDownIdx )
						{
					    lvl = iCommonObjects.LevelProvider()->NextLevelInGroup();
						}

				    if ( lvl )
						{
				    	iHermitLevel = lvl;
				    	CalculateLevelRect();
				    	UpdateDirectionButtons();
				    	UpdateLevelStoreAnimation();

				    	PlaySound( KHermitSoundButton2PressId );
						}
					}
				}
			}
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			iButtons[ i ].iSelected = EFalse;
			}
		}

	return EFalse;
	}

void CHermitMenuPageGame::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	//iState = EIdle;

	CHermitPageBase::AnimationFinished( aAnimation );

	if ( aAnimation == iAnimationButtonPressed )
		{
		THermitGuiButton* btn = iAnimationButtonPressed->AnimatedButton();
		if ( btn )
			{
			iObserver->PageAction( this, btn->iActionId );
			}
		}
	else if ( aAnimation == iStoreFadeAnimator )
		{
		if ( iStoreFadeAnimator->FadeType() == CHermitAnimationFade::EFadeOut )
			{
			iStoreTime = 0;
			iStoreMoves = 0;
			}
		}
	else if ( aAnimation == iLevelIdFadeAnimator )
		{
		if ( iLevelIdFadeAnimator->FadeType() == CHermitAnimationFade::EFadeOut )
			{
			iCommonObjects.Settings()->SetLevelIdVisible( EFalse );
			}
		}
	}

TBool CHermitMenuPageGame::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished();
	}

#ifndef _SGE_BADA_
void CHermitMenuPageGame::ShakeStarted()
    {
    }

void CHermitMenuPageGame::ShakeEnded( TBool aPassDuration )
    {
    if ( !aPassDuration )
        {
        return;
        }

    PlaySound( KHermitSoundButton2PressId );

    CHermitLevel* lvl = iCommonObjects.LevelProvider()->RandomNextLevel();
    if ( lvl )
        {
        iHermitLevel = lvl;
        CalculateLevelRect();
        UpdateDirectionButtons();
        UpdateLevelStoreAnimation();
        }
    }
#else
void CHermitMenuPageGame::OnSnapDetected(MotionSnapType snapType)
    {
    AppLog("OnSnapDetected\n");
    }

void CHermitMenuPageGame::OnDoubleTapDetected()
    {
    AppLog("OnDoubleTapDetected\n");
    }

void CHermitMenuPageGame::OnShakeDetected(MotionState motionState)
	{
	AppLog("OnShakeDetected\n");

    if ( motionState == MOTION_STARTED )
        {
        AppLog("STARTED\n");
        }
    else if ( motionState == MOTION_ENDED )
		{
		PlaySound( KHermitSoundButton2PressId );

        TInt64 t2;
        Osp::System::SystemTime::GetTicks( t2 );
        Osp::Base::Utility::Math::Srand( t2 );

		CHermitLevel* lvl = iCommonObjects.LevelProvider()->RandomNextLevel();
		if ( lvl )
			{
			iHermitLevel = lvl;
			CalculateLevelRect();
			UpdateDirectionButtons();
			UpdateLevelStoreAnimation();
			}
		}
	}
#endif
