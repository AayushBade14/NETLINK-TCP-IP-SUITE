// ----------------------------------------------------------------------------------------------
// ICS_SPRING_2024_MINOR-PROJECT-II
// NAME:- Aayush Raju Bade
// ROLL_NO:- B23MT1001
// ----------------------------------------------------------------------------------------------
// CLIENT CODE:

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <ctype.h>
#include <stdbool.h>

// Constants
#define MAX_TRIES 6
#define WORD_COUNT 10
#define MAX_WORD_LEN 20
#define IP 50
#define PORT 50 
#define BUFFOP 10000
#define MAXSTR 1024

// Creating a function to give error to the user 

void Error(const char *msg){
    perror(msg);
    exit(1);
}

void Chat(int sockfd); // Function for chatting 
void RemoteAccess(int sockfd); // Function for remote access
void RockPaperScissors(int sockfd); // Function for playing rock paper scissors
void NmapScan(int sockfd); // Function for performing nmap scans (Info gathering)
bool isValidPortList(const char *portList);
bool isValidIP(const char *ip);
bool isValidPort(const char *port);

int main(int argc, char *argv[]){

    // Checks if three arguments are passed or not in the command line i.e the 1) output file itself 2) server ip and 3) port no.
    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    // DECLARING VARIABLES:
    // sockfd -> socket file descriptor for client
    // portno -> servers port no.

    int sockfd, portno, n; 
    struct sockaddr_in serv_addr; // Declaring servers address
    struct hostent *server; // If client want's to connect using DNS hostname
    char Buffer[MAXSTR]; // Declaring a buffer 

    // CREATING SOCKET:
    // AF_INET -> IPv4 
    // SOCK_STREAM -> TCP

    portno = atoi(argv[2]);  // converting portno from string to int
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if(sockfd < 0){
        Error("ERROR Opening Socket !");
    }

    server = gethostbyname(argv[1]); // If user want's to connect using hostname
    if(server == NULL){
        fprintf(stderr, "ERROR ! No Such Host");
        exit(1);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // clearing out server address
    serv_addr.sin_family = AF_INET; 
    bcopy((char *)server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno); // host to network short

    // ESTABLISHING CONNECTION:

    if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        Error("Connection Failed !");
    }

   int choice,flag =524; // User's choice

    // THE MAIN MENU
    while(1){
        bzero(Buffer,MAXSTR); // clearing out buffer before storing output
        n = read(sockfd,Buffer,sizeof(Buffer)); // reading menu from server
        if(n<0) Error("ERROR Reading To server !");
        printf("%s\n",Buffer); // Printing menu
        if((scanf("%d",&choice))!=1){ // Taking choice as input from user
            printf("\nPlease Enter A Valid Choice !\nRedirecting.....\n");
            n = write(sockfd,&flag,sizeof(int));  // writing the choice to the server
            if(n<0) Error("ERROR Writing To server !");
            //printf("HIT ENTER TO CONTINUE !\n");
            while ((getchar()) != '\n');
            continue;
        } 
    
        printf("\nMy choice is %d",choice); 
        printf("\n");
        n = write(sockfd,&choice,sizeof(int));  // writing the choice to the server
        if(n<0) Error("ERROR Writing To server !");

        switch(choice){
            case 1:
                Chat(sockfd);
                continue; // Jumps back to the menu

            case 2:
                RemoteAccess(sockfd);
                continue; // Jumps back to the menu

            case 3:
                RockPaperScissors(sockfd);
                continue; // Jumps back to the menu

            case 4:
                NmapScan(sockfd);
                continue; // Jumps back to the menu

            case 5:
                printf("\nExiting......\n");
                close(sockfd); // closes the socket
                break; // exits

            default:
                printf("\nPlease Enter A Valid Choice !\n Redirecting....\n");
                continue; // jumps back to menu
        }

        return 0;
    }
    
}

// Function for chatting 

void Chat(int sockfd){

    char buff[MAXSTR]; // Buffer
    int n; // Temp

    printf("\n\nType Bye/bye for exiting chat\n\n\n");

    while(1){
        bzero(buff,MAXSTR); // Clearing out buff
        scanf("\n");
        fgets(buff,MAXSTR,stdin); // Taking input 
        n  = write(sockfd,buff,strlen(buff)); // Writing to the server
        if(n<0) Error("Error Writing !");

        if((strncmp(buff,"Bye",3)==0) || (strncmp(buff,"bye",3)==0)){ // Checks if the client has entered bye
            break;
        }

        bzero(buff,MAXSTR); // Again clears out buff

        n = read(sockfd,buff,MAXSTR); // Reads from the server
        if(n<0) Error("Error Reading !");
        printf("\n|Server|: %s\n",buff);

        if((strncmp(buff,"Bye",3)==0) || (strncmp(buff,"bye",3)==0)){ // Checks if the server has entered bye
            break;
        }

    }
}

// Function for accessing server remotely
void RemoteAccess(int sockfd){

    char com[BUFFOP],output[BUFFOP]; // com-> command 
    int n; // Temp

    while(1){
        bzero(com,sizeof(com)); // clears out command
        scanf("\n"); // clears out 
        fgets(com,BUFFOP,stdin); // Takes command as input from client
        int length = strlen(com); // length of string

        // As fgets terminates strings with '\n' so this condition checks that and null terminates it
        if (com[n - 1] == '\n') {
            com[n - 1] = '\0';
        }

        n = write(sockfd,com,strlen(com)+1); // writing the command to server for processing
        if(n<0) Error("Error Writing To Server!");

        if((strncmp(com,"Bye",3)==0)||(strncmp(com,"bye",3)==0)){ // checks if client wants to exit
            break;
        }

        bzero(output,sizeof(output)); // clears out output 

        n = read(sockfd,output,sizeof(output)); // reads output from server
        if(n<0) Error("Error Recieving From Server!");

        output[n] = '\0'; // null terminates the output

        printf("Output:- %s\n\n",output);

    }
    
}

// Function to play rock-paper-scissors

void RockPaperScissors(int sockfd){
    int n,choice,p1,quit = 0; // choices
    char buff[MAXSTR]; // buffer
    // Menu
    while(1){
        printf("\n\n===========WELCOME TO ROCK PAPER SCISSORS============\n\n");
        printf("[+] Press 1 for v/s Bot\n");
        printf("[+] Press 2 for v/s Player\n");
        printf("[+] Press 3 To Quit\n");
        printf("Enter Your Choice:- ");

        if(scanf("%d",&choice)!=1||choice>3){
            printf("\nPlease Enter A Right Choice !\nRedirecting....\n");
            while ((getchar()) != '\n');
            continue;
        }

        n = write(sockfd,&choice,sizeof(int)); // writing the gamemode choice to the server
        if(n<0) Error("Error Writing To Server !");

        // Play against bot
        if(choice==1){
            while(1){
                // Defaults
                printf("\n\n-----YOU ARE P1-----\n\n");
                printf("1 - Rock , 2 - Paper , 3 - Scissors , 4 - Quit This GameMode\n\n");
                printf("Please Enter Your Choice:- ");

                if((scanf("%d",&p1))!=1){ // takes client's choice
                    printf("\nPlease Enter A Right Choice !\nRedirecting....\n");
                    while ((getchar()) != '\n');
                    continue;
                } 

                n = write(sockfd,&p1,sizeof(int)); // writing client's choice to server
                if(n<0) Error("Error Writing To Server!");

                if(p1 == 4){
                    printf("\nQuitting Current Game Mode !\n");
                    break;
                }

                bzero(buff,MAXSTR); // clearing out buff
                n = read(sockfd,&buff,MAXSTR); // reading output (winner) from server
                if(n<0) Error("Error Reading From Server !");

                printf("\n%s \n",buff); // prints the results
            }
        }
        // Play against player
        else if(choice == 2){
            while(1){
                // Defaults
                printf("\n\n-----YOU ARE P1-----\n\n");
                printf("1 - Rock , 2 - Paper , 3 - Scissors , 4 - Quit This GameMode\n\n");
                printf("Please Enter Your Choice:- ");

                if((scanf("%d",&p1))!=1 || p1 >4){ // takes client's input
                    printf("\nPlease Enter A Right Choice !\nRedirecting....\n");
                    while ((getchar()) != '\n');
                    continue;
                } 

                n = write(sockfd,&p1,sizeof(int)); // writes client's choice to server
                if(n<0) Error("Error Writing To Server!");

                if(p1 == 4){
                    printf("\nQuitting This GameMode\n");
                    break;
                }

                bzero(buff,MAXSTR); // clearing out buffer
                n = read(sockfd,&buff,MAXSTR); // reads results from server
                if(n<0) Error("Error Reading From Server !");

                printf("%s ",buff); // prints the results

                if((strcmp(buff,"\nGame Quitted By P2\n"))==0){
                    break;
                }
            }
        
        }
        else if(choice == 3){
            printf("\nQuitting\n");
            break;
        }
        // Error handling , if client enters some other value
        else{
            printf("\nPlease Enter Valid Choice!\nRedirecting...\n");
            continue; // returned back to menu
        }

        
    }
}



// Function to perform Nmap scans

void NmapScan(int sockfd){
    int n,ch,quit=0; // self-explanatory
    char ip[IP],port[PORT],output[BUFFOP];

    // Menu
    while(1){
        if(quit == 1){
            break;
        }
        printf("----------N_MAP----------\n\n");
        printf("Which Scan Do You Want To Perform?\n\n");
        printf("1. Nmap Check If Host Is UP ?\n");
        printf("2. Nmap PortScanning\n");
        printf("3. Nmap Stealth Scan\n");
        printf("4. Nmap Os Detection\n");
        printf("5. Nmap Decoy\n");
        printf("6. Nmap Agressive Scan\n");
        printf("7. Nmap Vulnerability Scan\n");
        printf("8. Quit\n");

        printf("Enter Your Choice:- ");
        if((scanf("%d",&ch))!=1){ // Takes choice input
            printf("\nPlease Enter Valid Choice !\nRedirecting....\n");
            while ((getchar()) != '\n');
            continue;
        } 

        n = write(sockfd,&ch,sizeof(int)); // writes choice to server
        if(n<0) Error("Error Writing To Server!");

        switch(ch){
            case 1: // scan to check if host is up
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clearing out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){ // taking ip as input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 
                    n = write(sockfd,ip,strlen(ip)+1); // writing ip to server
                    if(n<0) Error("Error Sending To Server !");

                    bzero(output,sizeof(output)); // clearing out output
                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    n = read(sockfd,&output,sizeof(output)); // reading output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // printing output

                    break;
                }

                continue;

            case 2: // port scanning
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){ // takes ip input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    printf("Enter Port no's seperated by ',' :- ");
                    bzero(port,sizeof(port)); // clears out port 
                    if((scanf("%49s",port))!=1 || !isValidPortList(port)){ // takes port no. input
                        printf("\nPlease Enter A Valid Port No.'s!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    }; 

                    n = write(sockfd,ip,strlen(ip)+1); // writes ip to server
                    if(n<0) Error("Error Sending To Server !");

                    n = write(sockfd,port,strlen(port)+1); // writes port no. to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reads output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // prints output
                    break;
                }
                continue;

            case 3: // Performs a stealth scan
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){// takes ip input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    printf("Enter Port no's seperated by ',' :- ");
                    bzero(port,sizeof(port)); // clears out port
                    if((scanf("%s",port))!=1 || !isValidPortList(port)){ // takes port as input
                        printf("\nPlease Enter A Valid Port No.'s!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    n = write(sockfd,ip,strlen(ip)+1); // writing ip to server
                    if(n<0) Error("Error Sending To Server !");

                    n = write(sockfd,port,strlen(port)+1); // writing port no. to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reading output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // printing output
                    break;
                }

                continue;
        
            case 4:// scan for target OS detection
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){ // takes ip as input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    n = write(sockfd,ip,strlen(ip)+1); // writes ip to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reading output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // prints output
                    break;
                }
                continue;

            case 5: // Performs a decoy scan
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){ // takes target ip
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    printf("Enter Decoy IP:- ");
                    bzero(port,sizeof(port)); // clears out port (here i'm using port for taking decoy ip)
                    if((scanf("%s",port))!=1 || !isValidIP(port)){ // taking decoy ip input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    n = write(sockfd,ip,strlen(ip)+1); // writing target ip to server
                    if(n<0) Error("Error Sending To Server !");

                    n = write(sockfd,port,strlen(port)+1); // writing decoy ip to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reading output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // printing output
                    break;

                }
                continue;

            case 6: // Implements agressive scan
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%15s",ip))!=1 || !isValidIP(ip)){ // takes ip as input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    } 

                    n = write(sockfd,ip,strlen(ip)+1); // writes ip to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reads output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // prints output
                    break;

                }
                continue;

            case 7: // implements vulnerebility scan
                while(1){
                    printf("Enter Target IP:- ");
                    bzero(ip,sizeof(ip)); // clears out ip
                    if((scanf("%s",ip))!=1 || !isValidIP(ip)){ // takes ip as input
                        printf("\nPlease Enter A Valid IP Address!\nRedirecting...\n");
                        while ((getchar()) != '\n');
                        continue;
                    }

                    n = write(sockfd,ip,strlen(ip)+1); // writes ip to server
                    if(n<0) Error("Error Sending To Server !");

                    printf("\nPlease Wait Your Request Is Being Processed.....\n");
                    bzero(output,sizeof(output)); // clearing out output
                    n = read(sockfd,&output,sizeof(output)); // reads output from server
                    if(n<0) Error("Error Reciving From Server!");

                    printf("%s\n",output); // prints output
                    break;

                }
                continue;

            case 8:
                printf("\nQuitting!\n");
                quit =1;
                break;

            default:
                printf("\nPlease Enter A Valid Choice!\nRedirecting....\n");
                continue;

        
        }
    }

}

bool isValidIP(const char *ip) {
    int num, dots = 0;
    char *token;
    char ip_copy[16]; // Buffer for copy of IP 

    strcpy(ip_copy, ip); // copying IP to ip_copy

    // Tokenize the IP address using '.' 
    token = strtok(ip_copy, ".");
    while (token != NULL) {

        num = atoi(token); // Converting token to integer

        // Check if token is a valid decimal number (0 to 255)
        if (num < 0 || num > 255) {
            return false;
        }

        // Moving to the next token
        token = strtok(NULL, ".");
        dots++;
    }

    // Hence IPv4 address should have exactly 4 parts separated by dots, So checking that
    return (dots == 4);
}

bool isValidPort(const char *port) {
    int portNum = atoi(port);

    // Port number should be within the valid range (0 to 65535)
    return (portNum >= 0 && portNum <= 65535);
}

bool isValidPortList(const char *portList) {
    char *token;
    char port_copy[50]; // Buffer for copying the port list seperated by ','

    strcpy(port_copy, portList); // copying portlist to the copy

    // Tokenize the port list using ',' 
    token = strtok(port_copy, ",");
    while (token != NULL) {
        // Validating each individual port number
        if (!isValidPort(token)) {
            return false; // Returning false if any port number is invalid
        }
        // Moving to next token
        token = strtok(NULL, ",");
    }

    return true; // All port numbers are valid then it returns true
}

