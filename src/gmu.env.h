/*
 *  gmu.env.h
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */


#include "gmu.env.slot.h"
#include "gmu.env.shrbuf.manager.h"
#include "gmu.utils.h"

#include "gmu.env.trap.h"
#include "gmu.env.etrap.h"
//#include "gmu.env.buffer.h"

#ifndef GMU_ENV_H

#define GMU_ENV_H


#include "maxcpp5.h"



#define DEF_ENV_TABLE_SIZE 2048

enum e_env_type {
	e_type_buffer,
	e_type_trap
};


class gmu_env {

	public :
	gmu_env(t_gmu_env_shrbuf * p_shared_buffer);
	explicit gmu_env(const gmu_env & copy);
    ~gmu_env();
    
	
	virtual void init(float length, float srms) = 0;
	
	virtual void compute(float * samples, float amp, int start, int end) = 0;
	
	virtual char * get_infos() = 0;
	
	virtual gmu_env * clone() = 0;
	
	
	//gmu_env_slot *env_slot; // reference to the slot it comes from
	
	t_gmu_env_shrbuf * shared_buffer ; // reference to shared buffer
	
	// properties
	
	float f_srms;		// to convert ms to smpl ( = sr / 1000. )
	float f_length; // in ms
	
	// runtime variables
	
	double f_index; // between 0. and 1.
	double f_incr; 
	
	int i_length; // in samples
	int i_index; 
    
    
    // utils
    static t_linear_interp * interp_table;
	
	
	
};


#endif