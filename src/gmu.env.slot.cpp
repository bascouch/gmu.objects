/*
 *  gmu.env.slot.cpp
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.env.slot.h"

gmu_env_slot::gmu_env_slot(gmu_env_shrbuf_manager * p_shared_buffer_manager)
{
	shared_buffer_manager = p_shared_buffer_manager;
	shared_buffer = &shared_buffer_manager->empty_shrbuf;
}


gmu_env_slot::gmu_env_slot(gmu_env_shrbuf_manager * p_shared_buffer_manager,long ac, t_atom * av)
{
	shared_buffer_manager = p_shared_buffer_manager;
	shared_buffer = &shared_buffer_manager->empty_shrbuf;
	set(ac,av);
}

gmu_env * gmu_env_slot::set(long ac, t_atom * av)
{
	// ask for trash of shared_buffer
	if(shared_buffer->valid)
		shared_buffer->active = false;
	
	// treat arguments
	if(ac-- && av[0].a_type == A_SYM)
	{
		t_symbol * param, *stest;
		//t_atom test[5];
		param = atom_getsym(av);
		
        av++;
		
		// INITIALIZED TO DEFAULT
		
		dyn_env = true;
		shared_buffer = &shared_buffer_manager->empty_shrbuf;
		env = NULL;

		// TRAPEZOID
		if(param->s_name == NULL || strcmp(param->s_name, "trap")==0)
		{
			dyn_env = true;
            float att = 10, dec = 10;
            if(ac == 1 && (av[0].a_type == A_LONG || av[0].a_type == A_FLOAT))
            {
                att = dec = fabs(atom2float(av, 0));
            }else if(ac > 1)
            {
                att = fabs(atom2float(av, 0));
                dec = fabs(atom2float(av, 1));
            }
            
            env = new gmu_env_trap(shared_buffer,att,dec);
			
		}
        // EXP TRAPEZOID
		else if(param->s_name == NULL || strcmp(param->s_name, "etrap")==0)
		{
			dyn_env = true;
            
            float att = 10, dec = 10,eatt = 1., edec = 1.;
            
            if(ac == 2)
            {
                att = dec = fabs(atom2float(av, 0));
                eatt = edec = fabs(atom2float(av, 1));
                
            }else if(ac == 3)
            {
                att = fabs(atom2float(av, 0));
                dec = fabs(atom2float(av, 1));
                eatt = edec = fabs(atom2float(av, 2));
            }
            else if(ac >= 4)
            {
                att = fabs(atom2float(av, 0));
                eatt = fabs(atom2float(av, 1));
                dec = fabs(atom2float(av, 2));
                edec = fabs(atom2float(av, 3));
            }
            
            env = new gmu_env_etrap(shared_buffer,att,dec,eatt,edec);
			
		}
		// BUFFER
		else if(strcmp(param->s_name, "buffer")==0)
		{
			t_symbol * bname;
			
			if(ac-- && av[0].a_type == A_SYM)
			{
				dyn_env = false;
				
				// get buffer
				shared_buffer = shared_buffer_manager->get(DEF_ENV_TABLE_SIZE);
				
			}else {
				error("env buffer msg needs more args ... setting env to default (trap)");
			}
			
			
		}
		
		
		
	}

	return env;
}


gmu_env_slot::~gmu_env_slot()
{
	if(!dyn_env && shared_buffer->valid)
		shared_buffer->active = false;
	if(env) delete env;

}

gmu_env * gmu_env_slot::instanciate()
{
	if(env){
		if(!dyn_env && shared_buffer->valid)
			shared_buffer->used++;
		return env->clone();
	}
	else 
		return NULL;

}