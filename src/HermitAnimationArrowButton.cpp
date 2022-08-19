/*
 ============================================================================
  Name        : HermitAnimationArrowButton.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitAnimationArrowButton.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>



const TInt KAnimationStepTime = 60000; // 60 ms

const GLfixed KAnimationOffsetStep = 1 << 10;

const TInt KAnimationOffsetMax = 4;
const TInt KAnimationOffsetMin = 7;


CHermitAnimationArrowButton::CHermitAnimationArrowButton( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects,
														  THermitGuiButton& aAnimatedButton, TArrowDirection aButtonDirection )
	: CHermitAnimationBase( aObserver ), iCommonObjects( aCommonObjects ), iAnimatedButton( aAnimatedButton ),
	  iButtonDirection( aButtonDirection )
	{
	}

CHermitAnimationArrowButton::~CHermitAnimationArrowButton()
	{
	delete iObjectRect;
	}

CHermitAnimationArrowButton* CHermitAnimationArrowButton::NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects,
																THermitGuiButton& aAnimatedButton, TArrowDirection aButtonDirection  )
	{
	CHermitAnimationArrowButton* self = new (ELeave) CHermitAnimationArrowButton( aObserver, aCommonObjects,
																				  aAnimatedButton, aButtonDirection );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitAnimationArrowButton::ConstructL()
	{
	CHermitAnimationBase::BaseConstructL( 1 );

	iObjectRect = CGlObjectRect::NewL();

	DoStartAnimation();
	}

void CHermitAnimationArrowButton::RestartAnimation()
	{
	CHermitAnimationBase::RestartAnimation();
	iAnimationStarted = ETrue;
	iPosition = -KAnimationOffsetMin * KAnimationOffsetStep; //KAnimationOffsetStep;
	iPositionDirection = 1;
	}

void CHermitAnimationArrowButton::DoAnimationStep()
	{
	iPosition += iPositionDirection * KAnimationOffsetStep;

	if ( iPosition >= KAnimationOffsetMax * KAnimationOffsetStep || iPosition <= - KAnimationOffsetMin * KAnimationOffsetStep )
		{
		iPositionDirection *= -1;
		}

	UpdateTexCoords();
	}

void CHermitAnimationArrowButton::UpdateTexCoords()
	{
	const TInt KVertexCount = 4;
	const TInt KTexCoordSize = 2;

	GLfixed texCoords[ KVertexCount * KTexCoordSize ] =
	    {
	    0, 0,
	    ( 1 << 16 ), 0,
	    ( 1 << 16 ), ( 1 << 16 ),
	    0, ( 1 << 16 )
	    };

	texCoords[ 0 ] -= iPosition;
	texCoords[ 2 ] -= iPosition;
	texCoords[ 4 ] -= iPosition;
	texCoords[ 6 ] -= iPosition;

	iObjectRect->Primitive().VertexArray( 0 )->SetTexCoordData( 0, (TAny*) texCoords, KVertexCount * KTexCoordSize * sizeof( GLfixed ) );
	}

void CHermitAnimationArrowButton::Draw( const TRect /*aDrawRect*/ )
	{
	glPushMatrix();
	GlUtils::Translate2D( iAnimatedButton.iRect.iTl );
	iAnimatedButton.Rotate();
	GlUtils::Scale2D( iAnimatedButton.iTextureSize );
	iCommonObjects.TextureManager()->Texture( iAnimatedButton.iTextureId )->Use();
	iObjectRect->Draw();
	glPopMatrix();
	}

void CHermitAnimationArrowButton::AnimationSequenceChanged()
	{

	}

void CHermitAnimationArrowButton::AnimationSequenceLast()
	{

	}

TBool CHermitAnimationArrowButton::AnimationUpdate()
	{
	if ( iTimeCoutner > KAnimationStepTime )
		{
		iTimeCoutner -= KAnimationStepTime;

		DoAnimationStep();

		return ETrue;
		}

	return EFalse;
	}


