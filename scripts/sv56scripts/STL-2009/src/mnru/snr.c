/* General symbols */
#define get_dB(x) ((x)==0?-100:10*log10((x)))
#define get_sqr(x) ((x)*(x))

#define SNR_RESET	 1
#define SNR_MEASURE	 0
#define SNR_STOP	-1
#define SNR_DEALLOC	-2

/* Data structure */
typedef struct
{
  long            smpno, tot_smpno, count;
  float          *snr_vector, *sqr_vector;
  double          total_err, total_snr_dB, total_sqr_dB, max_sqr, min_sqr, avg_sqr,
                  avg_snr, var_sqr, var_snr, max_snr, min_snr, total_sqr,
                  thr, sqr, err;

}               SNR_state;

/* Prototypes */
float get_SNR ARGS ((int mode, short *a, short *b, long N, long N2, 
		     SNR_state *s));
int get_min_max ARGS ((float *x, int n, double *min, double *max));


/* ---------------------- Begin of get_SNR ------------------------------*/
float           get_SNR(mode, a, b, N, N2, s)
  char            mode;
  short          *a, *b;
  long            N, N2;
  SNR_state      *s;
{
  double          tmp;
  int             i, j;

/*
 *  DEALLOCATION OF MEMORY FOR SNR VALUES
 */

  if (mode == SNR_DEALLOC)
  {
    free(s->snr_vector);
    free(s->sqr_vector);
    return (-1000);
  }
  else
  {
    /* RESET STATE VARIABLES, IF REQUESTED */

    if (mode == SNR_RESET)
    {
      /* Allocate memory for SNR vector */
      if ((s->snr_vector = (float *) calloc(N2, sizeof(float))) == NULL)
      {
	fprintf(stderr, "Error allocating memory for SNR vector\n");
	return (-1001);
      }

      /* Allocate memory for squared samples' vector */
      if ((s->sqr_vector = (float *) calloc(N2, sizeof(float))) == NULL)
      {
	fprintf(stderr, "Error allocating memory for sqr vector\n");
	return (-1002);
      }
      s->count = 0;
      s->max_sqr = -1000;
      s->total_sqr = 0;
      s->thr = 50;
      s->smpno = 0;
      s->tot_smpno = 0;
      s->total_err = 0;
      s->total_sqr = 0;
    }


    /* MEASUREMENT OF INTERMEDIATE VALUES FOR POWER AND SNR */

    for (s->err = s->sqr = 0, s->smpno = 0, j = 0; j < N; j++)
    {
      s->sqr += (double) get_sqr(a[j]);
      s->err += (double) get_sqr(a[j] - b[j]);
      s->smpno++;
    }

    s->sqr /= s->smpno;
    s->err /= s->smpno;
    s->tot_smpno += s->smpno;

    /* Limit is -100 dB */
    if (s->sqr == 0)
      s->sqr = 10E-10;
    if (s->err == 0)
      s->err = 10E-10;

    s->snr_vector[s->count] = (float) get_dB(s->sqr / s->err);
    s->sqr_vector[s->count] = (float) get_dB(s->sqr);

    s->total_sqr += s->sqr;
    s->total_err += s->err;

    if (s->max_sqr < s->sqr_vector[s->count])
      s->max_sqr = s->sqr_vector[s->count];

    s->count++;


    /* MEASUREMENT OF FINAL VALUES FOR POWER AND SNR */

    if (mode == SNR_STOP)
    {
      /* Calculate the minimum value of the power to be considered */
      s->min_sqr = (s->max_sqr - s->thr) < 0.1 ? 0.1 : (s->max_sqr - s->thr);

      /* Calculate statistics */
      s->avg_sqr = s->avg_snr = s->var_sqr = s->var_snr = tmp = 0;
      for (i = 0; i < N2; i++)
      {
	if (s->sqr_vector[i] > (float) s->min_sqr)
	{
	  tmp++;		/* "active" blocks counter */
	  s->avg_sqr += s->sqr_vector[i];
	  s->avg_snr += s->snr_vector[i];
	  s->var_sqr += s->sqr_vector[i] * s->sqr_vector[i];
	  s->var_snr += s->snr_vector[i] * s->snr_vector[i];
	}
      }
      s->avg_sqr /= tmp;
      s->avg_snr /= tmp;
      s->var_sqr = sqrt(s->var_sqr / tmp - s->avg_sqr * s->avg_sqr);
      s->var_snr = sqrt(s->var_snr / tmp - s->avg_snr * s->avg_snr);

      get_min_max(s->sqr_vector, N2, &s->min_sqr, &s->max_sqr);
      get_min_max(s->snr_vector, N2, &s->min_snr, &s->max_snr);

      /* Output of results */
      s->total_sqr_dB = get_dB(s->total_sqr);
      s->total_snr_dB = (s->total_err == 0) ?
	100 /* dB */ :
	get_dB(s->total_sqr / s->total_err);
      s->count = tmp;
    }

    return (s->total_snr_dB);
  }
}
/* ---------------------- End of get_SNR ------------------------------*/

/* ---------------------- Begin of get_min_max ------------------------------*/
int             get_min_max(x, n, min, max)
  float          *x;
  double         *min, *max;
  int             n;
{
  int             i;

  *min = 1E+38;
  *max = -1E+38;

  for (i = 0; i < n; i++)
  {
    if (x[i] > *max)
      *max = x[i];
    else if (x[i] < *min)
      *min = x[i];
  }
  return (1);
}
/* ---------------------- End of get_min_max ------------------------------*/


