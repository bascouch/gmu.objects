//
//  gmu.env.etrap.cpp
//  gmu.objects
//
//  Created by charles bascou on 08/09/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "gmu.env.etrap.h"

gmu_env_etrap::gmu_env_etrap(t_gmu_env_shrbuf * p_shared_buffer):gmu_env(p_shared_buffer),att(10),dec(10)
{
}

gmu_env_etrap::gmu_env_etrap(t_gmu_env_shrbuf * p_shared_buffer,float att, float dec,float pow_a,float pow_d):gmu_env(p_shared_buffer)
{
	this->att = ( att > 0. )? att : 0.;
	this->dec = ( dec > 0. )? dec : 0.;
    this->pow_a = ( pow_a > 0. )? pow_a : 0.;
    this->pow_d = ( pow_d > 0. )? pow_d : 0.;
	
}

gmu_env_etrap::gmu_env_etrap(const gmu_env_etrap &copy):gmu_env(copy) // WARNING
{
    this->att = copy.att;
    this->dec = copy.dec;
    this->pow_a = copy.pow_a;
    this->pow_d = copy.pow_d;
    
}

/**** inherited from gmu.env ******/

void gmu_env_etrap::init(float length, float srms)
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
    c_pow = pow_a;
	
	if( remain_ind_phase )
		incr = 1. / remain_ind_phase;
	else {
		value = 1.;
		phase = 1;
		incr = 0.;
        c_pow = 1.;
		remain_ind_phase = f_srms * ( f_length - att - dec);
	}
    
}


void gmu_env_etrap::compute(float * samples, float amp, int start_n, int end_n)
{
	int i=0;
	
	for(i = start_n; i < end_n ; i++)
	{
		// env apply
		samples[i] = samples[i] * pow(value,c_pow) * amp;
		
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
                c_pow = 1.;
				remain_ind_phase = f_srms * ( f_length - att - dec);
			}else if (phase == 1) {
				phase = 2;
				remain_ind_phase = f_srms * dec;
                c_pow = pow_d;
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



char * gmu_env_etrap::get_infos()
{
	return "etrap env";
}