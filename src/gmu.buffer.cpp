/*
 *  gmu.buffer.cpp
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.buffer.h"

t_symbol * gmu_buffer::ps_buffer = gensym("buffer~");

gmu_buffer * gmu_buffer::valid_buffer = NULL;

gmu_buffer::gmu_buffer(t_buffer_config * conf, t_symbol * pname)
{
	
	if ( (msp_buffer = (t_buffer *)(pname->s_thing)) && ob_sym(msp_buffer) == ps_buffer && msp_buffer && msp_buffer->b_valid)
	{
		
		name = pname;
		filename = msp_buffer->b_filename;
		
		chans = msp_buffer->b_nchans;
		frames = msp_buffer->b_frames;
		samples = msp_buffer->b_samples;
		
		is_valid = true;
		
		sronsr = msp_buffer->b_sr / conf->sr;
		
		if(conf->loop)
		{    
			if(conf->loop == 1){   
				loopstart = 0;      
				loopend = frames;
				looplength = loopend - loopstart;}
			if(conf->loop == 2){
				loopstart = SAT(conf->loopstart*conf->srms,0,frames);
				loopend = SAT(conf->loopend*conf->srms,0,frames);
				looplength = loopend - loopstart;}
			
		}
		
		//bufGranul_info(x,0);
		
	} else {
		error("bufGranul~: no buffer~ %s", pname->s_name);
		is_valid = false;
	}
	used = 0;
}

int gmu_buffer::check_buffer(t_buffer_config * conf)
{
    if ( (msp_buffer = (t_buffer *)(name->s_thing)) && ob_sym(msp_buffer) == ps_buffer && msp_buffer && msp_buffer->b_valid)
	{
		filename = msp_buffer->b_filename;
		
		chans = msp_buffer->b_nchans;
		frames = msp_buffer->b_frames;
		samples = msp_buffer->b_samples;
		
		is_valid = true;
		
		sronsr = msp_buffer->b_sr / conf->sr;
		
		if(conf->loop)
		{    
			if(conf->loop == 1){   
				loopstart = 0;      
				loopend = frames;
				looplength = loopend - loopstart;}
			if(conf->loop == 2){
				loopstart = SAT(conf->loopstart*conf->srms,0,frames);
				loopend = SAT(conf->loopend*conf->srms,0,frames);
				looplength = loopend - loopstart;}
			
		}
        
		return 1;
		
	} else {
		//error("bufGranul~: no buffer~ %s", name->s_name);
		is_valid = false;
        return 0;
	}
}


gmu_buffer::gmu_buffer(gmu_buffer * from)
{
	msp_buffer = from->msp_buffer;
	
	samples = from->samples;
	
	name = from->name;
	filename = from->filename;
	
	// infos
	chans = from->chans;
	frames = from->frames;
	sronsr = from->sronsr;
	
	// loop
	loopstart = from->loopstart;
	loopend = from->loopend;
	looplength = from->looplength;
	
	
	used = 0;
	is_valid = from->is_valid;
}


gmu_buffer::~gmu_buffer()
{
}