/* > ferror.c
 *
 *
 *	(c) Tasos Sagiotis 2013
 *	University of Patras
 *	tsangiotis (at) gmail.com
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

#include <stdarg.h>

void fatal_error(const char *format, ...)
{
va_list arg_ptr;

    fflush(stdout);

    fprintf(stderr, "\nUP-Sat fatal run-time error:\n");

    va_start(arg_ptr, format);
    vfprintf(stderr, format, arg_ptr);
    va_end(arg_ptr);

    fprintf(stderr, "\nNow terminating the program...\n");
    fflush(stderr);

    exit(5);

}

/* ===================================================================== */
