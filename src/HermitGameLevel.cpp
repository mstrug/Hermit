/*
 ============================================================================
  Name        : HermitGameLevel.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */
#include "HermitGameLevel.h"
#include "HermitCommonObjects.h"
#include "HermitCompilationSettings.h"
#include "HermitConst.h"
#include "HermitLevel.h"
#include "HermitSettings.h"
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#ifdef _SGE_BADA_
#include <FBase.h>
#include <FIo.h>
using namespace Osp::Io;
#endif

#define ABS( x ) ( x > 0 ? (x) : -(x) )
#define SGN( x ) ( x > 0 ? 1 : ( x < 0 ? -1 : 0 ) )


const TInt KAnimationNextLevelStepTime = 20000; // 20ms

const GLfloat KAnimationFieldMin = 0.95;
const GLfloat KAnimationFieldMax = 1;
const GLfloat KAnimationFieldStep = 0.005;
const TInt KAnimationFieldStepTime = 100000; // 100ms


struct SGameStateFile_1_0
	{
	TUint32 iFileId;
	TUint8 iFileVersionMajor;
	TUint8 iFileVersionMinor;
	TInt iGameTime;
	TInt iGameMoves;
	TInt iLevelId;
	TBool iUndoPossible;
	};




CHermitGameLevel::CHermitGameLevel( CHermitCommonObjects& aCommonObjects ) : iCommonObjects( aCommonObjects ),
		iSelectedField( -1, -1 )
	{
	}

CHermitGameLevel::~CHermitGameLevel()
	{
    delete iLevel;
	delete iRect;
	delete iAnimationFade;
	delete iAnimationField;
	delete iAnimationHint;
	}

CHermitGameLevel* CHermitGameLevel::NewL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitGameLevel* self = new (ELeave) CHermitGameLevel( aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitGameLevel::ConstructL()
	{
	iRect = CGlObjectRect::NewL();

	// create temporary game level
//	static const TUint8 level[ 7 * 7 ] =
//	{	 0, 0, 2, 2, 2, 0, 0 ,
//		 0, 2, 2, 2, 2, 2, 0 ,
//		 2, 2, 2, 2, 2, 2, 2 ,
//		 2, 2, 2, 1, 2, 2, 2 ,
//		 2, 2, 2, 2, 2, 2, 2 ,
//		 0, 2, 2, 2, 2, 2, 0 ,
//		 0, 0, 2, 2, 2, 0, 0
//	};
	//iLevel = CHermitLevel::NewL( TSize( 7, 7 ), 0x01, level );

	iAnimationFade = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationFade->SetAnimationStepTime( KAnimationNextLevelStepTime );

	iAnimationField = CHermitAnimationTexZoom::NewL( this, iCommonObjects );
	iAnimationField->SetParameters( KAnimationFieldMin, KAnimationFieldMax, KAnimationFieldStep );
	iAnimationField->SetBouncing( ETrue );
	iAnimationField->SetAnimationStepTime( KAnimationFieldStepTime );

	iAnimationHint = CHermitAnimationTexZoom::NewL( this, iCommonObjects );
	iAnimationHint->SetParameters( KAnimationFieldMin, KAnimationFieldMax, KAnimationFieldStep );
	iAnimationHint->SetBouncing( ETrue );
	iAnimationHint->SetAnimationStepTime( KAnimationFieldStepTime );
	iAnimationHint->StartAnimation( CHermitAnimationTexZoom::EZoomIn );
	}

void CHermitGameLevel::LoadGameLevelL( const CHermitLevel &aLevel )
	{
	delete iLevel;
	iLevel = NULL;
	iLevel = aLevel.CopyL();
	iMovesCounter = 0;
	iGameFinished = EFalse;
	iUndoPossible = EFalse;
	iState = EIdle;
	}

void CHermitGameLevel::LoadGameLevelL( TSize aSize, const TAny* aFileData, TBool aDiagonalMoves )
	{
	delete iLevel;
	iLevel = NULL;
	iLevel = CHermitLevel::NewL( aSize, 0x01, CHermitLevel::EShapeSquare, aDiagonalMoves, aFileData );
	iMovesCounter = 0;
	iGameFinished = EFalse;
	iUndoPossible = EFalse;
	iState = EIdle;
	}

TPoint CHermitGameLevel::GetFieldPosition( TPoint aXYPos, TBool aCentered )
	{
	TInt x = iMargin.iWidth + (TInt)( (GLfloat)aXYPos.iX * iFieldSizeW );
	TInt y = iMargin.iHeight + (TInt)( (GLfloat)aXYPos.iY * iFieldSizeH );

	if ( aCentered )
		{
		x += (TInt)( iFieldSizeW / 2 );
		y += (TInt)( iFieldSizeH / 2 );
		}

	return TPoint( x, y );
	}

TInt CHermitGameLevel::MovesCounter() const
	{
	return iMovesCounter;
	}

CHermitLevel& CHermitGameLevel::Level() const
	{
	return *iLevel;
	}

void CHermitGameLevel::SetObserver( MHermitGameLevelObserver& aObserver )
	{
	iObserver = &aObserver;
	}

void CHermitGameLevel::Draw( const TRect /*aDrawRect*/ )
	{
	if ( iState == ENextLevelAnimation )
		{
		DrawNextLevelAnimation();
		return;
		}
	if ( !iLevel )
		{
		return;
		}

	DrawLevel( iLevel, iLevelRect, iMargin, iFieldSizeW, iFieldSizeH );
	}

void CHermitGameLevel::DrawLevel( CHermitLevel* aLevel, TRect aLevelRect, TSize aMargin, GLfloat aFieldSizeW, GLfloat aFieldSizeH )
	{
	TGlTexture* tex[ 5 ];
	tex[ CHermitLevel::EFieldNone ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId );
	tex[ CHermitLevel::EFieldEmpty ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEmptyId );
	tex[ CHermitLevel::EFieldItem ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldItemId );
	tex[ CHermitLevel::EFieldSelected ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldSelectedId );
	tex[ CHermitLevel::EFieldHint ] = iCommonObjects.TextureManager()->Texture( KHermitTexFieldHintId );
	TGlTexture* texGray = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEndId );
	TGlTexture* texGrayIndication = iCommonObjects.TextureManager()->Texture( KHermitTexFieldEndIndId );

	glPushMatrix();

	glTranslatef( aLevelRect.iTl.iX + aMargin.iWidth, aLevelRect.iTl.iY + aMargin.iHeight, 0 );

	glScalef( aFieldSizeW, aFieldSizeH, 0 );

	for ( TInt j = 0; j < aLevel->Size().iHeight; j++ )
	{
		for ( TInt i = 0; i < aLevel->Size().iWidth; i++ )
		{
			//if ( Osp::Base::Utility::Math::Rand() > Osp::Base::Utility::Math::RAND_VALUE_MAX / 2 )

			TInt ftype = aLevel->FieldC( i, j );
			TBool ffinish = aLevel->IsFieldFinish( TPoint( i, j ) );

			//AppLog( "%d %d: %d", i, j, ftype );

			if ( ffinish && ftype == CHermitLevel::EFieldEmpty && iCommonObjects.Settings()->OrangeIndicator() )
				{
				texGrayIndication->Use();
				( i == 0 && j == 0 ? iRect->Draw() : iRect->Redraw() );
				}


			if ( ftype == CHermitLevel::EFieldHint && !iCommonObjects.Settings()->GreenIndicator() )
				{
				tex[ CHermitLevel::EFieldEmpty ]->Use();
				}
			else
				{
				tex[ ftype ]->Use();
				}

			/*if ( iState == ENextLevelAnimation )
				{
				SetNextLevelAnimationFieldObject( aLevel->Field( i, j ) );
				}*/

			if ( ( ftype == CHermitLevel::EFieldHint && iCommonObjects.Settings()->GreenIndicator() ) ||
				 ftype == CHermitLevel::EFieldSelected )
				{
				if ( ftype == CHermitLevel::EFieldSelected )
					{
					iAnimationField->Draw( TRect() );
					}
				else
					{
					iAnimationHint->Draw( TRect() );
					}

				if ( ftype == CHermitLevel::EFieldSelected )
					{
					tex[ CHermitLevel::EFieldItem ]->Use();
					}
				else
					{
					tex[ CHermitLevel::EFieldEmpty ]->Use();
					}
				iRect->Draw();
				}
			else
				{
                ( i == 0 && j == 0 ? iRect->Draw() : iRect->Redraw() );
				}

			if ( ffinish && ( ftype != CHermitLevel::EFieldEmpty || !iCommonObjects.Settings()->OrangeIndicator() ) )
				{
				texGray->Use();
                ( i == 0 && j == 0 ? iRect->Draw() : iRect->Redraw() );
				}

			glTranslatef( 1, 0, 0 );
		}
		glTranslatef( -aLevel->Size().iWidth, 1, 0 );
	}

	glPopMatrix();


	// margins
	tex[ 0 ]->Use(); // black rect

	if ( aMargin.iWidth > 0 )
		{
		glPushMatrix();
		glTranslatef( aLevelRect.iTl.iX, aLevelRect.iTl.iY, 0 );
		glScalef( aMargin.iWidth, aLevelRect.Height(), 0 );
		iRect->Draw();
		glPopMatrix();
		glPushMatrix();
		glTranslatef( aLevelRect.iBr.iX - aMargin.iWidth, aLevelRect.iTl.iY, 0 );
		glScalef( aMargin.iWidth, aLevelRect.Height(), 0 );
		iRect->Draw();
		glPopMatrix();
		}
	if ( aMargin.iHeight > 0 )
		{
		glPushMatrix();
		glTranslatef( aLevelRect.iTl.iX, aLevelRect.iTl.iY, 0 );
		glScalef( aLevelRect.Width(), aMargin.iHeight, 0 );
		iRect->Draw();
		glPopMatrix();
		glPushMatrix();
		glTranslatef( aLevelRect.iTl.iX, aLevelRect.iBr.iY - aMargin.iHeight, 0 );
		glScalef( aLevelRect.Width(), aMargin.iHeight, 0 );
		iRect->Draw();
		glPopMatrix();
		}

	}

void CHermitGameLevel::PlaySound( TUint aSampleId )
	{
#ifdef USE_AUDIO
	iCommonObjects.AudioProvider()->Play( aSampleId, ESGEAudioRepeatNone );
#endif
	}

void CHermitGameLevel::PointerEvent( TRect aRect, TPoint aPos )
	{
	iPlaySoundAfterPointerEvent = EFalse;
	if ( iGameFinished )
		{
		return;
		}

	//AppLog( "r: (%d %d) (%d %d)", aRect.iTl.iX, aRect.iTl.iY, aRect.iBr.iX, aRect.iBr.iY );
	//AppLog( "select: %d %d", aPos.iX, aPos.iY );
	if ( aRect.Contains( aPos ) )
		{
		TInt x = (TInt)( (GLfloat)( aPos.iX - aRect.iTl.iX - iMargin.iWidth ) / iFieldSizeW );
		TInt y = (TInt)( (GLfloat)( aPos.iY - aRect.iTl.iY - iMargin.iHeight ) / iFieldSizeH );

		/*TInt pm = 2; // margin
		TInt x1 = ( aPos.iX + pm - aRect.iTl.iX - iMargin.iWidth ) / iFieldSizeW;
		TInt x2 = ( aPos.iX - pm - aRect.iTl.iX - iMargin.iWidth ) / iFieldSizeW;
		TInt y1 = ( aPos.iY + pm - aRect.iTl.iY - iMargin.iHeight ) / iFieldSizeH;
		TInt y2 = ( aPos.iY - pm - aRect.iTl.iY - iMargin.iHeight ) / iFieldSizeH;
		if ( !( x1 == x && x2 == x && y1 == y && y2 == y ) )
		{
			return;
		}*/

		//AppLog( "xy: %d %d", x, y );
		PointerEventXY( x, y );
		}
	else
		{
		Unselect();
		}
	}

void CHermitGameLevel::PointerEventXY( TInt aX, TInt aY )
	{
	if ( aX < iLevel->Size().iWidth && aY < iLevel->Size().iHeight && aX >= 0 && aY >= 0 )
		{
		if ( iLevel->FieldC( aX, aY ) == CHermitLevel::EFieldItem )
			{
			Select( TPoint( aX, aY ) );
			}
		else if ( iLevel->FieldC( aX, aY ) == CHermitLevel::EFieldSelected )
			{
			Unselect();
			}
		else if ( iLevel->FieldC( aX, aY ) == CHermitLevel::EFieldHint )
			{ // move taken
			PlaySound( KHermitSoundFieldPressId );
			iPlaySoundAfterPointerEvent = ETrue;

			iUndoFieldEmpty.SetXY( aX, aY );
			iUndoFieldItem1.SetXY( aX + SGN( iSelectedField.iX - aX ), aY + SGN( iSelectedField.iY - aY ) );
			iUndoFieldItem2 = iSelectedField;
			iUndoPossible = ETrue;

			iMovesCounter++;

			iLevel->SetFieldC( aX, aY, CHermitLevel::EFieldItem );
			iLevel->SetFieldC( aX + SGN( iSelectedField.iX - aX ), aY + SGN( iSelectedField.iY - aY ), CHermitLevel::EFieldEmpty );
			iLevel->SetFieldC( iSelectedField, CHermitLevel::EFieldEmpty );
			Unhint();
			iSelectedField.SetXY( -1, -1 );

			CheckGameEnd();
			}
		else
			{
			Unselect();
			}
		}
	else
		{
		Unselect();
		}
	}


void CHermitGameLevel::Select( TPoint aPos )
	{
	PlaySound( KHermitSoundFieldPressId );
	iPlaySoundAfterPointerEvent = ETrue;

	if ( iSelectedField.iX == -1 && iSelectedField.iY == -1 )
		{
		iAnimationField->StartAnimation( CHermitAnimationTexZoom::EZoomIn );
		iAnimationHint->StartAnimation( CHermitAnimationTexZoom::EZoomIn );
		}
	Unselect();

	iLevel->SetFieldC( aPos, CHermitLevel::EFieldSelected );
	iSelectedField = aPos;

	if ( iLevel->FieldCheck( aPos.iX + 1, aPos.iY ) == CHermitLevel::EFieldItem &&
		 iLevel->FieldCheck( aPos.iX + 2, aPos.iY ) == CHermitLevel::EFieldEmpty )
		{
		iLevel->SetFieldC( aPos.iX + 2, aPos.iY, CHermitLevel::EFieldHint );
		}
	if ( iLevel->FieldCheck( aPos.iX - 1, aPos.iY ) == CHermitLevel::EFieldItem &&
		 iLevel->FieldCheck( aPos.iX - 2, aPos.iY ) == CHermitLevel::EFieldEmpty )
		{
		iLevel->SetFieldC( aPos.iX - 2, aPos.iY, CHermitLevel::EFieldHint );
		}
	if ( iLevel->FieldCheck( aPos.iX, aPos.iY + 1 ) == CHermitLevel::EFieldItem &&
		 iLevel->FieldCheck( aPos.iX, aPos.iY + 2 ) == CHermitLevel::EFieldEmpty )
		{
		iLevel->SetFieldC( aPos.iX, aPos.iY + 2, CHermitLevel::EFieldHint );
		}
	if ( iLevel->FieldCheck( aPos.iX, aPos.iY - 1 ) == CHermitLevel::EFieldItem &&
		 iLevel->FieldCheck( aPos.iX, aPos.iY - 2 ) == CHermitLevel::EFieldEmpty )
		{
		iLevel->SetFieldC( aPos.iX, aPos.iY - 2, CHermitLevel::EFieldHint );
		}

	if ( iLevel->DiagonalMoves() )
		{
		if ( iLevel->FieldCheck( aPos.iX + 1, aPos.iY + 1 ) == CHermitLevel::EFieldItem &&
			 iLevel->FieldCheck( aPos.iX + 2, aPos.iY + 2 ) == CHermitLevel::EFieldEmpty )
			{
			iLevel->SetFieldC( aPos.iX + 2, aPos.iY + 2, CHermitLevel::EFieldHint );
			}
		if ( iLevel->FieldCheck( aPos.iX + 1, aPos.iY - 1 ) == CHermitLevel::EFieldItem &&
			 iLevel->FieldCheck( aPos.iX + 2, aPos.iY - 2 ) == CHermitLevel::EFieldEmpty )
			{
			iLevel->SetFieldC( aPos.iX + 2, aPos.iY - 2, CHermitLevel::EFieldHint );
			}
		if ( iLevel->FieldCheck( aPos.iX - 1, aPos.iY + 1 ) == CHermitLevel::EFieldItem &&
			 iLevel->FieldCheck( aPos.iX - 2, aPos.iY + 2 ) == CHermitLevel::EFieldEmpty )
			{
			iLevel->SetFieldC( aPos.iX - 2, aPos.iY + 2, CHermitLevel::EFieldHint );
			}
		if ( iLevel->FieldCheck( aPos.iX - 1, aPos.iY - 1 ) == CHermitLevel::EFieldItem &&
			 iLevel->FieldCheck( aPos.iX - 2, aPos.iY - 2 ) == CHermitLevel::EFieldEmpty )
			{
			iLevel->SetFieldC( aPos.iX - 2, aPos.iY - 2, CHermitLevel::EFieldHint );
			}
		}
	}

void CHermitGameLevel::Unselect()
	{
	if ( iSelectedField.iX >= 0 && iSelectedField.iX < iLevel->Size().iWidth &&
		 iSelectedField.iY >= 0 && iSelectedField.iY < iLevel->Size().iHeight &&
		 iLevel->FieldC( iSelectedField ) == CHermitLevel::EFieldSelected )
		{
		Unhint();
		iLevel->SetFieldC( iSelectedField, CHermitLevel::EFieldItem );
		iSelectedField.SetXY( -1, -1 );
		}
	}

void CHermitGameLevel::Unhint()
	{
	if ( iLevel->FieldCheck( iSelectedField.iX + 2, iSelectedField.iY ) == CHermitLevel::EFieldHint )
		{
		iLevel->SetFieldC( iSelectedField.iX + 2, iSelectedField.iY, CHermitLevel::EFieldEmpty );
		}
	if ( iLevel->FieldCheck( iSelectedField.iX - 2, iSelectedField.iY ) == CHermitLevel::EFieldHint )
		{
		iLevel->SetFieldC( iSelectedField.iX - 2, iSelectedField.iY, CHermitLevel::EFieldEmpty );
		}
	if ( iLevel->FieldCheck( iSelectedField.iX, iSelectedField.iY + 2 ) == CHermitLevel::EFieldHint )
		{
		iLevel->SetFieldC( iSelectedField.iX, iSelectedField.iY + 2, CHermitLevel::EFieldEmpty );
		}
	if ( iLevel->FieldCheck( iSelectedField.iX, iSelectedField.iY - 2 ) == CHermitLevel::EFieldHint )
		{
		iLevel->SetFieldC( iSelectedField.iX, iSelectedField.iY - 2, CHermitLevel::EFieldEmpty );
		}

	if ( iLevel->DiagonalMoves() )
		{
		if ( iLevel->FieldCheck( iSelectedField.iX + 2, iSelectedField.iY + 2 ) == CHermitLevel::EFieldHint )
			{
			iLevel->SetFieldC( iSelectedField.iX + 2, iSelectedField.iY + 2, CHermitLevel::EFieldEmpty );
			}
		if ( iLevel->FieldCheck( iSelectedField.iX + 2, iSelectedField.iY - 2 ) == CHermitLevel::EFieldHint )
			{
			iLevel->SetFieldC( iSelectedField.iX + 2, iSelectedField.iY - 2, CHermitLevel::EFieldEmpty );
			}
		if ( iLevel->FieldCheck( iSelectedField.iX - 2, iSelectedField.iY + 2 ) == CHermitLevel::EFieldHint )
			{
			iLevel->SetFieldC( iSelectedField.iX - 2, iSelectedField.iY + 2, CHermitLevel::EFieldEmpty );
			}
		if ( iLevel->FieldCheck( iSelectedField.iX - 2, iSelectedField.iY - 2 ) == CHermitLevel::EFieldHint )
			{
			iLevel->SetFieldC( iSelectedField.iX - 2, iSelectedField.iY - 2, CHermitLevel::EFieldEmpty );
			}
		}
	}

const TSize CHermitGameLevel::Size() const
	{
	return iLevel->Size();
	}

void CHermitGameLevel::SetLevelRect( TRect aRect, TSize aSize )
	{
	iLevelRect = aRect;

	iFieldSizeW = (TInt)((GLfloat)iLevelRect.Width() / (GLfloat)aSize.iWidth);
	iFieldSizeH = (TInt)((GLfloat)iLevelRect.Height() / (GLfloat)aSize.iHeight);

	if ( iFieldSizeW > 64 ) iFieldSizeW = 64;
	if ( iFieldSizeH > 64 ) iFieldSizeH = 64;
//	if ( iFieldSizeW < 64 ) iFieldSizeW = 52;
//	if ( iFieldSizeH < 64 ) iFieldSizeH = 52;

	if ( ((TInt)iFieldSizeW) % 2 != 0 ) iFieldSizeW -= 1;
	if ( ((TInt)iFieldSizeH) % 2 != 0 ) iFieldSizeH -= 1;

	if ( iFieldSizeW != iFieldSizeH )
		{
		if ( iFieldSizeW < iFieldSizeH )
			{
			iFieldSizeH = iFieldSizeW;
			}
		else
			{
			iFieldSizeW = iFieldSizeH;
			}
		}

	iMargin.iWidth = ( iLevelRect.Width() - (TInt) ( iFieldSizeW * (GLfloat) aSize.iWidth ) ) / 2;
	iMargin.iHeight = ( iLevelRect.Height() - (TInt) ( iFieldSizeH * (GLfloat) aSize.iHeight ) ) / 2;
	}

TRect CHermitGameLevel::BoardRect()
	{
	TRect ret = iLevelRect;
	ret.Grow( -iMargin.iWidth, -iMargin.iHeight );
	return ret;
	}

void CHermitGameLevel::Undo()
	{
	if ( iUndoPossible )
		{
		iLevel->SetFieldC( iUndoFieldEmpty, CHermitLevel::EFieldEmpty );
		iLevel->SetFieldC( iUndoFieldItem1, CHermitLevel::EFieldItem );
		iLevel->SetFieldC( iUndoFieldItem2, CHermitLevel::EFieldItem );
		Unhint();
		iSelectedField.SetXY( -1, -1 );
		iUndoPossible = EFalse;
		}
	}

TBool CHermitGameLevel::UndoPossible() const
	{
	return iUndoPossible;
	}

TBool CHermitGameLevel::GameFinished() const
	{
	return iGameFinished;
	}

TBool CHermitGameLevel::PlaySoundAfterPointerEvent() const
	{
	return iPlaySoundAfterPointerEvent;
	}

void CHermitGameLevel::CheckGameEnd()
	{
	if ( iLevel->CheckGameEndWin() )
		{
		iState = EGameFinished;
		//iUndoPossible = EFalse;
		iGameFinished = ETrue;
		if ( iObserver )
			{
			iObserver->GameFinished( ETrue );
			}
		}
	else if ( iLevel->CheckGameEndLose() )
		{
		iState = EGameFinished;
		iUndoPossible = EFalse;
		iGameFinished = ETrue;
		if ( iObserver )
			{
			iObserver->GameFinished( EFalse );
			}
		}
	}

void CHermitGameLevel::AnimateNextLevel( CHermitLevel* aLevel, TRect aNextLevelRect )
	{
	iNextLevel = aLevel;
	iNextLevelRect = aNextLevelRect;
	iState = ENextLevelAnimation;
	iNextLevelAnimationPhase = 0;

	if ( iNextLevel->Size() == iLevel->Size() )
		{
		iNextLevelAnimationType = ENLASameSize;
		// phase 0:  fade out current level item elements (and field end elements also)
		// phase 1:  fade in next level items (and new fields)
		}
	else
		{
		iNextLevelAnimationType = ENLASFullFade;
		// phase 0:  fade out current level item elements (and field end elements also)
		// phase 1:  fade out all to black
		// phase 2:  fade in whole next level
		}

	iNextLevelAnimationType = ENLASimple;
	// phase 0:  fade out current level
	// phase 1:  fade in next level
#ifdef USE_ANIMATIONS
	iAnimationFade->SetFadeRect( iLevelRect );
	iAnimationFade->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
	AnimationFinished( iAnimationFade );
#endif
	}

void CHermitGameLevel::DrawNextLevelAnimation()
	{
	if ( iNextLevelAnimationType == ENLASameSize )
		{
		}
	else if ( iNextLevelAnimationType == ENLASFullFade )
		{
		}
	else // ENLASimple
		{
		if ( iNextLevelAnimationPhase == 0 )
			{
			DrawLevel( iLevel, iLevelRect, iMargin, iFieldSizeW, iFieldSizeH );
			iAnimationFade->Draw( TRect() );
			}
		else
			{
			DrawLevel( iNextLevel, iNextLevelRect, iMargin, iFieldSizeW, iFieldSizeH );
			iAnimationFade->Draw( TRect() );
			}
		}
	}

TBool CHermitGameLevel::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
#ifdef USE_ANIMATIONS
	iAnimationField->Update( aTimeInterval );
	if ( iCommonObjects.Settings()->GreenIndicator() )
		{
		iAnimationHint->Update( aTimeInterval );
		}
#endif

	if ( iState != ENextLevelAnimation )
		{
		return EFalse;
		}

#ifdef USE_ANIMATIONS
	return iAnimationFade->Update( aTimeInterval );
#else
	return EFalse;
#endif
	}

void CHermitGameLevel::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	if ( aAnimation == iAnimationFade )
		{
		if ( iNextLevelAnimationPhase == 0 )
			{
			iNextLevelAnimationPhase = 1;
			SetLevelRect( iNextLevelRect, iNextLevel->Size() );
#ifdef USE_ANIMATIONS
			iAnimationFade->SetFadeRect( iNextLevelRect );
			iAnimationFade->StartAnimation( CHermitAnimationFade::EFadeIn );
#else
			AnimationFinished( iAnimationFade );
#endif
			}
		else // iNextLevelAnimationPhase == 1
			{
			LoadGameLevelL( *iNextLevel );

			iObserver->GameNextLevelAnimationFinished();
			iState = EIdle;

			iNextLevel = NULL;
			}
		}
	}

TBool CHermitGameLevel::SaveToFile( const TDesC& aFileName, TInt aGameTime )
	{
//
// File structure (1.0):
//
//  Game state header
//	Level header
//  Level data
//
	TSize ls = iLevel->Size();
	Unselect();

    SGameStateFile_1_0 header =
        {
        0x56415348, // HSAV
        0x01,
        0x00,
        aGameTime,
        iMovesCounter,
        iLevel->LevelId(),
        iUndoPossible
        };

    SLevelHeader_1_1 lheader =
        {
        0,
        ls.iWidth,
        ls.iHeight,
        iLevel->FieldShape(),
        iLevel->DiagonalMoves()
        };
    
#ifndef _SGE_BADA_
    RFile file;
    
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( aFileName );

    TInt err = file.Replace( iCommonObjects.FileProvider()->Fs(), fn, EFileWrite );
    if ( err != KErrNone )
        {
        file.Close();
        return EFalse;
        }
    
    TPtrC8 ptrHdr( (TUint8*)&header, sizeof( SGameStateFile_1_0 ) );
    if ( file.Write( ptrHdr ) != KErrNone )
        {
        file.Close();
        return EFalse;
        }

    TPtrC8 ptrLvl( (TUint8*)&lheader, sizeof( SLevelHeader_1_1 ) );
    if ( file.Write( ptrLvl ) != KErrNone )
        {
        file.Close();
        return EFalse;
        }

    TPtrC8 ptrData( iLevel->Data(), ls.iWidth * ls.iHeight );
    if ( file.Write( ptrData ) != KErrNone )
        {
        file.Close();
        return EFalse;
        }

    if ( iUndoPossible )
        {
        const TInt KPointCnt = 6;
        TInt tmp[ KPointCnt ] = { iUndoFieldEmpty.iX, iUndoFieldEmpty.iY,
                                  iUndoFieldItem1.iX, iUndoFieldItem1.iY,
                                  iUndoFieldItem2.iX, iUndoFieldItem2.iY };
        TPtrC8 ptrUndo( (TUint8*) &tmp, sizeof( TInt ) * KPointCnt );
        if ( file.Write( ptrUndo ) != KErrNone )
            {
            file.Close();
            return EFalse;
            }
        }
    
    file.Close();
    
#else
	AppLog( "Saving state to file" );
	File* file = new File();
	if ( file->Construct( aFileName, L"w" ) != E_SUCCESS )
		{
		AppLog( "CHermitGameLevel::SaveToFile file ERROR" );
		delete file;
		return EFalse;
		}

	if ( file->Write( (TAny*) &header, sizeof( SGameStateFile_1_0 ) ) != E_SUCCESS )
		{
		AppLog( "CHermitGameLevel::SaveToFile file write ERROR" );
		delete file;
		return EFalse;
		}

	if ( file->Write( (TAny*) &lheader, sizeof( SLevelHeader_1_1 ) ) != E_SUCCESS )
		{
		AppLog( "CHermitGameLevel::SaveToFile file write 2 ERROR" );
		delete file;
		return EFalse;
		}

	if ( file->Write( (TAny*) iLevel->Data(), ls.iWidth * ls.iHeight ) != E_SUCCESS )
		{
		AppLog( "CHermitGameLevel::SaveToFile file write 3 ERROR" );
		delete file;
		return EFalse;
		}

	if ( iUndoPossible )
		{
		const TInt KPointCnt = 6;
		TInt tmp[ KPointCnt ] = { iUndoFieldEmpty.iX, iUndoFieldEmpty.iY,
								  iUndoFieldItem1.iX, iUndoFieldItem1.iY,
								  iUndoFieldItem2.iX, iUndoFieldItem2.iY };
		if ( file->Write( (TAny*) &tmp, sizeof( TInt ) * KPointCnt ) != E_SUCCESS )
			{
			AppLog( "CHermitGameLevel::SaveToFile file write 4 ERROR" );
			delete file;
			return EFalse;
			}
		}

	delete file;
#endif
	return ETrue;
	}

TBool CHermitGameLevel::RestoreFromFile( const TDesC& aFileName, TInt& aGameTime )
	{
#ifndef _SGE_BADA_
    RFile file;
    
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( aFileName );

    TInt err = file.Open( iCommonObjects.FileProvider()->Fs(), fn, EFileRead | EFileShareAny );
    if ( err != KErrNone )
        {
        file.Close();
        return EFalse;
        }
    
    SGameStateFile_1_0 header;
    TPtr8 ptrHeader( (TUint8*)&header, sizeof( SGameStateFile_1_0 ) );
    err = file.Read( ptrHeader );
    if ( err )
        {
        file.Close();
        return EFalse;
        }

    if ( header.iFileId != 0x56415348 ) // HSAV -> VASH
        {
        file.Close();
        return EFalse;
        }
    if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
        {
        file.Close();
        return EFalse;
        }
    
    aGameTime = header.iGameTime;
    iMovesCounter = header.iGameMoves;

    SLevelHeader_1_1 hdrData = { 0 };
    TPtr8 ptrHeaderData( (TUint8*)&hdrData, sizeof( SLevelHeader_1_1 ) );
    err = file.Read( ptrHeaderData );
    if ( err )
        {
        file.Close();
        return EFalse;
        }

    TInt len = hdrData.iWidth * hdrData.iHeight;
    TUint8* data = new TUint8[ len ];
    if ( !data )
        {
        file.Close();
        return EFalse;
        }
    TPtr8 ptrData( data, len );
    err = file.Read( ptrData );
    if ( err || ptrData.Length() != len )
        {
        file.Close();
        delete [] data;
        return EFalse;
        }

    CHermitLevel *tmpLvl = NULL;
    TRAP_IGNORE( tmpLvl = CHermitLevel::NewL( TSize( hdrData.iWidth, hdrData.iHeight ), header.iLevelId, (CHermitLevel::TFieldShape) hdrData.iFieldShape, hdrData.iDiagonalMoves, (TAny*) data ) );
    if ( !tmpLvl )
        {
        file.Close();
        delete [] data;
        return EFalse;
        }
    delete iLevel;
    iLevel = tmpLvl;
    delete [] data;
    
    if ( header.iUndoPossible )
        {
        const TInt KPointCnt = 6;
        TInt tmp[ KPointCnt ];
        
        TPtr8 ptrTmp( (TUint8*) &tmp, sizeof( TInt ) * KPointCnt );
        err = file.Read( ptrTmp );
        if ( ptrTmp.Length() == sizeof( TInt ) * KPointCnt )
            {
            iUndoFieldEmpty.SetXY( tmp[ 0 ], tmp[ 1 ] );
            iUndoFieldItem1.SetXY( tmp[ 2 ], tmp[ 3 ] );
            iUndoFieldItem2.SetXY( tmp[ 4 ], tmp[ 5 ] );
            iUndoPossible = ETrue;
            }
        }

    file.Close();
    
#else
	AppLog( "Restoring state from file" );

	File* file = new File();
	if ( file->Construct( aFileName, L"r" ) != E_SUCCESS )
		{
		AppLog( "CHermitGameLevel::ReadFromFile file ERROR" );
		delete file;
		return EFalse;
		}

	SGameStateFile_1_0 header;
	if ( file->Read( (TAny*) &header, sizeof( SGameStateFile_1_0 ) ) != sizeof( SGameStateFile_1_0 ) )
		{
		AppLog( "CHermitGameLevel::ReadFromFile file read ERROR" );
		delete file;
		return EFalse;
		}

	if ( header.iFileId != 0x56415348 ) // HSAV -> VASH
		{
		AppLog( "CHermitGameLevel::ReadFromFile file read ID ERROR" );
		delete file;
		return EFalse;
		}
	if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
		{
		AppLog( "CHermitGameLevel::ReadFromFile file read ver ERROR" );
		delete file;
		return EFalse;
		}

	aGameTime = header.iGameTime;
	iMovesCounter = header.iGameMoves;

	SLevelHeader_1_1 hdrData = { 0 };

	TInt read = file->Read( (TAny*) &hdrData, sizeof( SLevelHeader_1_1 ) );
	if ( read != sizeof( SLevelHeader_1_1 ) )
		{
		AppLog( "CHermitGameLevel::ReadFromFile file read 1 level size ERROR %d", read );
		delete file;
		return EFalse;
		}

	TInt len = hdrData.iWidth * hdrData.iHeight;
	TUint8* data = new TUint8[ len ];
	CleanupStack::PushL( data );
	read = file->Read( (TAny*) data, len );
	if ( read != len )
		{
		AppLog( "CHermitGameLevel::ConstructL file read 2 level data ERROR" );
		delete file;
		CleanupStack::Pop();
		delete [] data;
		return EFalse;
		}

	delete iLevel;
	iLevel = NULL;
	iLevel = CHermitLevel::NewL( TSize( hdrData.iWidth, hdrData.iHeight ), header.iLevelId, (CHermitLevel::TFieldShape) hdrData.iFieldShape, hdrData.iDiagonalMoves, (TAny*) data );
	CleanupStack::Pop(); // data
	delete [] data;

	if ( header.iUndoPossible )
		{
		const TInt KPointCnt = 6;
		TInt tmp[ KPointCnt ];

		read = file->Read( (TAny*) &tmp, sizeof( TInt ) * KPointCnt );
		if ( read != sizeof( TInt ) * KPointCnt )
			{
			AppLog( "CHermitGameLevel::ReadFromFile file read 3 level size ERROR %d", read );
			}
		else
			{
			iUndoFieldEmpty.SetXY( tmp[ 0 ], tmp[ 1 ] );
			iUndoFieldItem1.SetXY( tmp[ 2 ], tmp[ 3 ] );
			iUndoFieldItem2.SetXY( tmp[ 4 ], tmp[ 5 ] );
			iUndoPossible = ETrue;
			}
		}

	delete file;

#endif

	return ETrue;
	}


