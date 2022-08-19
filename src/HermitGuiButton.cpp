/*
 ============================================================================
  Name        : HermitGuiButton.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitGuiButton.h"
#include <SGEGl/GlUtils.h>
#include <gles/gl.h>
#ifdef _SGE_BADA_
#include <FGraphicsOpengl.h>
using namespace Osp::Graphics::Opengl;
#endif


THermitGuiButton::THermitGuiButton() : iTextureId( 0 ), iActionId( 0 ), iSelected( EFalse ), iRotate( 0 ),
		iEnabled( ETrue ), iVisible( ETrue )
	{
	}

void THermitGuiButton::Rotate()
	{
	if ( iRotate == 90 || iRotate == -270 )
		{
		glRotatef( iRotate, 0, 0, 1 );
		GlUtils::Translate2D( TPoint( 0, -iRect.Height() ) );
		}
	else if ( iRotate == 180 )
		{
		glRotatef( iRotate, 0, 0, 1 );
		GlUtils::Translate2D( TPoint( -iRect.Width(), -iRect.Height() ) );
		}
	else if ( iRotate == 270 || iRotate == -90 )
		{
		glRotatef( iRotate, 0, 0, 1 );
		GlUtils::Translate2D( TPoint( -iRect.Width(), 0 ) );
		}
	}

