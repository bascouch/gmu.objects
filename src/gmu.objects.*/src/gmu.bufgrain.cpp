/*
 *  gmu.bufgrain.cpp
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.bufgrain.h"

t_linear_interp * gmu_bufgrain::interp_table = 0;


gmu_bufgrain::gmu_bufgrain()
{
    //post("gmu.bufgrain normal construct %p",this);
}

gmu_bufgrain::~gmu_bufgrain()
{
    buffer->used --;
    //post("gmu.bufgrain destruct %p",this);
}

char * debug_str()
{
  //  post();
}



int gmu_bufgrain::compute(float ** out_buffer, int frames)
{
    int retval = 1; // return 0 if the grain have to be killed
    int i,j;
    
    int dsp_i_begin, dsp_i_end;
    int nech_process, nech_process_sat;
    double target_d_ind;
    
    //trick interp long index
	long base_lindex;
	long lindex;
	long lincr;
    
    long buffer_index;
	int interp_table_index;
    
    float val;
    
    if(i_delay >= frames)
    {
        i_delay -= frames;
        return 1; 
    }
    
    nech_process = MIN(frames - i_delay , i_remain_index);
//    post("frames %f i_delay %f i_remain_index %f",frames, i_delay , i_remain_index);
//    post("nech_process %d d_ind %f d_inc %f",nech_process,d_ind,d_inc);
    
    // snd index cible en fin de boucle
    target_d_ind = d_ind + nech_process * d_inc;
    
    // si index source hors des bornes -> kill grain
    if( d_ind >  buffer->frames || d_ind <  0 )
        return 0;
    
    if( !buffer->is_valid )
        buffer = gmu_buffer::valid_buffer ;
    
    ATOMIC_INCREMENT(&buffer->msp_buffer->b_inuse);
    
    // initialisation des paramètre de boucle
    if( target_d_ind > buffer->frames )
    {
        // on met la voix off
       retval = 0;
        // jusqu'a quel index on calcule
        nech_process_sat = (long)(nech_process * (buffer->frames - d_ind) /  (target_d_ind - d_ind));
        
        dsp_i_begin = i_delay;
        dsp_i_end = i_delay + nech_process_sat;
        
        base_lindex = (long) d_ind;
        lindex = interp_index_scale( d_ind - (double) base_lindex );
        lincr  = interp_index_scale(d_inc);
    }else if( target_d_ind < 0 )
    {
        // on met la voix off
        retval = 0;
        // jusqu'a quel index on calcule   ATTENTION arrondi depassement
        nech_process_sat = (long)(nech_process * (d_ind) /  (d_ind - target_d_ind));
        
        dsp_i_begin = i_delay;
        dsp_i_end = i_delay + nech_process_sat;
        
        base_lindex = (long) d_ind;
        lindex = interp_index_scale( d_ind - (double) base_lindex );
        lincr  = interp_index_scale(d_inc);
        
    }
    else 
    {
        nech_process_sat = nech_process;
        
        dsp_i_begin = i_delay;
        dsp_i_end = i_delay + nech_process_sat;
        
        base_lindex = (long) d_ind;
        lindex = interp_index_scale( d_ind - (double) base_lindex );
        lincr  = interp_index_scale(d_inc);
        
    }
    
//    post(" i %i %i %i",dsp_i_begin,dsp_i_end,nech_process_sat);
//    post(" sl %i ",buffer->frames);
    
    /// //// ///
    // boucle de lecture du buffer
    for(i= dsp_i_begin; i < dsp_i_end; i++)
    {
        
        // Lecture de la forme d'onde
        buffer_index = base_lindex + interp_get_int( lindex );
        interp_table_index = interp_get_table_index( lindex );
        val = interp_table[interp_table_index].a * buffer->samples[buffer_index * buffer->chans] 
        + interp_table[interp_table_index].b * buffer->samples[(buffer_index + 1) * buffer->chans];
        
        lindex += lincr;
        
        // stockage temporaire
        tmp_buffer[i] = val;
        
    }

        //goto end;
    
    // apply env
    
    env->compute(tmp_buffer, amp, dsp_i_begin, dsp_i_end);
    
    // apply spat
    
    for(i= dsp_i_begin; i < dsp_i_end; i++)
    {
        for (j = 0; j < spat->num_speakers; j++)
            out_buffer[j][i] += tmp_buffer[i] * hp_gains[j];
    }
    
end: 
    
    //********************************
    //  MAJ de l'état des grains
    
    d_ind = target_d_ind;
    
    i_index += nech_process_sat;
    i_remain_index -= nech_process_sat;
    
    if( i_remain_index <= 0)
    {	
        retval = 0; // kill grain
    }
    
    // decremente delai
    
    i_delay = MAX(i_delay - frames,0);
    
    
    ATOMIC_DECREMENT(&buffer->msp_buffer->b_inuse);
    
	// TODO
	return retval;
}