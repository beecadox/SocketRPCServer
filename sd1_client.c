/*
 ΚΖΕΣΝΙΑΚ ΜΑΓΚΝΤΑΛΕΝΑ - ΙΖΑΜΠΕΛΑ
 ΑΜ : 161044 , ΤΜΗΜΑ : ΣΤ5
 */

#include "sd1.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <pthread.h>
#include <unistd.h>

/*struct που αποτελείται από το hostname και το socketnumber έτσι ώστε να μπορούμε
μέσα στην συνάρτηση που καλεί το thread να έχουμε και την socket επικοινωνία και την
rpc επικοινωνία*/
struct socketInfo{
    char *host_name;
    int sockfd;
};

// συνάρτηση για error messages
void error(char *msg)
{
    perror(msg);
    exit(1);
}

// εδώ βλέπουμε το πως θα λειτουργήσει και ως client και ως server
void *choose_function (void *arg)
{
    struct socketInfo *temporary = (struct socketInfo *) arg;
    int so = temporary->sockfd; //socket number- για επικοινωνία με Socket Client
    char *host = temporary->host_name; // host name - για επικοινωνία με RPC Server
    
	CLIENT *clnt;

	float  *result_1;
	N_array  averagefunc_1_arg;

	min_max  *result_2;
	N_array  minmaxfunc_1_arg;

	nProd  *result_3;
	a_N_array  productfunc_1_arg;

	#ifndef	DEBUG
		clnt = clnt_create (host, FUNC_PROG, FUNC_VERS, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
	#endif	/* DEBUG */

    int clnt_choice;
    int N;
    int i, t;
    int *temp;
    float *product;
    int minmax[2];
	int choice_flag=0;
	float a;

    printf("Connected.\n");
    do {
        t = recv(so,&clnt_choice,sizeof(int), 0);
        if (t < 0) 
            error("ERROR receiving from socket client");
		/*αν η επιλογή που στάλθηκε είναι ένας από τους 3 υπολογισμούς*/
        if(clnt_choice == 1 || clnt_choice == 2 || clnt_choice == 3 )
        {
            t = recv(so,&N,sizeof(int),0);
            if (t < 0) 
                error("ERROR receiving from socket client");

            temp =(int *)malloc(N*sizeof(int));
            t = recv(so, temp, N*sizeof(int), 0);
            if (t < 0) 
                error("ERROR receiving from socket client");

            switch (clnt_choice){
                case 1: /*επιλογή 1: μέση τιμή διανύσματος*/
					averagefunc_1_arg.N.N_len=N;
					averagefunc_1_arg.N.N_val=(int *) malloc(N*sizeof(int));
					averagefunc_1_arg.N.N_val=temp;
					result_1 = averagefunc_1(&averagefunc_1_arg, clnt);
                    send(so, result_1, sizeof(float), 0);
                    break;
                case 2: /*επιλογή 2: μέγιστη & ελάχιστη τιμή διανύσματος*/
					minmaxfunc_1_arg.N.N_len=N;
					minmaxfunc_1_arg.N.N_val=(int *) malloc(N*sizeof(int));
					minmaxfunc_1_arg.N.N_val=temp;
					result_2 = minmaxfunc_1(&minmaxfunc_1_arg, clnt);
					minmax[0] = result_2->minmax[0];
					minmax[1] = result_2->minmax[1];
                    send(so, minmax, 2*sizeof(int), 0);
                    break;
                case 3: /*επιλογή 3: γινόμενο α*διάνυσμα */
					productfunc_1_arg.N.N_len=N;
					productfunc_1_arg.N.N_val=(int *) malloc(N*sizeof(int));
					productfunc_1_arg.N.N_val=temp;

                    t = recv(so, &a, sizeof(float), 0);
                    if (t < 0) 
                         error("ERROR receiving from socket client");
					productfunc_1_arg.a=a;
					result_3 = productfunc_1(&productfunc_1_arg, clnt);
					product = malloc(N*sizeof(float));
					for(i=0;i<N;i++)
						product[i] = result_3->product.product_val[i];
                    send(so, product, N*sizeof(float), 0);
                    break;
            }      
        }
        else if(clnt_choice == 0)
        {
            choice_flag=1;	/*αν η επιλογή είναι 0 ο client έκανε τερματισμό*/

            printf("CLIENT DISCONNECTING....\n");
        }
        else
        {
            printf("CLIENT'S INVALID CHOICE..WAITING FOR NEW CHOICE...\n");
        }	
    }while(!choice_flag);

	#ifndef	DEBUG
		clnt_destroy (clnt);
	#endif	 /* DEBUG */

    close(so);
    pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	char *host;
    int sockfd, newsockfd, portno, clilen, i;
    struct sockaddr_in serv_addr, cli_addr;
    pthread_t thread[50];
	struct socketInfo info;

	if (argc < 3) {
		printf ("usage: %s server_host port\n", argv[0]); /*όταν τρέχουμε την sd1_client πρέπει 
															να δώσουμε 2 ορίσματα : hostname port*/
		exit (1);
	}

    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd < 0) 
        error("ERROR opening socket");

    bzero((char *) &serv_addr, sizeof(serv_addr));

    host = argv[1]; //hostname

    portno = atoi(argv[2]); //portnumber

    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(portno);

    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd,5);

    i=0;

    for (;;)  
    {
        printf("Waiting for a connection...\n");
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        info.host_name = host; 
        info.sockfd = newsockfd;
        if (newsockfd < 0) 
            error("ERROR on accept");
		// χρήση του struct σαν όρισμα στην choose_function
        pthread_create(&(thread[i++]), NULL, choose_function, (void *)&info);
    }      
    return 0; 
}