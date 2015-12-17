// Author: John Cairns <john@2ad.com>
// Date:   Jun 20, 2005

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


#ifndef GHISTOGRAM_H
#define GHISTOGRAM_H 1


#include <iostream>

#include "physicalconstants.h"


namespace GZKFast {

    class DLLEXPORT GHistogram {
      private:
        int	*histVals;
            
        int			nBins;
            
        int			nTot;
      protected:
            
        GFloat		minVal;
        GFloat		maxVal;
            
        GFloat		binWidth;
            
      protected:
            
        // return the bin appropriate to value
        int getBinNumber(GFloat val) const;
            
            
        // test if value is in our distribution
        bool inDist(GFloat val) const;
            
            
      public:
            
        // ctor
        GHistogram(int numBins, GFloat distMin, GFloat distMax);
            
        // dtor
        ~GHistogram();
            
        // get the number of bins
        int getNBins(void) const;

        // get the minimum of the distribution
        GFloat getDistMin(void) const;

        // get the maximum of the distribution
        GFloat getDistMax(void) const;

        // get the count on a particular bin
        GFloat		getBin(int n) const;

        // get the count corresponding to a particular dist value
        // return the bin for this value
        GFloat getBin(GFloat val) const;

        // add a count to a particular bin by bin index
        // return current bin value
        GFloat addBin(int n);

        // add a count to a particular bin by value
        GFloat addBin(GFloat val);

        // get the starting value represented by this bin
        GFloat getBinValue(int n) const;

        // get the total count
        int	   getCount(void) const;
        
        // evaluate the numerical derivative at a given value
        // in distribution
        GFloat deltaN(GFloat val, GFloat &err);
        
        // return the width of a bin in this system
        GFloat getBinWidth(void) const;
    };


    // printing
    static inline std::ostream &operator<<(std::ostream &os, const GZKFast::GHistogram &g) {

        int start=0;
        //while(g.getBin(start)==0.0) {start++;}

        std::streamsize prec = os.precision();
        os.precision(OUTPUT_PRECISION);
        os.setf(std::ios_base::scientific, std::ios_base::floatfield);

        for(int i=0; i<g.getNBins(); i++) {
            os << g.getBinValue(i) << ", " << g.getBin(i) << std::endl;
        };
                
        os << std::flush;
        os.precision(prec);

        return os;
    };
};

#endif
