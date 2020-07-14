/*
 * necessary imports
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

/*  error messages */
void error(char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	
    if (argc < 3)
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) 
    {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    printf("Trying to connect...\n");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) error("ERROR connecting");

    printf("CONNECTED....\n");
    int clnt_choice, t, N, i, choice_flag=0;
    int *temp;
    float a;
    float results1;
    int results2[2];
    float *results3;

    /* user interface */
    do
    {
        printf("------ CHOICES MENU ------\n");
        printf("--------------------------\n");
        printf("PRESS 1 FOR AVERAGE OF ARRAY\n");
        printf("PRESS 2 FOR MINIMUM AND MAXIMUM OF ARRAY\n");
        printf("PRESS 3 TO MULTIPLY ARRAY BY A NUMBER\n");
        printf("PRESS 0 TO EXIT\n");
        printf("--------------------------\n");
        printf("Choice: ");
        scanf("%d", &clnt_choice); /* client choice */
        printf("--------------------------\n\n");

        t = send(sockfd, &clnt_choice, sizeof(int),0);
        if (t < 0) error("ERROR sending to socket server");

        if(clnt_choice == 1 || clnt_choice == 2 || clnt_choice == 3 )
        {
            /* if client chooses on of the three options for calculation */
            printf("PROVIDE SIZE OF ARRAY: ");
            scanf("%d", &N); /* client provides an array length */
            t = send(sockfd, &N, sizeof(int), 0);

            if (t < 0) error("ERROR sending to socket server");

            temp = (int *)malloc(N*sizeof(int));

            for(i=0; i<N; i++)
            {
                printf("A[%d] = ", i);
                scanf("%d", temp+i);
            } /* client provides the values of the array */
            t = send(sockfd, temp, N*sizeof(int),0);
            if (t < 0) error("ERROR sending to socket server");

            switch (clnt_choice){
                case 1: /* choice 1 - average of array */
                    t = recv(sockfd, &results1, sizeof(float), 0);
                    if (t < 0) error("ERROR receiving from socket");
                    printf("---\n");
                    printf("Average of array is %.3f\n", results1);
                    printf("---\n");

                    break;
                case 2: /* choice 2 - minimum and maximum value of an array */
                    t = recv(sockfd, results2, 2*sizeof(int), 0);
                    if (t < 0) error("ERROR receiving from socket");

                    printf("---\n");
                    printf("Min of array is %d\n", results2[0]);
                    printf("Max of array %d\n", results2[1]);
                    printf("---\n");

                    break;
                case 3: /* choice 3 - original array multiplied by a float number a */
                    results3 =  malloc(N*sizeof(float));

                    printf("Provide a float number: ");
                    scanf("%f", &a);
                    t = send(sockfd, &a, sizeof(float), 0);
                    if (t < 0) error("ERROR writing to socket");

                    t = recv(sockfd, results3, N*sizeof(float), 0);
                    if (t < 0) error("ERROR receiving from socket");

                    printf("---\n");
                    printf("New array values multiplied by %f\n", a);
                    printf("---\n");
                    for(i=0; i<N; i++)
                        printf("A[%d]*%.2f == %.2f\n", i, a, *(results3+i));
                    printf("---\n");
                    break;
            }
        }
        else if(clnt_choice == 0 )
        {
            choice_flag = 1;                  /* terminate if choice is 0 */
            printf("DISCONNECTING....\n");
        }
        else
        {
            printf("TRY AGAIN! INVALID CHOICE!\n"); /* in case of an invalid choice - try again*/
        }
    }while(!choice_flag);

    close(sockfd);

    return 0;
}
