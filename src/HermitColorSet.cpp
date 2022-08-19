/*
 ============================================================================
  Name        : HermitColorSet.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitColorSet.h"

const TInt KDefaultTime = 500000; // 500ms

THermitColorSet::THermitColorSet()
	: iType( EBouncing ), iUpdateTime( KDefaultTime ), iUpdateTimeCounter( 0 )
	{
	}

void THermitColorSet::SetType( TType aType )
	{
	iType = aType;
	}

void THermitColorSet::SetIncrementTime( TInt aIncrementTime )
	{
	iUpdateTime = aIncrementTime;
	}

void THermitColorSet::SetColorsMin( const GLubyte* aData, TBool aSetColors )
	{
	Mem::Copy( iColorsMin.Begin(), aData, iColorsMin.Count() );
	if ( aSetColors )
		{
		Mem::Copy( iColors.Begin(), aData, iColors.Count() );
		iIncrementDirection = EUp;
		}
	}

void THermitColorSet::SetColorsMax( const GLubyte* aData, TBool aSetColors )
	{
	Mem::Copy( iColorsMax.Begin(), aData, iColorsMax.Count() );
	if ( aSetColors )
		{
		Mem::Copy( iColors.Begin(), aData, iColors.Count() );
		iIncrementDirection = EDown;
		}
	}

void THermitColorSet::SetColors( const GLubyte* aData )
	{
	Mem::Copy( iColors.Begin(), aData, iColors.Count() );
	}

void THermitColorSet::SetIncrements( const GLbyte* aData )
	{
	Mem::Copy( iIncrements.Begin(), aData, iIncrements.Count() );
	}

void THermitColorSet::SetValues( THermitColorSet& aColorSet )
	{
	SetColors( aColorSet.iColors.Begin() );
	SetIncrements( aColorSet.iIncrements.Begin() );
	iIncrementDirection = aColorSet.iIncrementDirection;
	iUpdateTimeCounter = aColorSet.iUpdateTimeCounter;
	}

void THermitColorSet::UpdateColros()
	{
	if ( iType == EMinMax )
		{
		for ( TInt i = 0; i < iColors.Count(); i++ )
			{
			if ( iIncrements[ i ] == 0 )
				{
				continue;
				}

			iColors[ i ] += iIncrements[ i ];

			if ( iColors[ i ] + iIncrements[ i ] == iColorsMin[ i ] || iColors[ i ] + iIncrements[ i ] == iColorsMax[ i ] )
				{
				iIncrements[ i ] *= -1;
				}
			}
		}
	else if ( iType == EMinMaxWait )
		{
		TInt state = 0;
		for ( TInt i = 0; i < iColors.Count(); i++ )
			{
			if ( iIncrements[ i ] == 0 )
				{
				state++;
				continue;
				}

			if ( iColors[ i ] + iIncrements[ i ] <= iColorsMin[ i ] || iColors[ i ] + iIncrements[ i ] >= iColorsMax[ i ] )
				{
				state++;
				}
			else
				{
				iColors[ i ] += iIncrements[ i ];
				}
			}
		if ( state == iColors.Count() )
			{
			for ( TInt i = 0; i < iIncrements.Count(); i++ )
				{
				iIncrements[ i ] *= -1;
				}
			}
		}
	else if ( iType == EMinMaxWait2 )
		{
		TInt state = 0;
		for ( TInt i = 0; i < iColors.Count(); i++ )
			{
			if ( iIncrements[ i ] == 0 )
				{
				state++;
				continue;
				}

			if ( iIncrementDirection == EDown )
				{
				if ( ( iIncrements[ i ] > 0 && iColors[ i ] + iIncrements[ i ] >= iColorsMin[ i ] ) ||
					 ( iIncrements[ i ] < 0 && iColors[ i ] + iIncrements[ i ] <= iColorsMin[ i ] ) ||
					 iIncrements[ i ] == 0 )
					{
					state++;
					}
				else
					{
					iColors[ i ] += iIncrements[ i ];
					}
				}
			else // EUp
				{
				if ( ( iIncrements[ i ] > 0 && iColors[ i ] + iIncrements[ i ] >= iColorsMax[ i ] ) ||
					 ( iIncrements[ i ] < 0 && iColors[ i ] + iIncrements[ i ] <= iColorsMax[ i ] ) ||
					 iIncrements[ i ] == 0 )
					{
					state++;
					}
				else
					{
					iColors[ i ] += iIncrements[ i ];
					}
				}
			}

		if ( state == iColors.Count() )
			{
			for ( TInt i = 0; i < iIncrements.Count(); i++ )
				{
				iIncrements[ i ] *= -1;
				}

			if ( iIncrementDirection == EUp )
				{
				iIncrementDirection = EDown;
				}
			else
				{
				iIncrementDirection = EUp;
				}
			}
		}
	else
		{
		for ( TInt i = 0; i < iColors.Count(); i++ )
			{
			if ( iIncrements[ i ] == 0 )
				{
				continue;
				}
			if ( (TInt)iColors[ i ] + (TInt)iIncrements[ i ] > 0xff || (TInt)iColors[ i ] + (TInt)iIncrements[ i ] < 0 )
				{
				iIncrements[ i ] *= -1;
				}

			iColors[ i ] += iIncrements[ i ];
			}
		}
	}

TBool THermitColorSet::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	iUpdateTimeCounter += aTimeInterval.Int();

	if ( iUpdateTimeCounter >= iUpdateTime )
		{
		//AppLog( "back Upd" );
		//iUpdateTimeCounter = 0;
		iUpdateTimeCounter -= iUpdateTime;

		UpdateColros();

		return ETrue;
		}

	return EFalse;
	}

