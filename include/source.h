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



#ifndef SOURCE_H
#define SOURCE_H 1

#include "physicalconstants.h"

#include "gvector.h"

namespace GZKFast {

	// forward declaration

	class Particle;

// This is a source of ultra high energy particles.
//    A source generates a particle by sampling an energy 
//    spectrum and producing a particle of suitable energy
//    with momentum oriented toward a given destination

    class DLLEXPORT Source {
      public:
        // Generate a particle by sampling an energy spectrum
        // and return the particle oriented with momentum toward
        // destination.

        virtual Particle *getEvent(const Position &dest) = 0;

        // get the position
        virtual const Position &getPosition(void) const = 0;
    };
};

#endif
