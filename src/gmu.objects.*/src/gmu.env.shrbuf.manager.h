/*
 *  gmu.env.shrbuf.manager.h
 *  gmu.objects
 *
 *  Created by charles on 10/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */


#ifndef GMU_ENV_SHRBUF_H

#define GMU_ENV_SHRBUF_H

#include <vector>

#include "maxcpp5.h"

typedef struct {
	float * mem;
	long frames;
	
	bool active; // if still in a gmu_env_slot
	int used;	// how many gmu_env uses it
    bool valid;
	
} t_gmu_env_shrbuf;


class gmu_env_shrbuf_manager {
	
	public : 
	gmu_env_shrbuf_manager();
	~gmu_env_shrbuf_manager();
	
	
	t_gmu_env_shrbuf * get(long frames);
	
	void clean();
	
	long get_length();
	
    static t_gmu_env_shrbuf empty_shrbuf;
    
	protected :
	std::vector<t_gmu_env_shrbuf> v_shrbuf;
	std::vector<t_gmu_env_shrbuf>::iterator v_shrbuf_it;
    
    
};

#endif