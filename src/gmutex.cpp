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


#ifdef WIN32
#include <windows.h>
#endif

#include "gmutex.h"

using namespace GZKFast;

#ifdef WIN32
// bogus win32 crap

GMutex::GMutex() {
    InitializeCriticalSection(&mutex);
}


GMutex::~GMutex() {
    DeleteCriticalSection(&mutex);
}

void GMutex::lock(void) {
    EnterCriticalSection(&mutex);
}

bool GMutex::trylock(void) {
    if(TryEnterCriticalSection(&mutex)) {
        return true;
    } 
    return false;
}

void GMutex::unlock(void) {
    LeaveCriticalSection(&mutex);
}

#else
// posix threads
GMutex::GMutex() {
    pthread_mutex_init(&mutex, NULL);
}


GMutex::~GMutex() {
    pthread_mutex_destroy(&mutex);
}

void GMutex::lock(void) {
    pthread_mutex_lock(&mutex);
}

bool GMutex::trylock(void) {
    return (pthread_mutex_trylock(&mutex) == 0);
}

void GMutex::unlock(void) {
    pthread_mutex_unlock(&mutex);
}

#endif
