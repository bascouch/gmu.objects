/*
 *  gmu.env.cpp
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.env.h"

t_linear_interp * gmu_env::interp_table = 0;

gmu_env::gmu_env(t_gmu_env_shrbuf * p_shared_buffer)
{
	shared_buffer = p_shared_buffer;
	
	if( shared_buffer->valid )
		shared_buffer->used++;
    
    //post("gmu.env normal construct %p",this);
	
}

gmu_env::gmu_env(const gmu_env & copy):f_srms(copy.f_srms),f_length(copy.f_length)
{
    //post("gmu.env COPY construct %p",this);
    f_index = 0.;
    f_incr = 0.;
    
    i_length = 0;
    i_index = 0;
    
    shared_buffer = copy.shared_buffer;
    
}

gmu_env::~gmu_env()
{
	if (shared_buffer->valid)
		shared_buffer->used--;

    //post("gmu.env destruct %p",this);
}