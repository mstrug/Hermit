/*
 ============================================================================
  Name        : HermitLevel.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITLEVEL_H_
#define HERMITLEVEL_H_

#include <e32def.h>
#include <e32base.h>


// used in file opertions
struct SLevelHeader_1_1
	{
	TUint8 iId;
	TUint8 iWidth;
	TUint8 iHeight;
	TUint8 iFieldShape;
	TUint8 iDiagonalMoves;
	};



class CHermitLevel : public CBase
	{
public:

	enum TFieldType
	{
		EFieldNone,
		EFieldEmpty,
		EFieldItem,
		EFieldSelected,
		EFieldHint,
		EFieldFinish = 0x10
	};

	enum TFieldShape
	{
		EShapeSquare,
		EShapeDiamond
	};

public:

	virtual ~CHermitLevel();

	static CHermitLevel* NewL( TSize aSize, TInt aLevelId, TFieldShape aFieldShape, TBool aDiagonalMoves, const TAny* aFileData );

	TInt LevelId() const;

	const TSize& Size() const;

	TFieldShape FieldShape() const;

	// masks out finish field
	inline const TFieldType FieldC( TInt aX, TInt aY ) const;

	// masks out finish field
	inline const TFieldType FieldC( TPoint aPos ) const;

	// masks out finish field
	inline void SetFieldC( TInt aX, TInt aY, TFieldType aType );

	// masks out finish field
	inline void SetFieldC( TPoint aPos, TFieldType aType );

	// RAW field type (including finish field)
	inline TFieldType Field( TInt aX, TInt aY ) const;

	// RAW field type (including finish field)
	inline TFieldType Field( TPoint aPos ) const;

	// checks if field is a finish field
	inline TBool IsFieldFinish( TPoint aPos ) const;

	// masks out finish field
	TFieldType FieldCheck( TInt aX, TInt aY ) const;

	CHermitLevel* CopyL() const;

	TBool CheckGameEndWin();

	TBool CheckGameEndLose();

	TBool DiagonalMoves() const;

	const TUint8* Data() const;

private:

	CHermitLevel( TSize aSize, TInt aLevelId, TFieldShape aFieldShape, TBool aDiagonalMoves );

	void ConstructL( const TAny* aFileData );

private: // data

	TSize iSize;

	TInt iLevelId;

	TFieldShape iFieldShape;

	TBool iDiagonalMoves;

	TUint8* iData; // owned

	TInt iFinishFieldsCount;

	};


#include "HermitLevel.inl"

#endif /* HERMITLEVEL_H_ */
