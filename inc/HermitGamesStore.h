/*
 ============================================================================
  Name        : HermitGamesStore.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITGAMESSTORE_H_
#define HERMITGAMESSTORE_H_

#include <e32def.h>
#include <e32base.h>
#include <e32hashtab.h> 

class CHermitCommonObjects;




class CHermitGamesStore : public CBase
	{
public:

	~CHermitGamesStore();

    static CHermitGamesStore* NewL( CHermitCommonObjects& aCommonObjects );

    void SetLevelCompleted( TUint32 aLevelId, TInt aTimeSeconds, TInt aMovesCount, TBool aAlways = EFalse );

    // returns ETrue when level has been found
    TBool GetLevelCompleted( TUint32 aLevelId, TInt& aTimeSeconds, TInt& aMovesCount );

    void SaveToFile();

    void ResetAllData();

    TBool ResetDataAvailable();

private:

	CHermitGamesStore( CHermitCommonObjects& aCommonObjects );

    void ConstructL();

    void ReadFromFile();

private: // data

    CHermitCommonObjects& iCommonObjects; // not owned 
    
    struct SStoreEntry
		{
    	TInt iTimeSeconds;
    	TInt iMovesCount;
		};

    RHashMap< TUint32, SStoreEntry > iData;

	};

#endif /* HERMITGAMESSTORE_H_ */
