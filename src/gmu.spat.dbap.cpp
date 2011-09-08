/*
 *  gmu.spat.dbap.cpp
 *  gmu.objects
 *
 *  Created by charles on 02/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */

#include "gmu.spat.dbap.h"

// CONSTRUCTOR DESTRUCTOR

gmu_spat_dbap::gmu_spat_dbap(int num_speakers):gmu_spat(num_speakers),rolloff(12.),blur(1.)
{
	position.x = 0.;
	position.y = 0.;
	position.z = 0.;
	
	// circle config by default
	init_spk_config(num_speakers);
	
	for(int i=0; i<num_speakers ; i++)
	{
		speakers[i].x = common_spk_config[i].x;
		speakers[i].y = common_spk_config[i].y;
		speakers[i].z = common_spk_config[i].z;
		
		speakers[i].weight = 1;
	}
	
	calculate_variance();
	
	blur = 0.000001;
	rolloff = 6; // 6 dB rolloff by default
	
    a = log(pow(10., (rolloff / 20.)))/log(2.);
    
	upd_flag = true;
	
}

gmu_spat_dbap::~gmu_spat_dbap()
{
	
}

int gmu_spat_dbap::config(long ac, t_atom * av)
{
	if(ac-- && av[0].a_type == A_SYM)
	{
		t_symbol * param;
		param = atom_getsym(av);
		
		// DST_POSITION
		
		if(strcmp(param->s_name, "dst_position")==0)
		{
			
			if(ac>=3)
			{
				long int num;
				float x,y,z;
				
				av++;
				num = atom_getlong(av++);
				x = atom_getfloat(av++);
				y = atom_getfloat(av++);
				
				if(ac>3)
					z = atom_getfloat(av++);
				
				num--;
				
				if(num >= 0 && num < num_speakers)
				{
					speakers[num].x = x;
					speakers[num].y = y;
					speakers[num].z = z;
				}
				else 
					perror("dst_position index out of range");
			
				// RECALC VARIANCE
				calculate_variance();
				
			}
			else
				perror("dst_position needs more args");
			
				
		}
		
		// DST_WEIGHT
		
		else if(strcmp(param->s_name, "dst_weight")==0)
		{
			
			if(ac>=2)
			{
				long int num;
				float w;
				
				av++;
				num = atom_getlong(av++);
				w = atom_getfloat(av++);
				
				num--;
	
				if(num >= 0 && num < num_speakers)
					speakers[num].weight = w > 0. ? w : 0.;
				else 
					perror("dst_weight index out of range");
				
			}
			else
				perror("dst_weight needs more args");
			
		}
		
		// BLUR
		
		else if(strcmp(param->s_name, "blur")==0)
		{
			
			if(ac>=1)
			{
				float b;
				
				av++;
				b = atom_getfloat(av++);

				if (b<0.000001) 
					b = 0.000001;
				
				blur = b;
				
			}
			else
				perror("blur needs more args");
			
		}
		
		
		// ROLLOFF
		
		else if(strcmp(param->s_name, "rolloff")==0)
		{
			
			if(ac>=1)
			{
				float r;
				
				av++;
				r = atom_getfloat(av++);
				
				if (r <= 0.) 
					r = 0.000001;
				
				rolloff = r;
				a = log(pow(10., (rolloff / 20.)))/log(2.);
				
				
			}
			else
				perror("rolloff needs more args");
			
		}
		
		
		/// END
        
        else
            post("gmu.bufgranul~ : dbap doesn't understand %s",param->s_name);
	}
	
	upd_flag = true;
	
	return 0;
}

void gmu_spat_dbap::calculate_mean_dst_position()
{
	long i;
	float ta,tb,tc;
	
	ta = 0;
	tb = 0;
	tc = 0;
	for (i=0; i<num_speakers; i++) {
		ta += speakers[i].x;
		tb += speakers[i].y;
		tc += speakers[i].z;
	}
	mean_dst_position.x = ta/num_speakers;
	mean_dst_position.y = tb/num_speakers;
	mean_dst_position.z = tc/num_speakers;
}

void gmu_spat_dbap::calculate_variance()
{
	long i;
	float dx, dy, dz;
	float d2=0;
	
	
	calculate_mean_dst_position();
	

	for (i=0; i<num_speakers; i++) {
		dx = speakers[i].x - mean_dst_position.x;
		dy = speakers[i].y - mean_dst_position.y;
		dz = speakers[i].z - mean_dst_position.z;
		d2 += dx*dx + dy*dy + dz*dz;
	}		
	
	variance = sqrt(d2/(num_speakers-1));
	
}

int gmu_spat_dbap::positionXYZ(float x, float y, float z)
{
	if(position.x !=x || position.y !=y || position.z !=z)
	{
		upd_flag = true;
		
		position.x = x;
		position.y = y;
		position.z = z;
	}
}

int gmu_spat_dbap::positionAED(float a, float e, float d)
{
		upd_flag = true;
}

void gmu_spat_dbap::calculate3D()
{
	float k;							// Scaling coefficient
	float k2inv;						// Inverse square of the scaling constant k
	float dx, dy, dz;						// Distance vector
	float r2;							// Bluriness ratio 
	float dia[MAX_NUM_SPEAKERS];	// Distance to ith speaker to the power of x->a.
	//float sdia[MAX_NUM_DESTINATIONS];	// Squared Distance to ith speaker (without bluriness ratio)
	long iC,iN;							// index of the the dest C and N dest in dst_position[]
	float sSC,sSN,sCN;					// squared Distance of the Source to C and N and [CN]
	t_xyz P;							// Projection point of Source on [CN], pointer to coord of S, C and N
	float kCN, dist, min_dist;
	float v, out;						// is the source out of the hull ? (-1 inside, 1 outside)
	long id_min;						// id of the closest dest
	long i,j;
	
	r2 = blur * variance;
	r2 = r2*r2;
	k2inv = 0;
	for (i=0; i<num_speakers; i++) {
		dx = position.x - speakers[i].x;
		dy = position.y - speakers[i].y;
		dz = position.z - speakers[i].z;
		dia[i] = pow(double(dx*dx + dy*dy + dz*dz + r2), double(0.5*a));
		
		k2inv = k2inv + (speakers[i].weight * speakers[i].weight)/(dia[i]*dia[i]);
	}
	
	k = sqrt(1./k2inv);
	
	for (i=0; i<num_speakers; i++)
		stored_gains[i] = speakers[i].weight * k/dia[i];
	
	
}

int gmu_spat_dbap::perform_gains(float * gain)
{
	if (upd_flag)
		// RECALC DBAP
		calculate3D();
	
	for (int i =0; i < num_speakers ; i++)
		gain[i] = stored_gains[i];
	
	upd_flag = false;
	
	return num_speakers;
}

int gmu_spat_dbap::perform(float input, float * outputs)
{
	if (upd_flag)
		// RECALC DBAP
		calculate3D();
	
	for (int i =0; i < num_speakers ; i++)
		outputs[i] = input * stored_gains[i];
	
	upd_flag = false;
	
	return num_speakers;
}