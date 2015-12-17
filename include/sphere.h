// Author: John Cairns <john@2ad.com>
// Date:   Aug 14, 2005


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



#ifndef SPHERE_H
#define SPHERE_H 1

#include "physicalconstants.h"
#include "gvector.h"

namespace GZKFast {
    

    // simple geometry of a sphere
    class DLLEXPORT Sphere {
      private:
        Position center;
        GFloat   radius;
        
      public:

        Sphere(const Position &c, GFloat r);

        // set center

        void setCenter(const Position &c);

        // get center
        const Position &getCenter(void) const;


        // get radius
        GFloat getRadius(void) const;

        // set radius

        void setRadius(GFloat r);
        
                
        // ray sphere intersection
        //
        // Does the ray extending from x1 to x2
        // intersect this object?
        //
        // if there is an intersection, return the solution and
        // hitPoint is set to the point of intersection
        // otherwise, hitPoint is ignored, and returns 0.0.
		//
		// The returned value is the value of t
		// that solves the parametric equation
		//
		//  x1 + t*(x2-x1)
		//
		//  If t < 0.0 No intersection
		//  If 0.0 <= t <= 1.0 this means, the hit is on ray x1-x2
		//  If t > 1.0  then the ray would intersect if it was longer
		//              The length is radius/t.
		// 

        GFloat	intersection(const Position &x2,
							  const Position &x1,
							  Position &hitPoint) const;


		bool    contains(const Position &x) const;


    };

};

#endif
