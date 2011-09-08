/*
 *  gmu.env.trap.cpp
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.env.trap.h"

gmu_env_trap::gmu_env_trap(t_gmu_env_shrbuf * p_shared_buffer):gmu_env(p_shared_buffer),att(10),dec(10)
{
}

gmu_env_trap::gmu_env_trap(t_gmu_env_shrbuf * p_shared_buffer,float att, float dec):gmu_env(p_shared_buffer)
{
	this->att = ( att > 0. )? att : 0.;
	this->dec = ( dec > 0. )? dec : 0.;
	
}

gmu_env_trap::gmu_env_trap(const gmu_env_trap &copy):gmu_env(copy) // WARNING
{
    this->att = copy.att;
    this->dec = copy.dec;
    
}

/**** inherited from gmu.env ******/

void gmu_env_trap::init(float length, float srms)
{
	float segm_length = att+dec;
	
	f_length = fabs(length); 
	f_srms = srms;
	
	// corrected att & dec if too long to fit in the grain
	if(f_length < segm_length)
	{
		float ratio = f_length / segm_length;
		att *= ratio;
		dec *= ratio;
	}
	
	// reversed env if length < 0.
	if (length < 0.)
	{
		float tmp;
		tmp = att;
		att = dec;
		dec = tmp;
	}
	
	// init
	value = 0.;
	phase = 0;
	remain_ind_phase = f_srms * att;
	
	if( remain_ind_phase )
		incr = 1. / remain_ind_phase;
	else {
		value = 1.;
		phase = 1;
		incr = 0.;
		remain_ind_phase = f_srms * ( f_length - att - dec);
	}

}


void gmu_env_trap::compute(float * samples, float amp, int start_n, int end_n)
{
	int i=0;
	
	for(i = start_n; i < end_n ; i++)
	{
		// env apply
		samples[i] = samples[i] * value * amp;
		
		// env update
		if(remain_ind_phase--)
		{
			value += incr;
		}
		else 
		{
			if (phase == 0) {
				phase = 1;
				value = 1.;
				incr = 0.;
				remain_ind_phase = f_srms * ( f_length - att - dec);
			}else if (phase == 1) {
				phase = 2;
				remain_ind_phase = f_srms * dec;
				if( remain_ind_phase )
					incr = -1. / remain_ind_phase;
				else {
					value = 0.;
					incr = 0.;
				}

			}
		}


	}
}



char * gmu_env_trap::get_infos()
{
	return "trap env";
}