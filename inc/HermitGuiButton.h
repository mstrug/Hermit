/*
 ============================================================================
  Name        : HermitGuiButton.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITGUIBUTTON_H_
#define HERMITGUIBUTTON_H_

#include <e32def.h>
#include <e32base.h>


class THermitGuiButton
	{
public:

	THermitGuiButton();

	void Rotate();

public: // data

	TRect iRect;

	TSize iTextureSize;

	TInt iTextureId;

	TInt iActionId;

	TBool iSelected;

	TInt iRotate;

	TBool iEnabled;

	TBool iVisible;

	};

#endif /* HERMITGUIBUTTON_H_ */
