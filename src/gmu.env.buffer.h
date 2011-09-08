/*
 *  gmu_env_buffer.h
 *  gmu.objects
 *
 *  Created by charles on 08/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#ifndef GMU_ENV_BUFFER_H

#define GMU_ENV_BUFFER_H

#include "gmu.env.h"

class gmu_env_buffer : public gmu_env {
	
	public :
		
	gmu_env_buffer(t_gmu_env_shrbuf * p_shared_buffer, t_symbol * p_buf_sym);
	~gmu_env_buffer();

	
	gmu_env_buffer * clone() { return new gmu_env_buffer(*this); }
	

	protected :
	
	t_symbol * buf_sym;

};

#endif