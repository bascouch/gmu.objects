/*
 *  gmu.bufgranul~.cpp
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.bufgranul~.h"


/******** ******** ******** ********
			THE CLASS
 ******** ******** ******** ********/


class gmu_bufgranul : public MspCpp5<gmu_bufgranul> 
{
	public :
	

	gmu_bufgranul(t_symbol * sym, long ac, t_atom * av)
	{
		int symcount = 0;
		float f;
		bool gotnum=false;
		t_symbol * bufsym = NULL;
		t_symbol * envsym = NULL;
		t_symbol * spatsym = NULL;
		
		t_atom def_env[5];
		
		// initialisation param 
		nspeakers = 2;
		nvoices_active = 0;
		active_env = 0;
		
		conf.sinterp = 1;
		conf.loop = false;
		conf.sr = sys_getsr();
		
		begin = 0.;
		transp = 1.;
		amp = 1.;
		length = 100.;
		
		position.x = 0.;
		position.y = 0.;
		position.z = 0.;
		
		
		// initialisation interp table
		linear_interp_table = (t_linear_interp *) sysmem_newptr( TABLE_SIZE * sizeof(struct linear_interp) );
		
		for(int i=0; i<TABLE_SIZE ; i++)
		{
			f = (float)(i)/TABLE_SIZE; // va de 0 a 1
			
			linear_interp_table[i].a = 1 - f;
			linear_interp_table[i].b = f;
		}
        
        gmu_bufgrain::interp_table = linear_interp_table;
        
        gmu_env::interp_table = linear_interp_table;
		
		// objects parameters
		for (int j=0; j < ac; j++){
			switch (av[j].a_type){
					
				case A_LONG:		// num speakers
					nspeakers= SAT(av[j].a_w.w_long,1,MAX_NUM_SPEAKERS);
					gotnum = true;
                    post("bufgranul~ : nouts %d",nspeakers);
					break;
					
				case A_SYM:
					if(gotnum)		// spat type
					{
						spatsym = av[j].a_w.w_sym;
                        post("bufgranul~ : spat %s",spatsym->s_name);
					}
					else
					if(symcount==1) //	env buffer // TODO: make possible choice of algos
					{
						envsym = av[j].a_w.w_sym;
						symcount++;
                        post("bufgranul~ : env %s",envsym->s_name);
						
					}else			//	buffer
					{						
						bufsym = av[j].a_w.w_sym;
						symcount++;
                        post("bufgranul~ : sound %s",bufsym->s_name);
					}
                    
					break;
					
				case A_FLOAT:
					post("argument %ld is a float : %lf, not assigned", (long)j,av[j].a_w.w_float);
					break;
			}	
		}
		
        
        // DSP init
		setupIO(&gmu_bufgranul::perform, 0, nspeakers);
        
		// SPAT ASSIGN
		if(spatsym)
		{
			if(!strcmp(spatsym->s_name,"dbap"))
			   spat_class = new gmu_spat_dbap(nspeakers);
			else
			   spat_class = new gmu_spat_dbap(nspeakers);
		}
		else
			spat_class = new gmu_spat_dbap(nspeakers);
			
		// ENV BUFFER MANAGER
		env_shrbuf_manager = new gmu_env_shrbuf_manager();
		// ENV ASSIGN
		int env_p_n;
		if(envsym)
		{
            atom_setsym(def_env, gensym("buffer"));
			atom_setsym(def_env+1, envsym);
			env_p_n = 2;
		}
		else 
		{
			atom_setsym(def_env, gensym("trap"));
			env_p_n = 1;
		}
		
		t_symbol * param;
		param = atom_getsym(def_env);
		
        for(int i = 0; i < MAX_ENV_SLOTS ; i++ )
            env_slots[i] = new gmu_env_slot(env_shrbuf_manager,env_p_n,def_env);
		
		// BUFFER ASSIGN
		
		if(bufsym != NULL)
		{
			default_buffer = new gmu_buffer(&conf,bufsym);
            if(default_buffer->is_valid)
            {
                buffer_slots[0] = default_buffer;
                buffer_pool[string(bufsym->s_name)] = default_buffer;
                gmu_buffer::valid_buffer = default_buffer;
            }
            else
                error("bufgranul~ : default sound buffer is not valid");
		}

        // TEMP BUFFER ALLOC for GRAINS
        gmu_bufgrain::tmp_buffer = (float *) sysmem_newptr(8192 * sizeof(float));
        
	}
	
	// DESTRUCTOR
	
	~gmu_bufgranul()
	{
		
		sysmem_freeptr(linear_interp_table);
		sysmem_freeptr(gmu_bufgrain::tmp_buffer);
		delete env_shrbuf_manager;
		delete spat_class;
		
	}
	
	// DSP
	
	void perform(int vs, t_sample ** inputs, t_sample ** outputs) 
	{
        int res;
    
        
        nvoices_active = grain_pool.size();
        
        // zeros the outputs
        for(int i = 0; i < nspeakers; i++)
            memset(outputs[i], 0, vs * sizeof(float));
          
        // check the buffers
        if(perform_check_buffers() == 0)
            return;
        
        for( grains_it = grain_pool.begin(); grains_it != grain_pool.end();)
        {
            res = (*grains_it)->compute(outputs,vs);
            
            if(res == 0)
            {
                delete (*grains_it);
                grains_it = grain_pool.erase(grains_it);
            }else
                grains_it++;
        }
        
	}
	
	// GRAIN
	
	void grain(long inlet, t_symbol *s, long ac, t_atom *av)
	{
		int j,p;
		int nvoices = nvoices_active;
		int xn = 0;
		
        
		if(ac < 9)
		{	
			post("bufgranul~ : grain args are <delay(ms)> <begin> <detune> <amp> <length> <x> <y> <buffer> <envbuffer>");
			return;
		}
		
		if( nvoices_active >= NVOICES )
		{
			post("bufgranul~ : too much active grains.");
			return;
		}
        
		int buffer,envbuffer;
        t_symbol * bufsym = NULL;
        gmu_buffer * tbuf;
		
		double delay = atom2float(av,0);
		double begin = atom2float(av,1);
		double detune = atom2float(av,2);
		double amp = atom2float(av,3);
		double length = atom2float(av,4);
		double x = atom2float(av,5);
		double y = atom2float(av,6);
		
        if(av[7].a_type == A_SYM)
            bufsym = atom2symbol(av, 7);
        else
            buffer = (int)atom2float(av,7);
        
		envbuffer = (int)atom2float(av,8);
		
		conf.srms = conf.sr*0.001; // double for precision
		
		begin = begin * conf.srms;
		delay = delay * conf.srms;
		p = nvoices;
		
        
		// grain creation
		gmu_bufgrain * g = new gmu_bufgrain();
		
        g->i_delay = delay;
        
        
        
		g->d_ind = g->begin = begin;
		g->transp = detune;
		g->d_inc = detune;
        
        g->amp = amp;
		
        // buffer assign
        if(bufsym)
        {
            tbuf = new gmu_buffer(&conf,bufsym);
            if(tbuf->is_valid)
            {
                buffer_pool[string(bufsym->s_name)] = tbuf;
                g->buffer = tbuf;
            }else
                g->buffer = default_buffer;    
            
            g->buffer->used ++;

        }else
        {
            if(buffer < 0 || buffer > MAX_BUF_SLOTS)
                buffer = 0;
            g->buffer = buffer_slots[buffer];
            if(!g->buffer)
                g->buffer = default_buffer;
            g->buffer->used++;
        }
        
		g->env = env_slots[envbuffer]->instanciate();
		        
		g->length = fabs(length) * conf.srms;
        g->i_index = 0;
        g->i_remain_index = (long)g->length;
		g->env->init(length,conf.srms);

		g->position.x = x;
        g->position.y = y;
        
        // spat
        
        g->spat = spat_class;
        
        spat_class->positionXYZ(x,y,0);
        spat_class->perform_gains(g->hp_gains);
        
		//post(" gains %f %f",g->hp_gains[0],g->hp_gains[1]);
        
		//∂ÏæÂÌ∂ÈæÂÏ∂ÈæÂÏ∂ÈÏÌÈÏæÌÂÈ∂Ï
		
		//œêîdjzlkeÏÌ∂ÈêÌÂÏÌÈÏÂ
		
		grain_pool.push_back(g);
		
	}
	
    void spat(long inlet, t_symbol *s, long ac, t_atom *av)
    {
        spat_class->config(ac,av);
    }
    
    void env(long inlet, t_symbol *s, long ac, t_atom *av)
    {
        if(ac < 1)
        {
            post("gmu.bufgranul~ : env needs args");
            return;
        }
        int slotnum = 0;
        int offset = 0;
        
        if (av[0].a_type == A_LONG){
            if(ac < 2)
            {
                post("gmu.bufgranul~ : env needs envtype after slot number");
                return;
            }
            slotnum = (int)atom2float(av, 0);
            
            if(slotnum < 0 || slotnum >= MAX_ENV_SLOTS)
            {
                post("gmu.bufgranul~ : env slot number out of range");
                return;
            }
            
            offset = 1;
        }
        
        
        if(av[offset].a_type == A_SYM)
        {
            env_slots[slotnum]->set(ac-offset,av+offset);
        }
        else
        {
            post("gmu.bufgranul~ : env bad syntax");
            return;
        }
            
    }
    
    void buffer(long inlet, t_symbol *s, long ac, t_atom *av)
    {
        t_symbol * bname;
        gmu_buffer * tbuf;
        
        if(ac < 1)
        {
            post("gmu.bufgranul~ : buffer needs args");
            return;
        }
        int slotnum = 0;
        int offset = 0;
        
        if (av[0].a_type == A_LONG){
            if(ac < 2)
            {
                post("gmu.bufgranul~ : buffer needs buffer_name after slot number");
                return;
            }
            slotnum = (int)atom2float(av, 0);
            
            if(slotnum < 0 || slotnum >= MAX_BUF_SLOTS)
            {
                post("gmu.bufgranul~ : buffer slot number out of range");
                return;
            }
            
            offset = 1;
        }
        
        if(av[offset].a_type == A_SYM)
        {
            bname = atom_getsym(av+offset);
            
            tbuf = new gmu_buffer(&conf,bname);
            if(tbuf->is_valid)
            {
                buffer_slots[slotnum] = tbuf;
                buffer_pool[string(bname->s_name)] = tbuf;
                if(slotnum == 0)
                {
                    default_buffer = tbuf;
                    gmu_buffer::valid_buffer = default_buffer;
                }
            }
            else
                error("bufgranul~ : buffer %s is not valid", bname->s_name);
        }
        else
        {
            post("gmu.bufgranul~ : buffer bad syntax");
            return;
        }

    }
    
    void clear(long inlet)
    {
        for(grains_it = grain_pool.begin(); grains_it != grain_pool.end(); grains_it ++)
            delete (*grains_it);
        
        grain_pool.clear();
    }
    
    int perform_check_buffers()
    {
        if( default_buffer->check_buffer(&conf) == 0)
            return 0;
        
        for( buffer_pool_it = buffer_pool.begin(); buffer_pool_it != buffer_pool.end(); buffer_pool_it++)
        {
            ((*buffer_pool_it).second)->check_buffer(&conf);
        }
        
        return 1;
    }
    
	//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
	//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
	//								DATAS
	//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\///\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
	
	
	//	config params
	int nspeakers;											// speakers num										
	int nvoices_active;										// the number of active voices
	float sigin;											// last value for signal 0 x-ing
	
	t_buffer_config conf;
	
	// input params
	float begin;
	float transp;
	float amp;
	float length;
	t_xyz position;
	
	
	// buffers
	gmu_buffer * default_buffer;							// valid default buffer also buffer_slot #0
	gmu_buffer * buffer_slots[MAX_BUF_SLOTS];				// buffer slots
	std::map<string,gmu_buffer*> buffer_pool;				// buffer pool
	std::map<string,gmu_buffer*>::iterator buffer_pool_it;	// iterator for buffer pool
	
	
	// env_slots
	int active_env;											// active env index
	gmu_env_slot * env_slots[MAX_ENV_SLOTS];					// env slots
	gmu_env_shrbuf_manager * env_shrbuf_manager;			// shrbuf manager
	
	// active grains vector
	vector<gmu_bufgrain*> grain_pool;							// THE GRAINS VECTOR
	vector<gmu_bufgrain*>::iterator grains_it;				// iterator for grain vector
    
	// spat
	gmu_spat * spat_class;										// the spat class

	// interp table
    t_linear_interp * linear_interp_table;
};


extern "C" int main(void) {
	// create a class with the given name:
	gmu_bufgranul::makeMaxClass("gmu.bufgranul~");
	//REGISTER_METHOD(gmu_bufgranul, bang);
	REGISTER_METHOD_GIMME(gmu_bufgranul, grain);
    REGISTER_METHOD_GIMME(gmu_bufgranul, spat);
    REGISTER_METHOD_GIMME(gmu_bufgranul, env);
    REGISTER_METHOD_GIMME(gmu_bufgranul, buffer);
    REGISTER_METHOD(gmu_bufgranul, clear);
    
    post("Copyright © 2011 gmu.bufgranul~ v3.0 RifRaf GMEM Marseille F") ;
	post("build %s %s",__DATE__,__TIME__);
}


