// Author:  John Cairns <john@2ad.com>
// Date:    July 19, 2005


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


#ifndef WIN32
#include <unistd.h>
#endif

#include "gthread.h"

using namespace GZKFast;

#ifdef WIN32

GThread::GThread(GThreadRoutine threadRoutine,
                 void*      arg) {

    thread = CreateThread(NULL, 
                          0,    
                          threadRoutine, 
                          arg,         
                          0,        
                          &threadId); 

    setPriority(NORMAL);
    
}


GThread::~GThread() {
    CloseHandle(thread);
}

void GThread::setPriority(Priority p) {

    //  THREAD_PRIORITY_TIME_CRITICAL Indicates 3 points above normal priority. 
    //  THREAD_PRIORITY_HIGHEST Indicates 2 points above normal priority. 
    //  THREAD_PRIORITY_ABOVE_NORMAL Indicates 1 point above normal priority. 
    //  THREAD_PRIORITY_NORMAL Indicates normal priority. 
    //  THREAD_PRIORITY_BELOW_NORMAL Indicates 1 point below normal priority. 
    //  THREAD_PRIORITY_LOWEST Indicates 2 points below normal priority. 
    //  THREAD_PRIORITY_ABOVE_IDLE Indicates 3 points below normal priority. 
    //  THREAD_PRIORITY_IDLE Indicates 4 points below normal priority. 

    int priority=THREAD_PRIORITY_NORMAL;
    switch(p) {
        case IDLE:
            priority=THREAD_PRIORITY_IDLE;
            break;
        case LOWEST:
            priority=THREAD_PRIORITY_LOWEST;
            break;
        case BELOW_NORMAL:
            priority=THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case ABOVE_NORMAL:
            priority=THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case HIGHEST:
            priority=THREAD_PRIORITY_HIGHEST;
            break;
        case CRITICAL:
            priority=THREAD_PRIORITY_TIME_CRITICAL;
            break;
        case NORMAL:
        default:
            break; // already set
    }

    SetThreadPriority(thread, priority);

}

// return the current thread priority
GThread::Priority GThread::getPriority(void) {
    switch(GetThreadPriority(thread)) {
        case THREAD_PRIORITY_IDLE:
            return IDLE;
        case THREAD_PRIORITY_LOWEST:
            return LOWEST;
        case THREAD_PRIORITY_BELOW_NORMAL:
            return BELOW_NORMAL;
        case THREAD_PRIORITY_ABOVE_NORMAL:
            return ABOVE_NORMAL;
        case THREAD_PRIORITY_HIGHEST:
            return HIGHEST;
        case THREAD_PRIORITY_TIME_CRITICAL:
            return CRITICAL;
        case THREAD_PRIORITY_NORMAL:
            return NORMAL;
    };
    return NORMAL;
}

int GThread::getThreadId(void) {
    return (int)threadId;
}

// terminate the thread if it is executing
int GThread::terminate(void) {

    if(TerminateThread(thread, EXIT_SUCCESS)) {
        return EXIT_SUCCESS;
    }
    
	return EXIT_FAILURE;
}

int GThread::join(void) {
    if(WaitForSingleObject(thread, INFINITE)==WAIT_TIMEOUT) {
        return -1;
    }
	return EXIT_SUCCESS;
}

void GThread::sleep(int msecs) {
	Sleep(msecs);
}
#else
// posix threads


GThread::GThread(GThreadRoutine threadRoutine,
                 void*      arg) {
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    //pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&thread, &attr, threadRoutine, arg);
    setPriority(NORMAL);
}

GThread::~GThread() {
    pthread_attr_destroy(&attr);
}


void GThread::setPriority(Priority p) {

    int priority=p;

    int policy;
    sched_param sp;
    
    pthread_getschedparam(thread, &policy, &sp);

    sp.sched_priority=p;

    pthread_setschedparam(thread, policy, &sp);
}

// return the current thread priority
GThread::Priority GThread::getPriority(void) {
    int policy;
    sched_param sp;
    pthread_getschedparam(thread, &policy, &sp);

    return (Priority)sp.sched_priority;
}

int GThread::getThreadId(void) {
    return (int)threadId;
}

// terminate the thread if it is executing
int GThread::terminate(void) {
    return join();
}

int GThread::join(void) {
    return pthread_join(thread, NULL);
}

void GThread::sleep(int msecs) {
	usleep(msecs*1000);
}

#endif
