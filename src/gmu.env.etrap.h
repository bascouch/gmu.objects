//
//  gmu.env.etrap.h
//  gmu.objects
//
//  Created by charles bascou on 08/09/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef gmu_objects_gmu_env_etrap_h
#define gmu_objects_gmu_env_etrap_h


#include "gmu.env.h"

class gmu_env_etrap : public gmu_env {
    
	public :
	gmu_env_etrap(t_gmu_env_shrbuf * p_shared_buffer);
	gmu_env_etrap(t_gmu_env_shrbuf * p_shared_buffer,float att, float dec,float pow_a,float pow_d);
    explicit gmu_env_etrap(const gmu_env_etrap &copy);
	~gmu_env_etrap();
	
	void init(float length, float srms);
	
	void compute(float * samples, float amp, int start, int end);
	
	char * get_infos();
	
	gmu_env_etrap * clone() { return new gmu_env_etrap(*this); }
	
	protected :
	
	float att;
	float dec;
    
    float pow_a;
    float pow_d;
	float c_pow;
	int phase;
	
	long remain_ind_phase;
	float incr;
	float value;
    float evalue;
    
};


#endif
