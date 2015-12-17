// Author: John Cairns <john@2ad.com>
// Date:   Aug 3, 2005

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


#ifndef BFIELDSPACE_H
#define BFIELDSPACE_H 1

#include "physicalconstants.h"
#include "space.h"

#include "universe.h"

namespace GZKFast {

    class DLLEXPORT BFieldSpace : public Space {
      protected:

        Universe                *universe;

      public:
    
        // ctor
        BFieldSpace(Universe *u);
    

        // propagate a particle through space
        bool propagate(Particle *particle, Particle::ParticleList &particleList);
        
        // the redshift due to propagation
        Particle *redshift(Particle *particle);
    };
};

#endif
