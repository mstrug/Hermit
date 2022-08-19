/*
 ============================================================================
  Name        : HermitBackground.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITBACKGROUND_H_
#define HERMITBACKGROUND_H_

#include <e32def.h>
#include <SGEEngine/SGEObjectInterfaces.h>
#include "HermitColorSet.h"
#include "HermitBackgroundGlObject.h"


// FORWARD DECLARATIONS

class CHermitSettings;
class TGlPolygonX;


const TInt KColorSets = 7 * 2;

typedef TFixedArray< THermitColorSet, KColorSets > THermitColorSets;


// CLASS DECLARATION

/**
 *  CHermitBackground
 *
 */
class CHermitBackground : public CHermitBackgroundGlObject, public MSGEUpdatableObject
	{
public:

    ~CHermitBackground();

    /**
     * Two-phased constructor.
     */
    static CHermitBackground* NewL( CHermitSettings& aSettings );

    /**
     * Two-phased constructor.
     */
    static CHermitBackground* NewLC( CHermitSettings& aSettings );

    TGlPolygonX& PrimitiveType() const;

    void SetCurrentColors( TInt aSet, THermitColorSet& aColorSet );

    static void CreateColorSets( THermitColorSets& aColorSets );

private:

    /**
     * Constructor for performing 1st stage construction
     */
    CHermitBackground( CHermitSettings& aSettings );

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

public: // from MSGEUpdatableObject

    TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

private:

    CHermitSettings& iSettings;

    TFixedArray< GLubyte, KVertexCount * KVertexColorSize > iColors;

    THermitColorSets iColorSets;

	};

#endif /* HERMITBACKGROUND_H_ */
