/*
 ============================================================================
  Name        : HermitGamesStore.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitGamesStore.h"
#include "HermitCommonObjects.h"
#include <SGEFile/SGEFileProvider.h>
#include <f32file.h>
#ifdef _SGE_BADA_
#include <FIo.h>
using namespace Osp::Io;
#endif

_LIT( KHermitGamesStoreFile, "games.dat" );

struct SGamesStoreFile_1_0
	{
	TUint32 iFileId;
	TUint8 iFileVersionMajor;
	TUint8 iFileVersionMinor;
	TInt iEntriesCount;
	};

struct SGamesStoreFileEntry_1_0
	{
	TUint32 iLevelId;
	TInt iGameTimeSeconds;
	TInt iMovesCount;
	};




CHermitGamesStore::CHermitGamesStore( CHermitCommonObjects& aCommonObjects ) : iCommonObjects( aCommonObjects )
	{
	}

CHermitGamesStore::~CHermitGamesStore()
	{
	iData.Close();
	}

CHermitGamesStore* CHermitGamesStore::NewL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitGamesStore *self = new (ELeave) CHermitGamesStore( aCommonObjects );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
	}

void CHermitGamesStore::ConstructL()
	{
	ReadFromFile();
	}

void CHermitGamesStore::SetLevelCompleted( TUint32 aLevelId, TInt aTimeSeconds, TInt aMovesCount, TBool aAlways )
	{
	SStoreEntry se = { aTimeSeconds, aMovesCount };
	if ( aAlways )
		{
		iData.Insert( aLevelId, se );
		}
	else
		{
		SStoreEntry *pse = iData.Find( aLevelId );
		if ( pse )
			{
			if ( aMovesCount <= pse->iMovesCount && aTimeSeconds <= pse->iTimeSeconds )
				{
				iData.Insert( aLevelId, se );
				}
			}
		else
			{
			iData.Insert( aLevelId, se );
			}
		}
	}

TBool CHermitGamesStore::GetLevelCompleted( TUint32 aLevelId, TInt& aTimeSeconds, TInt& aMovesCount )
	{
	SStoreEntry *pse = iData.Find( aLevelId );
	if ( pse )
		{
		aTimeSeconds = pse->iTimeSeconds;
		aMovesCount = pse->iMovesCount;
		return ETrue;
		}
	return EFalse;
	}

void CHermitGamesStore::SaveToFile()
	{
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitGamesStoreFile() );
    
	if ( iData.Count() == 0 )
	    {
#ifndef _SGE_BADA_	    
	    iCommonObjects.FileProvider()->Fs().Delete( fn );
#else
		File::Remove( KHermitGamesStoreFile );
#endif
		return;
	    }

    SGamesStoreFile_1_0 header =
        {
        0x54534748,
        0x01,
        0x00,
        iData.Count()
        };

#ifndef _SGE_BADA_
    RFile file;
    
    TInt err = file.Replace( iCommonObjects.FileProvider()->Fs(), fn, EFileWrite );
    CleanupClosePushL( file );
    User::LeaveIfError( err );
    
    TPtrC8 ptrHeader( (TUint8*)&header, sizeof( SGamesStoreFile_1_0 ) );
    err = file.Write( ptrHeader );
    User::LeaveIfError( err );

    //RHashMap< TUint32, SStoreEntry >::TIter iter( iData );
    THashMapIter< TUint32, SStoreEntry > iter( iData );
    iter.Reset();
    iter.NextKey();
    while ( iter.CurrentKey() )
        {
        SGamesStoreFileEntry_1_0 entry =
            {
            *iter.CurrentKey(),
            iter.CurrentValue()->iTimeSeconds,
            iter.CurrentValue()->iMovesCount
            };
        TPtrC8 ptrEntry( (TUint8*)&entry, sizeof( SGamesStoreFileEntry_1_0 ) );
        
        TInt err = file.Write( ptrEntry );
        if ( err /*|| ptrEntry.Length() != sizeof( SGamesStoreFileEntry_1_0 )*/ )
            {
            break;
            }
        if ( !iter.NextKey() )
            {
            break;
            }
        }

    CleanupStack::PopAndDestroy( &file );
    
#else
	File* file = new File();
	if ( file->Construct( KHermitGamesStoreFile, L"w" ) != E_SUCCESS )
		{
		AppLog( "KHermitGamesStoreFile::SaveToFile file ERROR" );
		delete file;
		return;
		}

	if ( file->Write( (TAny*) &header, sizeof( SGamesStoreFile_1_0 ) ) != E_SUCCESS )
			{
			AppLog( "KHermitGamesStoreFile::SaveToFile file write ERROR" );
			delete file;
			return;
			}

	RHashMap< TUint32, SStoreEntry >::TIter iter( iData );
	iter.Reset();
	while ( iter.CurrentKey() )
		{
		SGamesStoreFileEntry_1_0 entry =
			{
			*iter.CurrentKey(),
			iter.CurrentValue()->iTimeSeconds,
			iter.CurrentValue()->iMovesCount
			};
		TInt err = file->Write( (TAny*) &entry, sizeof( SGamesStoreFileEntry_1_0 ) );
		if ( err != E_SUCCESS )
			{
			AppLog( "CHermitGamesStore::SaveToFile file write 2 ERROR %d", err );
			break;
			}
		if ( !iter.NextKey() )
			{
			break;
			}
		}

	delete file;
#endif
	}

void CHermitGamesStore::ReadFromFile()
	{
    SGamesStoreFile_1_0 header;
    
#ifndef _SGE_BADA_
    RFile file;
    
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitGamesStoreFile() );
    
    TInt err = file.Open( iCommonObjects.FileProvider()->Fs(), fn, EFileRead | EFileShareAny );
    if ( err == KErrNotFound )
        {
        return;
        }
    CleanupClosePushL( file );

    TPtr8 ptrHeader( (TUint8*)&header, sizeof( SGamesStoreFile_1_0 ) );
    err = file.Read( ptrHeader );
    if ( err )
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }

    if ( header.iFileId != 0x54534748 ) // HGST -> TSGH
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }
    if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }

    for ( TInt i = 0; i < header.iEntriesCount; i++ )
        {
        SGamesStoreFileEntry_1_0 entry;
        TPtr8 ptrEntry( (TUint8*)&entry, sizeof( SGamesStoreFileEntry_1_0 ) );
        
        err = file.Read( ptrEntry );
        
        if ( err != KErrNone /*|| ptrEntry.Length() != sizeof( SGamesStoreFileEntry_1_0 )*/ )
            {
            break;
            }
        
        SetLevelCompleted( entry.iLevelId, entry.iGameTimeSeconds, entry.iMovesCount, ETrue );
        }
    
    CleanupStack::PopAndDestroy( &file );
    
#else
	File* file = new File();
	if ( file->Construct( KHermitGamesStoreFile, L"r" ) != E_SUCCESS )
		{
		AppLog( "CHermitGamesStore::ReadFromFile file ERROR" );
		delete file;
		return;
		}

	if ( file->Read( (TAny*) &header, sizeof( SGamesStoreFile_1_0 ) ) != sizeof( SGamesStoreFile_1_0 ) )
		{
		AppLog( "CHermitGamesStore::ReadFromFile file read ERROR" );
		delete file;
		return;
		}

	if ( header.iFileId != 0x54534748 ) // HGST -> TSGH
		{
		AppLog( "CHermitGamesStore::ReadFromFile file read ID ERROR" );
		delete file;
		return;
		}
	if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
		{
		AppLog( "CHermitGamesStore::ReadFromFile file read ver ERROR" );
		delete file;
		return;
		}

	for ( TInt i = 0; i < header.iEntriesCount; i++ )
		{
		SGamesStoreFileEntry_1_0 entry;
		if ( file->Read( (TAny*) &entry, sizeof( SGamesStoreFileEntry_1_0 ) ) != sizeof( SGamesStoreFileEntry_1_0 ) )
			{
			AppLog( "CHermitGamesStore::ReadFromFile file read 2 ERROR" );
			break;
			}
		SetLevelCompleted( entry.iLevelId, entry.iGameTimeSeconds, entry.iMovesCount, ETrue );
		}

	delete file;
#endif
	}

void CHermitGamesStore::ResetAllData()
	{
	iData.Close();
	}

TBool CHermitGamesStore::ResetDataAvailable()
	{
	return ( iData.Count() > 0 );
	}


