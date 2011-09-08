/*
 *  gmu.env.slot.h
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */


#ifndef GMU_ENV_SLOT_H

#define GMU_ENV_SLOT_H

#include "gmu.env.h"

class gmu_env_slot {
	
	public :
	gmu_env_slot(gmu_env_shrbuf_manager * p_shared_buffer_manager);
	gmu_env_slot(gmu_env_shrbuf_manager * p_shared_buffer_manager,long ac, t_atom * av);
	~gmu_env_slot();
	
	gmu_env * set(long ac, t_atom * av);
	
	gmu_env * instanciate();
	
	protected :
	
	gmu_env * env; // env model object
	
	// for persitence
	bool dyn_env; // is it dynamic ( not pre-calculated into a buffer )
	t_gmu_env_shrbuf * shared_buffer; // pre-calculated env
	
	gmu_env_shrbuf_manager * shared_buffer_manager;
	
	friend class gmu_env;
	
};

#endif