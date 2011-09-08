/*
 *  gmu.spat.cpp
 *  gmu.objects
 *
 *  Created by charles on 23/08/11.
 *  Copyright 2011 GMEM. All rights reserved.
 *
 */

#include "gmu.spat.h"

gmu_spat::gmu_spat(int num)
{
	num_speakers = num;
}

gmu_spat::~gmu_spat()
{
	
}

int gmu_spat::init_spk_config(int num_speakers)
{
	switch (num_speakers) {
			
		case 1: // mono
			common_spk_config[0].x = 0.;
			common_spk_config[0].y = 0.;
			common_spk_config[0].z = 0.;
			break;
			
		case 2: // stereo
			common_spk_config[0].x = -1.;
			common_spk_config[0].y = 0.;
			common_spk_config[0].z = 0.;
			
			common_spk_config[1].x = 1.;
			common_spk_config[1].y = 0.;
			common_spk_config[1].z = 0.;
			break;
			
		case 3: // stereo + 1 back
			aed2xyz(common_spk_config+0,-45.,0., 1.);
			
			aed2xyz(common_spk_config+1,45.,0., 1.);
			
			aed2xyz(common_spk_config+2,-180.,0., 1.);
			
			break;
			
			
		default:
			if(num_speakers%2)  // odd
			{
				aed2xyz(common_spk_config+0,0.,0., 1.);
				
				int k = (num_speakers-1)/2.;
				float ref_angle = 360. / num_speakers;
				
				for(int i=0; i<k ; i++)
				{
					aed2xyz(common_spk_config+(2*i)+1,-(i+1)*ref_angle,0., 1.);
					aed2xyz(common_spk_config+(2*i)+2,(i+1)*ref_angle,0., 1.);
				}
			}
			else {
				
				int k = num_speakers/2.;
				
				float ref_angle = 360. / num_speakers;
				float ref_angle_2 = ref_angle/2.;
				
				for(int i=0; i<k ; i++)
				{
					aed2xyz(common_spk_config+(2*i),-(i*ref_angle+ref_angle_2),0., 1.);
					aed2xyz(common_spk_config+(2*i)+1,(i*ref_angle+ref_angle_2),0., 1.);
				}
				
			}
			
			break;
	}
	
	
	
}