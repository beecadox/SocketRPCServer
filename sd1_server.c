#include "sd1.h"

float *
averagefunc_1_svc(N_array *argp, struct svc_req *rqstp)
{
	static float  result;

	printf("FUNCTION CALLED - AVERAGE\n");
	int sum=0;
	int i;
	for(i=0; i<argp->N.N_len; i++)
		sum+=argp->N.N_val[i];  /*άθροισμα των στοιχείων του πίνακα*/
		
	result = sum/argp->N.N_len; /*βρίσκουμε Μ.Ο διαιρόντας το άθροισμα με το πλήθος*/

	printf("RETURNING AVERAGE.....\n");
	return &result;
}

min_max *
minmaxfunc_1_svc(N_array *argp, struct svc_req *rqstp)
{
	static min_max  result;

	printf("FUNCTION CALLED - ΜΙΝ ΑΝD ΜΑΧ\n");
	int i;
	result.minmax[0] = 99999; /*αρχικοποίηση ελάχιστου με μια μεγάλη θετική τιμή*/
	result.minmax[1] = -99999; /*αρχικοποίηση μέγιστου με μια μεγάλη αρνητική τιμή*/
	/*Εύρεση ελάχιστης & μέγιστης τιμής του πίνακα*/	
	for(i=0; i<argp->N.N_len; i++)
	{
		if(argp->N.N_val[i] < result.minmax[0]) //εύρεση ελάχιστης
			result.minmax[0] = argp->N.N_val[i];
		if(argp->N.N_val[i]>result.minmax[1]) //εύρεση μέγιστης
			result.minmax[1] = argp->N.N_val[i];
	}
	printf("RETURNING MIN AND MAX OF ARRAY.....\n");
	return &result;
}

nProd *
productfunc_1_svc(a_N_array *argp, struct svc_req *rqstp)
{
	static nProd  result;

	printf("FUNCTION CALLED - PRODUCT - a*Array\n");

	result.product.product_len=argp->N.N_len; //πλήθος στοιχείων πίνακα
	result.product.product_val=(float *)malloc(argp->N.N_len*sizeof(float));
	int i;
	for(i=0; i<argp->N.N_len; i++)
		result.product.product_val[i] = argp->a*argp->N.N_val[i];
	printf("RETURNING PRODUCT.....\n");
	return &result;
}
