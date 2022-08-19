/*
 ============================================================================
 Name        : HermitEntry.cpp
 Author      : Michal Strug
 Version     : 1.0
 Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
 Description : 
 ============================================================================
 */

#include "HermitApplication.h"

// -----------------------------------------------------------------------------
// E32Main()
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
    {
    return CHermitApplication::Start( CHermitApplication::NewApplication );
    }

// End of file
