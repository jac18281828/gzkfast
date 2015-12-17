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


#include "grunthread.h"
#include "gthread.h"

using namespace GZKFast;

#ifdef WIN32

// local static function to start this thread
static DWORD    WINAPI  ThreadFunction(void* arg);
#else
static void*            ThreadFunction(void* arg);
#endif

GRunThread::GRunThread() {
    thread=NULL;
    status=RUNNING;
}

GRunThread::~GRunThread() {
    if(thread) delete(thread);
}

void GRunThread::start(void) {
    if(!thread) {
        thread = new GThread(ThreadFunction, (void*)this);
    }
}

// tell the thread to stop
void GRunThread::stop(void) {
    status=STOPPED;
}

int GRunThread::terminate(void) {
    if(thread) {
        int exitCd = thread->terminate();
        delete(thread);
        thread=NULL;

        return exitCd;
    }
    // ? unknown exit status
    return 0;
}

int GRunThread::waitForExit(void) {
    // join and never timeout
    if(thread) 
        return thread->join();

    return 0;
}


bool GRunThread::isStarted(void) {
    return (thread!=NULL);
}

void GRunThread::setPriority(GThread::Priority p) {
    if(thread) thread->setPriority(p);
}

GRunThread::Status GRunThread::getStatus(void) const {
    return status;
}

// run the thread
#ifdef WIN32
DWORD    WINAPI  ThreadFunction(void* arg) {

    GRunThread *thrObj = (GRunThread*)arg;

    return thrObj->run();
}
#else
void*  ThreadFunction(void* arg) {

    GRunThread *thrObj = (GRunThread*)arg;
    if(thrObj) return (void*)thrObj->run();
    
    return NULL;
}
#endif

