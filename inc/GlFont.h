/*
 ============================================================================
 Name		: GlFont.h
 Author	  : Michal Strug
 Version	 : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : TGlFont declaration
 ============================================================================
 */

#ifndef GLFONT_H
#define GLFONT_H


// INCLUDES
#ifdef _SGE_SYMBIAN_
#include <e32std.h>
#include <e32base.h>
#include <gles/gl.h>
#include <SGEFont/SGEFont.h>
#include <SGEGl/GlTexture.h>
#elif defined _SGE_BADA_
#include <FGraphicsOpengl.h>
using namespace Osp::Graphics::Opengl;
#include <e32def.h>
#include <SGE/SGEFont.h>
#include <SGEGl/GlTexture.h>
#endif

// CLASS DECLARATION

class CGlObjectRect;


const TText KDefaultFirstCharOffset = 0x20; // space


/**
 *  CGlFont
 *
 */
class CGlFont : public CSGEFont
    {
public:

	virtual ~CGlFont();

	static CGlFont* NewL( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset = KDefaultFirstCharOffset );

private:

	CGlFont( TGlTexture& aFontTexture, TSize aLetterSize, TText aOffset );

	void ConstructL();

public: // from base class

    TInt GetTextWidth( const TDesC& aText );

    TInt GetMaxHeight();

    TSize LetterSize() const;

    void DrawText( TPoint aPosition, const TDesC& aText );

    void DrawText( CBitmapContext &aGc, TPoint aPosition, const TDesC& aText );
    
    void DrawText( CBitmapContext &aGc, TRect aRect, const TDesC& aText, 
                   TSGEFontAlignHorizontal aHorizontalAlign = ESGEFontAlignHorizontalCenter,
                   TSGEFontAlignVertical aVerticalAlign = ESGEFontAlignVerticalCenter );
    
    void AddLetterMargins( TText aLetter, TInt aLeftPixels, TInt aRightPixels );

private: // data

	TGlTexture& iFontTexture;

	TSize iLetterSize;

	TText iCharacterOffset;

	class TLetterMargins
		{
	public:
		TLetterMargins();
		TBool IsZero();

		TInt16 iLeft;
		TInt16 iRight;
		};

	RArray< TLetterMargins > iLettersMargins;

	CGlObjectRect* iRect; // owned

    };


#endif // GLFONT_H
