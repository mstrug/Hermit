/*
 ============================================================================
  Name        : HermitLevel.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitLevel.h"



CHermitLevel::CHermitLevel( TSize aSize, TInt aLevelId, TFieldShape aFieldShape, TBool aDiagonalMoves )
		: iSize( aSize ), iLevelId( aLevelId ), iFieldShape( aFieldShape ), iDiagonalMoves( aDiagonalMoves )
	{
	}

CHermitLevel::~CHermitLevel()
	{
	User::Free( iData );
	}

CHermitLevel* CHermitLevel::NewL( TSize aSize, TInt aLevelId, TFieldShape aFieldShape, TBool aDiagonalMoves, const TAny* aFileData )
	{
	CHermitLevel* self = new (ELeave) CHermitLevel( aSize, aLevelId, aFieldShape, aDiagonalMoves );
	CleanupStack::PushL( self );
	self->ConstructL( aFileData );
	CleanupStack::Pop();
	return self;
	}

void CHermitLevel::ConstructL( const TAny* aFileData )
	{
	iData = (TUint8*) User::Alloc( iSize.iWidth * iSize.iHeight );
	Mem::FillZ( iData, iSize.iWidth * iSize.iHeight );
	Mem::Copy( iData, aFileData, iSize.iWidth * iSize.iHeight );

//	AppLog( "Size: %d %d\n", iSize.iWidth, iSize.iHeight );

	for ( TInt j = 0; j < iSize.iHeight; j++ )
		for ( TInt i = 0; i < iSize.iWidth; i++ )
		{
			//AppLog( "B: %d %d\t%d\n", i, j, Field( i, j ) );
			if ( IsFieldFinish( TPoint( i, j ) ) )
			{
				iFinishFieldsCount++;
			}
		}
	}

TInt CHermitLevel::LevelId() const
	{
	return iLevelId;
	}

const TSize& CHermitLevel::Size() const
	{
	return iSize;
	}

CHermitLevel::TFieldShape CHermitLevel::FieldShape() const
	{
	return iFieldShape;
	}

CHermitLevel::TFieldType CHermitLevel::FieldCheck( TInt aX, TInt aY ) const
	{
	if ( aX < 0 || aX >= iSize.iWidth || aY < 0 || aY >= iSize.iHeight )
		{
		return EFieldNone;
		}
	return (TFieldType) ( Field( aX, aY ) & KHermitLevelFieldTypeMask );
	}

CHermitLevel* CHermitLevel::CopyL() const
	{
	CHermitLevel* copy = CHermitLevel::NewL( iSize, iLevelId, iFieldShape, iDiagonalMoves, iData );
//	for ( TInt j = 0; j < copy->iSize.iHeight; j++ )
//		for ( TInt i = 0; i < copy->iSize.iWidth; i++ )
//		{
//			AppLog( "C: %d %d\t%d\n", i, j, copy->Field( i, j ) );
//		}
	return copy;
	}

TBool CHermitLevel::CheckGameEndWin()
	{
	TInt cnt = 0;
	TInt cntF = 0;

	for ( TInt j = 0; j < iSize.iHeight; j++ )
		{
		for ( TInt i = 0; i < iSize.iWidth; i++ )
			{
			TFieldType ft = FieldC( i, j );

			if ( ft == EFieldNone )
				{
				continue;
				}

			if ( IsFieldFinish( TPoint( i, j ) ) )
				{
				if ( ft != EFieldItem )
					{
					return EFalse;
					}
				else
					{
					cntF++;
					}
				}
			else
				{
				if ( ft == EFieldItem )
					{
					if ( cntF > 0 )
						{
						return EFalse;
						}
					cnt++;
					if ( cnt > 1 )
						{
						return EFalse;
						}
					}
				}
			}
		}

	if ( cntF > 0 && cnt > 0 )
		{
		return EFalse;
		}

	return ETrue;
	}

TBool CHermitLevel::CheckGameEndLose()
	{
	// 1. check if items count is not lower than end fields
	if ( iFinishFieldsCount > 0 )
		{
		TInt cnt = 0;
		for ( TInt j = 0; j < iSize.iHeight; j++ )
			{
			for ( TInt i = 0; i < iSize.iWidth; i++ )
				{
				if ( FieldC( i, j ) == EFieldItem )
					{
					cnt++;
					}
				}
			}
		if ( cnt <= iFinishFieldsCount )
			{ // assumption: before CheckGameEndLose() function CheckGameEndWin() function has been called
			return ETrue;
			}
		}

	// 2. check if items can be moved
	for ( TInt j = 0; j < iSize.iHeight; j++ )
		{
		for ( TInt i = 0; i < iSize.iWidth; i++ )
			{
			TFieldType ft = FieldC( i, j );

			if ( ft == EFieldNone )
				{
				continue;
				}

			if ( i < iSize.iWidth - 2 && ft == EFieldEmpty && FieldC( i + 1, j ) == EFieldItem && FieldC( i + 2, j ) == EFieldItem )
				{
				return EFalse;
				}
			else if ( j < iSize.iHeight - 2 && ft == EFieldEmpty && FieldC( i, j + 1 ) == EFieldItem && FieldC( i, j + 2 ) == EFieldItem )
				{
				return EFalse;
				}
			else if ( i < iSize.iWidth - 2 && ft == EFieldItem && FieldC( i + 1, j ) == EFieldItem && FieldC( i + 2, j ) == EFieldEmpty )
				{
				return EFalse;
				}
			else if ( j < iSize.iHeight - 2 && ft == EFieldItem && FieldC( i, j + 1 ) == EFieldItem && FieldC( i, j + 2 ) == EFieldEmpty )
				{
				return EFalse;
				}
			else if ( iDiagonalMoves )
				{
				if ( i < iSize.iWidth - 2 && j < iSize.iHeight - 2 && ft == EFieldEmpty && FieldC( i + 1, j + 1 ) == EFieldItem && FieldC( i + 2, j + 2 ) == EFieldItem )
					{
					return EFalse;
					}
				else if ( i < iSize.iWidth - 2 && j < iSize.iHeight - 2 && ft == EFieldItem && FieldC( i + 1, j + 1 ) == EFieldItem && FieldC( i + 2, j + 2 ) == EFieldEmpty )
					{
					return EFalse;
					}
				else if ( i < iSize.iWidth - 2 && j >= 2 && ft == EFieldEmpty && FieldC( i + 1, j - 1 ) == EFieldItem && FieldC( i + 2, j - 2 ) == EFieldItem )
					{
					return EFalse;
					}
				else if ( i < iSize.iWidth - 2 && j >= 2 && ft == EFieldItem && FieldC( i + 1, j - 1 ) == EFieldItem && FieldC( i + 2, j - 2 ) == EFieldEmpty )
					{
					return EFalse;
					}
				}
			}
		}

	return ETrue;
	}

TBool CHermitLevel::DiagonalMoves() const
	{
	return iDiagonalMoves;
	}

const TUint8* CHermitLevel::Data() const
	{
	return iData;
	}

