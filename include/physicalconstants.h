// Author: John Cairns <john@2ad.com>
// Date:   May 12, 2005


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


// This program is written to use units of eV and cm where
// possible.  Still some unit conversions are very useful.

#ifndef PHYSICALCONSTANTS_H
#define PHYSICALCONSTANTS_H 1

// system constants required for WIN32
#ifdef WIN32
#pragma warning( disable: 4244 4251 4786 4290 4018	 )
#endif

#ifndef DLLEXPORT
#ifdef WIN32
// Bill Gates you had your fun now leave us alone!
#define DLLEXPORT __declspec(dllimport)
#else
#define DLLEXPORT
#endif
#endif

#include <limits>

#include <math.h>

namespace GZKFast {
    
    // Global Types
    
    // allow use of float or double here to
    // strike performance vs precision optimization
    typedef double      GFloat;

    // allow use of 64bit large integers
#ifdef WIN32
    typedef _int64			 GLongInt;
    typedef unsigned _int64		 GULongInt;
#else
    // required for random
    typedef long long	        GLongInt;
    typedef unsigned long long  GULongInt;
#endif
                   
                   
    // these are useful global physical constants

    const GFloat        PrecisionGoal   = 1e-5;
                   
    // default system of units is eV, cm, s
    const GFloat eV                        = 1.0;
                   
    const GFloat cm                        = 1.0;
    
    const GFloat s                         = 1.0;
                   
    const GFloat Gauss						= 1.0;
    
    const GFloat esu						= 1.0;
    
    const GFloat Coulomb					= 2.99792458e9*esu;

    // Kelvin 
    const GFloat Kelvin                    = 1.0;
    

    // SI Units
    const GFloat Meter			   = 1e2*cm;
    
    const GFloat km			   = 1e3*Meter;
    
    // Boltzmann constant+
    const GFloat kBoltzmann = 8.617385e-05;
    
    // EeV to eV
    const GFloat EeV    =       1e18*eV;

    // PeV to eV
    const GFloat PeV    =       1e15*eV;

    // TeV to eV
    const GFloat TeV    =       1e12*eV;

    // GeV to eV
    const GFloat GeV    =       1e9*eV;

    // MeV to eV
    const GFloat MeV    =       1e6*eV;

    // KeV to eV
    const GFloat KeV	=	1e3*eV;

    // Ergs
    const GFloat erg	=		6.24150974e11*eV;
    
    // parsec to cm
    const GFloat pc     =       3.08568025e18*cm;
    
    // kilo parsec to cm
    const GFloat Kpc    =       1e3*pc;

    // mega parsec to cm
    const GFloat Mpc    =       1e6*pc;

    // barn to cm^2
    const GFloat barn   =       1e-24*cm*cm;
    
    // milli barn
    const GFloat mbarn  =       1e-3*barn;

    // PI
    // alternatively use 4.0*atan(1.0)
    const GFloat PI = 3.1415926535897932385;

    // cm/s
    const GFloat SpeedOfLight  =  2.99792458e10*cm/s;

    const GFloat hbar          =  6.582122e-16*eV*s;


    // actually the Proton mass
    const GFloat NucleonMass    = 938.271998*MeV;

    // Stefan Boltzman Constant
    const GFloat StefanBoltzmann = PI*PI*pow(kBoltzmann, 4.0)/60.0/pow(hbar, 3.0)/(SpeedOfLight*SpeedOfLight);

    // CMB temp
    const GFloat CMBTemp = 2.725*Kelvin;

    // Wein's Law (Landau S63)
    const GFloat Wein_max = 2.822*eV/(Kelvin*Meter*Meter);

    // Cairns' Law for the CMB dist
    // slight correction to Weins law for CMB.
    const GFloat Cairns_max = Wein_max/2.05;

    // Zeta function
    const GFloat Zeta_3 = 1.2020569031595942854;

    // Waxman P0
    const GFloat Waxman_P0		= 4.5e44*erg;

    // Hubble constant
    const GFloat	H0		= 71.0*km/s/Mpc;

    // electron charge
    const GFloat        ElectronCharge = 4.80320441e-10*esu;

    // electron mass
    const GFloat	ElectronMass = .510998918*MeV;

    // Thompson cross section
    const GFloat	SigThomson  = 0.665245873*barn;

    // Normalization of proton spectrum
    const GFloat	ProtonNormalization = 1e-19*1e-5;

    const int           OUTPUT_PRECISION = 6;

    const GFloat        INFTY = std::numeric_limits<float>::max();

}

#endif
