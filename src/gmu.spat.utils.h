/*
 *  gmu.spat.utils.h
 *  gmu.objects
 *
 *  Created by charles on 02/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */


#ifndef GMU_SPAT_UTILS_H

#define GMU_SPAT_UTILS_H



#include <math.h>

#define MAX_NUM_SPEAKERS 64

typedef struct xyz 
{
	float x;
	float y;
	float z;;
} t_xyz;


// Constants used for trigonometric convertions:
static const double kRadiansToDegrees = 180.0 / 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;
static const double kDegreesToRadians = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068 / 180.0;

// conversion

void aed2xyz(t_xyz * xyz, float a, float e, float d);



#endif