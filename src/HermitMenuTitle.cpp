/*
 ============================================================================
  Name        : HermitMenuTitle.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitMenuTitle.h"
#include "HermitCommonObjects.h"
#include "HermitConst.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlTextureManager.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>



const TUint32 KTitleElements[ KTitleSizeY ] =
	{
		0x577457,
		0x5456D2,
		0x767552,
		0x546452,
		0x575452
	};



CHermitMenuTitle::TTitleElemet::TTitleElemet() : iTextureId( 0 ), iVisible( EFalse )
	{
	}




CHermitMenuTitle::CHermitMenuTitle( CHermitCommonObjects& aCommonObjects )
	: iCommonObjects( aCommonObjects )
	{
	}

CHermitMenuTitle::~CHermitMenuTitle()
	{
	delete iRect;
	}

CHermitMenuTitle* CHermitMenuTitle::NewL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitMenuTitle* self = new (ELeave) CHermitMenuTitle( aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitMenuTitle::ConstructL()
	{
	iRect = CGlObjectRect::NewL();

	SetTitleElements();
	}

void CHermitMenuTitle::SetTitleElements( TUint32* aTitleElements )
	{
	if ( !aTitleElements )
		{
		aTitleElements = (TUint32*) KTitleElements;
		}

	for ( TInt i = 0; i < iTitleElemets.Count(); i++ )
		{
		for ( TInt j = 0; j < iTitleElemets[ i ].Count(); j++ )
			{
			TUint32 mask = 1;
			mask <<= ( iTitleElemets[ i ].Count() - j - 1 );
			iTitleElemets[ i ][ j ].iTextureId = ( aTitleElements[ i ] & mask ? 1 : 0 );
//			if ( aTitleElements[ i ] & mask )
//				{
//				iTitleElemets[ i ][ j ].iTextureId = KHermitTexImageTitleId;
//				}
//			else
//				{
//				iTitleElemets[ i ][ j ].iTextureId = 0;
//				}
			}
		}
	}

void CHermitMenuTitle::SetRect( TRect aRect )
	{
	iTitleRect = aRect;
    //iTitleRect.iTl.iX = ( 360 - 15 * 23 ) / 2; 
    //iTitleRect.iBr.iX = iTitleRect.iTl.iX + 15 * 23; 
	}

TRect CHermitMenuTitle::Rect() const
	{
	return iTitleRect;
	}

TSize CHermitMenuTitle::ElementsCount()
	{
	return TSize( iTitleElemets[ 0 ].Count(), iTitleElemets.Count() );
	}

TSize CHermitMenuTitle::ElementsSize()
	{
	return TSize( 15, 15 );
	}

void CHermitMenuTitle::Draw( const TRect /*aDrawRect*/ )
	{
    GlUtils::EnableTexturing();
    GlUtils::EnableTransparency();
	
	glPushMatrix();
	GlUtils::Translate2D( iTitleRect.iTl );
	GlUtils::Scale2D( TSize( 16, 16 ) );
	glTranslatef( 0, 15.0/16.0 * ( iTitleElemets.Count() - 1 ), 0 );
	for ( TInt i = 0; i < iTitleElemets.Count(); i++ )
		{
		for ( TInt j = 0; j < iTitleElemets[ i ].Count(); j++ )
			{
			TInt tex = KHermitTexFieldNoneId;
			if ( iTitleElemets[ i ][ j ].iTextureId )
				{
				tex = KHermitTexImageTitleId;
				}
			iCommonObjects.TextureManager()->Texture( tex )->Use();

			if ( i == 0 && j == 0 )
			    {
			    iRect->Draw();
			    }
			else
			    {
			    iRect->Redraw();
			    }

			glTranslatef( 15.0/16.0, 0, 0 );
			}
		glTranslatef( -15.0/16.0 * iTitleElemets[ i ].Count(), -15.0/16.0, 0 );
		}
	glPopMatrix();
	}

TBool CHermitMenuTitle::Update( const TTimeIntervalMicroSeconds32 /*aTimeInterval*/ )
	{
	return ETrue;
	}


