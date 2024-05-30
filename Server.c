// ----------------------------------------------------------------------------------------------
// ICS_SPRING_2024_MINOR-PROJECT-I
// NAME:- Aayush Raju Bade
// ROLL_NO:- B23MT1001
// ----------------------------------------------------------------------------------------------
// SERVER CODE:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

// Constants 
#define MAX_TRIES 6
#define WORD_COUNT 10
#define MAX_WORD_LEN 20
#define BUFFOP 10000
#define IP 50
#define PORT 50
#define MAXSTR 1024

// Function To generate random numbers with an upper bound
int randomnumgen(int max){
    srand(time(NULL));
    return rand() % max;
}

// Function for printing errors 
void Error(const char *msg){
    perror(msg);
    exit(1);
}
void Chat(int new_sockfd); // Chatting Function - Server Client Chats
void RemoteAccess(int new_sockfd); //  Function to remotely access the server
void RockPaperScissors(int new_sockfd);
int logicrps(int p1,int p2);
void NmapScan(int new_sockfd);
void CommandOp(char *command,char *output);
void PlayerChoiceAssigner(int p,char *pc);

int main(int argc, char* argv[]){

    // As our server requires 2 arguments <./server> <portno>
    // This checks 
    if(argc < 2){
        fprintf(stderr, "Port Number Not Provided Or Not Valid. Program Terminated\n");
        exit(1);
    }
    else if(argc>2){
        fprintf(stderr,"Too many arguments are provided. Program Terminated\n");
        exit(1);
    }
    // Defining variables
    
    // sockfd -> socket file descriptor for server
    // new_sockfd -> socket file descripter for client
    // portno holds the port no. entered as command line argument & n is a temp variable

    int sockfd, new_sockfd, portno, n; 
    char Buffer[MAXSTR];                
    struct sockaddr_in serv_addr, cli_addr;  // Declaring server and client address
    socklen_t clilen; // Declaring length of client address

    // Creating SOCKET

    // AF_INET -> specifies that we accept IPv4 address
    // SOCK_STREAM -> TCP 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        Error("Error Opening Socket !");
    }

    bzero((char *)&serv_addr, sizeof(serv_addr)); // Clearing the server address

    portno = atoi(argv[1]); // converting portno from string to integer
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = INADDR_ANY; // Any address in system
    serv_addr.sin_port = htons(portno); // htons -> host to network short

    // BINDING: binding socket to address

    if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        Error("Binding Failed !");
    }

    // LISTENING: listening for connection requests at sockfd , and allowed queue length is 5

    listen(sockfd, 5);
    clilen = sizeof(cli_addr); // declaring length of client address

    //ACCEPTING: accepting connections

    new_sockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen); 
    if(new_sockfd < 0){
        Error("Error On Accept !");
    }

   int choice;

    // THE MAIN MENU
  while(1){
    n = write(new_sockfd,"----------MENU----------\n\nEnter Your Choice:-\n\n1.Chat Room\n2.Remote Access\n3.Play Rock Paper Scissors\n4.Run Nmap\n5.Quit",strlen("----------MENU----------\n\nEnter Your Choice:-\n\n1.Chat Room\n2.Remote Access\n3.Play Rock Paper Scissors\n4.Run Nmap\n5.Quit"));
    if(n<0) Error("ERROR Writing To Socket");
    read(new_sockfd,&choice,sizeof(int));

    if(choice == 524){
        continue;
    }
    printf("\nClient:= Choice Is:- %d\n",choice);

    // Handling client's requests

   switch (choice)
   {
    case 1:
        Chat(new_sockfd);
        continue; // jumps back to menu

    case 2:
        RemoteAccess(new_sockfd);
        continue; // jumps back to menu
    case 3:
        RockPaperScissors(new_sockfd);
        continue;; // jumps back to menu

    case 4:
        NmapScan(new_sockfd);
        continue; // jumps back to menu

    case 5:
        printf("\nExiting...\n");
        close(new_sockfd); // closes socket for both server and client
        close(sockfd);
        break; // exits
   
   default:
        printf("\nPlease Enter A Valid Choice !\n Redirecting.....\n");
        continue; // jumps back to menu
   }
  } 
    return 0;
}

// Chatting function : Implements logic for chatting with server

void Chat(int new_sockfd){ // passing new_sockfd (client's socket) as a parameter

    char buff[MAXSTR]; // Buffer string
    int n; // Temp 

    printf("\n\nType Bye/bye for exiting chat\n\n\n");

    while(1){
        bzero(buff,MAXSTR); //Clears out the buff 

        n = read(new_sockfd,buff,sizeof(buff)); //Reads Input from the Client
        if(n<0) Error("Error On Reading !");
        printf("\n|Client|: %s\n",buff);

        if((strncmp(buff,"Bye",3)==0) || (strncmp(buff,"bye",3)==0)){ // Checks if the client has entered Bye 
            break;
        }

        bzero(buff,MAXSTR); // Clears buff again for new input

        fgets(buff,MAXSTR,stdin); // Taking input

        n = write(new_sockfd,buff,strlen(buff)); // Writing to the client
        if(n<0) Error("Error On Writing !");

        if((strncmp(buff,"Bye",3)==0) || (strncmp(buff,"bye",3)==0)){ //Again compares if the server has entered bye or not
            break;
        }
        
    }
}

// Remote Access function: establishes remote connection to server , so that client can use the server as a pivot 

void RemoteAccess(int new_sockfd){

    char com[BUFFOP]; // String to store the command command to be executed by the client
    int n; // Temp
    char output[BUFFOP]; // stores the output generated by executing the unix command

    while(1){
        bzero(com,BUFFOP); // Clears out command before reading a new command
        n = read(new_sockfd,com,sizeof(com)); // Reads from client
        if(n<0) Error("Error Recieving From The Client!");

        // As fgets on client side appends a '\n' at end of string , so checking that and null terminating the string 
        if (com[n - 1] == '\n') {
            com[n - 1] = '\0';
        }

        // Checking if user wanna exit
        if((strncmp(com,"Bye",3)==0)||(strncmp(com,"bye",3)==0)){
            break;
        }

        bzero(output,BUFFOP); // Clearing output before storing a new one
        CommandOp(com,output); // Creating a child process and storing the result in output
        n = write(new_sockfd,output,strlen(output)+1); // Writing output to client
        if(n<0) Error("Error Sending To Client!");
    }
}

// Function to play rock-paper-scissors:
// Gives functionality to play v/s bot or other player

void RockPaperScissors(int new_sockfd){
    int ch,n,p1,p2; // ch -> gamemode choice , n-> temp,p1-> client's choice , p2-> server's choice
    char Finalmsg[255],p1_Choice[255],p2_Choice[255]; // Finalmsg -> winer text , p1_choice-> client's choice in words , p2_choice-> server's choice in words
    
    while(1){
        bzero(Finalmsg,sizeof(Finalmsg)); // All these 3 bzero's clear out the respetive char strings
        bzero(p1_Choice,sizeof(p1_Choice));
        bzero(p2_Choice,sizeof(p2_Choice));
        n = read(new_sockfd,&ch,sizeof(int)); // Reads choice from client
        if(n<0) Error("Error Reading From Client !");

        // For Playing against bot
        if(ch == 1){
            while(1){
                n = read(new_sockfd,&p1,sizeof(int)); // Reads p1's input from client
                if(n<0) Error("Error Reading From Client");

                srand(time(NULL));  // Seeding the random number generator with current time
                int randomch =  1 + rand()%3; // Storing the random choice 

                p2 = randomch; // The random choice is assigned to p2

                int res = logicrps(p1,p2); // Logic decides the winner
                PlayerChoiceAssigner(p1,p1_Choice); // Assigns p1 and p2's choice
                PlayerChoiceAssigner(p2,p2_Choice);

                // Printing according to the condition returned by logicrps
                if(res == 0){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nIt's A Draw !\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");
                }
                else if(res == p1){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nP1 WON!\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");
                }
                else if(res == p2){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nP2 WON!\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");
                }
                else if(res == 100){
                    printf("\nQuitting This GameMode\n");
                    break;
                }
            }

        }

        // Playing against P2
        else if(ch == 2){
            while(1){
                printf("\n\n-----YOU ARE P2-----\n\n");
                printf("\n\n Please Wait For Your Turn.....\n\n");
                n = read(new_sockfd,&p1,sizeof(int)); // Reading p1's input
                if(n<0) Error("Error Reading From Client");

                if(p1 == 4){
                    printf("\nQuitting Current Game Mode !\n");
                    break;
                }

            R:  printf("1 - Rock , 2 - Paper , 3 - Scissors , 4 - Quit \n\n"); 
                printf("Please Enter Your Choice:- ");
                if((scanf("%d",&p2))!=1 || p2 > 4){ // Taking p2's input
                    printf("\nPlease Enter A Valid Choice!\nRedirecting.....\n");
                    while ((getchar()) != '\n');
                    goto R;
                } 

                int res = logicrps(p1,p2); // Logic decides the winner
                PlayerChoiceAssigner(p1,p1_Choice); // Assigns p1 and p2's choice
                PlayerChoiceAssigner(p2,p2_Choice);

                // Printing according to the condition returned by logicrps
                if(res == 0){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nIt's A Draw !\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");

                    printf("\n%s",Finalmsg);
                }
                else if(res == p1){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nP1 WON!\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");

                    printf("\n%s",Finalmsg);
                }
                else if(res == p2){
                    sprintf(Finalmsg,"P1 Chose %s \nP2 Chose %s\nP2 WON!\n\n",p1_Choice,p2_Choice);
                    n = write(new_sockfd,Finalmsg,strlen(Finalmsg));
                    if(n<0) Error("Error Writing To Client");

                    printf("\n%s",Finalmsg);
                }
                else if(res == 100){
                    printf("\nGame Ended By P1\nQuitting Current GameMode.....\n");
                    break;
                }
                else if(res == 200){
                    n = write(new_sockfd,"\nGame Quitted By P2\n",strlen("\nGame Quitted By P2\n"));
                    if(n<0) Error("Error Writing To Client");

                    printf("\nGame Quitted By You\n");
                    break;
                }
            }
        }
        else if(ch == 3){
            break;
        }
    }
}

// Function to implement the logic of rock-paper-scissors game
int logicrps(int p1,int p2){
    if(p1 == 1){ // If p1 chooses Rock
        if(p2 == 1){
            return 0;
        }
        else if(p2 == 2){
            return p2;
        }
        else if(p2 == 3){
            return p1;
        }
        else if(p2 == 4){
            return 200;
        }
    }
    else if(p1 == 2){ // If p1 chooses Paper
        if(p2 == 1){
            return p1;
        }
        else if(p2 == 2){
            return 0;
        }
        else if(p2 == 3){
            return p2;
        }
        else if(p2 == 4){
            return 200;
        }
    }
    else if(p1 == 3){ // If p1 chooses Scissors
        if(p2 == 1){
            return p2;
        }
        else if(p2 == 2){
            return p1;
        }
        else if(p2 == 3){
            return 0;
        }
        else if(p2 == 4){
            return 200;
        }
    }
    else if(p1 == 4){
        return 100;
    }
}

// Function to perform various Nmap scan's on a target
// Scan for open ports, running services and vulnerebilities

void NmapScan(int new_sockfd){
    int ch,n,quit = 0; // ch-> client's choice , n -> temp
    char ip[IP],port[PORT],output[BUFFOP],command[BUFFOP]; // self explanatory

    while(1){
        if(quit == 1){
            break;
        }
        n = read(new_sockfd,&ch,sizeof(int)); // Reads the choice from the client
        if(n<0) Error("Error Reading From Client !");

        switch(ch){
            case 1: // Implements basic scan to check if a host is up or not by pinging
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // Reads target ip from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -sP %s",ip); // formats the command and appends the target ip

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // Writes the output to client
                if(n<0) Error("Error Sending To Client!");

                continue;
        
            case 2: // Implements port scanning
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // Reads target ip from client
                if(n<0) Error("Error Reciving From Client !");

                bzero(port,sizeof(port)); // clears out port no.
                n = read(new_sockfd,&port,sizeof(port)); // Reads target port no. from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -sT -p %s %s",port,ip); // formats command and appends target ip and portno.

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!");

                continue;
        
                case 3: // Implements a stealth scan 
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // reads ip from client
                if(n<0) Error("Error Reciving From Client !");

                bzero(port,sizeof(port)); // clears out port
                n = read(new_sockfd,&port,sizeof(port)); // reads port no. from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -sS -p %s %s",port,ip); // formats command

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!");

                continue;

            case 4:// Implements OS detection scan
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // reads ip from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -O %s",ip); // formats command

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!");

                continue;

            case 5: // Implements A Decoy scan , i.e also probes using a decoy ip (proxy)
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // reads ip from client
                if(n<0) Error("Error Reciving From Client !");

                bzero(port,sizeof(port)); // clears out port
                n = read(new_sockfd,&port,sizeof(port)); // reads port no. from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -sS -D %s %s",port,ip); // formats command

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!");

                continue;

            case 6: // Performs a agressive scan
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // read ip from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap -A %s",ip); // formats command

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!"); 

                continue;

            case 7: // Scans for target's vulnerebilities from the known/exisiting vulnerebilities
                bzero(ip,sizeof(ip)); // clears out ip
                n = read(new_sockfd,&ip,sizeof(ip)); // reads ip from client
                if(n<0) Error("Error Reciving From Client !");

                sprintf(command,"nmap --script vuln %s",ip); // formats command

                printf("\nRunning: %s\n",command);

                CommandOp(command,output); // Creates a child process for the command and pipes the o/p to client

                n = write(new_sockfd,output,strlen(output)+1); // writes o/p to client
                if(n<0) Error("Error Sending To Client!");

                continue;

            case 8:
                printf("\nQuitting !\n");
                quit = 1;
                break;

            default:
                printf("\nValid Choice Wasn't Entered!\nRedirecting....\n");
                continue;

        }
    }

}

// This function creates a child process and pipes the output to client
void CommandOp(char *command,char *output ){
    FILE *fp;

    char Buffer[BUFFOP];

    // Opens A Pipe For The Command

    fp = popen(command,"r");

    if(fp == NULL){
        strcpy(output,"Error Executing Command !");
        return;
    }

    size_t bytesRead = fread(output, 1, BUFFOP - 1, fp);
    if (bytesRead > 0) {
        output[bytesRead] = '\0'; // Null-terminate the string
    } else {
        strcpy(output, "No output from command\n");
    }

    pclose(fp);
}

void PlayerChoiceAssigner(int p,char *pc){ // Assigns players choice equivalent to the number in RPC

    if(p == 1){
        strcpy(pc,"Rock");
    }
    else if(p == 2){
        strcpy(pc,"Paper");
    }
    else{
        strcpy(pc,"Scissors");
    }

}

