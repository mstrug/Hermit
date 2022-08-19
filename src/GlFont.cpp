/*
 ============================================================================
 Name		: GlFont.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlFont implementation
 ============================================================================
 */

#include "GlFont.h"
#include <SGEGl/GlTexture.h>
#include <SGEGl/GlObjectRect.h>
#include <SGEGl/GlUtils.h>
#include <SGEGl/GlPrimitive.h>
#include <SGEGl/GlVertexArray.h>
#include <gles/gl.h>


CGlFont::TLetterMargins::TLetterMargins() : iLeft( 0 ), iRight( 0 )
	{
	}
TBool CGlFont::TLetterMargins::IsZero()
	{
	return ( iLeft == 0 && iRight == 0 );
	}




CGlFont::CGlFont( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset )
	: iFontTexture( aFontTexture ), iLetterSize( aLetterSize ), iCharacterOffset( aOffset )
	{
	}

CGlFont::~CGlFont()
	{
	iLettersMargins.Close();
	delete iRect;
	}

CGlFont* CGlFont::NewL( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset )
	{
	CGlFont* self = new (ELeave) CGlFont( aFontTexture, aLetterSize, aOffset );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CGlFont::ConstructL()
	{
	iRect = CGlObjectRect::NewL();

	for ( TInt i = 0; i < iFontTexture.Size().iHeight / iLetterSize.iHeight; i++ )
		{
		iLettersMargins.AppendL( TLetterMargins() );
		}
	}

TInt CGlFont::GetTextWidth( const TDesC& aText )
	{
	TInt ret = 0;
	for ( TInt i = 0; i < aText.Length(); i++ )
		{
		TInt idx = aText[ i ] - iCharacterOffset;
		if ( idx >= 0 && idx < iLettersMargins.Count() && !iLettersMargins[ idx ].IsZero() )
			{
			ret += iLetterSize.iWidth - iLettersMargins[ idx ].iLeft - iLettersMargins[ idx ].iRight;
			}
		else  if ( idx >= 0 )
			{
			ret += iLetterSize.iWidth;
			}
		}
	return ret;
#ifdef _SGE_BADA_
	//return ( aText.GetLength() * iLetterSize.iWidth );
#endif
	}

TInt CGlFont::GetMaxHeight()
	{
	return iLetterSize.iHeight;
	}

TSize CGlFont::LetterSize() const
	{
	return iLetterSize;
	}

void CGlFont::AddLetterMargins( TText aLetter, TInt aLeftPixels, TInt aRightPixels )
	{
	TInt idx = aLetter - iCharacterOffset;
	if ( idx >= 0 && idx < iLettersMargins.Count() )
		{
		iLettersMargins[ idx ].iLeft = aLeftPixels;
		iLettersMargins[ idx ].iRight = aRightPixels;
		}
	}

void CGlFont::DrawText( TPoint aPosition, const TDesC& aText )
	{
	glPushMatrix();
	GlUtils::Translate2D( aPosition );

	//GLfixed lcx = ( iLetterSize.iWidth << 16 ) / iFontTexture.Size().iWidth;
	GLfixed lcy = ( iLetterSize.iHeight << 16 ) / iFontTexture.Size().iHeight;

	for ( TInt i = 0; i < aText.Length(); i++ )
		{
		TInt idx = aText[ i ] - iCharacterOffset;
		if ( idx >= 0 && idx < iLettersMargins.Count() && !iLettersMargins[ idx ].IsZero() )
			{
			glPushMatrix();
			TInt scaleX = iLetterSize.iWidth - iLettersMargins[ idx ].iLeft - iLettersMargins[ idx ].iRight;
			glScalef( scaleX, iLetterSize.iHeight, 0 );

			iFontTexture.Use();

			GLfixed v1 = lcy * idx;
			GLfixed v2 = lcy * ( idx + 1 );
			GLfixed w1 = ( ( 1 << 16 ) / iLetterSize.iWidth ) * iLettersMargins[ idx ].iLeft;
			GLfixed w2 = ( 1 << 16 ) - ( ( 1 << 16 ) / iLetterSize.iWidth ) * iLettersMargins[ idx ].iRight;

			GLfixed KTexCoords[ 8 ] =
			    {
			    w1, v1,
			    w2, v1,
			    w2, v2,
			    w1, v2
			    };

			iRect->Primitive().VertexArray( 0 )->SetTexCoordData( 0, (void*) KTexCoords, 8 * sizeof( GLfixed ) );
			iRect->Draw();

			glPopMatrix();
			glTranslatef( scaleX, 0, 0 );
			}
		else if ( idx >= 0 )
			{
			glPushMatrix();
			GlUtils::Scale2D( iLetterSize );

			iFontTexture.Use();

			GLfixed v1 = lcy * idx;
			GLfixed v2 = lcy * ( idx + 1 );

			GLfixed KTexCoords[ 8 ] =
			    {
			    0, v1,
			    1 << 16, v1,
			    1 << 16, v2,
			    0, v2
			    };

			iRect->Primitive().VertexArray( 0 )->SetTexCoordData( 0, (void*) KTexCoords, 8 * sizeof( GLfixed ) );
			iRect->Draw();

			glPopMatrix();
			glTranslatef( iLetterSize.iWidth, 0, 0 );

			//AppLog( "i: %d       idx: %d       v1: %x  v2: %x", i, idx, v1, v2 );
			}
		}


	iFontTexture.Discard();
	glPopMatrix();
	}

void CGlFont::DrawText( CBitmapContext& /*aGc*/, TPoint aPosition, const TDesC& aText )
    {
    DrawText( aPosition, aText );
    }

void CGlFont::DrawText( CBitmapContext& /*aGc*/, TRect /*aRect*/, const TDesC& /*aText*/, 
               TSGEFontAlignHorizontal /*aHorizontalAlign*/,
               TSGEFontAlignVertical /*aVerticalAlign*/ )
    {
    // not supported
    }

