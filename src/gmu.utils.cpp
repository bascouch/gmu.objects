/*
 *  gmu.utils.cpp
 *  gmu.objects
 *
 *  Created by charles on 20/05/11.
 *  Copyright 2011 GMEM. All rights reserved.
 *
 */

#include "gmu.utils.h"

float atom2float(t_atom * av, int ind)
{
	switch (av[ind].a_type)
	{
		case A_FLOAT :
			return av[ind].a_w.w_float;
			break;
		case A_LONG :
			return av[ind].a_w.w_long;
			break;
		default :
			return 0.;
			
	}
	
	
}

t_symbol * atom2symbol(t_atom * av, int ind)
{
	switch (av[ind].a_type)
	{
		case A_SYM :
			return av[ind].a_w.w_sym;
			break;
		default :
			return 0;
			
	}
	
}