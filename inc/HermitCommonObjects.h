/*
 ============================================================================
  Name        : HermitCommonObjects.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITCOMMONOBJECTS_H_
#define HERMITCOMMONOBJECTS_H_

// INCLUDES
#include <SGEEngine/SGEEngineCommonObjects.h>

// FORWARD DECLARATIONS
class CGlTextureManager;
class CHermitLevelProvider;
class CHermitBackground;
class CHermitSettings;
class CHermitMenuPageSettings;
class CHermitMenuTitle;
class CHermitGamesStore;
class CGlFont;

// CLASS DECLARATION

/**
 *  CHermitCommonObjects
 *
 */
class CHermitCommonObjects : public CSGEEngineCommonObjects
	{
public:

    /**
     * Constructor.
     */
	CHermitCommonObjects();

    /**
     * Destructor.
     */
    ~CHermitCommonObjects();

    /**
     * Sets texture manager.
     */
    void SetTextureManager( CGlTextureManager *aTextureManager );

    /**
     * Returns texture manager.
     */
    CGlTextureManager* TextureManager() const;

    /**
     * Sets level provider.
     */
    void SetLevelProvider( CHermitLevelProvider *aLevelProvider );

    /**
     * Returns level provider.
     */
    CHermitLevelProvider* LevelProvider() const;

    /**
     * Sets background object.
     */
    void SetBackground( CHermitBackground *aBackground );

    /**
     * Returns background object.
     */
    CHermitBackground* Background() const;

    /**
     * Sets settings object.
     */
    void SetSettings( CHermitSettings *aSettings );

    /**
     * Returns settings object.
     */
    CHermitSettings* Settings() const;

    /**
     * Sets GUI page settings object.
     */
    void SetPageSettings( CHermitMenuPageSettings *aPageSettings );

    /**
     * Returns GUI page settings object.
     */
    CHermitMenuPageSettings* PageSettings() const;

    /**
     * Sets GUI page title object.
     */
    void SetMenuTitle( CHermitMenuTitle *aTitle );

    /**
     * Returns GUI page title object.
     */
    CHermitMenuTitle* MenuTitle() const;

    /**
     * Sets games store object.
     */
    void SetGamesStore( CHermitGamesStore *aGamesStore );

    /**
     * Returns games store object.
     */
    CHermitGamesStore* GamesStore() const;

    /**
     * Sets Font 1 object.
     */
    void SetFont1( CGlFont *aFont1 );

    /**
     * Returns Font 1 object.
     */
    CGlFont* Font1() const;

private: // data

    CGlTextureManager *iTextureManager; // owned

    CHermitLevelProvider *iLevelProvider; // owned

    CHermitBackground *iBackground; // owned

    CHermitSettings *iSettings; // owned

    CHermitMenuPageSettings *iPageSettings; // not owned (owned by menu engine)

    CHermitMenuTitle *iTitle; // not owned (owned by menu main page)

    CHermitGamesStore *iGamesStore; // owned

    CGlFont *iFont1; // not owned (owned by menu game page)

	};


#endif /* HERMITCOMMONOBJECTS_H_ */
