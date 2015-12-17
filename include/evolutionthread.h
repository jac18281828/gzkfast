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


#ifndef EVOLUTIONTHREAD_H
#define EVOLUTIONTHREAD_H 1

#include <list>


#include "universe.h"

#include "space.h"
#include "detector.h"
#include "grunthread.h"
#include "gmutex.h"

namespace GZKFast {

// 
// simulate the propagation of particle events through
// space

    class DLLEXPORT EvolutionThread : public GRunThread {
      protected:
        Universe *universe;

        int     maxEvents;
        
        int     nParticles;

        // all known spaces
        std::list<Space *>       spaceList;
        
        // all known detectors
        std::list<Detector *> detectorList;

        // all known particles
        std::list<Particle *> particleList;

        static GMutex     universeMutex;

        // remove particle from list
        void remove(Particle *particle, bool wasHit);
        

      public:
        // ctor
        EvolutionThread(int events, Universe *u);

        virtual int run(void);

        // add a detector
        void addDetector(Detector *d);

        // add a space
        void addSpace(Space *s);

        // add a particle
        void addParticle(Particle *p);

        // get the maximum number of events
        int getMaxEvents(void) const;

        // get the current number of events
        int getEventQueueSize(void) const;

        // wait for the queue to reach zero then stop
        void stop(void);

    };

};

#endif
