/*
 *  gmu.grain.cpp
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.grain.h"

float * gmu_grain::tmp_buffer = 0;

gmu_grain::gmu_grain()
{
    //post("gmu.grain normal construct %p",this);
}

gmu_grain::~gmu_grain()
{
    //post("gmu.grain destruct %p",this);
	delete env;
}

