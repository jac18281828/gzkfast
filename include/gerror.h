// Author: John Cairns <john@2ad.com>
// Date:   Jun 23 2006

//     GZKFast - An environment for astrophysical simulation.
//     Copyright (C) 2006 John A Cairns <john@2ad.com>

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


#ifndef GERROR_H
#define GERROR_H 1

#include <string>
#include <cerrno>

#include "physicalconstants.h"

namespace GZKFast {

// Error class for throwing exceptions
    class DLLEXPORT  GError {
      private:
        int number;

        std::string errorMessage;
        
      public:
        
        // ctor
        GError(const char *msg, int num=errno);

        // return the error number
        int getNumber(void) const;

        // return the message
        const char *getMessage(void) const;
    };

}
#endif
