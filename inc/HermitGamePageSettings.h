/*
 ============================================================================
  Name        : HermitGamePageSettings.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITGAMEPAGESETTINGS_H_
#define HERMITGAMEPAGESETTINGS_H_

#include "HermitMenuPageSettings.h"



class CHermitGamePageSettings : public CHermitMenuPageSettings
	{

    CHermitGamePageSettings( MHermitPageObserver& aObserver, CHermitCommonObjects& aCommonObjects, TSize aSize )
        : CHermitMenuPageSettings( aObserver, aCommonObjects, aSize )
        {
        }

	};


#endif /* HERMITGAMEPAGESETTINGS_H_ */
