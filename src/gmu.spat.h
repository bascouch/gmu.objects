/*
 *  gmu.spat.h
 *  gmu.objects
 *
 *  Created by charles on 02/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */



#ifndef GMU_SPAT_H

#define GMU_SPAT_H


#include "maxcpp5.h"
#include "gmu.spat.utils.h"


class gmu_spat {
	
	public :
	
	gmu_spat(int num_speakers);//:num_speakers(num_speakers);
	~gmu_spat();
	
	virtual int config(long ac, t_atom * av) = 0;
	
	virtual int positionXYZ(float x, float y, float z) = 0;
	virtual int positionAED(float a, float e, float d) = 0;
	
	virtual int perform_gains(float * gain) = 0;
	
	virtual int perform(float input, float * outputs) = 0;
	
	// circle config 
    
    int init_spk_config(int num_speakers);
	
    int num_speakers;
    bool upd_flag;
	
    float stored_gains[MAX_NUM_SPEAKERS];
    
    t_xyz common_spk_config[MAX_NUM_SPEAKERS];
	
	
};

#include "gmu.spat.dbap.h"

#endif