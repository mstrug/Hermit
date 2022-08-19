/*
 ============================================================================
  Name        : HermitBackgroundColors.h
  Author      : Michal Strug
  Version     : 1.0
  Copyright   : Copyright (c) 2022 Michal Strug. All rights reserved.
  Description :
 ============================================================================
 */

#ifndef HERMITBACKGROUNDCOLORS_H_
#define HERMITBACKGROUNDCOLORS_H_

#include "HermitBackgroundGlObject.h"



static const GLbyte KIncrementsOne[ KVertexCount * KVertexColorSize ] =
	{
	1, 1, 1, 0,
	1, 1, 1, 0,
	1, 1, 1, 0,
	1, 1, 1, 0,
	1, 1, 1, 0
	};


/*******************************************
 *  COLORS SET 1 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet1[ KVertexCount * KVertexColorSize ] =
    {
    0,	 93,  129, 255,
    3, 	 96,  132, 255,
    0,   93,  129, 255,
    111, 233, 235, 255,
    111, 233, 235, 255
    };

static const GLubyte KColorsMaxSet1[ KVertexCount * KVertexColorSize ] =
    {
	60,  153, 229, 255,
    103, 176, 232, 255,
    40,  193, 249, 255,
    231, 253, 255, 255,
    251, 253, 255, 255
    };


static const GLbyte KIncrementsSet1[ KVertexCount * KVertexColorSize ] =
	{
	3, 3, 5, 0,
	5, 4, 5, 0,
	2, 5, 6, 0,
	6, 1, 1, 0,
	7, 1, 1, 0
	};


/*******************************************
 *  COLORS SET 2 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet2[ KVertexCount * KVertexColorSize ] =
    {
    169, 40,  40,  255,
    186, 66,  66,  255,
    169, 40,  40,  255,
    255, 166, 166, 255,
    255, 166, 166, 255
    };

static const GLubyte KColorsMaxSet2[ KVertexCount * KVertexColorSize ] =
    {
	213, 84,  84,  255,
	223, 94,  94,  255,
    213, 84,  84,  255,
    255, 224, 224, 255,
    255, 224, 224, 255
    };



/*******************************************
 *  COLORS SET 3 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet3[ KVertexCount * KVertexColorSize ] =
    {
    109, 191, 135, 255,
    89,  171, 115, 255,
    109, 191, 135, 255,
    181, 255, 169, 255,
    181, 255, 169, 255
    };

static const GLubyte KColorsMaxSet3[ KVertexCount * KVertexColorSize ] =
    {
	179, 255, 165, 255,
    199, 255, 185, 255,
    179, 255, 165, 255,
    243, 255, 242, 255,
    243, 255, 242, 255
    };



/*******************************************
 *  COLORS SET 4 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet4[ KVertexCount * KVertexColorSize ] =
    {
	140, 129, 0,   255,
	147, 136, 8,   255,
    140, 129, 0,   255,
    255, 248, 166, 255,
    255, 248, 166, 255
    };

static const GLubyte KColorsMaxSet4[ KVertexCount * KVertexColorSize ] =
    {
	204, 192, 53,  255,
    205, 193, 57,  255,
    204, 192, 53,  255,
    255, 249, 230, 255,
    255, 249, 230, 255
    };



/*******************************************
 *  COLORS SET 5 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet5[ KVertexCount * KVertexColorSize ] =
    {
	140, 0,   140, 255,
	143, 4,   143, 255,
	140, 0,   140, 255,
    255, 160, 255, 255,
    255, 160, 255, 255
    };

static const GLubyte KColorsMaxSet5[ KVertexCount * KVertexColorSize ] =
    {
	228, 130, 228, 255,
	238, 140, 238, 255,
    228, 130, 228, 255,
    255, 229, 255, 255,
    255, 229, 255, 255
    };





/*******************************************
 *  COLORS SET 6 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet6[ KVertexCount * KVertexColorSize ] =
    {
    0,	 93,  129, 255,
    3, 	 96,  132, 255,
    0,   93,  129, 255,
    167, 255, 255, 255,
    167, 255, 255, 255
    };


/*******************************************
 *  COLORS SET 6_1 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet6_1[ KVertexCount * KVertexColorSize ] =
    {
    128, 255, 0,   255,
    128, 255, 0,   255,
    128, 255, 0,   255,
    0,   128, 255, 255,
    0,   128, 255, 255
    };

/*******************************************
 *  COLORS SET 6_2 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet6_2[ KVertexCount * KVertexColorSize ] =
    {
	100, 100, 100, 255,
	113, 113, 113, 255,
	100, 100, 100, 255,
	250, 250, 250, 255,
	250, 250, 250, 255,
    };

static const GLubyte KColorsMaxSet6_2[ KVertexCount * KVertexColorSize ] =
    {
	220, 220, 220, 255,
	255, 255, 255, 255,
	235, 235, 235, 255,
	255, 255, 255, 255,
	255, 255, 255, 255,
    };




/*******************************************
 *  COLORS SET 7 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet7[ KVertexCount * KVertexColorSize ] =
    {
	128, 255, 0  , 255,
	128, 255, 0  , 255,
	128, 255, 0  , 255,
	0  , 128, 255, 255,
	0  , 128, 255, 255,
    };

static const GLubyte KColorsMaxSet7[ KVertexCount * KVertexColorSize ] =
    {
	196, 255, 128, 255,
	196, 255, 128, 255,
	196, 255, 128, 255,
	 64, 196, 196, 255,
	 64, 196, 196, 255,
    };

static const GLbyte KIncrementsSet7[ KVertexCount * KVertexColorSize ] =
	{
	1, 0, 1, 0,
	1, 0, 1, 0,
	1, 0, 1, 0,
	1, 1, -1, 0,
	1, 1, -1, 0
	};


/*******************************************
 *  COLORS SET 8 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet8[ KVertexCount * KVertexColorSize ] =
    {
	139, 255, 100, 255,
	177, 255, 119, 255,
	139, 255, 100, 255,
	68 , 190, 210, 255,
	68 , 190, 210, 255,
    };

static const GLubyte KColorsMaxSet8[ KVertexCount * KVertexColorSize ] =
    {
	65 , 181, 218, 255,
	66,  185, 216, 255,
	65 , 181, 218, 255,
	177, 255, 119, 255,
	177, 255, 119, 255,
    };

static const GLbyte KIncrementsSet8[ KVertexCount * KVertexColorSize ] =
	{
	-1, -1, 1, 0,
	-1, -1, 1, 0,
	-1, -1, 1, 0,
	1, 1, -1, 0,
	1, 1, -1, 0
	};

/*******************************************
 *  COLORS SET 9 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet9[ KVertexCount * KVertexColorSize ] =
    {
	255, 204, 0  , 255,
	255, 204, 0  , 255,
	255, 204, 0  , 255,
	 47, 203, 204, 255,
	 34, 204, 177, 255,
    };

static const GLubyte KColorsMaxSet9[ KVertexCount * KVertexColorSize ] =
    {
	254, 255,  51, 255,
	235, 255,  62, 255,
	160, 255,  98, 255,
	150, 255, 152, 255,
	 52, 255, 152, 255,
    };

static const GLbyte KIncrementsSet9[ KVertexCount * KVertexColorSize ] =
	{
	-1, 1, 1, 0,
	-1, 1, 1, 0,
	-1, 1, 1, 0,
	1, 1, -1, 0,
	1, 1, -1, 0
	};


/*******************************************
 *  COLORS SET 10 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet10[ KVertexCount * KVertexColorSize ] =
    {
	144, 252, 112, 255,
	106, 255, 144, 255,
	144, 252, 112, 255,
	241, 252,  43, 255,
	241, 252,  43, 255,
    };

static const GLubyte KColorsMaxSet10[ KVertexCount * KVertexColorSize ] =
    {
//   255, 116,  80, 255,
//	255, 116,  80, 255,
//	255, 116,  80, 255,
	128,   2,  80, 255,
	128,   2,  80, 255,
	128,   2,  80, 255,
	207, 255, 169, 255,
	207, 255, 169, 255,
    };

static const GLbyte KIncrementsSet10[ KVertexCount * KVertexColorSize ] =
	{
	1, -1, -1, 0,
	1, -1, -1, 0,
	1, -1, -1, 0,
	-1, 1, 2, 0,
	-1, 1, 2, 0
	};

/*******************************************
 *  COLORS SET 11 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet11[ KVertexCount * KVertexColorSize ] =
    {
	174, 210, 255, 255,
	174, 210, 255, 255,
	174, 210, 255, 255,
	143, 134, 163, 255,
	149,  74, 204, 255,
    };

static const GLubyte KColorsMaxSet11[ KVertexCount * KVertexColorSize ] =
    {
	 91, 149, 223, 255,
	124, 144, 232, 255,
	 91, 149, 223, 255,
	214, 150, 255, 255,
	214, 150, 255, 255,
    };

static const GLbyte KIncrementsSet11[ KVertexCount * KVertexColorSize ] =
	{
	-1, -1, -1, 0,
	-1, -1, -1, 0,
	-1, -1, -1, 0,
	1, 1, 1, 0,
	1, 1, 1, 0
	};

/*******************************************
 *  COLORS SET 12 - min max wait 2
 *******************************************/

static const GLubyte KColorsMinSet12[ KVertexCount * KVertexColorSize ] =
    {
	 58,  85, 255, 255,
	 69,  87, 244, 255,
	 58,  85, 255, 255,
	251, 163,  62, 255,
	251, 163,  62, 255,
    };

static const GLubyte KColorsMaxSet12[ KVertexCount * KVertexColorSize ] =
    {
	128, 146, 255, 255,
	137, 149, 248, 255,
	128, 146, 255, 255,
	253, 206, 152, 255,
	253, 206, 152, 255,
    };

static const GLbyte KIncrementsSet12[ KVertexCount * KVertexColorSize ] =
	{
	1, 1, 0, 0,
	1, 1, 1, 0,
	1, 1, 0, 0,
	1, 1, 1, 0,
	1, 1, 1, 0
	};


/*******************************************
 *  COLORS SET 13 - bouncing
 *******************************************/

static const GLubyte KColorsMinSet13[ KVertexCount * KVertexColorSize ] =
    {
    128, 255, 0,   255,
    128, 255, 128, 255,
    128, 255, 0,   255,
    255, 128, 128, 255,
    255, 128, 0, 255
    };



#endif /* HERMITBACKGROUNDCOLORS_H_ */
