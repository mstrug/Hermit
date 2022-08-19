/*
 ============================================================================
  Name        : HermitBackgroundGlObject.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitBackgroundGlObject.h"
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlPolygon.h>
#include <SGEGl/GlVertexArray.h>




/** Vertice coordinates for the rect. */
static const GLfixed KVertices[ KVertexCount * KVertexSize ] =
     {
      0,  1 << 16,
	  1 << 15,  1 << 16,
	  1 << 16,  1 << 16,
	  1 << 16,  0,
	  0,  0,
     };

/**
 * Indices for drawing the triangles.
 * The color of the triangle is determined by
 * the color of the last vertex of the triangle.
 */
static const GLubyte KTriangles[ KTrianglesPerFace * KVertexPerTriangle ] =
    {
    /* front */
    0, 4, 1,
    4, 3, 1,
    1, 3, 2
    };

/** Colors for vertices (Red, Green, Blue, Alpha). */
static const GLubyte KColors[ KVertexCount * KVertexColorSize ] =
    {
    0    , 93,  129, 255,
    3    , 96,  132, 255,
    0    , 93,  129, 255,
    167  ,255,  255, 255,
    167  ,255,  255, 255,
    };


CHermitBackgroundGlObject::CHermitBackgroundGlObject()
	{
	}

CHermitBackgroundGlObject::~CHermitBackgroundGlObject()
	{
	delete iData;
	}

CHermitBackgroundGlObject* CHermitBackgroundGlObject::NewLC()
    {
	CHermitBackgroundGlObject* self = new ( ELeave ) CHermitBackgroundGlObject();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

CHermitBackgroundGlObject* CHermitBackgroundGlObject::NewL()
    {
	CHermitBackgroundGlObject* self = CHermitBackgroundGlObject::NewLC();
    CleanupStack::Pop(); // self;
    return self;
    }

void CHermitBackgroundGlObject::ConstructL()
    {
	CGlObject::ConstructL();

    iPrimitive = CGlPrimitive::NewL();
    iPrimitiveOwner = ETrue;

    TGlVertexArrayConfig cfg( EVertexTypeFixed, ENormalTypeNone, EColorTypeUnsignedByte,
                              EPointSizeTypeNone, ETexCoordTypeNone,
                              EVertexSizeTwo, ENormalSizeNone, EColorSizeFour, EPointSizeNone,
                              ETexCoordSizeNone, 0,
                              EIndicesTypeUnsignedByte, KTrianglesPerFace * KVertexPerTriangle );

    iData = CGlVertexArray::NewL( cfg, KVertexCount );

    iData->SetVertexData( (TAny*) KVertices, KVertexCount * KVertexSize * sizeof( GLfixed ) );
    iData->SetColorData( (TAny*) KColors, KVertexCount * KVertexColorSize * sizeof( GLubyte ) );
    iData->SetIndices( (TAny*) KTriangles, KTrianglesPerFace * KVertexPerTriangle * sizeof( GLubyte ), EFalse );

    iPrimitive->AddVertexArrayL( iData );

    iPrimitiveType = new (ELeave) TGlPolygonX();
    iPrimitiveTypeOwner = ETrue;
    }

TInt CHermitBackgroundGlObject::SetColors( TAny* aData, TInt aSize )
	{
    return iData->SetColorData( aData, aSize );
	}


