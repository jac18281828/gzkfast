// Author: John Cairns <john@2ad.com>
// Date:   May 24, 2005

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


#ifndef GLORENTZ_H
#define GLORENTZ_H 1


#include "physicalconstants.h"

#include "gmatrix.h"

namespace GZKFast {
    
    class DLLEXPORT GLorentz : public GMatrix {
	public:
        // produce the Lorentz transformation

        GLorentz(GFloat gamma, const GVector<GFloat> &beta) :
            GMatrix(4)
        {

            // jackson 11.7
            const GFloat        xBoostArry[16] = 
                {gamma, -gamma*beta.getX(), -gamma*beta.getY(), -gamma*beta.getZ(),
                 -gamma*beta.getX(), 1.0+(gamma-1.0)*beta.getX()*beta.getX()/(beta*beta), 1.0+(gamma-1.0)*beta.getX()*beta.getY()/(beta*beta), 1.0+(gamma-1.0)*beta.getX()*beta.getZ()/(beta*beta),
                 -gamma*beta.getY(), 1.0+(gamma-1.0)*beta.getX()*beta.getY()/(beta*beta), 1.0+(gamma-1.0)*beta.getY()*beta.getY()/(beta*beta), 1.0+(gamma-1.0)*beta.getY()*beta.getZ()/(beta*beta),
                 -gamma*beta.getZ(), 1.0+(gamma-1.0)*beta.getX()*beta.getZ()/(beta*beta), 1.0+(gamma-1.0)*beta.getY()*beta.getZ()/(beta*beta), 1.0+(gamma-1.0)*beta.getZ()*beta.getZ()/(beta*beta)};

            for(int i=0; i<4; i++) {
                for(int j=0; j<4; j++) {
                    set(i,j, xBoostArry[i+4*j]);
                }
            }
        }
        
    };
};




#endif
