/*
 ============================================================================
  Name        : HermitMenuPageTutorial.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageTutorial.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitGameLevel.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitAnimationFade.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <e32def.h>



const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KTitleSize( 64, 64 );
const TSize KTitleTexSize( 64, 64 );

const TSize KIndicatorSize( 32, 32 );
const TSize KIndicatorTexSize( 32, 32 );

const TInt KBtnBackIdx = 0;


const TInt KOneSec = 1000000;

#ifdef USE_ANIMATIONS
const TInt KTutorialStatTimes[] =
	{
	0,					// ELevelLoading
	KOneSec / 2,		// ELevelIntro
	0,					// ELevelIndicator	->  animation
	20000,		// ELevelHintAndIndicator
	0,					// ELevelHint	->  animation
	KOneSec / 2,		// ELevelAfterHint
	0,					// ELevelOutro
	0					// ELevelUnloading
	};
#else
const TInt KTutorialStatTimes[] =
	{
	0,					// ELevelLoading
	KOneSec / 2,		// ELevelIntro
	KOneSec / 2,		// ELevelIndicator
	200000,				// ELevelHintAndIndicator
	20000,				// ELevelHint
	KOneSec / 2,		// ELevelAfterHint
	KOneSec / 2,		// ELevelOutro
	0					// ELevelUnloading
	};
#endif


const TInt KTutorialLevelsCount = 3;
const TInt KTutorialLevelSize = 4;

static const TUint8 KTutorialLevel[ KTutorialLevelsCount ][ KTutorialLevelSize * KTutorialLevelSize ] =
	{
		{
		2, 1, 1, 2,
		1, 2, 1, 2,
		2, 1, 2, 1,
		1, 1, 2, 1
		},
		{
		2, 1, 1, 1,
		2, 2, 0x10 | 1, 1,
		2, 2, 0x10 | 1, 1,
		2, 1, 1, 1
		},
		{
		1, 2, 1, 1,
		1, 1, 2, 2,
		2, 1, 2, 1,
		1, 1, 1, 1
		}
	};


const TInt KTutorialMovesCountMax = 12;

static const TPoint KTutorialMoves[ KTutorialLevelsCount ][ KTutorialMovesCountMax ] =
	{
		{
			TPoint( 3, 0 ), TPoint( 3, 2 ),
			TPoint( 3, 2 ), TPoint( 1, 2 ),
			TPoint( 0, 2 ), TPoint( 2, 2 ),
			TPoint( 2, 3 ), TPoint( 2, 1 ),
			TPoint( 2, 1 ), TPoint( 0, 1 ),
			TPoint( 0, 0 ), TPoint( 0, 2 )
		},
		{
			TPoint( 0, 2 ), TPoint( 2, 2 ),
			TPoint( 0, 0 ), TPoint( 0, 2 ),
			TPoint( 0, 3 ), TPoint( 0, 1 ),
			TPoint( 0, 1 ), TPoint( 2, 1 ),
			TPoint( -1, -1 ), TPoint( -1, -1 ),
			TPoint( -1, -1 ), TPoint( -1, -1 )
		},
		{
			TPoint( 1, 0 ), TPoint( 3, 2 ),
			TPoint( 3, 1 ), TPoint( 3, 3 ),
			TPoint( 3, 3 ), TPoint( 1, 1 ),
			TPoint( 0, 2 ), TPoint( 2, 0 ),
			TPoint( -1, -1 ), TPoint( -1, -1 ),
			TPoint( -1, -1 ), TPoint( -1, -1 )
		}
};

static const TInt KTutorialMovesCount[ KTutorialMovesCountMax ] = { 12, 8, 8 };

static const TBool KTutorialDiagonalMoves[ KTutorialLevelsCount ] = { EFalse, EFalse, ETrue };




CHermitMenuPageTutorial::CHermitMenuPageTutorial( MHermitPageObserver& aObserver,
												  CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
    {
    }

CHermitMenuPageTutorial::~CHermitMenuPageTutorial()
    {
	delete iGameLevel;
	delete iAnimationFadeLevel;
	delete iAnimationFadeIndicator;
	delete iAnimationFadeIndicator2;
    }

CHermitMenuPageTutorial* CHermitMenuPageTutorial::NewL( MHermitPageObserver& aObserver,
													    CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageTutorial* self = new (ELeave) CHermitMenuPageTutorial( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageTutorial::ConstructL()
	{
	BaseConstructL();

	iAnimationFadeLevel = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationFadeLevel->SetStartIncrement( 20 );
	iAnimationFadeLevel->SetAnimationStepTime( 20000 );

	iAnimationFadeIndicator = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationFadeIndicator->SetStartIncrement( 20 );
	iAnimationFadeIndicator->SetAnimationStepTime( 20000 );
	iAnimationFadeIndicator->SetFadeColor( TRgb( 255, 255, 255 ) );
	iAnimationFadeIndicator->UseExternalTextures( ETrue );

	iAnimationFadeIndicator2 = CHermitAnimationFade::NewL( this, iCommonObjects );
	iAnimationFadeIndicator2->SetStartIncrement( 20 );
	iAnimationFadeIndicator2->SetAnimationStepTime( 30000 );
	iAnimationFadeIndicator2->SetFadeColor( TRgb( 255, 255, 255 ) );
	iAnimationFadeIndicator2->UseExternalTextures( ETrue );
	iAnimationFadeIndicator2->EnableScaleAnimation( 20000, 0.05 );

	iGameLevel = CHermitGameLevel::NewL( iCommonObjects );
	SetLevelL( 0 );

	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnBackIdx ].iActionId = KTutorialActionBtnBack;

	SizeChanged( iSize );

	iState = EIdle;
	}

void CHermitMenuPageTutorial::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );

	CalculateLevelRect();
	}

void CHermitMenuPageTutorial::CalculateLevelRect()
	{
    TSize ls = iGameLevel->Size();
    iLevelRect = TRect( TPoint( 0, ( iSize.iHeight - iSize.iWidth ) / 2 ), TSize( iSize.iWidth, iSize.iWidth ) );
    iGameLevel->SetLevelRect( iLevelRect, ls );

    iBoardRect = iGameLevel->BoardRect();

    TRect fr( iLevelRect );
    fr.iTl.iY -= 32;
    iAnimationFadeLevel->SetFadeRect( fr );
	}

void CHermitMenuPageTutorial::SetLevelL( TInt aLevel )
	{
	if ( aLevel >= 0 && aLevel < KTutorialLevelsCount )
		{
		iCurrentMove = 0;
		iCurrentLevel = aLevel;
		iGameLevel->LoadGameLevelL( TSize( KTutorialLevelSize, KTutorialLevelSize ), KTutorialLevel[ iCurrentLevel ], KTutorialDiagonalMoves[ iCurrentLevel ] );
		CalculateLevelRect();
		}
	}

void CHermitMenuPageTutorial::SetIndicator( TPoint aPos )
	{
	iIndicatorRect = TRect( TPoint( iLevelRect.iTl.iX + aPos.iX - KIndicatorSize.iWidth / 2, iLevelRect.iTl.iY + aPos.iY - KIndicatorSize.iHeight / 2 ), KIndicatorSize );
	//AppLog( "ind: %d %d", iIndicatorRect.iTl.iX, iIndicatorRect.iTl.iY );
	iDrawIndicator = ETrue;
	iAnimationFadeIndicator->SetFadeRect( iIndicatorRect );

	TSize ls = iGameLevel->Size();
    TRect tmp = iIndicatorRect;
    tmp.Grow( 8, 8 );
    iAnimationFadeIndicator2->SetFadeRect( tmp );
	}

void CHermitMenuPageTutorial::ResetState()
	{
	iCurrentMove = 0;
	iLevelState = ELevelIntro;
	iDrawIndicator = EFalse;
	iNextLevelAnimation = EFalse;
	iLevelStateTimeCounter = 0;
	SetLevelL( 0 );
	iAnimationFadeLevel->RestartAnimation();
	iAnimationFadeIndicator->RestartAnimation();
	iAnimationFadeIndicator2->RestartAnimation();
	}

void CHermitMenuPageTutorial::DrawFills()
	{
	//iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );
	
	// title right
	glPushMatrix();
	GlUtils::Translate2D( TPoint( KTitleSize.iHeight, iSize.iHeight - KTitleSize.iHeight ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - KTitleSize.iWidth, KTitleSize.iHeight ) );
	iRect->Draw();
	glPopMatrix();

	// title bottom
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iLevelRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iSize.iHeight - KTitleSize.iHeight - iLevelRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom left to back button
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, 0 ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// back button top to text
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iLevelRect.iTl.iY - 32 - iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

    glColor4ub( 255, 255, 255, 255 );
	
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );

    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
	}

void CHermitMenuPageTutorial::Draw( const TRect aDrawRect )
	{
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
    
	// level
	if ( iAnimationFadeLevel->IsAnimationStarted() || ( iLevelState != ELevelLoading && iLevelState != ELevelUnloading ) )
		{
		glPushMatrix();
		iGameLevel->Draw( aDrawRect );
		glPopMatrix();

		// text
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iSize.iWidth / 2 - 512 / 2, iLevelRect.iTl.iY - 32 ) );
		GlUtils::Scale2D( TSize( 512, 32 ) );
		iCommonObjects.TextureManager()->Texture( KHermitTexTutorialText1Id + iCurrentLevel )->Use();
		iRect->Draw();
		glPopMatrix();
		}
	else
		{
		// level empty fill + text
		iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();
		glPushMatrix();
		GlUtils::Translate2D( TPoint( iLevelRect.iTl.iX, iLevelRect.iTl.iY - 32 ) );
		GlUtils::Scale2D( TSize( iLevelRect.Width(), iLevelRect.Height() + 32 ) );
		iRect->Draw();
		glPopMatrix();
		}

	// buttons
	for ( TInt i = 0; i < iButtons.Count(); i++ )
		{
		if ( iButtons[ i ].iSelected )
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( KButtonSize );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			iRect->Redraw();
			glPopMatrix();
			}
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( KButtonTexSize );
		iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
		iRect->Redraw();
		glPopMatrix();
		}
    
	// title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iSize.iHeight - KTitleSize.iHeight ) );
	GlUtils::Scale2D( KTitleTexSize );
	iCommonObjects.TextureManager()->Texture( KHermitTexTutorialTitleId )->Use();
	iRect->Draw();
	glPopMatrix();

	DrawFills();

	// indicator
	if ( iDrawIndicator && !iAnimationFadeIndicator->IsAnimationStarted() )
		{
		iCommonObjects.TextureManager()->Texture( KHermitTexTutorialIndicator1Id )->Use();
		glPushMatrix();
		GlUtils::Translate2D( iIndicatorRect.iTl );
		GlUtils::Scale2D( KIndicatorTexSize );
		iRect->Draw();
		glPopMatrix();
		}
	iCommonObjects.TextureManager()->Texture( KHermitTexTutorialIndicator1Id )->Use();
	iAnimationFadeIndicator->Draw( TRect() );
	iCommonObjects.TextureManager()->Texture( KHermitTexTutorialIndicator2Id )->Use();
	iAnimationFadeIndicator2->Draw( TRect() );


	// button pressed animation
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		}
	//if ( iLevelState == ELevelLoading || iLevelState == ELevelOutro )
		{
		iAnimationFadeLevel->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );
	}

TBool CHermitMenuPageTutorial::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	iGameLevel->Update( aTimeInterval );

	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}

	iAnimationFadeIndicator2->Update( aTimeInterval );
	iAnimationFadeIndicator->Update( aTimeInterval );
	if ( iAnimationFadeIndicator->IsAnimationStarted() /*|| iAnimationFadeIndicator2->IsAnimationStarted()*/ )
		{
		iAnimationFadeLevel->Update( aTimeInterval );
		return ETrue;
		}

	iAnimationFadeLevel->Update( aTimeInterval );
	if ( /*( iLevelState == ELevelLoading || iLevelState == ELevelOutro ) &&*/
		 iAnimationFadeLevel->IsAnimationStarted() )
		{
		return ETrue;
		}

	if ( iNextLevelAnimation )
		{
		return ETrue;
		}

	iLevelStateTimeCounter += aTimeInterval.Int();

	if ( iLevelState < ELevelStateMax && iLevelStateTimeCounter >= KTutorialStatTimes[ iLevelState ] )
		{
		iLevelStateTimeCounter = 0;

		TPoint  p = KTutorialMoves[ iCurrentLevel ][ iCurrentMove ];
		p.iY = KTutorialLevelSize - 1 - p.iY;

		switch ( iLevelState )
			{
			case ELevelLoading:
#ifdef USE_ANIMATIONS
					iAnimationFadeLevel->RestartAnimation();
					iAnimationFadeLevel->StartAnimation( CHermitAnimationFade::EFadeIn );
#else
					iLevelState = ELevelIntro;
#endif
					iCurrentMove = 0;
				break;
			case ELevelIntro:
					iLevelState = ELevelIndicator;
				break;
			case ELevelIndicator:
					SetIndicator( iGameLevel->GetFieldPosition( p, ETrue ) );
#ifdef USE_ANIMATIONS
					iAnimationFadeIndicator->RestartAnimation();
					iAnimationFadeIndicator->StartAnimation( CHermitAnimationFade::EFadeOut );

					iAnimationFadeIndicator2->RestartAnimation();
					iAnimationFadeIndicator2->ResetScale();
					iAnimationFadeIndicator2->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
					iLevelState = ELevelHintAndIndicator;
#endif
				break;
			case ELevelHintAndIndicator:
					iLevelState = ELevelHint;
					iGameLevel->PointerEventXY( p.iX, p.iY );
				break;
			case ELevelHint:
					//iLevelState = ELevelAfterHint;
					//iDrawIndicator = EFalse;
#ifdef USE_ANIMATIONS
					if ( iAnimationFadeIndicator->FadeType() == CHermitAnimationFade::EFadeOut )
						{
						iAnimationFadeIndicator->RestartAnimation();
						iAnimationFadeIndicator->StartAnimation( CHermitAnimationFade::EFadeIn );
						}
#else
					iLevelState = ELevelAfterHint;
					iDrawIndicator = EFalse;
#endif
				break;
			case ELevelAfterHint:
					if ( iCurrentMove < KTutorialMovesCount[ iCurrentLevel ] - 1 )
						{
						iLevelState = ELevelIndicator;
						iCurrentMove++;
						}
					else
						{
						iLevelState = ELevelOutro;
						}
				break;
			case ELevelOutro:
#ifdef USE_ANIMATIONS
					iAnimationFadeLevel->RestartAnimation();
					iAnimationFadeLevel->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
					iLevelState = ELevelUnloading;
#endif
				break;
			case ELevelUnloading:
					if ( iCurrentLevel < KTutorialLevelsCount - 1 )
						{
						SetLevelL( iCurrentLevel + 1 );
						}
					else
						{
						SetLevelL( 0 );
						}
					iCurrentMove = 0;
					iLevelState = ELevelLoading;
				break;
			default:
				break;
			}
		}

	return ETrue;
	}

TBool CHermitMenuPageTutorial::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState != EIdle && iState != EEntering )
		{
		return EFalse;
		}

	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		iButtonDownPos = aPointerEvent.iPosition;
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = ETrue;
				return ETrue;
				}
			}
		if ( !iNextLevelAnimation && iLevelState != ELevelLoading &&
			 iBoardRect.Contains( aPointerEvent.iPosition ) )
			{
#ifdef USE_ANIMATIONS
			iNextLevelAnimation = ETrue;
			iAnimationFadeLevel->RestartAnimation();
			iAnimationFadeLevel->StartAnimation( CHermitAnimationFade::EFadeOut );
#else
			iLevelState = ELevelOutro;
#endif

			PlaySound( KHermitSoundButton2PressId );

			return ETrue;
			}
		}
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iSelected && iButtons[ i ].iActionId &&
				 iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = EFalse;
				// button pressed -> start animation
#ifdef USE_ANIMATIONS
				iState = EAnimation;
				iAnimationButtonPressed->StartAnimation( &iButtons[ i ] );
#else
				iObserver->PageAction( this, iButtons[ i ].iActionId );
#endif

				PlaySound( KHermitSoundButtonPressId );

				return ETrue;
				}
			}
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			iButtons[ i ].iSelected = EFalse;
			}
		}

	return EFalse;
	}

void CHermitMenuPageTutorial::AnimationFinished( CHermitAnimationBase* aAnimation )
	{
	//iState = EIdle;

	CHermitPageBase::AnimationFinished( aAnimation );

	if ( aAnimation == iAnimationButtonPressed )
		{
		THermitGuiButton* btn = iAnimationButtonPressed->AnimatedButton();
		if ( btn )
			{
			iObserver->PageAction( this, btn->iActionId );
			}
		}
	else if ( aAnimation == iAnimationFadeLevel )
		{
		iLevelStateTimeCounter = 0;
		if ( iLevelState == ELevelLoading )
			{
			iLevelState = ELevelIntro;
			}
		else //if ( iLevelState == ELevelOutro )
			{
			iNextLevelAnimation = EFalse;
			iDrawIndicator = EFalse;

			iLevelState = ELevelUnloading;

			iAnimationFadeIndicator->RestartAnimation();
			iAnimationFadeIndicator2->RestartAnimation();
			}
		}
	else if ( aAnimation == iAnimationFadeIndicator )
		{
		if ( iLevelState == ELevelIndicator )
			{
			iLevelStateTimeCounter = 0;
			iLevelState = ELevelHintAndIndicator;
			}
		else if ( iLevelState == ELevelHint /*&& iAnimationFadeIndicator2->IsAnimationFinished()*/ )
			{
			//iLevelState = ELevelAfterHint;
			iDrawIndicator = EFalse;
			}
		}
	else if ( aAnimation == iAnimationFadeIndicator2 )
		{
		if ( iAnimationFadeIndicator2->FadeType() == CHermitAnimationFade::EFadeOut )
			{
			iAnimationFadeIndicator2->StartAnimation( CHermitAnimationFade::EFadeIn );

			if ( iCurrentMove == KTutorialMovesCount[ iCurrentLevel ] - 1 &&
			     !iNextLevelAnimation )
				{
				PlaySound( KHermitSoundEndGameWinId );
				}
			}
		else
			{
			if ( !iAnimationFadeLevel->IsAnimationStarted() )
				{
				iLevelState = ELevelAfterHint;
				iDrawIndicator = EFalse;
				}
			}
		}
	}

TBool CHermitMenuPageTutorial::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished() && iAnimationFadeLevel->IsAnimationFinished();
	}


