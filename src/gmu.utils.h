/*
 *  gmu.utils.h
 *  gmu.objects
 *
 *  Created by charles on 20/05/11.
 *  Copyright 2011 GMEM. All rights reserved.
 *
 */

#ifndef GMU_UTILS_H

#define GMU_UTILS_H


#include "maxcpp5.h"

// MACRO UTILES
#define SAT(a,min,max) (a < min) ? min : ( (a > max) ? max : a )
#define MIN(a,min) (a < min) ? a : min
#define MAX(a,max) (a > max) ? a : max 
#define MOD(val,mod) val - (mod * floor( (float)val / mod ))

#define MODI(val,mod) val - (mod * ((val/mod) + (val >> 31))) // modulo LONG 32 bits version 

#define MIN_LOOP_LENGTH 0.1


// INTERPOLATION TRICK DEFINES ///    TODO : EXPLAIN

#define TABLE_BITS 10
#define TABLE_SIZE (1 << TABLE_BITS)

#define LOST_BITS 	6

#define FRAC_BITS (TABLE_BITS + LOST_BITS)
#define FRAC_SIZE (1 << FRAC_BITS)

// INTERPOLATION COEFF STRUCTURE

typedef struct linear_interp
{
	float a;
	float b;
	
} t_linear_interp; 

// INTERPOLATION TRICK MACRO

#define interp_index_scale(f) 	((f) * FRAC_SIZE)
#define interp_get_int(i) 	 	((i) >> FRAC_BITS)
#define interp_get_frac(i)		((i) & (FRAC_SIZE - 1))

#define interp_get_table_index(i)	(((i) >> LOST_BITS) & (TABLE_SIZE - 1))

// LOOP LONG INDEX DEFINES

#define LOOP_FRAC_BITS 30
#define LOOP_FRAC_SIZE (unsigned long)(1 <<  LOOP_FRAC_BITS)

#define loop_index_scale(f) (f * (LOOP_FRAC_SIZE))
#define loop_index_sat(i) (i & (LOOP_FRAC_SIZE - 1))
#define loop_index_get_int(i) (i & (1 << LOOP_FRAC_BITS))


float atom2float(t_atom * av, int ind);

t_symbol * atom2symbol(t_atom * av, int ind);

#endif