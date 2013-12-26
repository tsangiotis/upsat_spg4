SGP4/SDP4 orbit propagator for UP-Sat.

Originaly from http://www.sat.dundee.ac.uk/~psc/sgp4.html

Private bitbucket.org repository:
https://bitbucket.org/tsagi/upsat_spg4/

(c) Tasos Sagiotis 2013-2014
University of Patras
tsagi (at) gmail.com

Released under the terms of the GNU LGPL V3
http://www.gnu.org/licenses/lgpl-3.0.html

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

====================================================================
                             Parts
==================================================================== 

 ->Installation
 ->Introduction
 ->Configuration

==================================================================== 
                          Installation
==================================================================== 

Make a copy of the upsat_spg4 directory (Out of Dropbox please).
Then in a terminal (UNIX System with gcc installed preferable) type:

    $ make all

To test your installation after compilation is finished type:
    
    $ make test

It should finish with no errors.

==================================================================== 
                          Introduction
====================================================================

\TODO

==================================================================== 
                          Configuration
====================================================================

The programm uses data to compute from the file "data/twoline.txt".

The code that reads the file, seats on "satutl.c".

These are the variables and the corresponding characters in each line.

\TODO: explanation of variables

      line 1
____________________
  var   |   char    |
====================
ep_year |   19:20   |
ep_day  |   21:32   |
bm      |   54:59   |
bx      |   60:61   |

      line 2
____________________
  var   |   char    |
====================  
eqinc   |    9:16   |
ascn    |   18:25   |
ecc     |   27:33   |
argp    |   35:42   |
mnan    |   44:51   |
rev     |   53:63   |
norb    |   64:68   |
