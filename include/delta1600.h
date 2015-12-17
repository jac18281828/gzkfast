// Author: John Cairns
// Date:   Aug 15, 2005

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


#ifndef DELTA1600_H
#define DELTA1600_H 1

#include <math.h>

#include "delta.h"

#include "physicalconstants.h"

namespace GZKFast {


    class DLLEXPORT Delta1600 : public Delta {
      public:
        // ctor
        // construct a delta of a given energy at postion
        Delta1600(const G4Vector &p4, 
                  GFloat s_Ecm, 
                  const Position &rVec, 
                  GFloat q,
                  const Position &origin);

        // Tell the particle to decay, return an array of resulting particles
        // which are then injected into the cmb
        bool decay(const Universe &universe, 
                   ParticleList &particleList);

        bool interact(const Particle *particle, 
                      GFloat density,
                      GFloat dx,
                      ParticleList &particleList) {
            return false;
        }

        // get the width
        GFloat getWidth(void) const {
            return 350.0*MeV;
        }

    };

};


#endif
