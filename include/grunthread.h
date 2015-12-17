// Author: John Cairns <john@2ad.com>
// Date:   July 19, 2005


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

#ifndef GRUNTHREAD_H
#define GRUNTHREAD_H 1

#include "gthread.h"

namespace GZKFast {

// to create a thread, simply subclass this object and
// implement the run method
    class DLLEXPORT GRunThread {
      public:
	typedef enum { RUNNING, STOPPED } Status;
      private:
        GThread *thread;

      protected:

	Status	status;
        
      public:
        // ctor
        GRunThread();
        
        // dtor
        ~GRunThread();

        // start running by calling "run" in a new thread
        void start(void);

        // YOU (the caller) must implement this method
        virtual int run(void) = 0;

	// tell the running thread to stop itself
        void stop(void);

	// terminate the running thread
	int terminate(void);

        // wait for run to return
        int waitForExit(void);

        // test to see if the thread is running
        bool isStarted(void);

        // set the thread priority
        void setPriority(GThread::Priority p);

		// get the current status, RUNNING or STOPPED
		// 
		// Note: In this object STOPPED is a request for
		// the implementor, STOPPED is not enforced.
		Status getStatus(void) const;
    };
};

#endif
