/*
 ============================================================================
  Name        : HermitAnimationTitle.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitAnimationTitle.h"
#include "HermitCommonObjects.h"
#include "HermitMenuTitle.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <e32math.h>
#ifdef _SGE_BADA_
#include <FBase.h>
#endif


const TInt KAnimationStepTime = 10000; // 1 s

const GLfloat KScaleStep = 0.1;

const GLfloat KScaleOutStep = 1.1;


CHermitAnimationTitle::CHermitAnimationTitle( MHermitAnimationObserver* aObserver,
											  CHermitCommonObjects& aCommonObjects,
											  TSize aSize )
	: CHermitAnimationBase( aObserver ), iCommonObjects( aCommonObjects ), iSize( aSize )
	{
	}

CHermitAnimationTitle::~CHermitAnimationTitle()
	{
	delete iObjectRect;
	delete iTitle;
#ifdef _SGE_BADA_
	delete iRandom;
#endif
	}

CHermitAnimationTitle* CHermitAnimationTitle::NewL( MHermitAnimationObserver* aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitAnimationTitle* self = new (ELeave) CHermitAnimationTitle( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitAnimationTitle::ConstructL()
	{
	CHermitAnimationBase::BaseConstructL( 1 );

	iObjectRect = CGlObjectRect::NewL();

	iTitle = CHermitMenuTitle::NewL( iCommonObjects );

#ifdef _SGE_BADA_
	iRandom = new Osp::Security::AesSecureRandom();
#endif

	UpdateTitleRect();
	}

void CHermitAnimationTitle::UpdateTitleRect()
	{
	TSize elements = iTitle->ElementsCount();
	TSize es = iTitle->ElementsSize();
	TSize titleSize( elements.iWidth * es.iWidth, elements.iHeight * es.iHeight );
	iTitleRect = TRect( TPoint( ( iSize.iWidth - titleSize.iWidth ) / 2, ( iSize.iHeight - titleSize.iHeight ) / 2 ), titleSize );

	iTitle->SetRect( TRect( TPoint(), iTitleRect.Size() ) );
	}

void CHermitAnimationTitle::StopAnimation()
	{
	iAnimationStarted = EFalse;
	}

void CHermitAnimationTitle::StartAnimation()
	{
	DoStartAnimation();

	iTimeCoutner = 0;

	iAnimationCounter = 0;

	iTitleScale = 5;

	iTitleRotation = ( iTitleScale - 1 ) / KScaleStep;

	iAnimationCounterEnd = (TInt)iTitleRotation + 30;

	iPhase = ERotation;
	}

void CHermitAnimationTitle::DoAnimationStep()
	{
	iAnimationCounter++;
	if ( iAnimationCounter == iAnimationCounterEnd )
		{
		iTitleScaleStep = KScaleOutStep;
		iPhase = EScaleOut;
		}
	else if ( iAnimationCounter >= iAnimationCounterEnd + 10 )
		{
		DoFinishAnimation();
		return;
		}

	if ( iPhase == ERotation )
		{
		iTitleScale -= KScaleStep;
		iTitleRotation -= 1;
		if ( iTitleScale <= 1 )
			{
			iTitleRotation = 0;
			iTitleScale = 1;
			iTitle->SetTitleElements();
			}
		else
			{
#ifdef _SGE_BADA_
			Osp::Base::ByteBuffer* buf = iRandom->GenerateRandomBytesN( KTitleSizeY * sizeof( TUint32 ) );

			TUint32 tmp[ KTitleSizeY ];
			for ( TInt i = 0; i < KTitleSizeY; i++ )
				{
				buf->GetLong( i * sizeof( TUint32 ), (long&) tmp[ i ] );
				//tmp[ i ] = (TUint32)Osp::Base::Utility::Math::Rand() | (((TUint32)Osp::Base::Utility::Math::Rand()) << ;
				//AppLog( "%x", tmp[ i ] );
				}

			delete buf;
#else
			TUint32 tmp[ KTitleSizeY ];
            for ( TInt i = 0; i < KTitleSizeY; i++ )
                {
                tmp[ i ] = Math::Random();
                }
#endif
			iTitle->SetTitleElements( tmp );
			}
		}
	else // EScaleOut
		{
		iTitleScale *= iTitleScaleStep;
		iTitleScaleStep *= 1.1;
		}
	}

void CHermitAnimationTitle::Draw( const TRect aDrawRect )
	{
	glTranslatef( iSize.iWidth / 2, iSize.iHeight / 2, 0 );

	if ( iPhase == ERotation )
		{
		GLfloat scale = iTitleScale * iTitleScale;
		glScalef( scale, scale, 0 );
		glRotatef( iTitleRotation, 0, 0, 1 );
		}
	else
		{
		glScalef( iTitleScale, iTitleScale, 0 );
		}

    glTranslatef( - iTitleRect.Width() / 2, - iTitleRect.Height() / 2, 0 );

	iTitle->Draw( aDrawRect );

	// fills
	iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();

	// top of title
	glPushMatrix();
	glTranslatef( 0, iTitleRect.Height(), 0 );
	glScalef( iTitleRect.Width(), iSize.iHeight / 2, 0 );
	iObjectRect->Draw();
	glPopMatrix();

	// bottom of title
	glPushMatrix();
	glTranslatef( 0, -iSize.iHeight / 2, 0 );
	glScalef( iTitleRect.Width(), iSize.iHeight / 2, 0 );
	iObjectRect->Draw();
	glPopMatrix();

	// left size of title
	glPushMatrix();
	glTranslatef( -iSize.iWidth / 2, -iSize.iHeight / 2, 0 );
	glScalef( iSize.iWidth / 2, iSize.iHeight + iTitleRect.Height(), 0 );
	iObjectRect->Draw();
	glPopMatrix();

	// right size of title
	glPushMatrix();
	glTranslatef( iTitleRect.Width(), -iSize.iHeight / 2, 0 );
	glScalef( iSize.iWidth / 2, iSize.iHeight + iTitleRect.Height(), 0 );
	iObjectRect->Draw();
	glPopMatrix();

	}

void CHermitAnimationTitle::AnimationSequenceChanged()
	{

	}

void CHermitAnimationTitle::AnimationSequenceLast()
	{

	}

TBool CHermitAnimationTitle::AnimationUpdate()
	{
	//AppLog( "test" );
	if ( iTimeCoutner > KAnimationStepTime )
		{
		iTimeCoutner -= KAnimationStepTime;

		DoAnimationStep();

		return ETrue;
		}

	return EFalse;
	}




