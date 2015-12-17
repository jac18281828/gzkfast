// Author: John Cairns
// Date:   May 31, 2005


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



#ifndef PROTONSPECTRUM_H
#define PROTONSPECTRUM_H 1

#include "physicalconstants.h"
#include "gmath.h"
#include "proton.h"


namespace GZKFast {

// 
//  simulate an energy spectrum of protons
//  by returning a number weighted energy distribution
//  by monte carlo method

    class DLLEXPORT ProtonSpectrum {
      protected:
        // starting energy for this spectrum
        GFloat          startEnergy;
        // ending energy for this spectrum
        GFloat          endEnergy;
        // value of alpha as in pow(Energy, alpha);
        GFloat          alpha;

        GFloat          normEnergy;

      public:
        // ctor
        ProtonSpectrum(GFloat startE, GFloat endE, GFloat a);

        // copy ctor
        ProtonSpectrum(const ProtonSpectrum &spec);

        //
        // return an E^alpha weighted energy distribution
        GFloat getDistEnergy(void) const;

        // return the least energy of this distribution
        GFloat getMinEnergy(void) const;

        // return the largest energy of this distribution
        GFloat getMaxEnergy(void) const;

        // return the energy distribution
        GFloat			getdNdE(GFloat E);


    };


};

#endif
