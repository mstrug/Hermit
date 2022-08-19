/*
 ============================================================================
  Name        : HermitLevel.inl
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITLEVEL_INL_
#define HERMITLEVEL_INL_

const TUint8 KHermitLevelFieldTypeMask = 0x0F;


const CHermitLevel::TFieldType CHermitLevel::FieldC( TInt aX, TInt aY ) const
	{
	return (TFieldType) ( iData[ ( iSize.iHeight - aY - 1 ) * iSize.iWidth + aX ] & KHermitLevelFieldTypeMask );
	}

const CHermitLevel::TFieldType CHermitLevel::FieldC( TPoint aPos ) const
	{
	return (TFieldType) ( iData[ ( iSize.iHeight - aPos.iY - 1 ) * iSize.iWidth + aPos.iX ] & KHermitLevelFieldTypeMask );
	}

void CHermitLevel::SetFieldC( TInt aX, TInt aY, CHermitLevel::TFieldType aType )
	{
	iData[ ( iSize.iHeight - aY - 1 ) * iSize.iWidth + aX ] &= EFieldFinish;
	iData[ ( iSize.iHeight - aY - 1 ) * iSize.iWidth + aX ] |= aType;
	}

void CHermitLevel::SetFieldC( TPoint aPos, TFieldType aType )
	{
	iData[ ( iSize.iHeight - aPos.iY - 1 ) * iSize.iWidth + aPos.iX ] &= EFieldFinish;
	iData[ ( iSize.iHeight - aPos.iY - 1 ) * iSize.iWidth + aPos.iX ] |= aType;
	}


CHermitLevel::TFieldType CHermitLevel::Field( TInt aX, TInt aY ) const
	{
	return (TFieldType) iData[ ( iSize.iHeight - aY - 1 ) * iSize.iWidth + aX ];
	}

CHermitLevel::TFieldType CHermitLevel::Field( TPoint aPos ) const
	{
	return (TFieldType) iData[ ( iSize.iHeight - aPos.iY - 1 ) * iSize.iWidth + aPos.iX ];
	}

TBool CHermitLevel::IsFieldFinish( TPoint aPos ) const
	{
	return ( iData[ ( iSize.iHeight - aPos.iY - 1 ) * iSize.iWidth + aPos.iX ] & EFieldFinish );
    }



#endif /* HERMITLEVEL_INL_ */
