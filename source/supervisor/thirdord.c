#include "./../signals/signal_definitions_internal.h"
#include "./../signals/signal_definitions_custom.h"

#include "./../suplib/suplib.h"

#include "./supdata.h"

#include <math.h>

//#define _LOG
#ifdef _LOG
	FILE * pFile;
#endif

//void thirdord( REAL p, REAL v, REAL a, REAL j, REAL Ts, matrix *tx, matrix *xp )
void thirdord( REAL p, REAL v, REAL a, REAL j, REAL Ts, matrix *xp )
{
	#ifdef _LOG
		pFile = fopen("logthirdord.txt","w") ;
	#endif
	// Init Variables
	int i, k, l;
	REAL t1;
	REAL jd;
	REAL t2;
	REAL t3;
	matrix t;
	t.M   = 1;
	t.N   = 3;
	REAL t_t[] = {R_(0.0),R_(0.0),R_(0.0)};
	t.Mat = &t_t[0];
	matrix tconst;
	tconst.M   = 3;
	tconst.N   = 8;
	REAL t_tconst[] = {R_(0.0),R_(0.0),R_(0.0), R_(1.0),R_(0.0),R_(0.0), R_(1.0),R_(1.0),R_(0.0), R_(2.0),R_(1.0),R_(0.0), R_(2.0),R_(1.0),R_(1.0), R_(3.0),R_(1.0),R_(1.0), R_(3.0),R_(2.0),R_(1.0), R_(4.0),R_(2.0),R_(1.0)};
	tconst.Mat = &t_tconst[0];
	matrix tt;
	tt.M = t.M;
	tt.N = tconst.N;
	REAL t_tt[] = {R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0)};
	tt.Mat = &t_tt[0];
	matrix ttest;
	ttest.M = tt.M;
	ttest.N = tt.N + 1;
	REAL t_ttest[] = {R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0),R_(0.0)};
	ttest.Mat = &t_ttest[0];
	int len;
	matrix xj;
	xj.N = 1;
	matrix xa;
	xa.N = 1;
	matrix xv;
	xv.N = 1;

	#ifdef _LOG
		fprintf (pFile, "Variables Inited %d \n",0);
		fflush (pFile);
	#endif

	// % PART 1
	p = fabs(p);
	v = fabs(v);
	a = fabs(a);
	j = fabs(j);

	// % Calculation t1
    t1 = pow(p/(R_(2.0)*j),(R_(1.0)/R_(3.0))) ; // % largest t1 with bound on jerk
    t1 = ceil(t1/Ts)*Ts;
    jd = (R_(1.0)/R_(2.0))*p/(pow(t1,R_(3.0)));
	#ifdef _LOG
    	fprintf (pFile, "Position test \n");
		fprintf (pFile, "t1 %3.3f \n",t1);
		fprintf (pFile, "jd %3.3f \n",jd);
		fflush (pFile);
	#endif
    // % velocity test
	if ( v < jd*(pow(t1,R_(2.0))) )         // % v bound violated ?
	{
	   t1 = pow((v/j),(R_(1.0)/R_(2.0))) ;  // % t1 with bound on velocity not violated
	   t1 = ceil(t1/Ts)*Ts;
	   jd = v/(pow(t1,R_(2.0)));
	}
	#ifdef _LOG
		fprintf (pFile, "Velocity test \n");
		fprintf (pFile, "t1 %3.3f \n",t1);
		fprintf (pFile, "jd %3.3f \n",jd);
		fflush (pFile);
	#endif
	// % acceleration test
	if (a < jd*t1)     // % a bound violated ?
	{
	   t1 = a/j ;    // % t1 with bound on acceleration not violated
	   t1 = ceil(t1/Ts)*Ts;
	   jd = a/t1;
	}
	j = jd;  // % as t1 is now fixed, jd is the new bound on jerk

	#ifdef _LOG
		fprintf (pFile, "Acceleration test \n");
		fprintf (pFile, "t1 %3.3f \n",t1);
		fprintf (pFile, "jd %3.3f \n",jd);
		fprintf (pFile, "Calculation t1 %d \n",1);
		fflush (pFile);
	#endif

	// % Calculation t2
	t2 = pow((pow(t1,R_(2.0))/R_(4.0)+p/j/t1),(R_(1.0)/R_(2.0))) - (R_(3.0)/R_(2.0))*t1 ;   // % largest t2 with bound on acceleration
	t2 = ceil(t2/Ts)*Ts;
	jd = p/( R_(2.0)*(pow(t1,R_(3.0))) + R_(3.0)*(pow(t1,R_(2.0)))*t2 + t1*(pow(t2,R_(2.0))) );
	#ifdef _LOG
    	fprintf (pFile, "Position test \n");
		fprintf (pFile, "t2 %3.3f \n",t2);
		fprintf (pFile, "jd %3.3f \n",jd);
		fflush (pFile);
	#endif

	// % velocity test
	if ( v < (jd*pow(t1,R_(2.0)) + jd*t1*t2) )   // % v bound violated ?
	{
	   t2 = v/(j*t1) - t1 ;       // % t2 with bound on velocity not violated
	   t2 = ceil(t2/Ts)*Ts;
	   jd = v/( pow(t1,R_(2.0)) + t1*t2 );
	}
	j = jd;  // % as t2 is now fixed, jd is the new bound on jerk
	#ifdef _LOG
    	fprintf (pFile, "Velocity test \n");
		fprintf (pFile, "t2 %3.3f \n",t2);
		fprintf (pFile, "jd %3.3f \n",jd);
		fflush (pFile);
	#endif


	#ifdef _LOG
		fprintf (pFile, "Calculation t2 %d \n",2);
		fflush (pFile);
	#endif

	// % Calculation t3
	t3 = ( p - R_(2.0)*j*(pow(t1,R_(3.0))) - R_(3.0)*j*(pow(t1,R_(2.0)))*t2 - j*t1*(pow(t2,R_(2.0))) )/v ; // % t3 with bound on velocity
	t3 = ceil(t3/Ts)*Ts;
	jd = p/( R_(2.0)*(pow(t1,R_(3.0))) + R_(3.0)*(pow(t1,R_(2.0)))*t2 + t1*(pow(t2,R_(2.0))) + (pow(t1,R_(2.0)))*t3 + t1*t2*t3 );

	#ifdef _LOG
		fprintf (pFile, "Position test \n");
		fprintf (pFile, "t3 %3.3f \n",t3);
		fprintf (pFile, "jd %3.3f \n",jd);
		fprintf (pFile, "Calculation t3 %d \n",3);
		fflush (pFile);
	#endif

	// % All time intervals are now calculated
	t.Mat[0] = t1;
	t.Mat[1] = t2;
	t.Mat[2] = t3;

	#ifdef _LOG
		fprintf (pFile, "t1 = %3.3f, t2 = %3.3f and t3 = %3.3f  \n",t1,t2,t3);
		fflush (pFile);
	#endif

	mat_mult(&t, &tconst, &tt);

	#ifdef _LOG
		fprintf (pFile, "Mat Mult %d \n",0);
		fflush (pFile);
	#endif

	for ( i = 0; i < tt.N; i++ )
	{
		ttest.Mat[i] = tt.Mat[i];
	}
	ttest.Mat[tt.N] = R_(1.5)*tt.Mat[7];

	len = (int) lround(R_(1.2)*tt.Mat[7]/Ts + R_(1.0));

	#ifdef _LOG
		fprintf (pFile, "Mat[7] = %3.3f and len = %d \n",tt.Mat[7],len);
		fflush (pFile);
	#endif

	xj.M = len;
	xj.Mat = (REAL*) calloc((xj.M)*(xj.N), sizeof(REAL));
	xj.Mat[0] = jd;
	xa.M = len;
	xa.Mat = (REAL*) calloc((xa.M)*(xa.N), sizeof(REAL));
	xv.M = len;
	xv.Mat = (REAL*) calloc((xv.M)*(xv.N), sizeof(REAL));
	xp->M = len;
	xp->N = 1;
	xp->Mat = (REAL*) calloc((xp->M)*(xp->N), sizeof(REAL));
	#ifdef _LOG
		fprintf (pFile, "Memory allocated %d \n",0);
		fflush (pFile);
	#endif


	for ( k = 0; k < (len-1); k++ )
	{
		i = -1;
		for ( l = 0; l < ttest.N; l++ )
		{
			if ( Ts*((k+1) + R_(0.5)) <= ttest.Mat[l] )
			{
				i = l - 1;
				break;
			}
		}
		if ( i == 0 || i == 6 )
		{
			xj.Mat[k+1] = jd;
		}
		else if ( i == 2 || i == 4 )
		{
			xj.Mat[k+1] = -jd;
		}
		else
		{
			xj.Mat[k+1] = 0;
		}
		xa.Mat[k+1] = xa.Mat[k] + xj.Mat[k]*Ts;
		xv.Mat[k+1] = xv.Mat[k] + xa.Mat[k]*Ts;
		xp->Mat[k+1] = xp->Mat[k] + xv.Mat[k]*Ts;
	}

	#ifdef _LOG
		fprintf (pFile, "Setpoint done %d \n",0);
		fflush (pFile);
	#endif

	free(xj.Mat);
	free(xa.Mat);
	free(xv.Mat);
	// ATTENTION: memory for tx and xp is allocated here, but must be freed in the calling function !!!

	#ifdef _LOG
		fprintf (pFile, "Memory released %d \n",0);
		fflush (pFile);
	#endif

}
