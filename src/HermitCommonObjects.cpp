/*
 ============================================================================
  Name        : HermitCommonObjects.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitCommonObjects.h"
#include "HermitLevelProvider.h"
#include "HermitBackground.h"
#include "HermitSettings.h"
#include "HermitGamesStore.h"
#include <SGEGl/GlTextureManager.h>



// -----------------------------------------------------------------------------
// CHermitCommonObjects::CHermitCommonObjects()
// -----------------------------------------------------------------------------
//
CHermitCommonObjects::CHermitCommonObjects()
    {
    }

// -----------------------------------------------------------------------------
// CHermitCommonObjects::~CHermitCommonObjects()
// -----------------------------------------------------------------------------
//
CHermitCommonObjects::~CHermitCommonObjects()
    {
    delete iTextureManager;
    delete iLevelProvider;
    delete iBackground;
    delete iSettings;
    delete iGamesStore;
    }

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetTextureManager()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetTextureManager( CGlTextureManager *aTextureManager )
    {
    delete iTextureManager;
    iTextureManager = aTextureManager;
    }

// -----------------------------------------------------------------------------
// CHermitCommonObjects::TextureManager()
// -----------------------------------------------------------------------------
//
CGlTextureManager* CHermitCommonObjects::TextureManager() const
    {
    return iTextureManager;
    }

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetLevelProvider()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetLevelProvider( CHermitLevelProvider *aLevelProvider )
	{
	delete iLevelProvider;
	iLevelProvider = aLevelProvider;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::LevelProvider()
// -----------------------------------------------------------------------------
//
CHermitLevelProvider* CHermitCommonObjects::LevelProvider() const
	{
	return iLevelProvider;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::LevelProvider()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetBackground( CHermitBackground *aBackground )
	{
	delete iBackground;
	iBackground = aBackground;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::LevelProvider()
// -----------------------------------------------------------------------------
//
CHermitBackground* CHermitCommonObjects::Background() const
	{
	return iBackground;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetSettings()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetSettings( CHermitSettings *aSettings )
	{
	delete iSettings;
	iSettings = aSettings;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::Settings()
// -----------------------------------------------------------------------------
//
CHermitSettings* CHermitCommonObjects::Settings() const
	{
	return iSettings;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetPageSettings()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetPageSettings( CHermitMenuPageSettings *aPageSettings )
	{
	iPageSettings = aPageSettings;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::PageSettings()
// -----------------------------------------------------------------------------
//
CHermitMenuPageSettings* CHermitCommonObjects::PageSettings() const
	{
	return iPageSettings;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetMenuTitle()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetMenuTitle( CHermitMenuTitle *aTitle )
	{
	iTitle = aTitle;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::MenuTitle()
// -----------------------------------------------------------------------------
//
CHermitMenuTitle* CHermitCommonObjects::MenuTitle() const
	{
	return iTitle;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetGamesStore()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetGamesStore( CHermitGamesStore *aGamesStore )
	{
	delete iGamesStore;
	iGamesStore = aGamesStore;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::GamesStore()
// -----------------------------------------------------------------------------
//
CHermitGamesStore* CHermitCommonObjects::GamesStore() const
	{
	return iGamesStore;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::SetFont1()
// -----------------------------------------------------------------------------
//
void CHermitCommonObjects::SetFont1( CGlFont *aFont1 )
	{
	iFont1 = aFont1;
	}

// -----------------------------------------------------------------------------
// CHermitCommonObjects::Font1()
// -----------------------------------------------------------------------------
//
CGlFont* CHermitCommonObjects::Font1() const
	{
	return iFont1;
	}


// End of file
