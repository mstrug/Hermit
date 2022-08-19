/*
 ============================================================================
  Name        : HermitMenuTitle.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */


#ifndef HERMITMENUTITLE_H_
#define HERMITMENUTITLE_H_

#include <e32def.h>
#include <e32base.h>
#include <SGEEngine/SGEObjectInterfaces.h>

class CHermitCommonObjects;
class CGlObjectRect;

const TInt KTitleSizeX = 23;
const TInt KTitleSizeY = 5;



class CHermitMenuTitle : public CBase, public MSGEGlDrawableObject,
						 public MSGEUpdatableObject
	{
public:

	virtual ~CHermitMenuTitle();

	static CHermitMenuTitle* NewL( CHermitCommonObjects& aCommonObjects );

	void SetRect( TRect aRect );

	TRect Rect() const;

	TSize ElementsCount();

	TSize ElementsSize();

	void SetTitleElements( TUint32* aTitleElements = NULL ); // KTitleSizeY

private:

	CHermitMenuTitle( CHermitCommonObjects& aCommonObjects );

	void ConstructL();

public: // from MSGEGlDrawableObject

	void Draw( const TRect aDrawRect );

public: // from MSGEUpdatableObject

	TBool Update( const TTimeIntervalMicroSeconds32 aTimeInterval );

protected: // data

	CHermitCommonObjects& iCommonObjects;

	CGlObjectRect *iRect; // owned

	TRect iTitleRect;

	class TTitleElemet
		{
	public:
		TTitleElemet();

		TInt iTextureId;
		TBool iVisible;
		};

	TFixedArray< TFixedArray< TTitleElemet, KTitleSizeX >, KTitleSizeY > iTitleElemets;

	};

#endif /* HERMITMENUTITLE_H_ */
