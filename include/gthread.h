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


#ifndef GTHREAD_H
#define GTHREAD_H 1

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "physicalconstants.h"

namespace GZKFast {
// global type for thread function
#ifdef WIN32
    typedef  DLLEXPORT LPTHREAD_START_ROUTINE   GThreadRoutine;
#else
    typedef  DLLEXPORT void * (*GThreadRoutine)(void*);
#endif

    class DLLEXPORT  GThread {
      private:
#ifdef WIN32
        HANDLE                      thread;
        DWORD		            threadId;
#else 
	pthread_t		    thread;
	int			    threadId;
	pthread_attr_t		    attr;
#endif
      public:
        
        // public types
        typedef enum { IDLE, LOWEST,
                       BELOW_NORMAL, NORMAL, ABOVE_NORMAL, HIGHEST,
                       CRITICAL} Priority;

        // construct a new thread
        GThread(GThreadRoutine threadRoutine, // function to call
                void*           arg);           // argument to pass
	
		~GThread();

        // set the priority
        void setPriority(Priority p);

        // get the priority
        Priority getPriority(void);

        // return the thread id
        int  getThreadId(void);

        // terminate a running thread
        int terminate(void);


        // join this thread
        //
        int join(void);

		static void	sleep(int msecs);
    };
};

#endif
