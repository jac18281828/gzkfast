#     GZKFast - An environment for astrophysical simulation.
#     Copyright (C) 2005 John A Cairns <john@2ad.com>

#     This program is free software; you can redistribute it and/or modify
#     it under the terms of the GNU General Public License as published by
#     the Free Software Foundation; either version 2 of the License, or
#     (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
#     02110-1301  USA

#     John Cairns
#     2271 Coventry Rd
#     Upper Arlington, OH, 43221

Depends on GNU Scientific Library

ubuntu:

Install dependencies
1.  apt-get install libgsl0-dev

Build
2.  make -j2

Run
3.  bin/gzkfast

etc.

Windows:

Win32 project file and workspce provided
