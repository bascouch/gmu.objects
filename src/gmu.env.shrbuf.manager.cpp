/*
 *  gmu.env.shrbuf.manager.cpp
 *  gmu.objects
 *
 *  Created by charles on 10/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.env.shrbuf.manager.h"

t_gmu_env_shrbuf gmu_env_shrbuf_manager::empty_shrbuf;

gmu_env_shrbuf_manager::gmu_env_shrbuf_manager()
{
    // empty common shrbuf
	empty_shrbuf.valid = false;
    empty_shrbuf.frames = 0;
    empty_shrbuf.used = 0;
    
}


gmu_env_shrbuf_manager::~gmu_env_shrbuf_manager()
{
	for (v_shrbuf_it = v_shrbuf.begin(); v_shrbuf_it < v_shrbuf.end(); v_shrbuf_it++)
	{
		sysmem_freeptr((*v_shrbuf_it).mem);
		v_shrbuf.erase(v_shrbuf_it);
			
	}
}

t_gmu_env_shrbuf * gmu_env_shrbuf_manager::get(long frames)
{
	t_gmu_env_shrbuf new_mem;
	
	new_mem.mem = (float *) sysmem_newptr(frames * sizeof(float));
	new_mem.frames = frames;
	new_mem.active = true;
	new_mem.used = 0;
    new_mem.valid = true;
	
	v_shrbuf.push_back(new_mem);
	
	return &new_mem;
}

long gmu_env_shrbuf_manager::get_length()
{
	return v_shrbuf.size();
}


void gmu_env_shrbuf_manager::clean()
{
	for (v_shrbuf_it = v_shrbuf.begin(); v_shrbuf_it < v_shrbuf.end(); v_shrbuf_it++) {
		if( ! (*v_shrbuf_it).active && (*v_shrbuf_it).used < 1 )
		{
			sysmem_freeptr((*v_shrbuf_it).mem);
			v_shrbuf.erase(v_shrbuf_it);
		}
		
	}
}