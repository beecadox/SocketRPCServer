/*
 * necessary imports
*/

#include "sd1.h"

/* function for calculating the average of an integer array */
float *
averagefunc_1_svc(N_array *argp, struct svc_req *rqstp)
{
	static float  result;

	printf("FUNCTION CALLED - AVERAGE\n");
	int sum = 0;
	int i;
	for(i=0; i<argp->N.N_len; i++)
		sum+=argp->N.N_val[i];  /* sum of an integer array */
		
	result = sum/argp->N.N_len; /* dividing the sum with the length of the array to find the average */

	printf("RETURNING AVERAGE.....\n");
	return &result;
}

/* function for searching the minimum and maximum value of an array */
min_max *
minmaxfunc_1_svc(N_array *argp, struct svc_req *rqstp)
{
	static min_max  result;

	printf("FUNCTION CALLED - ΜΙΝ ΑΝD ΜΑΧ\n");
	int i;
	result.minmax[0] = 99999; /* initializing minimum with a huge value */
	result.minmax[1] = -99999; /* initializing maximum with a small value */
	/* Searching for minimum and maximum */	
	for(i=0; i<argp->N.N_len; i++)
	{
		if(argp->N.N_val[i] < result.minmax[0]) /* Searching for minimum */	
			result.minmax[0] = argp->N.N_val[i];
		if(argp->N.N_val[i]>result.minmax[1]) /* Searching for maximum */	
			result.minmax[1] = argp->N.N_val[i];
	}
	printf("RETURNING MIN AND MAX OF ARRAY.....\n");
	return &result;
}

/* function for calculating a new float array by multiplying the original one with a float number */
nProd *
productfunc_1_svc(a_N_array *argp, struct svc_req *rqstp)
{
	static nProd  result;

	printf("FUNCTION CALLED - PRODUCT - a*Array\n");

	result.product.product_len=argp->N.N_len; /* length of array*/	
	result.product.product_val=(float *)malloc(argp->N.N_len*sizeof(float));
	int i;
	for(i=0; i<argp->N.N_len; i++)
		result.product.product_val[i] = argp->a*argp->N.N_val[i];
	printf("RETURNING PRODUCT.....\n");
	return &result;
}
