/*
 *  gmu.spat.utils.cpp
 *  gmu.objects
 *
 *  Created by charles on 02/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.spat.utils.h"

void aed2xyz(t_xyz * xyz, float a, float e, float d)
{
	a = a * kDegreesToRadians;
	e = e * kDegreesToRadians;
	
	float temp = cos(e) * d;
	
	xyz->x = sin(a) * temp; 
	xyz->y = cos(a) * temp; 
	xyz->z = sin(e) * d; 	
	
}


