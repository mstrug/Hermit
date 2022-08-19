/*
 ============================================================================
  Name        : HermitLevelProvider.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitLevelProvider.h"
#include "HermitLevelGroup.h"
#include "HermitLevel.h"
#include "HermitConst.h"
#include <e32math.h>
#ifdef _SGE_BADA_
#include <FSystem.h>
#endif



CHermitLevelProvider::CHermitLevelProvider() : iCurrentGroupIdx( 0 )
	{
	}

CHermitLevelProvider::~CHermitLevelProvider()
	{
	iGroups.ResetAndDestroy();
	}

CHermitLevelProvider* CHermitLevelProvider::NewL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitLevelProvider* self = new (ELeave) CHermitLevelProvider();
	CleanupStack::PushL( self );
	self->ConstructL( aCommonObjects );
	CleanupStack::Pop();
	return self;
	}

void CHermitLevelProvider::ConstructL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitLevelGroup* lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup1FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup2FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup3FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup4FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup5FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup6FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup7FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup8FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup9FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup10FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();

	lvg = CHermitLevelGroup::NewL( aCommonObjects, KHermitLevelGroup11FileName );
	CleanupStack::PushL( lvg );
	iGroups.AppendL( lvg );
	CleanupStack::Pop();
	}

TBool CHermitLevelProvider::SetCurrentLevelFromLevelId( TInt aLevelId )
	{
	for ( TInt i = 0; i < iGroups.Count(); i++ )
		{
		RPointerArray< CHermitLevel >& levels = iGroups[ i ]->Levels();
		TInt lgid = iGroups[ i ]->LevelGroupId();

		if ( ( lgid & aLevelId ) != lgid )
			{
			continue;
			}

		for ( TInt j = 0; j < levels.Count(); j++ )
			{
			TInt lid = levels[ j ]->LevelId();
			if ( lid == aLevelId )
				{
				iCurrentGroupIdx = i;
				iGroups[ i ]->SetCurrentLevelByIdx( j );
				return ETrue;
				}
			}
		}

	return EFalse;
	}

CHermitLevel* CHermitLevelProvider::CurrentLevel()
	{
	if ( iCurrentGroupIdx == KErrNotFound )
		{
		iCurrentGroupIdx = 0;
		}
	return iGroups[ iCurrentGroupIdx ]->GetCurrentLevel();
	}

CHermitLevel* CHermitLevelProvider::NextLevel()
	{
	if ( iCurrentGroupIdx == KErrNotFound )
		{
		iCurrentGroupIdx = 0;
		return iGroups[ 0 ]->GetLevelByIdx( 0 );
		}

	CHermitLevel* ret = iGroups[ iCurrentGroupIdx ]->GetLevelNext();
	if ( !ret )
		{
		if ( iCurrentGroupIdx < iGroups.Count() - 1 )
			{
#ifndef STORE_LEVEL_PROVIDER_GROUPS
			iGroups[ iCurrentGroupIdx ]->SetCurrentLevelByIdx( 0 ); // reset current level
#endif
			iCurrentGroupIdx++;
			ret = iGroups[ iCurrentGroupIdx ]->GetCurrentLevel();
			}
		}

	if ( ret == NULL )
		{
		iCurrentGroupIdx = 0;
		return iGroups[ 0 ]->GetLevelByIdx( 0 );
		}

	return ret;
	}

CHermitLevel* CHermitLevelProvider::NextLevelGroup( TInt aDirection )
	{
	if ( iCurrentGroupIdx == KErrNotFound )
		{
		iCurrentGroupIdx = 0;
		return iGroups[ 0 ]->GetLevelByIdx( 0 );
		}

	CHermitLevel* ret = NULL;
	if ( aDirection > 0 && iCurrentGroupIdx < iGroups.Count() - 1 )
		{
#ifndef STORE_LEVEL_PROVIDER_GROUPS
		iGroups[ iCurrentGroupIdx ]->SetCurrentLevelByIdx( 0 ); // reset current level
#endif
		iCurrentGroupIdx++;
		ret = iGroups[ iCurrentGroupIdx ]->GetCurrentLevel();
		}
	else if ( aDirection < 0 && iCurrentGroupIdx > 0 )
		{
#ifndef STORE_LEVEL_PROVIDER_GROUPS
		iGroups[ iCurrentGroupIdx ]->SetCurrentLevelByIdx( 0 ); // reset current level
#endif
		iCurrentGroupIdx--;
		ret = iGroups[ iCurrentGroupIdx ]->GetCurrentLevel();
		}

	return ret;
	}

CHermitLevel* CHermitLevelProvider::NextLevelInGroup( TInt aDirection )
	{
	if ( iCurrentGroupIdx == KErrNotFound )
		{
		iCurrentGroupIdx = 0;
		return iGroups[ 0 ]->GetLevelByIdx( 0 );
		}

	CHermitLevel* ret = NULL;
	if ( aDirection > 0 )
		{
		ret = iGroups[ iCurrentGroupIdx ]->GetLevelNext();
		}
	else if ( aDirection < 0 )
		{
		ret = iGroups[ iCurrentGroupIdx ]->GetLevelPrev();
		}

	return ret;
	}

TBool CHermitLevelProvider::IsNextLevelGroupAvailable( TInt aDirection )
	{
	if ( aDirection > 0 )
		{
		return ( iCurrentGroupIdx < iGroups.Count() - 1 );
		}
	else
		{
		return ( iCurrentGroupIdx != 0 );
		}
	}

TBool CHermitLevelProvider::IsNextLevelInGroupAvailable( TInt aDirection )
	{
	if ( aDirection > 0 )
		{
		return iGroups[ iCurrentGroupIdx ]->IsNextLevelAvailable();
		}
	else
		{
		return iGroups[ iCurrentGroupIdx ]->IsPrevLevelAvailable();
		}
	}

CHermitLevel* CHermitLevelProvider::RandomNextLevel()
	{
	CHermitLevel* ret = NULL;

	TReal groupCnt = iGroups.Count();

	TInt groupIdx = KErrNotFound;
	do
	{
#ifndef _SGE_BADA_
	    TReal random = Math::Random();
	    TReal round = 0;
	    if ( Math::Round( round, groupCnt * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
	        {
	        groupIdx = (TInt)round;
	        }
#else
		TReal random = Osp::Base::Utility::Math::Rand();
		groupIdx = Osp::Base::Utility::Math::Round( groupCnt * random / (TReal) Osp::Base::Utility::Math::RAND_VALUE_MAX );
#endif
	}
	while ( groupIdx == iCurrentGroupIdx );
	if ( groupIdx < 0 || groupIdx >= iGroups.Count() )
		{
		groupIdx = iGroups.Count() - 1;
		}

	TReal itemCnt = iGroups[ groupIdx ]->Levels().Count();
	TInt itemIdx = KErrNotFound;
	do
	{
#ifndef _SGE_BADA_
        TReal random = Math::Random();
        TReal round = 0;
        if ( Math::Round( round, itemCnt * random / (TReal) 0xFFFFFFFF, 0 ) == KErrNone )
            {
            itemIdx = (TInt)round;
            }
#else
		TReal random = Osp::Base::Utility::Math::Rand();
		itemIdx = Osp::Base::Utility::Math::Round( itemCnt * random / (TReal) Osp::Base::Utility::Math::RAND_VALUE_MAX );
#endif
	}
	while ( itemIdx == iGroups[ groupIdx ]->GetCurrentLevelIdx() );
	if ( itemIdx < 0 || itemIdx >= iGroups[ groupIdx ]->Levels().Count() )
		{
		itemIdx = 0;
		}

#ifndef STORE_LEVEL_PROVIDER_GROUPS
	iGroups[ iCurrentGroupIdx ]->SetCurrentLevelByIdx( 0 ); // reset current level
#endif

	ret = iGroups[ groupIdx ]->GetLevelByIdx( itemIdx );
	iGroups[ groupIdx ]->SetCurrentLevelByIdx( itemIdx );

	iCurrentGroupIdx = groupIdx;

	return ret;
	}

void CHermitLevelProvider::ResetLevelPosition()
	{
	iCurrentGroupIdx = 0;
	iGroups[ 0 ]->SetCurrentLevelByIdx( 0 );
	}

SHermitLevelProviderStorage CHermitLevelProvider::GetStoreData()
	{
#ifdef STORE_LEVEL_PROVIDER_STATE
	CurrentLevel(); // set indexes
	CHermitLevel* lvl = iGroups[ iCurrentGroupIdx ]->GetCurrentLevel();

	SHermitLevelProviderStorage ret = { 0 };
	ret.iLevelGroupId = iGroups[ iCurrentGroupIdx ]->LevelGroupId();
	ret.iLevelItemId = ( lvl ? lvl->LevelId() : KErrNotFound );

#ifdef STORE_LEVEL_PROVIDER_GROUPS_STATE
	for ( TInt i = 0; i < iGroups.Count() && i < KHermitLevelProviderStorageMax; i++ )
		{
		ret.iCurrentLevels[ i ] = iGroups[ i ]->GetCurrentLevelIdx();
		}
#endif

#else
	SHermitLevelProviderStorage ret = { 0 };
	ret.iLevelGroupId = KErrNotFound;
	ret.iLevelItemId = KErrNotFound;
#endif

	return ret;
	}

void CHermitLevelProvider::SetStoreDate( const SHermitLevelProviderStorage& aStoreData )
	{
#ifdef STORE_LEVEL_PROVIDER_STATE
	TInt gId = aStoreData.iLevelGroupId;
	TInt lId = aStoreData.iLevelItemId;

#ifdef STORE_LEVEL_PROVIDER_GROUPS_STATE
	for ( TInt i = 0; i < iGroups.Count() && i < KHermitLevelProviderStorageMax; i++ )
		{
		iGroups[ i ]->SetCurrentLevelByIdx( aStoreData.iCurrentLevels[ i ] );
		}
#endif

	SetCurrentLevelFromLevelId( gId | lId );
#endif
	}



