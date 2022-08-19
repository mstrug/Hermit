/*
 ============================================================================
  Name        : HermitLevel.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITLEVELGROUP_H_
#define HERMITLEVELGROUP_H_

#include <e32def.h>
#include <e32base.h>


class CHermitLevel;
class CHermitCommonObjects;


class CHermitLevelGroup : public CBase
	{
public:

	virtual ~CHermitLevelGroup();

	static CHermitLevelGroup* NewL( CHermitCommonObjects& aCommonObjects, const TDesC& aFileName );

	TInt LevelGroupId() const;

	RPointerArray< CHermitLevel >& Levels();

	CHermitLevel* GetLevelById( TInt aId ) const;

	CHermitLevel* GetLevelByIdx( TInt aIdx ) const;

	CHermitLevel* GetCurrentLevel();

	TInt GetCurrentLevelIdx() const;

	CHermitLevel* GetLevelNext();

	CHermitLevel* GetLevelPrev();

	void SetCurrentLevelByIdx( TInt aIdx );

	TBool IsNextLevelAvailable() const;

	TBool IsPrevLevelAvailable() const;

private:

	CHermitLevelGroup();

	void ConstructL( CHermitCommonObjects& aCommonObjects, const TDesC& aFileName );

private: // data

	TInt iLevelGroupId;

	TInt iCurrentLevel;

	RPointerArray< CHermitLevel > iLevels; // owned

	};

#endif /* HERMITLEVELGROUP_H_ */
