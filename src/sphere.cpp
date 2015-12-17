// Author: John Cairns <john@2ad.com>
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


#include <stdlib.h>
#include <math.h>

#include "physicalconstants.h"
#include "sphere.h"
#include "gmath.h"

using namespace GZKFast;

Sphere::Sphere(const Position &c, GFloat r) : center(c), radius(r) {
};

void Sphere::setCenter(const Position &c) {
    center = c;
}

const Position &Sphere::getCenter(void) const {
    return center;
}

GFloat Sphere::getRadius(void) const {
    return radius;
}


void Sphere::setRadius(GFloat r) {
    radius = r;
}


GFloat	Sphere::intersection(const Position &x2,
                          const Position &x1,
                          Position &hitPoint) const {
    
    // find the point of intersection of the particle on the detector
    // sphere

    // parameterize line
    // r = x1 + (x2 - x1) t

    Position dx(x2);

    dx -= x1;
    
    // parameterize the sphere
    // r^2 = (x - cx)^2 + (y - cy)^2 + (z - cz)^2
    //
    // then the solution is
    // at^2 + bt + c = 0

    
    GFloat a = dx*dx;
    
    Position offset(x1);
    offset -= center;
    
    GFloat b = dx*offset;
    b *=2.0;
    
    GFloat c = center*center + x1*x1 - 2.0*(center*x1) - radius*radius;
    
    GFloat disc = b*b - 4.0*a*c;
    if(disc > 0) { // ray intersection
        // else if disc = 0 ray is tangent
        // else if disc < 0 no intersection

        // The precision of the calculation is far too
        // low using double... result is always zero

        // there are two solutions
        // corresponding to the two points of intersection of
        // a ray and sphere
        // the smaller t value is nearest

        // quadratic equation
            // try series expansion for extreme cases
            //GFloat t = -c/b;
            //t -= a*t*t/b;

        GFloat t = (-b-sqrt(disc))/(2.0*a);
    

        // plug solution back into equation 1
        hitPoint = x1;
        hitPoint += dx*t; 
        
        return t;
    }
    return -1e20;
}


bool Sphere::contains(const Position &x) const {
	Position x1(x);

	x1 -= center;

	return (x1.mag() < radius);
}

