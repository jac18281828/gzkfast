// Author: John Cairns <john@2ad.com>
// Date:   July 26, 2005


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

#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H 1


#include <fstream>
#include <list>


#include "grunthread.h"

#include "universe.h"
#include "gmutex.h"
#include "source.h"
#include "evolutionthread.h"
#include "gloghistogram.h"

namespace GZKFast {


//
// create particle events from
// a list of sources

    class DLLEXPORT EventGenerator : public GRunThread {
        std::fstream         protonHistFile;
        std::fstream         protonMapFile;
        GLogHistogram        protonHistogram;
        
        GFloat               energyFlux;
        std::list<EvolutionThread*>    threadList;

        std::list<Source*>             sourceList;
        
        int                  eventCount;
        
        GFloat               time;
        GFloat               totalEnergy;

        Universe            *universe;

        // the target for proton events
        Position             dest;
      public:

        EventGenerator(const char *eventFile,
                       GFloat expectedE, 
                       Universe *u,
                       std::list<EvolutionThread*> &threads,
                       int nBins,
                       GFloat minEnergy,
                       GFloat maxEnergy);
        
        ~EventGenerator();

        virtual int run(void);

        // return the number of events generated so far
        int             getEventCount(void) const;

        // add a source
        void            add(Source *s);

        // get total injected energy
        GFloat			getTotalEnergy(void) const;
    };

};


#endif
