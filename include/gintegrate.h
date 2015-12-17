// Author: John Cairns <john@2ad.com>
// Date:   June 21, 2006

//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2006 John A Cairns <john@2ad.com>

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


#ifndef GINTEGRATE_H
#define GINTEGRATE_H 1

#include "physicalconstants.h"
#include "gerror.h"
#include "gfunction.h"

namespace GZKFast {

// a simple integrator for smooth functions
// this is an implementation of the NR qromb
    class DLLEXPORT  GIntegrate {
      public:

        // integrate a 1d function using simpsons rule
        static GFloat integrate(GFunction &func,
                                GFloat a, GFloat b)
            throw( GError );

#ifdef GINTEGRATE2D
        // integrate a 2d function using simpsons rule
        static GFloat integrate2d(GFunction &func,
                                  GFloat a1, GFloat b1,
                                  GFloat a2, GFloat b2)
            throw( GError );

#endif
        
        static GFloat rombIntegrate(GFunction &func,
                                    GFloat a, GFloat b)
            throw( GError );
        
        
        static GFloat vegasIntegrate(GFunction &func,
                                     GFloat a, 
                                     GFloat b,
                                     int maxIter=1000)
            throw( GError );

    };
};
#endif
