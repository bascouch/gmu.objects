/*
 *  gmu.bufgranul~.h
 *  gmu.objects
 *
 *  Created by charles on 09/09/10.
 *  Copyright 2010 GMEM. All rights reserved.
 *
 */



// STL includes
#include <algorithm>
#include <map>
#include <list>
#include <vector>
#include <string>


#ifndef GMU_BUFGRANUL_H

#define GMU_BUFGRANUL_H

using namespace std;

// MAX includes
#include "maxcpp5.h"

// GMU includes
#include "gmu.env.h"
#include "gmu.spat.h"
#include "gmu.buffer.h"
#include "gmu.grain.h"
#include "gmu.bufgrain.h"


#define NVOICES 1024 // max number of voices 
#define MAX_BUF_SLOTS 512	// nombre maximum de buffers son
#define MAX_ENV_SLOTS 512	// nombre maximum de buffers enveloppe
#define MIN_LENGTH 0.1 // longueur minimum des grains en ms  // DISABLED 


#endif
