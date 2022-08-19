/*
 ============================================================================
  Name        : HermitLevelGroup.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitLevelGroup.h"
#include "HermitLevel.h"
#include "HermitConst.h"
#include "HermitCommonObjects.h"
#include "HermitCompilationSettings.h"
#include <SGEFile/SGEFileProvider.h>
#include <SGEFile/SGEFile.h>
#include <f32file.h>
#ifdef _SGE_BADA_
#include <FIo.h>
using namespace Osp::Io;
#endif


struct SLevelGroupFileHeader_1_1
	{
	TUint32 iFileId;
	TUint8 iFileVersionMajor;
	TUint8 iFileVersionMinor;
	TUint16 iLevelGroupId;
	TUint8 iLevelCount;
	TUint8 iReserved[ 3 ];
	};



CHermitLevelGroup::CHermitLevelGroup() : iLevelGroupId( KErrNotFound )
	{
	}

CHermitLevelGroup::~CHermitLevelGroup()
	{
	iLevels.ResetAndDestroy();
	}

CHermitLevelGroup* CHermitLevelGroup::NewL( CHermitCommonObjects& aCommonObjects, const TDesC& aFileName )
	{
	CHermitLevelGroup* self = new (ELeave) CHermitLevelGroup();
	CleanupStack::PushL( self );
	self->ConstructL( aCommonObjects, aFileName );
	CleanupStack::Pop();
	return self;
	}

void CHermitLevelGroup::ConstructL( CHermitCommonObjects& aCommonObjects, const TDesC& aFileName )
	{
#ifndef _SGE_BADA_
#ifdef USE_SGEFILE
    TInt fsize = 0;
    CSGEFile& sgeFile = aCommonObjects.FileProvider()->GetSGEFileL( KHermitSGEFileId );
    RReadStream file = sgeFile.OpenFileL( aFileName, &fsize );
#else
    RFs iFs;
    User::LeaveIfError( iFs.Connect() );
    
    RFile file;
    TInt err = file.Open( iFs, aFileName, EFileRead | EFileShareAny );
    User::LeaveIfError( err );
#endif
    SLevelGroupFileHeader_1_1 header;
    TPtr8 ptrHeader( (TUint8*)&header, sizeof( SLevelGroupFileHeader_1_1 ) );
#ifdef USE_SGEFILE
    file.ReadL( ptrHeader );
#else
    err = file.Read( ptrHeader );
    User::LeaveIfError( err );
#endif

    if ( header.iFileId != 0x47564c48 ) // HLVG -> GVLH
        {
        }
    if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x01)
        {
        }
    
    iLevelGroupId = header.iLevelGroupId;

    for ( TInt i = 0; i < header.iLevelCount; i++ )
        {
        SLevelHeader_1_1 hdrData = { 0 };
        TPtr8 ptrHdrData( (TUint8*)&hdrData, sizeof( SLevelHeader_1_1 ) );
        
#ifdef USE_SGEFILE
        file.ReadL( ptrHdrData );
#else
        err = file.Read( ptrHdrData );
        User::LeaveIfError( err );
#endif

        TInt len = hdrData.iWidth * hdrData.iHeight;
        TUint8* data = new TUint8[ len ];
        CleanupStack::PushL( data );
        TPtr8 ptrData( data, len );
#ifdef USE_SGEFILE
        file.ReadL( ptrData );
#else
        err = file.Read( ptrData );
        User::LeaveIfError( err );
#endif

        CHermitLevel* level = CHermitLevel::NewL( TSize( hdrData.iWidth, hdrData.iHeight ), iLevelGroupId | hdrData.iId, (CHermitLevel::TFieldShape) hdrData.iFieldShape, hdrData.iDiagonalMoves, (TAny*) data );
        CleanupStack::PushL( level );
        iLevels.AppendL( level );
        CleanupStack::Pop( level );

        CleanupStack::Pop(); // data
        delete [] data;

        TBuf8<2> fill;
#ifdef USE_SGEFILE
        file.ReadL( fill );
#else
        err = file.Read( fill );
        User::LeaveIfError( err );
#endif

        if ( fill[ 0 ] != 0xFF && fill[ 1 ] != 0xFF )
            {
            // error ?
            }
        }
    
    file.Close();
#ifndef USE_SGEFILE
    iFs.Close();
#endif
    
#else
	File* file = new File();
	if ( file->Construct( aFileName, L"r" ) != E_SUCCESS )
		{
		AppLog( "CHermitLevelGroup::ConstructL file ERROR" );
		delete file;
		return;
		}

	SLevelGroupFileHeader_1_1 header;
	if ( file->Read( (TAny*) &header, sizeof( SLevelGroupFileHeader_1_1 ) ) != sizeof( SLevelGroupFileHeader_1_1 ) )
		{
		AppLog( "CHermitLevelGroup::ConstructL file read header ERROR" );
		delete file;
		return;
		}

	if ( header.iFileId != 0x47564c48 ) // HLVG -> GVLH
		{
		AppLog( "CHermitLevelGroup::ConstructL file read header ID ERROR" );
		delete file;
		return;
		}
	if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x01)
		{
		AppLog( "CHermitLevelGroup::ConstructL file read header ver ERROR" );
		delete file;
		return;
		}

	iLevelGroupId = header.iLevelGroupId;
	AppLog( "G ID: %x", iLevelGroupId );

	//AppLog( "cnt: %d   s: %d", header.iLevelCount, sizeof( SLevelGroupFileHeader_1_0 ) );

	for ( TInt i = 0; i < header.iLevelCount; i++ )
		{
		SLevelHeader_1_1 hdrData = { 0 };

		TInt read = file->Read( (TAny*) &hdrData, sizeof( SLevelHeader_1_1 ) );
		if ( read != sizeof( SLevelHeader_1_1 ) )
			{
			AppLog( "CHermitLevelGroup::ConstructL file read level size ERROR %d %d", i, read );
			delete file;
			return;
			}

		TInt len = hdrData.iWidth * hdrData.iHeight;
		TUint8* data = new TUint8[ len ];
		CleanupStack::PushL( data );
		read = file->Read( (TAny*) data, len );
		if ( read != len )
			{
			AppLog( "CHermitLevelGroup::ConstructL file read level data ERROR %d", i );
			delete file;
			CleanupStack::Pop();
			delete [] data;
			return;
			}

		CHermitLevel* level = CHermitLevel::NewL( TSize( hdrData.iWidth, hdrData.iHeight ), iLevelGroupId | hdrData.iId, (CHermitLevel::TFieldShape) hdrData.iFieldShape, hdrData.iDiagonalMoves, (TAny*) data );
		CleanupStack::PushL( level );
		iLevels.AppendL( level );
		CleanupStack::Pop( level );

		CleanupStack::Pop(); // data
		delete [] data;

		TUint tmp[2];
		if ( file->Read( (TAny*) tmp, 2 ) != 2 )
			{
			AppLog( "CHermitLevelGroup::ConstructL file read level ff ERROR %d", i );
			delete file;
			return;
			}

		if ( tmp[ 0 ] != 0xFF && tmp[ 1 ] != 0xFF )
			{
			// error ?
			}
		}

	delete file;
#endif
	}

TInt CHermitLevelGroup::LevelGroupId() const
	{
	return iLevelGroupId;
	}

RPointerArray< CHermitLevel >& CHermitLevelGroup::Levels()
	{
	return iLevels;
	}

CHermitLevel* CHermitLevelGroup::GetLevelById( TInt aId ) const
	{
	for ( TInt i = 0; i < iLevels.Count(); i++ )
		{
		if ( iLevels[ i ]->LevelId() == aId )
			{
			return iLevels[ i ];
			}
		}

	return NULL;
	}

CHermitLevel* CHermitLevelGroup::GetLevelByIdx( TInt aIdx ) const
	{
	if ( aIdx < 0 || aIdx >= iLevels.Count() )
		{
		return NULL;
		}
	return iLevels[ aIdx ];
	}

CHermitLevel* CHermitLevelGroup::GetCurrentLevel()
	{
	if ( iCurrentLevel >= 0 && iCurrentLevel < iLevels.Count() )
		{
		return iLevels[ iCurrentLevel ];
		}
	return NULL;
	}

TInt CHermitLevelGroup::GetCurrentLevelIdx() const
	{
	return iCurrentLevel;
	}

CHermitLevel* CHermitLevelGroup::GetLevelNext()
	{
	if ( iCurrentLevel + 1 < iLevels.Count() )
		{
		iCurrentLevel++;
		return iLevels[ iCurrentLevel ];
		}
	return NULL;
	}

CHermitLevel* CHermitLevelGroup::GetLevelPrev()
	{
	if ( iCurrentLevel - 1 >= 0 )
		{
		iCurrentLevel--;
		return iLevels[ iCurrentLevel ];
		}
	return NULL;
	}

void CHermitLevelGroup::SetCurrentLevelByIdx( TInt aIdx )
	{
	if ( aIdx >= 0 && aIdx < iLevels.Count() )
		{
		iCurrentLevel = aIdx;
		}
	}

TBool CHermitLevelGroup::IsNextLevelAvailable() const
	{
	return ( iCurrentLevel < iLevels.Count() - 1 );
	}

TBool CHermitLevelGroup::IsPrevLevelAvailable() const
	{
	return ( iCurrentLevel > 0 );
	}

