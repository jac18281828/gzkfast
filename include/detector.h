// Author: John Cairns <john@2ad.com>
// Date:   July 26, 2005

//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2005 John A Cairns <john@2ad.com>

//     This program is free software; you can redistribute it and/or modify
//     it under the terms of the GNU General Public License as published by
//     the Free Software Foundation; either version 2 of the License, or
//     (at your option) any later version.

//     This program is distributed in the hope that it will be useful,
//     but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.

//     You should have received a copy of the GNU General Public License
//     along with this program; if not, write to the Free Software
//     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
//     02110-1301  USA

//     John Cairns
//     2271 Coventry Rd    
//     Upper Arlington, OH, 43221

#ifndef DETECTOR_H
#define DETECTOR_H 1

#include "physicalconstants.h"
#include "particle.h"
#include "gvector.h"

namespace GZKFast {

// evaluate an interaction with propagating 
// particles
    

    class DLLEXPORT Detector {
      public:

		// given a particle at a certain distance and a given momentum
		// evaluate if this particle would be detected by detector
		
		// return particle if hit
		// return NULL if no hit
		
		virtual const Particle *hit(const Universe *universe, const Particle *particle) = 0;

		// return the total number of observed events so far
		virtual int	getEventsDetected(void) const = 0;


		// call this before the particle is removed from the system
		// for tracking statistics about misses
		virtual void onRemove(const Universe *universe, const Particle *particle, bool hit) =0;
	
    };
};

#endif
