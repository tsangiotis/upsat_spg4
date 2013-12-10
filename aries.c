/* > aries.c
 *
 *	(c) Paul Crawford & Andrew Brooks 1994-2010
 *	University of Dundee
 *	psc (at) sat.dundee.ac.uk
 *	arb (at) sat.dundee.ac.uk
 *
 *	Released under the terms of the GNU LGPL V3
 *	http://www.gnu.org/licenses/lgpl-3.0.html
 *	
 *	This software is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 */

#include "sgdp4h.h"

/* ====================================================================== */

#define C1		(1.72027916940703639E-2)
#define C1P2P	(C1 + TWOPI)
#define THGR70	(1.7321343856509374)
#define FK5R	(5.07551419432269442E-15)

double gha_aries(double jd)
{
/* Method from SGP4SUB.F code. */
	double ts70, ds70, trfac, theta, days50;
	long ids70;

	days50 = jd - 2400000.5 - 33281.0;

    ts70 = days50 - 7305.0;
    ids70 = (long)(ts70+1.0e-8);
    ds70 = ids70;

    trfac = ts70 - ds70;

	/* CALCULATE GREENWICH LOCATION AT EPOCH */
	theta = THGR70 + C1*ds70 + C1P2P*trfac + ts70*ts70*FK5R;
	theta = fmod(theta, TWOPI);
    if (theta < 0.0) theta += TWOPI;

return theta;
}

/* ===================================================================== */
