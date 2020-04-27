/*
 ΚΖΕΣΝΙΑΚ ΜΑΓΚΝΤΑΛΕΝΑ - ΙΖΑΜΠΕΛΑ
 ΑΜ : 161044 , ΤΜΗΜΑ : ΣΤ5
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

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
	
    if (argc < 3) {
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

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

        printf("CONNECTED....\n");
        int clnt_choice, t, N, i, choice_flag=0;
        int *temp;
        float a;
        float results1;
        int results2[2];
        float *results3;
        /*διεπαφή χρήστη*/
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
            scanf("%d", &clnt_choice); /*επιλογή υπολογισμού - εξόδου*/
            printf("--------------------------\n\n");
            t = send(sockfd, &clnt_choice, sizeof(int),0);
            if (t < 0) 
                error("ERROR sending to socket server"); 
            if(clnt_choice == 1 || clnt_choice == 2 || clnt_choice == 3 )
            {
                /*αν η επιλογή του είναι ένας από τους 3 υπολογισμούς*/
                printf("PROVIDE SIZE OF ARRAY: ");
                scanf("%d", &N); /*ο client δίνει το μέγεθος του πίνακα*/
                t = send(sockfd, &N, sizeof(int), 0);

                if (t < 0) 
                    error("ERROR sending to socket server");

                temp =(int *)malloc(N*sizeof(int));

                for(i=0; i<N; i++)
                {
                    printf("A[%d] = ", i);
                    scanf("%d", temp+i);	
                } /*ο client δίνει τα στοιχεία του πίνακα*/
                t = send(sockfd, temp, N*sizeof(int),0);
                if (t < 0) 
                    error("ERROR sending to socket server");

                switch (clnt_choice){
                    case 1: /*επιλογή 1: μέση τιμή διανύσματος*/
                        t = recv(sockfd, &results1, sizeof(float), 0);
                        if (t < 0) 
                            error("ERROR receiving from socket");
                        printf("---\n");
                        printf("Average of array is %.3f\n", results1);
                        printf("---\n");
                        
                        break;
                    case 2: /*επιλογή 2: μέγιστη & ελάχιστη τιμή διανύσματος*/
                        t = recv(sockfd, results2, 2*sizeof(int), 0);
                        if (t < 0) 
                            error("ERROR receiving from socket");

                        printf("---\n");
                        printf("Min of array is %d\n", results2[0]);
                        printf("Max of array %d\n", results2[1]);
                        printf("---\n");
                        
                        break;
                    case 3: /*επιλογή 3: γινόμενο α*διάνυσμα */
                        results3 =  malloc(N*sizeof(float));

                        printf("Provide a float number: ");
                        scanf("%f", &a);
                        t = send(sockfd, &a, sizeof(float), 0);
                        if (t < 0) 
                            error("ERROR writing to socket");

                        t = recv(sockfd, results3, N*sizeof(float),0);
                        if (t < 0) 
                            error("ERROR receiving from socket");

                        printf("---\n");
                        printf("New array values multiplied by %f\n", a);
                        printf("---\n");
                        for(i=0;i<N;i++)
                            printf("A[%d]*%.2f == %.2f\n", i, a, *(results3+i));
                        printf("---\n");
                        break;
                }       
            }
            else if(clnt_choice == 0 )
            {
                choice_flag=1;                  /*αν η επιλογή είναι 0 τερμάτησε*/
                printf("DISCONNECTING....\n");
            }
            else
            {   
                printf("TRY AGAIN! INVALID CHOICE!\n"); /*αν η επιλογή του δεν είναι ούτε 
                                                        1,2,3 αλλά ούτε 0 τότε ξαναζήτησε μια επιλογή*/
            }	
        }while(!choice_flag);

    close(sockfd);

    return 0;
}