/*
 ============================================================================
  Name        : HermitGuiImage.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITGUIIMAGE_H_
#define HERMITGUIIMAGE_H_

#include <e32def.h>
#include <e32base.h>


class THermitGuiImage
	{
public:

	THermitGuiImage();

public: // data

	TRect iRect;

	TSize iTextureSize;

	TInt iTextureId;

	};

#endif /* HERMITGUIIMAGE_H_ */
