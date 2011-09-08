/*
 *  gmu.grain.h
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#ifndef GMU_GRAIN_H

#define GMU_GRAIN_H

#include "gmu.common.h"

class gmu_grain {
	
	
	public :
	
	// constructor
	gmu_grain();
	~gmu_grain();
	
	// generate portion of the grain into buffer
	virtual int compute(float ** buffer, int frames) = 0;
	
	
	// grains common parameters
	float amp;
	float length;
	
	// env
	gmu_env * env;
	
	
	// spat
    gmu_spat * spat;
	t_xyz position;
	float hp_gains[MAX_NUM_SPEAKERS]; // gains for each speakers
	
	// grains runtime variables
	long i_index;			// index of whole grain in samples
	long i_remain_index;	// remain index
	
	long i_delay;			// trig delay in samples
    
    // static temp memory
    static float * tmp_buffer;
	
};

#endif