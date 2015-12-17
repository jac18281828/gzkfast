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



#ifndef UNIVERSE_H
#define UNIVERSE_H 1

#include "physicalconstants.h"
#include "source.h"
#include "sphere.h"
#include "gvector.h"

namespace GZKFast {

    // store global state concerning the size,
    // fields and other pertinent data regarding the Universe
    // as a whole
    class DLLEXPORT Universe {
      private:
        BField          bfield;

        // sphere defines the universe
        Sphere          sphere;

        // sphere that defines the target volume
        Sphere			target;

        // timestep
        GFloat          dt;
        
      public:
        
        // a universe centered at p, and B field field.
        Universe(const BField &field, 
                 const Position &p, 
                 const GFloat timeStep,
                 const GFloat targetRad);

        // return the hubble constant of this system
        static GFloat getHubble(void);

        // return the z associated with a particular distance from
        // a coordinate
        static GFloat getRedshift(GFloat r);

        // return the z associated with a particular vector relative
        // to the center of the universe
        GFloat getRedshift(const Position &p) const;

        // return the distance associated with a particular value of z.
        static GFloat getDistance(GFloat z);

        // return the intergalactic magnetic field
        const BField &getBField(void) const;

        // return the position of the center of this universe
        const Position &getPosition(void) const;

        // return the radius of this universe
        GFloat getRadius(void) const;

        // return true if p is inside this universe, and false otherwise
        bool contains(const Position &p) const;

        GFloat getVolume(void) const;

        void add(const Source *s);

        // return the timestep
        GFloat getTimeStep(void) const;

        const Sphere &getTarget(void) const;
    };

};

#endif
