/*
 ============================================================================
  Name        : HermitMenuPageSettings.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuPageSettings.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include "HermitSettings.h"
#include "HermitBackgroundGlObject.h"
#include "HermitBackground.h"
#include "HermitGamesStore.h"
#include "HermitAnimationButtonPressed.h"
#include "HermitCompilationSettings.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <SGEAudio/SGEAudioProvider.h>
#include <e32def.h>


const TSize KButtonSize( 64, 64 );
const TSize KButtonTexSize( 64, 64 );

const TSize KButtonBigSize( 128, 128 );
const TSize KButtonBigTexSize( 128, 128 );

const TSize KButtonSmallSize( 64, 64 );     // indicators
const TSize KButtonSmallTexSize( 64, 64 );  // mip maps used

const TSize KImageSoundSize( 64, 64 );
const TSize KImageSoundTexSize( 64, 64 );

const TSize KImageMusicSize( 64, 64 );
const TSize KImageMusicTexSize( 64, 64 );

const TSize KImageLevelSize( 128, 64 );
const TSize KImageLevelTexSize( 128, 64 );

const TSize KImageTitleSize( 128, 64 );
const TSize KImageTitleTexSize( 128, 64 );

const TSize KImageColorsSize( 50, 50 );
const TSize KImageColorsTexSize( 64, 64 );

const TInt KBtnSndPIdx = 0;
const TInt KBtnSndMIdx = 1;
const TInt KBtnMusPIdx = 2;
const TInt KBtnMusMIdx = 3;
const TInt KBtnResetIdx = 4;
const TInt KBtnBackIdx = 5;
const TInt KBtnIndGreenIdx = 6;
const TInt KBtnIndOrangeIdx = 7;

const TInt KImgSndIdx = 0;
const TInt KImgSndLevIdx = 1;
const TInt KImgMusIdx = 2;
const TInt KImgMusLevIdx = 3;
const TInt KImgTitleIdx = 4;
const TInt KImgIndOrangeIdx = 5;

const TInt KColorsCount = KColorSets;
const TInt KColorsCountPerLine = KColorsCount / 2;


/** Used in setting music and sound colors */
/** Colors for vertices (Red, Green, Blue, Alpha). */
const TInt KColorsSize = 4 * 4 * sizeof( GLubyte );
const TInt KColorsLevelsCount = 5;
const TInt KColorsLevelsTransparency = 164;
static const GLubyte KColorsWhite[ KColorsSize ] =
    {
    255  ,255,  255, 255,
    255  ,255,  255, 255,
    255  ,255,  255, 255,
    255  ,255,  255, 255
    };
static const GLubyte KColorsLevel[ KColorsLevelsCount ][ KColorsSize ] =
{
	{   0, 255, 0, KColorsLevelsTransparency,
		51, 204, 0, KColorsLevelsTransparency,
	    51, 204, 0, KColorsLevelsTransparency,
	    0, 255, 0, KColorsLevelsTransparency		},
	{   0, 255, 0, KColorsLevelsTransparency,
		102, 153, 0, KColorsLevelsTransparency,
	    102, 153, 0, KColorsLevelsTransparency,
	    0, 255, 0, KColorsLevelsTransparency		},
	{   0, 255, 0, KColorsLevelsTransparency,
		153, 102, 0, KColorsLevelsTransparency,
		153, 102, 0, KColorsLevelsTransparency,
		0, 255, 0, KColorsLevelsTransparency		},
	{   0, 255, 0, KColorsLevelsTransparency,
		204, 51, 0, KColorsLevelsTransparency,
	    204, 51, 0, KColorsLevelsTransparency,
	    0, 255, 0, KColorsLevelsTransparency		},
	{   0, 255, 0, KColorsLevelsTransparency,
	    255, 0, 0, KColorsLevelsTransparency,
	    255, 0, 0, KColorsLevelsTransparency,
	    0, 255, 0, KColorsLevelsTransparency      }
};



CHermitMenuPageSettings::CHermitMenuPageSettings( MHermitPageObserver& aObserver,
												  CHermitCommonObjects& aCommonObjects, TSize aSize )
	: CHermitPageBase( aObserver, aCommonObjects, aSize )
    {
    }

CHermitMenuPageSettings::~CHermitMenuPageSettings()
    {
	delete iRectColorSet;
    }

CHermitMenuPageSettings* CHermitMenuPageSettings::NewL( MHermitPageObserver& aObserver,
													    CHermitCommonObjects& aCommonObjects, TSize aSize )
	{
	CHermitMenuPageSettings* self = new (ELeave) CHermitMenuPageSettings( aObserver, aCommonObjects, aSize );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuPageSettings::ConstructL()
	{
	BaseConstructL();

	iButtons[ KBtnSndPIdx ].iTextureId = KHermitTexMenuBtnPlusId;
	iButtons[ KBtnSndMIdx ].iTextureId = KHermitTexMenuBtnMinusId;
	iButtons[ KBtnMusPIdx ].iTextureId = KHermitTexMenuBtnPlusId;
	iButtons[ KBtnMusMIdx ].iTextureId = KHermitTexMenuBtnMinusId;
	iButtons[ KBtnResetIdx ].iTextureId = KHermitTexMenuBtnResetId;
	iButtons[ KBtnBackIdx ].iTextureId = KHermitTexMenuBtnBackId;
	iButtons[ KBtnIndGreenIdx ].iTextureId = KHermitTexFieldHintId;
	iButtons[ KBtnIndOrangeIdx ].iTextureId = KHermitTexFieldEmptyId; // + KHermitTexFieldEndIndId;

	iButtons[ KBtnBackIdx ].iActionId = KSettingsActionBtnBack;
	iButtons[ KBtnResetIdx ].iActionId = KSettingsActionBtnResetLevels;

	iButtons[ KBtnSndPIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnSndMIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnMusPIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnMusMIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnResetIdx ].iTextureSize = KButtonBigTexSize;
	iButtons[ KBtnBackIdx ].iTextureSize = KButtonTexSize;
	iButtons[ KBtnIndGreenIdx ].iTextureSize = KButtonSmallTexSize;
	iButtons[ KBtnIndOrangeIdx ].iTextureSize = KButtonSmallTexSize;

	iImages[ KImgSndIdx ].iTextureId = KHermitTexSettingsSoundId;
	iImages[ KImgSndLevIdx ].iTextureId = KHermitTexSettingsLevelId;
	iImages[ KImgMusIdx ].iTextureId = KHermitTexSettingsMusicId;
	iImages[ KImgMusLevIdx ].iTextureId = KHermitTexSettingsLevelId;
	iImages[ KImgTitleIdx ].iTextureId = KHermitTexSettingsTitleId;
	iImages[ KImgIndOrangeIdx ].iTextureId = KHermitTexFieldEndIndId;

	iImages[ KImgSndIdx ].iTextureSize = KImageSoundTexSize;
	iImages[ KImgSndLevIdx ].iTextureSize = KImageLevelTexSize;
	iImages[ KImgMusIdx ].iTextureSize = KImageMusicTexSize;
	iImages[ KImgMusLevIdx ].iTextureSize = KImageLevelTexSize;
	iImages[ KImgTitleIdx ].iTextureSize = KImageTitleTexSize;
	iImages[ KImgIndOrangeIdx ].iTextureSize = KButtonSmallTexSize;

	//iColorsImage.iTextureId = KHermitTexSettingsColorsId;
	//iColorsImage.iTextureSize = KImageColorsTexSize;

	for ( TInt i = 0; i < iColorsButtons.Count(); i++ )
		{
		iColorsButtons[ i ].iTextureId = KHermitTexSettingsColorsId;
		iColorsButtons[ i ].iTextureSize = KImageColorsTexSize;
		}

	SizeChanged( iSize );

	CHermitBackground::CreateColorSets( iColorSets );
	for ( TInt i = 0; i < iColorSets.Count(); i++ )
		{
		iColorSets[ i ].SetIncrementTime( 10000 );
		}

	iRectColorSet = CHermitBackgroundGlObject::NewL();

	iState = EIdle;
	}

void CHermitMenuPageSettings::SetObserver( MHermitPageObserver* aObserver )
	{
	iObserver = aObserver;
	}

void CHermitMenuPageSettings::ResetState()
	{
	iButtons[ KBtnResetIdx ].iVisible = iCommonObjects.GamesStore()->ResetDataAvailable();
	}

void CHermitMenuPageSettings::SetMusicTrackId( TInt aTrackId )
	{
	iMusicTrackId = aTrackId;
	}

void CHermitMenuPageSettings::SizeChanged( TSize aSize )
	{
	iSize = aSize;

	TInt marginX = iSize.iWidth / 24;
	TInt marginY = iSize.iHeight / 6 + KButtonSize.iHeight;
	TInt step = iSize.iHeight / 26;

	iButtons[ KBtnSndMIdx ].iRect = TRect( TPoint( marginX, iSize.iHeight - marginY ), KButtonSize );
	iButtons[ KBtnSndPIdx ].iRect = TRect( TPoint( iSize.iWidth - marginX - KButtonSize.iWidth, iSize.iHeight - marginY ), KButtonSize );
	iImages[ KImgSndIdx ].iRect = TRect( TPoint( iButtons[ KBtnSndMIdx ].iRect.iBr.iX, iButtons[ KBtnSndMIdx ].iRect.iTl.iY ), KImageSoundSize );
	iImages[ KImgSndLevIdx ].iRect = TRect( TPoint( iImages[ KImgSndIdx ].iRect.iBr.iX, iImages[ KImgSndIdx ].iRect.iTl.iY ), KImageLevelSize );

	iButtons[ KBtnMusMIdx ].iRect = TRect( TPoint( marginX, iSize.iHeight - marginY - step - KButtonSize.iHeight ), KButtonSize );
	iButtons[ KBtnMusPIdx ].iRect = TRect( TPoint( iSize.iWidth - marginX - KButtonSize.iWidth, iSize.iHeight - marginY - step - KButtonSize.iHeight ), KButtonSize );
	iImages[ KImgMusIdx ].iRect = TRect( TPoint( iButtons[ KBtnMusMIdx ].iRect.iBr.iX, iButtons[ KBtnMusMIdx ].iRect.iTl.iY ), KImageMusicSize );
	iImages[ KImgMusLevIdx ].iRect = TRect( TPoint( iImages[ KImgMusIdx ].iRect.iBr.iX, iImages[ KImgMusIdx ].iRect.iTl.iY ), KImageLevelSize );

	iButtons[ KBtnResetIdx ].iRect = TRect( TPoint( marginX, marginX + step ), KButtonBigSize );
	iButtons[ KBtnBackIdx ].iRect = TRect( TPoint( iSize.iWidth - KButtonSize.iWidth, 0 ), KButtonSize );
	iButtons[ KBtnIndGreenIdx ].iRect = TRect( TPoint( iSize.iWidth / 2 + step, iButtons[ KBtnResetIdx ].iRect.iBr.iY - KButtonSmallSize.iHeight ), KButtonSmallSize );
	iButtons[ KBtnIndOrangeIdx ].iRect = TRect( TPoint( iSize.iWidth - marginX - KButtonSmallSize.iWidth, iButtons[ KBtnIndGreenIdx ].iRect.iTl.iY ), KButtonSmallSize );

	iImages[ KImgTitleIdx ].iRect = TRect( TPoint( 0, iSize.iHeight - KImageTitleSize.iHeight ), KImageTitleSize );
	iImages[ KImgIndOrangeIdx ].iRect = iButtons[ KBtnIndOrangeIdx ].iRect;

	// color sets

	iColorsButtons[ 0 ].iRect = TRect( TPoint( ( iSize.iWidth - KColorsCountPerLine * KImageColorsSize.iWidth ) / 2, iButtons[ KBtnMusMIdx ].iRect.iTl.iY - 3 * step / 2 - KImageColorsSize.iHeight ), KImageColorsSize );
	for ( TInt i = 1; i < iColorsButtons.Count() / 2; i++ )
		{
		iColorsButtons[ i ].iRect = iColorsButtons[ i - 1 ].iRect;
		iColorsButtons[ i ].iRect.Move( KImageColorsSize.iWidth, 0 );
		}

	iColorsButtons[ KColorSets / 2 ].iRect = iColorsButtons[ 0 ].iRect;
	iColorsButtons[ KColorSets / 2 ].iRect.Move( 0, -KImageColorsSize.iHeight );
	for ( TInt i = KColorSets / 2 + 1; i < iColorsButtons.Count(); i++ )
		{
		iColorsButtons[ i ].iRect = iColorsButtons[ i - 1 ].iRect;
		iColorsButtons[ i ].iRect.Move( KImageColorsSize.iWidth, 0 );
		}
	}

void CHermitMenuPageSettings::DrawFills()
	{
//	iCommonObjects.TextureManager()->Texture( KHermitTexFieldNoneId )->Use();

    GlUtils::DisableTexturing();
    GlUtils::DisableTransparency();

    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, EFalse );
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( EFalse );

    glColor4ub( 0, 0, 0, 255 );    

	TInt marginX = iSize.iWidth / 24;
	TInt step = iSize.iHeight / 26;

	// right side of title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iImages[ KImgTitleIdx ].iRect.iBr.iX, iImages[ KImgTitleIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iImages[ KImgTitleIdx ].iRect.iBr.iX, step * 3 ) );
	iRect->Draw();
	glPopMatrix();

	// under title
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnSndMIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iImages[ KImgTitleIdx ].iRect.iTl.iY - iButtons[ KBtnSndMIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// left side
	glPushMatrix();
	GlUtils::Scale2D( TSize( marginX, iImages[ KImgTitleIdx ].iRect.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// right side
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iSize.iWidth - marginX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( marginX, iSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// bottom
	glPushMatrix();
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX, iButtons[ KBtnResetIdx ].iRect.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// between sound & music
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnMusMIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, step ) );
	iRect->Redraw();
	glPopMatrix();

	// between music & colors
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iColorsButtons[ 0 ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, 3 * step / 2 ) );
	iRect->Redraw();
	glPopMatrix();

	// between colors & indicators
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnIndGreenIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth, iColorsButtons[ iColorsButtons.Count() - 1 ].iRect.iTl.iY - iButtons[ KBtnIndGreenIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// sound level right fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iImages[ KImgSndLevIdx ].iRect.iBr.iX, iImages[ KImgSndLevIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnSndPIdx ].iRect.iTl.iX - iImages[ KImgSndLevIdx ].iRect.iBr.iX, KButtonSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// music level right fill
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iImages[ KImgMusLevIdx ].iRect.iBr.iX, iImages[ KImgMusLevIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnMusPIdx ].iRect.iTl.iX - iImages[ KImgMusLevIdx ].iRect.iBr.iX, KButtonSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// above reset
	glPushMatrix();
	GlUtils::Translate2D( TPoint( 0, iButtons[ KBtnResetIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnIndGreenIdx ].iRect.iTl.iX, iButtons[ KBtnIndGreenIdx ].iRect.iBr.iY - iButtons[ KBtnResetIdx ].iRect.iBr.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// left side of indicator
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnResetIdx ].iRect.iBr.iX, iButtons[ KBtnIndGreenIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnIndGreenIdx ].iRect.iTl.iX - iButtons[ KBtnResetIdx ].iRect.iBr.iX, KButtonSmallSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// between indicators
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnIndGreenIdx ].iRect.iBr.iX, iButtons[ KBtnIndGreenIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnIndOrangeIdx ].iRect.iTl.iX - iButtons[ KBtnIndGreenIdx ].iRect.iBr.iX, KButtonSmallSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// right side of indicator
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnIndOrangeIdx ].iRect.iBr.iX, iButtons[ KBtnIndOrangeIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnIndOrangeIdx ].iRect.iBr.iX, KButtonSmallSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// below indocators
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnResetIdx ].iRect.iBr.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY ) );
	GlUtils::Scale2D( TSize( iSize.iWidth - iButtons[ KBtnResetIdx ].iRect.iBr.iX, iButtons[ KBtnIndOrangeIdx ].iRect.iTl.iY - KButtonSize.iHeight ) );
	iRect->Redraw();
	glPopMatrix();

	// below indocators lower
	glPushMatrix();
	GlUtils::Translate2D( TPoint( iButtons[ KBtnResetIdx ].iRect.iBr.iX, iButtons[ KBtnResetIdx ].iRect.iTl.iY ) );
	GlUtils::Scale2D( TSize( iButtons[ KBtnBackIdx ].iRect.iTl.iX - iButtons[ KBtnResetIdx ].iRect.iBr.iX, iButtons[ KBtnBackIdx ].iRect.iBr.iY - iButtons[ KBtnResetIdx ].iRect.iTl.iY ) );
	iRect->Redraw();
	glPopMatrix();

	// disabled reset levels button
	if ( !iButtons[ KBtnResetIdx ].iVisible )
		{
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ KBtnResetIdx ].iRect.iTl );
		GlUtils::Scale2D( iButtons[ KBtnResetIdx ].iRect.Size() );
		iRect->Redraw();
		glPopMatrix();
		}

    
    iRect->Primitive().VertexArray( 0 )->ColorArrayEnabled( ETrue );
    GlUtils::EnableTransparency();  

    glColor4ub( 255, 255, 255, 255 );    

	// sound level fill
	//iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Discard();
	TInt m1 = 7;
	TInt s1 = 23;
	TInt volstep = KVolumeStep;
	TInt lev = iCommonObjects.Settings()->SoundVolume();
	if ( lev / volstep - 1 >= 0 && lev / volstep - 1 < KColorsLevelsCount )
		{
		glPushMatrix();
		iRect->Primitive().VertexArray( 0 )->SetColorData( (TAny*) KColorsLevel[ lev / volstep - 1 ], KColorsSize );
		GlUtils::Translate2D( TPoint( iImages[ KImgSndLevIdx ].iRect.iTl.iX + m1, iImages[ KImgSndLevIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( s1 * lev / volstep, KButtonSize.iHeight ) );
		iRect->Draw();
		glPopMatrix();
		}

	// music level fill
	lev = iCommonObjects.Settings()->MusicVolume();
	if ( lev / volstep - 1 >= 0 && lev / volstep - 1 < KColorsLevelsCount )
		{
		glPushMatrix();
		iRect->Primitive().VertexArray( 0 )->SetColorData( (TAny*) KColorsLevel[ lev / volstep - 1 ], KColorsSize );
		GlUtils::Translate2D( TPoint( iImages[ KImgMusLevIdx ].iRect.iTl.iX + m1, iImages[ KImgMusLevIdx ].iRect.iTl.iY ) );
		GlUtils::Scale2D( TSize( s1 * lev / volstep, KButtonSize.iHeight ) );
            iRect->Draw();
		glPopMatrix();
		}
	iRect->Primitive().VertexArray( 0 )->SetColorData( (TAny*) KColorsWhite, KColorsSize );
	
    iRect->Primitive().VertexArray( 0 )->TexCoordArrayEnabled( 0, ETrue );
    GlUtils::EnableTexturing();
	}

void CHermitMenuPageSettings::Draw( const TRect aDrawRect )
	{
	DrawFills();
        
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();

	// buttons
	TBool drawed = EFalse;
	for ( TInt i = 0; i < iButtons.Count(); i++ )
		{
		if ( !iButtons[ i ].iVisible )
			{
			continue;
			}
		if ( iButtons[ i ].iSelected )
			{
			glPushMatrix();
			GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( iButtons[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			if ( drawed ) iRect->Redraw();
			else { drawed = ETrue; iRect->Draw(); }
			glPopMatrix();
			}
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( iButtons[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iButtons[ i ].iTextureId )->Use();
		if ( drawed ) iRect->Redraw();
		else { drawed = ETrue; iRect->Draw(); }
		glPopMatrix();
		}

	// images
	for ( TInt i = 0; i < iImages.Count(); i++ )
		{
		if ( ( i == KImgSndIdx && ( iButtons[ KBtnSndPIdx ].iSelected || iButtons[ KBtnSndMIdx ].iSelected  ) ) ||
			 ( i == KImgMusIdx && ( iButtons[ KBtnMusPIdx ].iSelected || iButtons[ KBtnMusMIdx ].iSelected  ) )	)
			{
			glPushMatrix();
			GlUtils::Translate2D( iImages[ i ].iRect.iTl );
			GlUtils::Scale2D( iImages[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			iRect->Redraw();
			glPopMatrix();
			}

		glPushMatrix();
		GlUtils::Translate2D( iImages[ i ].iRect.iTl );
		GlUtils::Scale2D( iImages[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iImages[ i ].iTextureId )->Use();
		iRect->Redraw();
		glPopMatrix();
		}

	// backgrounds
	for ( TInt i = 0; i < iColorsButtons.Count(); i++ )
		{
		// animated background
		glPushMatrix();
		GlUtils::Translate2D( iColorsButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( iColorsButtons[ i ].iRect.Size() );
		iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Discard();
		iRectColorSet->SetColors( (TAny*) iColorSets[ i ].iColors.Begin(), iColorSets[ i ].iColors.Count() );
		iRectColorSet->Draw();
		glPopMatrix();

		// selected
		if ( iColorsButtons[ i ].iSelected )
			{
			glPushMatrix();
			GlUtils::Translate2D( iColorsButtons[ i ].iRect.iTl );
			GlUtils::Scale2D( iColorsButtons[ i ].iRect.Size() );
			iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSelectedId )->Use();
			iRect->Draw();
			glPopMatrix();
			}

		// black fill
		glPushMatrix();
		GlUtils::Translate2D( iColorsButtons[ i ].iRect.iTl );
		GlUtils::Scale2D( iColorsButtons[ i ].iTextureSize );
		iCommonObjects.TextureManager()->Texture( iColorsButtons[ i ].iTextureId )->Use();
		iRect->Draw();
		if ( i == iCommonObjects.Settings()->ColorSet() )
			{
			iCommonObjects.TextureManager()->Texture( KHermitTexSettingsSelId )->Use();
			iRect->Redraw();
			}
		glPopMatrix();
		}

	// indicators
	if ( iCommonObjects.Settings()->GreenIndicator() )
		{
		glPushMatrix();
        GlUtils::Translate2D( iButtons[ KBtnIndGreenIdx ].iRect.iTl );
        GlUtils::Translate2D( TPoint( 7, 7 ) ); // offset: 50x50 check sign -> 64x64
		GlUtils::Scale2D( KButtonSmallTexSize );
		iCommonObjects.TextureManager()->Texture( KHermitTexSettingsSelId )->Use();
		iRect->Redraw();
		glPopMatrix();
		}
	if ( iCommonObjects.Settings()->OrangeIndicator() )
		{
		glPushMatrix();
		GlUtils::Translate2D( iButtons[ KBtnIndOrangeIdx ].iRect.iTl );
        GlUtils::Translate2D( TPoint( 7, 7 ) ); // offset: 50x50 check sign -> 64x64
		GlUtils::Scale2D( KButtonSmallTexSize );
		iCommonObjects.TextureManager()->Texture( KHermitTexSettingsSelId )->Use();
		iRect->Redraw();
		glPopMatrix();
		}

	// button pressed animation
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Draw( aDrawRect );
		}

	CHermitPageBase::Draw( aDrawRect );

	iCommonObjects.TextureManager()->Texture( KHermitTexMenuBtnSettingsId )->Discard();
	}

TBool CHermitMenuPageSettings::Update( const TTimeIntervalMicroSeconds32 aTimeInterval )
	{
	CHermitPageBase::Update( aTimeInterval );

	if ( iState > EEntering && iState < ELeaving  )
	    {
        for ( TInt i = 0; i < iColorSets.Count(); i++ )
            {
            iColorSets[ i ].Update( aTimeInterval );
            }
	    }
	
	if ( iState == EAnimation )
		{
		iAnimationButtonPressed->Update( aTimeInterval );
		}
	return ETrue;
	}

TBool CHermitMenuPageSettings::PointerEvent( const TPointerEvent &aPointerEvent )
	{
	if ( iState != EIdle && iState != EEntering )
		{
		return EFalse;
		}

	TBool ret = EFalse;

	if ( aPointerEvent.iType == TPointerEvent::EButton1Down )
		{
		iButtonDownPos = aPointerEvent.iPosition;
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iVisible && iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iButtons[ i ].iSelected = ETrue;
				return ETrue;
				}
			}
		for ( TInt i = 0; i < iColorsButtons.Count(); i++ )
			{
			if ( iColorsButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iColorsButtons[ i ].iSelected = ETrue;
				return ETrue;
				}
			}
		}
	else if ( aPointerEvent.iType == TPointerEvent::EButton1Up )
		{
		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			if ( iButtons[ i ].iSelected &&
				 iButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				if ( iButtons[ i ].iActionId )
					{
					iButtons[ i ].iSelected = EFalse;
					if ( i == KBtnBackIdx )
						{
						iCommonObjects.Settings()->SaveToFile();
						}
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
				else
					{
					if ( i == KBtnIndGreenIdx )
						{
						iCommonObjects.Settings()->ToggleGreenIndicator();
						}
					else if ( i == KBtnIndOrangeIdx )
						{
						iCommonObjects.Settings()->ToggleOrangeIndicator();
						}
					else if ( i == KBtnMusMIdx )
						{
						TInt oldMv = iCommonObjects.Settings()->MusicVolume();
						TInt mv = iCommonObjects.Settings()->StepMusicVolume( -1 );
						TInt mv1 = CalculateMusicVolumeShift( mv );
						iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupMusic, mv1 );
						iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupMusic, mv1 );
						if ( oldMv > 0 && mv == 0 && iMusicTrackId >= 0 )
							{ // pause music playback if music was played previously
							iCommonObjects.AudioProvider()->Pause( iMusicTrackId );
							}
						}
					else if ( i == KBtnMusPIdx )
						{
						TInt oldMv = iCommonObjects.Settings()->MusicVolume();
						TInt mv = iCommonObjects.Settings()->StepMusicVolume( +1 );
						TInt mv1 = CalculateMusicVolumeShift( mv );
						iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupMusic, mv1 );
						iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupMusic, mv1 );
						if ( oldMv == 0 && mv > 0 )
							{ // start music playback
							if ( iMusicTrackId >= 0 )
								{ // resume
								iCommonObjects.AudioProvider()->Resume( iMusicTrackId );
								}
							else
								{ // start playing
								iMusicTrackId = iCommonObjects.AudioProvider()->Play( KHermitMusic1Id, ESGEAudioRepeatInfinite );
								}
							}
						}
					else if ( i == KBtnSndMIdx )
						{
						TInt sv = iCommonObjects.Settings()->StepSoundVolume( -1 );
				        sv = CalculateSoundVolumeShift( sv );
						iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupSound, sv );
						iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupSound, sv );
						}
					else if ( i == KBtnSndPIdx )
						{
						TInt sv = iCommonObjects.Settings()->StepSoundVolume( +1 );
				        sv = CalculateSoundVolumeShift( sv );
						iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupSound, sv );
						iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupSound, sv );
						}

					PlaySound( KHermitSoundButton2PressId );

					ret = ETrue;
					}
				}
			}
		for ( TInt i = 0; i < iColorsButtons.Count(); i++ )
			{
			if ( iColorsButtons[ i ].iSelected &&
				iColorsButtons[ i ].iRect.Contains( aPointerEvent.iPosition ) )
				{
				iColorsButtons[ i ].iSelected = EFalse;
				iCommonObjects.Settings()->SetColorSet( i );
				iCommonObjects.Background()->SetCurrentColors( i, iColorSets[ i ] );

				PlaySound( KHermitSoundButton2PressId );
				}
			}

		for ( TInt i = 0; i < iButtons.Count(); i++ )
			{
			iButtons[ i ].iSelected = EFalse;
			}
		for ( TInt i = 0; i < iColorsButtons.Count(); i++ )
			{
			iColorsButtons[ i ].iSelected = EFalse;
			}
		}

	return ret;
	}

TInt CHermitMenuPageSettings::CalculateSoundVolumeShift( TInt aValue )
    {
    if ( aValue > 80 )
        {
        return 3;
        }
    else if ( aValue > 60 )
        {
        return 2;
        }
    else if ( aValue > 40 )
        {
        return 1;
        }
    else if ( aValue > 20 )
        {
        return 0;
        }
    else if ( aValue > 0 )
        {
        return -1;
        }
    else // 0
        {
        return -15;
        }
    }

TInt CHermitMenuPageSettings::CalculateMusicVolumeShift( TInt aValue )
    {
    if ( aValue > 80 )
        {
        return 1;
        }
    else if ( aValue > 60 )
        {
        return 0;
        }
    else if ( aValue > 40 )
        {
        return -1;
        }
    else if ( aValue > 20 )
        {
        return -2;
        }
    else if ( aValue > 0 )
        {
        return -3;
        }
    else // 0
        {
        return -15;
        }
    }

void CHermitMenuPageSettings::KeyEventVolumeUp()
	{
	TInt mv = iCommonObjects.Settings()->MusicVolume();
	TInt sv = iCommonObjects.Settings()->SoundVolume();
	TInt oldMv = mv;
	TInt mv1;
	
	if ( mv == 0 && sv != 0 )
		{
		sv = iCommonObjects.Settings()->StepSoundVolume( +1 );
		sv = CalculateSoundVolumeShift( sv );
		iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupSound, sv );
		iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupSound, sv );
		}
	else if ( mv != 0 && sv == 0 )
		{
		mv = iCommonObjects.Settings()->StepMusicVolume( +1 );
        mv1 = CalculateMusicVolumeShift( mv );
		iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupMusic, mv1 );
		iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupMusic, sv );
		}
	else
		{
		mv = iCommonObjects.Settings()->StepMusicVolume( +1 );
        mv1 = CalculateMusicVolumeShift( mv );
		sv = iCommonObjects.Settings()->StepSoundVolume( +1 );
        sv = CalculateSoundVolumeShift( sv );
		iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupMusic, mv1 );
		iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupMusic, mv1 );
		iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupSound, sv );
		iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupSound, sv );
		}

	if ( oldMv == 0 && mv > 0 )
		{ // start music playback
		if ( iMusicTrackId >= 0 )
			{ // resume
			iCommonObjects.AudioProvider()->Resume( iMusicTrackId );
			}
		else
			{ // start playing
			iMusicTrackId = iCommonObjects.AudioProvider()->Play( KHermitMusic1Id, ESGEAudioRepeatInfinite );
			}
		}
	}

void CHermitMenuPageSettings::KeyEventVolumeDown()
	{
	TInt oldMv = iCommonObjects.Settings()->MusicVolume();
	TInt mv = iCommonObjects.Settings()->StepMusicVolume( -1 );
	TInt sv = iCommonObjects.Settings()->StepSoundVolume( -1 );
	TInt mv1 = CalculateMusicVolumeShift( mv );
    sv = CalculateSoundVolumeShift( sv );
	iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupMusic, mv1 );
	iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupMusic, mv1 );
	iCommonObjects.AudioProvider()->SetVolumeGroup( ESGEAudioGroupSound, sv );
	iCommonObjects.AudioProvider()->SetVolumeDefault( ESGEAudioGroupSound, sv );

	if ( oldMv > 0 && mv == 0 && iMusicTrackId >= 0 )
		{ // pause music playback if music was played previously
		iCommonObjects.AudioProvider()->Pause( iMusicTrackId );
		}
	}

void CHermitMenuPageSettings::AnimationFinished( CHermitAnimationBase* aAnimation )
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
	}

TBool CHermitMenuPageSettings::AllAnimationsFinished()
	{
	return iAnimationButtonPressed->IsAnimationFinished();
	}

