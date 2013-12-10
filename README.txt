Example of the fixed (AFSPC compatible) SGP4/SDP4 orbit propagator.

	(c) Paul Crawford & Andrew Brooks 1994-2010

	Released under the terms of the GNU LGPL V3
	http://www.gnu.org/licenses/lgpl-3.0.html

	This software is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	For questions on code and licensing, contact emails are:

	psc (at) sat.dundee.ac.uk
	arb (at) sat.dundee.ac.uk

To build software enter then command:

make all

Then to run the test:

make test

The resulting diff.txt file should show virtually zero
errors if the build worked OK, and the compvec program
can be used to compare results with other SGP4 implementations.

Used as basis for the paper:

"AIAA 2006-6753 Revisiting Spacetrack Report 3"

by:

David A. Vallado
Analytical Graphics, Inc., Colorado Springs, Colorado, 80920

Paul Crawford
Crawford Communications Ltd., Dundee, DD2 1EW, UK

Richard Hujsak
Analytical Graphics, Inc., Exton, PA, 19341

T. S. Kelso
Analytical Graphics, Inc., Colorado Springs, Colorado, 80920
