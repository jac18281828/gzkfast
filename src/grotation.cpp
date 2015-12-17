// Author:  John Cairns <john@2ad.com>
// Date:    Sept 15 2005


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


#include "grotation.h"

using namespace GZKFast;

// specify a 3x3 rotation matrix
GRotation::GRotation(GFloat theta, Axis axis) : GMatrix(3) {
    setRotation(theta, axis);
}

GRotation::GRotation(GFloat a, GFloat b, GFloat c, GFloat d, Axis axis) :
    GMatrix(3) {
    setRotation(a, b, c, d, axis);
}


void GRotation::setRotation(GFloat theta, Axis axis) {
    switch(axis) {
        case Z: // same as X
        case X:
            setRotation(cos(theta), sin(theta), -sin(theta), cos(theta), axis);
            break;
        case Y:
            setRotation(cos(theta), -sin(theta), sin(theta), cos(theta), axis);
            break;
    }
}


void GRotation::setRotation(GFloat a, 
                            GFloat b, 
                            GFloat c, 
                            GFloat d, 
                            Axis axis) {
    rotationAxis=axis;

    identity(); // make it a 3x3 identity
    
    // setup rotations
    switch(axis) {
        case X:
            set(1, 1, a);
            set(2, 1, b);
            set(1, 2, c);
            set(2, 2, d);
            break;
        case Y:
            set(0, 0, a);
            set(2, 0, b);
            set(0, 2, c);
            set(2, 2, d);
            break;
        case Z:
            set(0, 0, a);
            set(1, 0, b);
            set(0, 1, c);
            set(1, 1, d);
            break;
    }
}
    
void GRotation::invert(void) {
    // this is the same as changing the sign of the angle of rotation
    // this would have no effect on cosine terms so
    // all we do is change the sign of the "sine" terms

    switch(rotationAxis) {
        case X:
            set(2, 1, -get(2, 1));
            set(1, 2, -get(1, 2));
            break;
        case Y:
            set(2, 0, -get(2, 0));
            set(0, 2, -get(0, 2));
            break;
        case Z:
            set(1, 0, -get(1, 0));
            set(0, 1, -get(0, 1));
            break;
    }
}
