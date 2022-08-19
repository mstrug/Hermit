/*
 ============================================================================
  Name        : HermitBackgroundGlObject.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITBACKGROUNDGLOBJECT_H_
#define HERMITBACKGROUNDGLOBJECT_H_

#include <e32def.h>
#include <SGEGl/GlObject.h>

// FORWARD DECLARATIONS

class CGlVertexArray;


const TInt KVertexSize = 2;
const TInt KVertexCount = 5;
const TInt KVertexColorSize = 4;
const TInt KVertexPerTriangle = 3;
const TInt KTrianglesPerFace = 3;


// CLASS DECLARATION

/**
 *  CHermitBackgroundGlObject
 *
 */
class CHermitBackgroundGlObject : public CGlObject
	{
public:

	virtual ~CHermitBackgroundGlObject();

    /**
     * Two-phased constructor.
     */
    static CHermitBackgroundGlObject* NewL();

    /**
     * Two-phased constructor.
     */
    static CHermitBackgroundGlObject* NewLC();

    TInt SetColors( TAny* aData, TInt aSize );

protected:

	CHermitBackgroundGlObject();

    /**
     * EPOC default constructor for performing 2nd stage construction
     */
    void ConstructL();

protected:

    CGlVertexArray* iData; // owned

	};

#endif /* HERMITBACKGROUNDGLOBJECT_H_ */
