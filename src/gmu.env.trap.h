/*
 *  gmu.env.trap.h
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */


#ifndef GMU_ENV_TRAP_H

#define GMU_ENV_TRAP_H

#include "gmu.env.h"

class gmu_env_trap : public gmu_env {

	public :
	gmu_env_trap(t_gmu_env_shrbuf * p_shared_buffer);
	gmu_env_trap(t_gmu_env_shrbuf * p_shared_buffer,float att, float dec);
    explicit gmu_env_trap(const gmu_env_trap &copy);
	~gmu_env_trap();
	
	void init(float length, float srms);
	
	void compute(float * samples, float amp, int start, int end);
	
	char * get_infos();
	
	gmu_env_trap * clone() { return new gmu_env_trap(*this); }
	
	protected :
	
	float att;
	float dec;
	
	int phase;
	
	long remain_ind_phase;
	float incr;
	float value;

};

#endif