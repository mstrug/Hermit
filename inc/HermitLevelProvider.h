/*
 ============================================================================
  Name        : HermitLevelProvider.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITLEVELPROVIDER_H_
#define HERMITLEVELPROVIDER_H_

#include <e32def.h>
#include <e32base.h>


class CHermitLevelGroup;
class CHermitLevel;
class CHermitCommonObjects;


const TInt KHermitLevelProviderStorageMax = 20;
struct SHermitLevelProviderStorage
	{
	TInt iLevelGroupId;
	TInt iLevelItemId;
	TInt iCurrentLevels[ KHermitLevelProviderStorageMax ];
	};


class CHermitLevelProvider : public CBase
	{
public:

	virtual ~CHermitLevelProvider();

	static CHermitLevelProvider* NewL( CHermitCommonObjects& aCommonObjects );

	TBool SetCurrentLevelFromLevelId( TInt aLevelId );

	CHermitLevel* CurrentLevel();

	CHermitLevel* NextLevel();

	CHermitLevel* NextLevelGroup( TInt aDirection = 1 );

	CHermitLevel* NextLevelInGroup( TInt aDirection = 1 );

	TBool IsNextLevelGroupAvailable( TInt aDirection = 1 );

	TBool IsNextLevelInGroupAvailable( TInt aDirection = 1 );

	CHermitLevel* RandomNextLevel();

	void ResetLevelPosition();

	SHermitLevelProviderStorage GetStoreData();

	void SetStoreDate( const SHermitLevelProviderStorage& aStoreData );

private:

	CHermitLevelProvider();

	void ConstructL( CHermitCommonObjects& aCommonObjects );

private: // data

	RPointerArray< CHermitLevelGroup > iGroups;

	TInt iCurrentGroupIdx;

	};


#endif /* HERMITLEVELPROVIDER_H_ */
