/*
 *  gmu.buffer.h
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#ifndef GMU_BUFFER_H

#define GMU_BUFFER_H



#include "maxcpp5.h"

#include "gmu.utils.h"

typedef struct buffer_config 
{
	float	sr;
	float	srms; // sample per ms
	
	bool	loop;
	float	loopstart;
	float	loopend;
	
	bool	sinterp; // type of sample interpolation
	
} t_buffer_config;


class gmu_buffer {
	
	public :
	
	gmu_buffer(t_buffer_config * conf,t_symbol * bufname);
	gmu_buffer(gmu_buffer * from);
	
	~gmu_buffer();
	
    int check_buffer(t_buffer_config * conf);
    
	t_buffer * msp_buffer;

	float * samples;
	
	t_symbol * name;
	t_symbol * filename;
	
	// infos
	int chans;
	long frames;
	float sronsr;
	
	// loop
	long loopstart;
	long loopend;
	long looplength;
	
	// protect
	int used;
	bool is_valid;
	static t_symbol * ps_buffer;
	static gmu_buffer * valid_buffer;
};

#endif