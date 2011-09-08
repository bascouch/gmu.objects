/*
 *  gmu.bufgrain.h
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#ifndef GMU_BUFGRAIN_H

#define GMU_BUFGRAIN_H

#include "gmu.common.h"

class gmu_bufgrain : public gmu_grain {
	
	public :
	
    gmu_bufgrain();
    ~gmu_bufgrain();
    
	int compute(float ** buffer, int frames);
    
    char * debug_str();
	
	gmu_buffer * buffer;
	
	// PARAMETERS
	double begin;
	double transp;
    
	// VARIABLES
	
	double d_ind; // float sample index in buffer
    double d_inc; // increment
    
    // utils
    static t_linear_interp * interp_table;
	
};

#endif