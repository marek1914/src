#include "fftw3.h"
#include <stdio.h>
#define N 10

main()
{
	int i;
	fftw_complex *in, *out;
	fftw_plan p;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	in[0][0]=1;
	in[1][0]=2;
	in[2][0]=3;
	in[3][0]=1;
	in[4][0]=1;
	in[5][0]=2;
	in[6][0]=1;
	in[7][0]=1;
	in[8][0]=1;
	in[9][0]=9;

	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	/* repeat as needed */
	fftw_execute(p); 
	fftw_destroy_plan(p);
	for(i=0; i<10; i++)
		printf("%f+%fi ", out[i][0], out[i][1]);
	printf("\n");
	fftw_free(in); 
	fftw_free(out);
}

