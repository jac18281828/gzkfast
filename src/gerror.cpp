// Author:  John Cairns <john@2ad.com>
// Date:    Jun 23 2006


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


#include "gerror.h"

using namespace GZKFast;

GZKFast::GError::GError(const char *msg, int num) :
    errorMessage(msg), number(num) {
}

int GZKFast::GError::getNumber(void) const {
    return number;
}

const char *GZKFast::GError::getMessage(void) const {
    return errorMessage.c_str();
}
