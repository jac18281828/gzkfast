// Author: John Cairns <john@2ad.com>
// Date:   May 17, 2005


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



#ifndef SPACE_H
#define SPACE_H 1

#include "physicalconstants.h"
#include "particle.h"
#include "gvector.h"

namespace GZKFast {


//
// a space is an environment for particle propagation
// the space can propagate the particle by solving the
// equation of motion, monte carloing the equation of
// motion or estimating.
// 
// the space also supports red shifting the particle
// as it propagates

    class DLLEXPORT Space {
      public:

        // propagate the particle through this space
        // by solving the equation of motion
	
        // if particle production results from propagation modify
        // particleList and return true
        virtual bool propagate(Particle *particle, 
                               Particle::ParticleList &particleList) = 0;


        // redshift a particle corresponding to a translation
        // from the specified position
        virtual Particle *redshift(Particle *particle) = 0;
    };


};

#endif
