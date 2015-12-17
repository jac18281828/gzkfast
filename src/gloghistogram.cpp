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


#include <math.h>

#include <iostream>

#include "gloghistogram.h"


using namespace GZKFast;

// construct  a new log histogram object
GLogHistogram::GLogHistogram(GFloat normalization, int numBins, GFloat distMin, GFloat distMax, GFloat a):
    GHistogram(numBins, distMin, distMax)
{
    norm = normalization;

    minVal=log10(distMin);
    maxVal=log10(distMax);

    binWidth=(maxVal - minVal) / numBins;

    alpha=a;

}         


// get the value from a particular bin
GFloat GLogHistogram::getBin(int n) const
{
    if(getCount()) {
        GFloat dN = GHistogram::getBin(n)/(GFloat)getCount();
        GFloat dE = pow(10.0F, binWidth);
        GFloat E = pow(10.0F, minVal+n*binWidth);
        return pow(E, alpha) * (dN/dE);
    }
    return 0.0;
}

// add a count to a particular bin by value
GFloat GLogHistogram::addBin(GFloat val)
{
    return GHistogram::addBin((GFloat)log10(val));
        
}

// return the sum of the bins
GFloat GLogHistogram::integrate(void) const {
	if(getCount()) {
		GFloat sum=0.0;
		for(int i=0; i<getNBins(); i++) {
			sum += getBin(i);
		}

		return sum;
	}
	return 0.0;
}

GFloat GLogHistogram::getNormalization(void) const {
	return norm; // /pow(getWeightedAvgBinValue(), alpha);
}

GFloat GLogHistogram::getWeightedAvgBinValue(void) const {
	if(getCount()) {
		GFloat sum=0.0;
		for(int i=0; i<getNBins(); i++) {
			sum += getBinValue(i)*getBin(i);
		}
		if(integrate()) {
			return sum/integrate();
		}
	}
	return 0.0;
}
