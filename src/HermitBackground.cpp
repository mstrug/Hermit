/*
 ============================================================================
  Name        : HermitBackground.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitBackground.h"
#include "HermitBackgroundColors.h"
#include "HermitSettings.h"
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlPolygon.h>
#include <SGEGl/GlVertexArray.h>




CHermitBackground::CHermitBackground( CHermitSettings& aSettings ) : iSettings( aSettings )
	{
	}

CHermitBackground::~CHermitBackground()
    {
    }

CHermitBackground* CHermitBackground::NewLC( CHermitSettings& aSettings )
    {
	CHermitBackground* self = new ( ELeave ) CHermitBackground( aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CHermitBackground* CHermitBackground::NewL( CHermitSettings& aSettings )
    {
	CHermitBackground* self = CHermitBackground::NewLC( aSettings );
    CleanupStack::Pop(); // self;
    return self;
    }

void CHermitBackground::ConstructL()
    {
	CHermitBackgroundGlObject::ConstructL();

    CreateColorSets( iColorSets );
    }

void CHermitBackground::CreateColorSets( THermitColorSets& aColorSets )
	{
	aColorSets[ 0 ].SetType( THermitColorSet::EMinMaxWait );
	//aColorSets[ 0 ].SetType( THermitColorSet::EMinMax );
	aColorSets[ 0 ].SetIncrementTime( 50000 );
	aColorSets[ 0 ].SetColorsMin( KColorsMinSet1, ETrue );
	aColorSets[ 0 ].SetColorsMax( KColorsMaxSet1 );
	//aColorSets[ 0 ].SetIncrements( KIncrementsSet1 );

	aColorSets[ 1 ].SetType( THermitColorSet::EMinMaxWait );
	aColorSets[ 1 ].SetIncrementTime( 50000 );
	aColorSets[ 1 ].SetColorsMin( KColorsMinSet2, ETrue );
	aColorSets[ 1 ].SetColorsMax( KColorsMaxSet2 );
	aColorSets[ 1 ].SetIncrements( KIncrementsOne );

	aColorSets[ 2 ].SetType( THermitColorSet::EMinMaxWait );
	aColorSets[ 2 ].SetIncrementTime( 50000 );
	aColorSets[ 2 ].SetColorsMin( KColorsMinSet3, ETrue );
	aColorSets[ 2 ].SetColorsMax( KColorsMaxSet3 );
	aColorSets[ 2 ].SetIncrements( KIncrementsOne );

	aColorSets[ 3 ].SetType( THermitColorSet::EMinMaxWait );
	aColorSets[ 3 ].SetIncrementTime( 50000 );
	aColorSets[ 3 ].SetColorsMin( KColorsMinSet4, ETrue );
	aColorSets[ 3 ].SetColorsMax( KColorsMaxSet4 );
	aColorSets[ 3 ].SetIncrements( KIncrementsOne );

	aColorSets[ 4 ].SetType( THermitColorSet::EMinMaxWait );
	aColorSets[ 4 ].SetIncrementTime( 50000 );
	aColorSets[ 4 ].SetColorsMin( KColorsMinSet5, ETrue );
	aColorSets[ 4 ].SetColorsMax( KColorsMaxSet5 );
	aColorSets[ 4 ].SetIncrements( KIncrementsOne );


//	aColorSets[ 5 ].SetType( THermitColorSet::EMinMax );
//	aColorSets[ 5 ].SetIncrementTime( 30000 );
//	aColorSets[ 5 ].SetColorsMin( KColorsMinSet7, ETrue );
//	aColorSets[ 5 ].SetColorsMax( KColorsMaxSet7 );
//	aColorSets[ 5 ].SetIncrements( KIncrementsSet7 );

	aColorSets[ 5 ].SetType( THermitColorSet::EMinMaxWait );
	aColorSets[ 5 ].SetIncrementTime( 50000 );
	aColorSets[ 5 ].SetColorsMin( KColorsMinSet6_2, ETrue );
	aColorSets[ 5 ].SetColorsMax( KColorsMaxSet6_2 );
	aColorSets[ 5 ].SetIncrements( KIncrementsOne );

	aColorSets[ 6 ].SetType( THermitColorSet::EBouncing );
	aColorSets[ 6 ].SetIncrementTime( 10000 );
	aColorSets[ 6 ].SetColorsMin( KColorsMinSet6_1, ETrue );
	aColorSets[ 6 ].SetIncrements( KIncrementsOne );


	// line 2

	aColorSets[ 7 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 7 ].SetIncrementTime( 20000 );
	aColorSets[ 7 ].SetColorsMin( KColorsMinSet7, ETrue );
	aColorSets[ 7 ].SetColorsMax( KColorsMaxSet7 );
	aColorSets[ 7 ].SetIncrements( KIncrementsSet7 );

	aColorSets[ 8 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 8 ].SetIncrementTime( 10000 );
	aColorSets[ 8 ].SetColorsMin( KColorsMinSet8, ETrue );
	aColorSets[ 8 ].SetColorsMax( KColorsMaxSet8 );
	aColorSets[ 8 ].SetIncrements( KIncrementsSet8 );

	aColorSets[ 9 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 9 ].SetIncrementTime( 30000 );
	aColorSets[ 9 ].SetColorsMin( KColorsMinSet11, ETrue );
	aColorSets[ 9 ].SetColorsMax( KColorsMaxSet11 );
	aColorSets[ 9 ].SetIncrements( KIncrementsSet11 );

	aColorSets[ 10 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 10 ].SetIncrementTime( 30000 );
	aColorSets[ 10 ].SetColorsMin( KColorsMinSet9, ETrue );
	aColorSets[ 10 ].SetColorsMax( KColorsMaxSet9 );
	aColorSets[ 10 ].SetIncrements( KIncrementsSet9 );

	aColorSets[ 11 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 11 ].SetIncrementTime( 30000 );
	aColorSets[ 11 ].SetColorsMin( KColorsMinSet10, ETrue );
	aColorSets[ 11 ].SetColorsMax( KColorsMaxSet10 );
	aColorSets[ 11 ].SetIncrements( KIncrementsSet10 );

	aColorSets[ 12 ].SetType( THermitColorSet::EMinMaxWait2 );
	aColorSets[ 12 ].SetIncrementTime( 30000 );
	aColorSets[ 12 ].SetColorsMin( KColorsMinSet12, ETrue );
	aColorSets[ 12 ].SetColorsMax( KColorsMaxSet12 );
	aColorSets[ 12 ].SetIncrements( KIncrementsSet12 );

	aColorSets[ 13 ].SetType( THermitColorSet::EBouncing );
	aColorSets[ 13 ].SetIncrementTime( 10000 );
	aColorSets[ 13 ].SetColorsMin( KColorsMinSet13, ETrue );
	aColorSets[ 13 ].SetIncrements( KIncrementsOne );
	}

TGlPolygonX& CHermitBackground::PrimitiveType() const
    {
    return dynamic_cast<TGlPolygonX&>( *iPrimitiveType );
    }

void CHermitBackground::SetCurrentColors( TInt aSet, THermitColorSet& aColorSet )
	{
	if ( aSet >= 0 && aSet < iColorSets.Count() )
		{
		iColorSets[ aSet ].SetValues( aColorSet );
		}
	}

TBool CHermitBackground::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	TInt idx = iSettings.ColorSet();
	TBool ret = iColorSets[ idx ].Update( aTimeInterval );
	if ( ret )
		{
	    iData->SetColorData( (TAny*) iColorSets[ idx ].iColors.Begin(), iColorSets[ idx ].iColors.Count() );
		}
	return ret;
	}




