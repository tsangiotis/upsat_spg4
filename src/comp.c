/* > comp.c
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
 *
 *
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef struct xyz_s {
	double x, y, z;
} xyz_t;

void fcompare(FILE *fp1, FILE *fp2);
int read_line(FILE *fp, char *buf);
int read_t_pos_vel(char *buf, double *t, xyz_t *pos, xyz_t *vel);
double diff_xyz(xyz_t *a, xyz_t *b);
double abs_xyz(const xyz_t *v);
int read_sat(char *buf, long *s, double *period, double *perigee, double *apogee, int *imode);
double angle_diff_xyz(xyz_t *a, xyz_t *b);

static double DSQR(double x) { return x*x; }

#define ST_SIZE 256
#define EQRAD 6378.135
#define PI 3.141592653589793
#define TWOPI (2.0*PI)
#define DEG(x) ((x)*180.0/PI)

int Satcount=0;
double AveP=0.0;
double AveR=0.0;
double MaxP=0.0;

int Verbose = 0;

FILE *sum_fp = NULL;

/* ===================================================================== */

int main(int argc, char *argv[])
{
char *fname1=NULL, *fname2=NULL;
FILE *fp1=NULL, *fp2=NULL;

    if(argc == 4 || argc == 5)
        {
		fname1 = argv[1];
		fname2 = argv[2];
		Verbose = atoi(argv[3]);

		if(argc == 5)
			{
			sum_fp = fopen(argv[4], "wb");
			if(sum_fp == NULL)
				{
				printf("Failed to open %s (%s)\n", argv[4], strerror(errno));
				exit(1);
				}
			}
        }
    else
        {
        fprintf(stderr, "Usage is:\n%s <filename1> <filename2> <verbosity> [summary_filename]\n", argv[0]);
        exit(1);
        }

	fp1 = fopen(fname1, "rb");
	fp2 = fopen(fname2, "rb");

    if(fp1 == NULL || fp2 == NULL)
        {
        printf("File open failed\n");
        exit(1);
        }

	fcompare(fp1, fp2);

	printf("\nFor all satellites, Average position error = %.3g (max = %.3g, relative = %.3g)\n",
		Satcount? AveP/Satcount : 0.0,
		MaxP,
		Satcount? AveR/Satcount : 0.0);

	if(MaxP > 1.0) fprintf(stderr, "Warning, errors bigger than 1km\n");

	if(sum_fp != NULL) fclose(sum_fp);

exit(0);
return 0;
}

/* ===================================================================== */

void fcompare(FILE *fp1, FILE *fp2)
{
char buf1[ST_SIZE], buf2[ST_SIZE];
double t=0.0;
xyz_t pos1, pos2, vel1, vel2;
long s1=0, s2=0;
int scount=0;
double saver=0.0, vaver=0.0, raver=0.0;
double smax=0.0, vmax=0.0, rmax=0.0;
int ibad1=0, ibad2=0, iend=0;
double period=0.0, perigee=0.0, apogee=0.0;
int imode=0;

	do
		{
		ibad1 = read_line(fp1, buf1);
		ibad2 = read_line(fp2, buf2);

		if(ibad1 == 0 && ibad2 == 0)
			{
			ibad1 = read_t_pos_vel(buf1, &t, &pos1, &vel1);
			ibad2 = read_t_pos_vel(buf2, &t, &pos2, &vel2);
			iend = 0;
			}
		else
			{
			ibad1 = ibad2 = iend = 2;
			}

		if(ibad1 == 0 && ibad2 == 0)
			{
			double dp = diff_xyz(&pos1, &pos2);
			double dv = diff_xyz(&vel1, &vel2);
			double r1, r2, dr;
			/*double dh = angle_diff_xyz(&pos1, &pos2);*/

			iend = 0;

			r1 = abs_xyz(&pos1);
			r2 = abs_xyz(&pos2);
			dr = dp/(0.5*(r1+r2));

			if(Verbose) printf("%14.3f %12.6f     %12.10f    %12.3f %12.6f\n", t, dp, dv, r2-EQRAD, r1-r2);

			/* Per-satellite local figures. */
			saver += dp;
			if(dp > smax) smax = dp;

			vaver += dv;
			if(dv > vmax) vmax = dv;

			raver += dr;
			if(dr > rmax) rmax = dr;

			scount++;

			/* Global figures. */
			Satcount++;
			AveP += dp;
			AveR += dr;
			if(MaxP < dp) MaxP = dp;
			}
		else
			{
			/* One or both sets of data has ended. Read any of the
			 * non-finished ones until it fails, then we should have
			 * re-synced for the next satellite.
			 */
			iend++;
			if(!ibad2)
				{
					do{
						if(read_line(fp2, buf2))
							{
							iend = 2;
							break;
							}
						ibad2 = read_t_pos_vel(buf2, &t, &pos2, &vel2);
					}while(!ibad2);
				}

			if(!ibad1)
				{
					do{
						if(read_line(fp1, buf1))
							{
							iend = 2;
							break;
							}
						ibad1 = read_t_pos_vel(buf1, &t, &pos1, &vel1);
					}while(!ibad1);

				}
			}

		if(iend)
			{
			/* Not a set of XYZ position & velocity. */
			if(scount)
				{
				/* Already done some work, print results. */
				printf("Figures for sat %05ld are:\n", s1);
				printf("Average Position error = %.3e km     (max = %.3e)\n", saver /= scount, smax);
				printf("Average Velocity error = %.3e km/sec (max = %.3e)\n", vaver /= scount, vmax);
				printf("Relative position error = %.3e       (max = %.3e)\n", raver /= scount, rmax);

				if(sum_fp != NULL)
					{
					fprintf(sum_fp, "%05ld %12.3f %12.3f %12.3f %2d %12.3e %12.3e\n",
								s1, period, perigee, apogee, imode, saver, smax);
					}
				}

			period = perigee = apogee = 0.0;
			imode = 0;
			scount=0;
			raver=rmax=vaver=vmax=smax=saver=0.0;

			if(iend < 2)
				{
				ibad1 = read_sat(buf1, &s1, &period, &perigee, &apogee, &imode);
				ibad2 = read_sat(buf2, &s2, &period, &perigee, &apogee, &imode);

				if(ibad1 == 0 && ibad2 == 0)
					{
					if(s1 != s2)
						{
						printf("Non-matching satellites (%ld != %ld)!\n", s1, s2);
						printf("%s\n", buf1);
						printf("%s\n", buf2);
						return;
						}

					if(Verbose) printf("      Time       P error (km)   V error (km/sec)     Altitude    Delta-R\n");
					}
				else if(ibad1 && ibad2)
					{
					printf("\nEnd of both files/data now found\n");
					return;
					}
				else
					{
					printf("\nCompare failed on:\n%s\n%s\n", buf1, buf2);
					return;
					}
				}
			}
		} while(iend < 2);

}

/* ===================================================================== */

int read_line(FILE *fp, char *buf)
{
int ii;

	while(fgets(buf, ST_SIZE-1, fp) != NULL)
		{
		/* Look for comment or end characters, delete text beyond them. */
		for(ii=0; ii<ST_SIZE; ii++)
			{
			if(buf[ii] == 0   ||
			   buf[ii] == '#' ||
			   buf[ii] == '\n'||
			   buf[ii] == '\r')
				{
				buf[ii] = 0;
				break;
				}
			}

		for(ii=0; ii<ST_SIZE; ii++)
			{
			if(buf[ii] == 0) break;
			if(isalnum(buf[ii])) return 0;
			}
		}

	/*printf("FAILED: \"%s\"\n", buf);*/

return -1;
}

/* ===================================================================== */

int read_t_pos_vel(char *buf, double *t, xyz_t *pos, xyz_t *vel)
{
int nread;

	*t = 0.0;

	nread = sscanf(buf, " %lf %lf %lf %lf %lf %lf %lf",
	     			t,
	     			&pos->x, &pos->y, &pos->z,
	     			&vel->x, &vel->y, &vel->z);
	if(nread == 7)
		{
		return 0;
		}

	if(nread == 6)
		{
		sscanf(buf, " %lf %lf %lf %lf %lf %lf",
	     			&pos->x, &pos->y, &pos->z,
	     			&vel->x, &vel->y, &vel->z);
		return 0;
		}

	pos->x = pos->y = pos->z = 0.0;
	vel->x = vel->y = vel->z = 0.0;

return 1;
}


/* ===================================================================== */

double diff_xyz(xyz_t *a, xyz_t *b)
{
	/* Quick'n'dirty - beware of overflow/underflow! */
	return sqrt(DSQR(a->x - b->x) + DSQR(a->y - b->y) + DSQR(a->z - b->z));
}

/* ===================================================================== */

double abs_xyz(const xyz_t *v)
{
return sqrt(DSQR(v->x)+DSQR(v->y)+DSQR(v->z));
}

/* ===================================================================== */

int read_sat(char *buf, long *s, double *period, double *perigee, double *apogee, int *imode)
{
int inum;

	inum = sscanf(buf, " %ld %lf %lf %lf %d", s, period, perigee, apogee, imode);

	if(inum == 5)
		{
		return 0;
		}

	/*
	*period = (*perigee) = (*apogee) = 0.0;
	*imode = 0;
	*/

	if(inum == 1)
		{
		return 0;
		}

	*s = 0;

return 1;
}

double angle_diff_xyz(xyz_t *a, xyz_t *b)
{
double ha1, ha2, diff;

	ha1 = atan2(a->x, a->y);
	ha2 = atan2(b->x, b->y);

	diff = ha1 - ha2;
	if(diff > PI) diff -= TWOPI;
	if(diff < -PI) diff += TWOPI;

return DEG(diff);
}
