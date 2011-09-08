/*
 *  gmu.spat.dbap.h
 *  gmu.objects
 *
 *  Created by charles on 02/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */



#ifndef GMU_SPAT_DBAP_H

#define GMU_SPAT_DBAP_H

#include "gmu.spat.h"

typedef struct dbap_speaker 
{
	float x;
	float y;
	float z;
	float weight;
} t_dbap_speaker;


class gmu_spat_dbap : public gmu_spat
{
	public :
	gmu_spat_dbap(int num_speakers);
	~gmu_spat_dbap();
	
	int config(long ac, t_atom * av);
	
	int positionXYZ(float x, float y, float z);
	int positionAED(float a, float e, float d);
	
	int perform_gains(float * gain);
	
	int perform(float input, float * outputs);
	
	void calculate_mean_dst_position();
	void calculate_variance();
	
	void calculate3D();
	
	protected :
	
	t_dbap_speaker	speakers[MAX_NUM_SPEAKERS];
	
	float			blur;
	float			rolloff;
	float			a;
	
	t_xyz			position;
	t_xyz			mean_dst_position;
	float			variance;

	
};

#endif