struct N_array
{
	int N <>;
};

struct min_max
{
	int minmax[2];
};

struct a_N_array
{
	float a;
	int N <>;	
};

struct nProd
{
	float product <>;
};

program FUNC_PROG
{
	version FUNC_VERS
	{
		float averagefunc(N_array) = 1;
		min_max minmaxfunc(N_array) = 2;
		nProd productfunc(a_N_array) = 3;
	} = 1;
} = 0x23451111;
