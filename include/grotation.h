// Author: John Cairns <john@2ad.com>
// Date:   Sept 15, 2005

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


#ifndef GROTATION_H
#define GROTATION_H 1

#include "physicalconstants.h"
#include "gmatrix.h"

namespace GZKFast {

    class DLLEXPORT GRotation : public GMatrix {
      public:
        // public types
        typedef enum {X, Y, Z} Axis;

      private:
        Axis                    rotationAxis;
      public:

        // construct a rotation matrix with angle theta 
        // around the specified axis
        GRotation(GFloat theta, Axis axis);

        // construct a rotation matrix out of four specified 
        // rotation coeffs
        GRotation(GFloat a, GFloat b, GFloat c, GFloat d, Axis axis);

        // setup the rotation matrix by angle theta about axis a
        void setRotation(GFloat theta, Axis axis);
        
        // setup the rotation matrix using four rotation coeffs provided
        void setRotation(GFloat a, GFloat b, GFloat c, GFloat d, Axis axis);

        // it is easy to invert these rotation matricies
		void invert(void);
        
    };
};

#endif
