/*
 ============================================================================
  Name        : HermitGameLevel.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITGAMELEVEL_H_
#define HERMITGAMELEVEL_H_

#include <e32def.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include "HermitAnimationFade.h"
#include "HermitAnimationTexZoom.h"
#ifdef _SGE_BADA_
#include <FGraphicsOpengl.h>
using namespace Osp::Graphics::Opengl;
#endif

class CHermitLevel;
class CHermitCommonObjects;
class CGlObjectRect;


class MHermitGameLevelObserver
	{
public:

	virtual void GameFinished( TBool aWin ) = 0;

	virtual void GameNextLevelAnimationFinished() = 0;

	};



class CHermitGameLevel : public CBase, public MSGEGlDrawableObject,
						 public MSGEUpdatableObject, public MHermitAnimationObserver
	{

	enum TGameLevelState
	{
		ELoading,
		EIdle,
		ESelecting,
		EUnselecting,
		ESelAndUnsel,
		EGameFinished,
		ENextLevelAnimation,
		EUnloading
	};

	enum TNLAnimationType
	{
		ENLASameSize,
		ENLASFullFade,
		ENLASimple
	};

public:

	virtual ~CHermitGameLevel();

	static CHermitGameLevel* NewL( CHermitCommonObjects& aCommonObjects );

	// creates copy of passed level
	void LoadGameLevelL( const CHermitLevel &aLevel );

	void LoadGameLevelL( TSize aSize, const TAny* aFileData, TBool aDiagonalMoves = EFalse );

	void PointerEvent( TRect aRect, TPoint aPos );

	void PointerEventXY( TInt aX, TInt aY );

	void Select( TPoint aPos );

	void Unselect();

	void Unhint();

	const TSize Size() const;

	void SetLevelRect( TRect aRect, TSize aSize );

	// level rect - margins
	TRect BoardRect();

	void Undo();

	TBool UndoPossible() const;

	TBool GameFinished() const;

	TBool PlaySoundAfterPointerEvent() const;

	TPoint GetFieldPosition( TPoint aXYPos, TBool aCentered = EFalse );

	TInt MovesCounter() const;

	CHermitLevel& Level() const;

	void SetObserver( MHermitGameLevelObserver& aObserver );

	void AnimateNextLevel( CHermitLevel* aLevel, TRect aNextLevelRect );

	// used when saving state on close:
	TBool SaveToFile( const TDesC& aFileName, TInt aGameTime );

	TBool RestoreFromFile( const TDesC& aFileName, TInt& aGameTime );

private:

	CHermitGameLevel( CHermitCommonObjects& aCommonObjects );

	void ConstructL();

	void CheckGameEnd();

	void DrawLevel( CHermitLevel* aLevel, TRect aLevelRect, TSize aMargin, GLfloat aFieldSizeW, GLfloat aFieldSizeH );

	void DrawNextLevelAnimation();

	void PlaySound( TUint aSampleId );

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

public: // from MSGEUpdatableObject

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

public: // MHermitAnimationObserver

    void AnimationFinished( CHermitAnimationBase* aAnimation );

private: // data

	CHermitCommonObjects& iCommonObjects;

	MHermitGameLevelObserver* iObserver;

	CHermitLevel *iLevel; // owned

	CGlObjectRect *iRect; // owned

	TPoint iSelectedField;

	GLfloat iFieldSizeW;
	GLfloat iFieldSizeH;

	TRect iLevelRect;

	TSize iMargin;

	TGameLevelState iState;

	TPoint iUndoFieldEmpty;
	TPoint iUndoFieldItem1;
	TPoint iUndoFieldItem2;
	TBool iUndoPossible;

	TInt iMovesCounter;

	TBool iGameFinished;

	TInt iSwitchingLevelCounter;

	// used in next level animation

	CHermitLevel *iNextLevel; // not owned

	TRect iNextLevelRect;

	TNLAnimationType iNextLevelAnimationType;

	TInt iNextLevelAnimationPhase;

	CHermitAnimationFade* iAnimationFade; // owned

	CHermitAnimationTexZoom* iAnimationField; // owned

	CHermitAnimationTexZoom* iAnimationHint; // owned

	TBool iPlaySoundAfterPointerEvent;

	};

#endif /* HERMITGAMELEVEL_H_ */
