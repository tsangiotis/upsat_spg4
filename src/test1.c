static const char SCCSid[] = "@(#)test.c        1.00 (C) 2013  psc sgdp4 demo - main file";

#include "sgdp4h.h"
// #include <getopt.h>
/**
 * Created by tsangiotis on 10/12/13.
 */

#include <unistd.h>

int getopt(int argc, char * const argv[], const char *optstring);
extern char *optarg;
extern int optind, opterr, optopt;

long Isat=0;

extern double SGDP4_jd0;    /* From sgdp4.c */
int Verbose = 1;

/* ===================================================================== */

double Vmag(xyz_t p)
{
return sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
}

/* ===================================================================== */

double Vmag2(xyz_t p)
{
return p.x*p.x + p.y*p.y + p.z*p.z;
}

/* ===================================================================== */

xyz_t Vsub(xyz_t v, xyz_t u)
{
    xyz_t diff;
    diff.x = v.x - u.x;
    diff.y = v.y - u.y;
    diff.z = v.z - u.z;
    return diff;
}

/* ===================================================================== */

double vdiff2(xyz_t v, xyz_t u)
{
	return Vmag2(Vsub(v, u));
}

/* ===================================================================== */

#define GETOPT(c, options) c = optind = 1; while (c && ((c=getopt(argc,argv,options)) != -1)) switch(c)

char *options = "c:i:s:v:";

char *usage =
"Usage is:\n%s [options]\nWith options from:\n"
"-c ccc  control file name (default = ssd.txt)\n"
"-i ccc  input TLE file name (default = twoline.txt)\n"
"-s N    satellite number to search for (default = 0 for all in TLE list)\n"
"-v N    verbosity\n"
;


int main(int argc, char *argv[])
{
orbit_t orb;
xyz_t pos, vel;
double jd, tsince;
long satno=0;
int imode;

char filename[ST_SIZE] = "twoline.txt";
FILE *fp=NULL;

char ctrl_name[ST_SIZE] = "ssd.txt";

long ii,imax=0,iend=0;

/* Data for the prediction type and time period */
double ts = 0.0, delta=0.0;    /* Time since TLE epoch to start predictions */
int bad=1;
int c;
time_t tnow;

	time(&tnow);
	fprintf(stderr, "# Start time = %s", ctime(&tnow));

    GETOPT(c, options)
        {
		case 'c': strncpy(ctrl_name, optarg, ST_SIZE-1); break;
        case 'i': strncpy(filename, optarg, ST_SIZE-1); break;
        case 's': satno = atol(optarg); break;
        case 'v': Verbose = atoi(optarg); break;

        default:
            fprintf(stderr, usage, argv[0]);
            exit(1);
            break;
        }

	if((fp=fopen(ctrl_name, "rb")) != NULL)
		{
		if(fscanf(fp, " %lf %lf %ld %ld %ld", &ts, &delta, &imax, &iend, &satno) == 5)
			{
			bad=0;
			}
		fclose(fp);
		}


	if(bad)
		{
		fprintf(stderr, "Failed to read ssd.txt file for 'tstart,delta,imax,isat'\n");
		return 1;
		}

	fp = fopen(filename, "rb");
    if(fp == NULL)
        {
        fatal_error("File open failed for reading \"%s\"", filename);
        }


    while(read_twoline(fp, satno, &orb) == 0)
        {
		print_orb(&orb);

		Isat = orb.satno;
		imode = init_sgdp4(&orb);

		switch(imode)
			{
			case SGDP4_ERROR     :  printf("# SGDP error\n"); break;
			case SGDP4_NOT_INIT  :  printf("# SGDP not init\n"); break;
			case SGDP4_ZERO_ECC  :  printf("# SGDP zero ecc\n"); break;
			case SGDP4_NEAR_SIMP :  printf("# SGP4 simple\n"); break;
			case SGDP4_NEAR_NORM :  printf("# SGP4 normal\n"); break;
			case SGDP4_DEEP_NORM :  printf("# SDP4 normal\n"); break;
			case SGDP4_DEEP_RESN :  printf("# SDP4 resonant\n"); break;
			case SGDP4_DEEP_SYNC :  printf("# SDP4 synchronous\n"); break;
			default: 				printf("# SGDP mode not recognised!\n");
			}

		if(imode == SGDP4_ERROR) continue;


		printf("# Satellite number\n");
		printf("%05ld\n", (long)orb.satno);

		printf("#     Tsince         X                Y                Z            Xdot             Ydot             Zdot\n");


		/* Run test like the original SGP doccument */
		for(ii=0; ii <= imax; ii++)
			{
			if(ii != iend)
				{
				tsince=ts + ii*delta;
				}
			else
				{
				tsince=ts + delta;
				}

			jd = SGDP4_jd0 + tsince / 1440.0;
			if(satpos_xyz(jd, &pos, &vel) == SGDP4_ERROR) break;

			printf("%12.4f   %16.8f %16.8f %16.8f %16.12f %16.12f %16.12f\n",
				tsince,
				pos.x, pos.y, pos.z,
				vel.x, vel.y, vel.z);
			}
        }

	time(&tnow);
	fprintf(stderr, "# End time = %s", ctime(&tnow));

exit(0);
return 0;
}
