/*
 ============================================================================
  Name        : HermitSettings.cpp
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#include "HermitSettings.h"
#include "HermitCommonObjects.h"
#include "HermitLevelProvider.h"
#include <SGEAudio/SGEAudioProvider.h>
#include <SGEFile/SGEFileProvider.h>
#include <f32file.h>
#ifdef _SGE_BADA_
#include <FBase.h>
#include <FSystem.h>
#include <FIo.h>
using namespace Osp::Io;
using namespace Osp::Base;
using namespace Osp::System;
#endif

const TInt KDefaultSoundVolume = ( KMaxVolume / 5 ) * 2;
const TInt KDefaultMusicVolume = ( KMaxVolume / 5 ) * 2;
const TInt KDefaultColorSet = 6;

_LIT( KHermitSettingsFile, "settings.dat" );

struct SSettingsFile_1_0
	{
	TUint32 iFileId;
	TUint8 iFileVersionMajor;
	TUint8 iFileVersionMinor;
	TInt iSoundVolume;
	TInt iMusicVolume;
	TInt iColorSet;
	TBool iGreenIndicator;
	TBool iOrangeIndicator;
	TBool iLevelIdVisible;
	};



CHermitSettings::CHermitSettings( CHermitCommonObjects& aCommonObjects ) : iCommonObjects( aCommonObjects ),
		iSoundVolume( KDefaultSoundVolume ), iMusicVolume( KDefaultMusicVolume ),
		iColorSet( KDefaultColorSet ), iGreenIndicator( ETrue ), iOrangeIndicator( ETrue ),
		iLevelIdVisible( ETrue )
	{
	}

CHermitSettings::~CHermitSettings()
	{
	}

CHermitSettings* CHermitSettings::NewL( CHermitCommonObjects& aCommonObjects )
	{
	CHermitSettings* self = new (ELeave) CHermitSettings( aCommonObjects );
	CleanupStack::PushL( self );
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}

void CHermitSettings::ConstructL()
	{
	ReadFromFile();
	CheckSilentMode();
	}

void CHermitSettings::SaveToFile()
	{
    TBool sndRestore = EFalse;
    TBool musRestore = EFalse;
    if ( iSilentMode )
        {
        if ( iSoundVolume == 0 )
            {
            iSoundVolume = iSilentModeSoundVolume;
            sndRestore = ETrue;
            }
        if ( iMusicVolume == 0 )
            {
            iMusicVolume = iSilentModeMusicVolume;
            musRestore = ETrue;
            }
        }

    SSettingsFile_1_0 header =
        {
        0x54455348,
        0x01,
        0x00,
        iSoundVolume,
        iMusicVolume,
        iColorSet,
        iGreenIndicator,
        iOrangeIndicator,
        iLevelIdVisible
        };

    if ( sndRestore )
        {
        iSoundVolume = 0;
        }
    if ( musRestore )
        {
        iMusicVolume = 0;
        }
    
#ifndef _SGE_BADA_
    RFile file;
    
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitSettingsFile() );
    
    TInt err = file.Replace( iCommonObjects.FileProvider()->Fs(), fn, EFileWrite );
    CleanupClosePushL( file );
    User::LeaveIfError( err );

    TPtrC8 ptrHeader( (TUint8*)&header, sizeof( SSettingsFile_1_0 ) );
    err = file.Write( ptrHeader );
    User::LeaveIfError( err );

    SHermitLevelProviderStorage storage = iCommonObjects.LevelProvider()->GetStoreData();
    TPtrC8 ptrStorage( (TUint8*)&storage, sizeof( SHermitLevelProviderStorage ) );
    err = file.Write( ptrStorage );
    User::LeaveIfError( err );

    CleanupStack::PopAndDestroy( &file );
    
#else
	File* file = new File();
	if ( file->Construct( KHermitSettingsFile, L"w" ) != E_SUCCESS )
		{
		AppLog( "CHermitSettings::SaveToFile file ERROR" );
		delete file;
		return;
		}

	if ( file->Write( (TAny*) &header, sizeof( SSettingsFile_1_0 ) ) != E_SUCCESS )
		{
		AppLog( "CHermitSettings::SaveToFile file write ERROR" );
		delete file;
		return;
		}

	SHermitLevelProviderStorage storage = iCommonObjects.LevelProvider()->GetStoreData();
	if ( file->Write( (TAny*) &storage, sizeof( SHermitLevelProviderStorage ) ) != E_SUCCESS )
		{
		AppLog( "CHermitSettings::SaveToFile file write 2 ERROR" );
		delete file;
		return;
		}

	delete file;
#endif
	}

void CHermitSettings::ReadFromFile()
	{
#ifndef _SGE_BADA_
    RFile file;
    
    TFileName fn = iCommonObjects.FileProvider()->AppendToPrivateFolder( KHermitSettingsFile() );
    
    TInt err = file.Open( iCommonObjects.FileProvider()->Fs(), fn, EFileRead | EFileShareAny );
    if ( err == KErrNotFound )
        {
        return;
        }
    CleanupClosePushL( file );
    
    SSettingsFile_1_0 header;
    TPtr8 ptrHeader( (TUint8*)&header, sizeof( SSettingsFile_1_0 ) );
    err = file.Read( ptrHeader );
    if ( err )
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }
    
    if ( header.iFileId != 0x54455348 ) // HSET -> TESH
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }
    if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }
    
    iColorSet = header.iColorSet;
    iGreenIndicator = header.iGreenIndicator;
    iMusicVolume = header.iMusicVolume;
    iOrangeIndicator = header.iOrangeIndicator;
    iSoundVolume = header.iSoundVolume;
    iLevelIdVisible = header.iLevelIdVisible;

    SHermitLevelProviderStorage storage;
    TPtr8 ptrStorage( (TUint8*)&storage, sizeof( SHermitLevelProviderStorage ) );
    err = file.Read( ptrStorage );
    if ( err )
        {
        CleanupStack::PopAndDestroy( &file );
        return;
        }
    
    iCommonObjects.LevelProvider()->SetStoreDate( storage );
    
    CleanupStack::PopAndDestroy( &file );
    
#else
	File* file = new File();
	if ( file->Construct( KHermitSettingsFile, L"r" ) != E_SUCCESS )
		{
		AppLog( "CHermitSettings::ReadFromFile file ERROR" );
		delete file;
		return;
		}

	SSettingsFile_1_0 header;
	if ( file->Read( (TAny*) &header, sizeof( SSettingsFile_1_0 ) ) != sizeof( SSettingsFile_1_0 ) )
		{
		AppLog( "CHermitSettings::ReadFromFile file read ERROR" );
		delete file;
		return;
		}

	if ( header.iFileId != 0x54455348 ) // HSET -> TESH
		{
		AppLog( "CHermitSettings::ReadFromFile file read ID ERROR" );
		delete file;
		return;
		}
	if ( header.iFileVersionMajor != 0x01 || header.iFileVersionMinor != 0x00)
		{
		AppLog( "CHermitSettings::ReadFromFile file read ver ERROR" );
		delete file;
		return;
		}

	iColorSet = header.iColorSet;
	iGreenIndicator = header.iGreenIndicator;
	iMusicVolume = header.iMusicVolume;
	iOrangeIndicator = header.iOrangeIndicator;
	iSoundVolume = header.iSoundVolume;
	iLevelIdVisible = header.iLevelIdVisible;

	SHermitLevelProviderStorage storage;
	if ( file->Read( (TAny*) &storage, sizeof( SHermitLevelProviderStorage ) ) != sizeof( SHermitLevelProviderStorage ) )
		{
		AppLog( "CHermitSettings::ReadFromFile file read 2 ERROR" );
		delete file;
		return;
		}
	iCommonObjects.LevelProvider()->SetStoreDate( storage );

	delete file;
#endif
	}

void CHermitSettings::CheckSilentMode()
	{
#ifndef _SGE_BADA_
    iSilentMode = iCommonObjects.AudioProvider()->IsSilentProfileL( EFalse );
#else
    bool value;

    String key( L"SilentMode" );

    iSilentMode = EFalse;

    if ( SettingInfo::GetValue( key, value ) == E_SUCCESS )
		{
   		iSilentMode = value;
		}

    AppLog( "SilentMode: %d", iSilentMode );
#endif
    
    if ( iSilentMode )
        {
        iSilentModeMusicVolume = iMusicVolume;
        iSilentModeSoundVolume = iSoundVolume;

        iMusicVolume = 0;
        iSoundVolume = 0;
        }
	}

TInt CHermitSettings::MusicVolume() const
	{
	return iMusicVolume;
	}

void CHermitSettings::SetMusicVolume( TInt aMusicVolume )
	{
	if ( aMusicVolume < 0 )
		{
		iMusicVolume = 0;
		}
	else if ( aMusicVolume > KMaxVolume )
		{
		iMusicVolume = KMaxVolume;
		}
	else
		{
		iMusicVolume = aMusicVolume;
		}
	}

TInt CHermitSettings::StepMusicVolume( TInt aDirection )
	{
	if ( aDirection > 0 )
		{
		SetMusicVolume( iMusicVolume + KVolumeStep );
		}
	else if ( aDirection < 0 )
		{
	    SetMusicVolume( iMusicVolume - KVolumeStep );
		}

	return iMusicVolume;
	}


TInt CHermitSettings::SoundVolume() const
	{
	return iSoundVolume;
	}

void CHermitSettings::SetSoundVolume( TInt aSoundVolume )
	{
	if ( aSoundVolume < 0 )
		{
		iSoundVolume = 0;
		}
	else if ( aSoundVolume > KMaxVolume )
		{
		iSoundVolume = KMaxVolume;
		}
	else
		{
		iSoundVolume = aSoundVolume;
		}
	}

TInt CHermitSettings::StepSoundVolume( TInt aDirection )
	{
	if ( aDirection > 0 )
		{
		SetSoundVolume( iSoundVolume + KVolumeStep );
		}
	else if ( aDirection < 0 )
		{
        SetSoundVolume( iSoundVolume - KVolumeStep );
		}

	return iSoundVolume;
	}

TInt CHermitSettings::ColorSet() const
	{
	return iColorSet;
	}

void CHermitSettings::SetColorSet( TInt aColorSet )
	{
	iColorSet = aColorSet;
	}

TBool CHermitSettings::GreenIndicator() const
	{
	return iGreenIndicator;
	}

void CHermitSettings::SetGreenIndicator( TInt aGreenIndicator )
	{
	iGreenIndicator = aGreenIndicator;
	}

void CHermitSettings::ToggleGreenIndicator()
	{
	iGreenIndicator = !iGreenIndicator;
	}

TBool CHermitSettings::OrangeIndicator() const
	{
	return iOrangeIndicator;
	}

void CHermitSettings::SetOrangeIndicator( TInt aOrangeIndicator )
	{
	iOrangeIndicator = aOrangeIndicator;
	}

void CHermitSettings::ToggleOrangeIndicator()
	{
	iOrangeIndicator = !iOrangeIndicator;
	}

TBool CHermitSettings::LevelIdVisible() const
	{
	return iLevelIdVisible;
	}

void CHermitSettings::SetLevelIdVisible( TInt aLevelIdVisible )
	{
	iLevelIdVisible = aLevelIdVisible;
	}

void CHermitSettings::ToggleLevelIdVisible()
	{
	iLevelIdVisible = !iLevelIdVisible;
	}



