/*
 ============================================================================
  Name        : HermitColorSet.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITCOLORSET_H_
#define HERMITCOLORSET_H_

#include <e32def.h>
#include <SGEGl/GlObject.h>
#include <SGEEngine/SGEObjectInterfaces.h>


const TInt KHermitBackgroundColors = 5;
const TInt KHermitBackgroundColorsSize = 4;


class THermitColorSet : public MSGEUpdatableObject
	{
	enum TIncrementDirection
	{
		EUp,
		EDown
	};

public:

	enum TType
	{
		EBouncing,   // ColorsMin -> 0xff || 0x00 -> ColrosMin, ColorsMax is not used
		EMinMax,	 // ColorsMin <-> ColorsMax
		EMinMaxWait, // ColorsMin <-> ColorsMax with wait on min and max
		EMinMaxWait2 // ColorsMin <-> ColorsMax with wait on min and max
	};

	THermitColorSet();

	void SetType( TType aType );

	// increment time in microseconds
	void SetIncrementTime( TInt aIncrementTime );

	// ensure aData has proper lenght!
	void SetColorsMin( const GLubyte* aData, TBool aSetColors = ETrue );

	// ensure aData has proper lenght!
	void SetColorsMax( const GLubyte* aData, TBool aSetColors = EFalse );

	// ensure aData has proper lenght!
	void SetColors( const GLubyte* aData );

	// ensure aData has proper lenght!
	void SetIncrements( const GLbyte* aData );

	// ensure aData has proper lenght!
	void SetValues( THermitColorSet& aColorSet );

private:

	void UpdateColros();

public: // from MSGEUpdatableObject

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

public: // data

	TFixedArray< GLubyte, KHermitBackgroundColors * KHermitBackgroundColorsSize > iColors;

	TFixedArray< GLubyte, KHermitBackgroundColors * KHermitBackgroundColorsSize > iColorsMin;

	TFixedArray< GLubyte, KHermitBackgroundColors * KHermitBackgroundColorsSize > iColorsMax;

	TFixedArray< GLbyte, KHermitBackgroundColors * KHermitBackgroundColorsSize > iIncrements;

private: // data

	TType iType;

	TInt iUpdateTime;

	TInt iUpdateTimeCounter;

	TIncrementDirection iIncrementDirection;

	};



#endif /* HERMITCOLORSET_H_ */
