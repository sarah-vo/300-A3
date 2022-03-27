#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "pcb.h"
#include "list.h"

void commands(){
    char command;
    switch(command){
        case 'C':
        break;

        case 'F':
        break;

        case 'K':
        break;

        case 'E':
        break;

        case 'Q':
        break;

        case 'S':
        break;

        case 'R':
        break;

        case 'Y':
        break;

        case 'N':
        break;

        case 'P':
        break;

        case 'V':
        break;

        case 'I':
        break;

        case 'T':
        break;

        // default:

    }
}

void pcb_command_list(){
    printf("C: Create");
    printf("");
    printf("");
    printf("");
    printf("");
    printf("");
    printf("");
    printf("");
    printf("");

}

int main(){
    char command;
    bool run = true;

    printf("Initial process initialized...\n");
    pcb_initialize();

    //except for exit and help, all command should run pcb_totalinfo() after executing
    while(true){
        printf("Enter a command: \n");
        fflush(stdin);
        scanf("%c", &command);

        if(command == 'C' || command == 'c'){ //create
            printf("Type in the PCB input: ");
            char input;
            char* endptr;
            //fflush(stdin);
            scanf("%d",&input);
            int priority = (int) input;
            if(priority == 0 || priority == 1 || priority == 2){
                pcb_create(priority);
                //pcb_totalinfo();
            }
            else{
                printf("\nPriority value must be either 0, 1, or 2. Please try again.\n");
            }
        }
        else if(command == 'F'){ //fork
            pcb_fork();
        }
        else if(command == 'K'){ //kill
            printf("Type in the PID#: ");
            char input;
            //fflush(stdin);
            scanf("%s", &input);
            int PID = atoi(&input);
            pcb_kill(PID);


        }else if(command == 'E'){//exit
            int exitCode = pcb_exit();
            if(exitCode == EXIT_SIGNAL){
                break;
            }

        }else if(command == 'Q'){
            pcb_quantum();

        }else if(command == 'S'){
            char pid_input;
            printf("Type in the PID#: ");
            scanf("%s", &pid_input);
            int PID = atoi(&pid_input);

            char input[MAX_LEN];
            printf("\n Type in the message: ");

            scanf("%s", input);
            char* msg = input;
            msg[strcspn(msg,"\n")] = '\0';
            int len = (int)strlen(msg);

            char* pMessage = (char*)malloc(len + 1);
            strcpy(pMessage, msg);
            printf("%s", msg);
            pcb_send(PID, pMessage);

        }else if(command == 'R'){
            pcb_receive();

        }else if(command == 'Y'){
            //pcb_reply();

        }else if(command == 'N'){
            //pcb_create_semaphore();

        }else if(command == 'P'){
            //pcb_P();

        }else if(command == 'V'){
            //pcb_V();

        }else if(command == 'I'){
            pcb_totalinfo();

        }else if(command == 'T'){
            pcb_totalinfo();

        }else if(command == 'H'){
            pcb_command_list();
        }else{
//            printf("Invalid command: please enter the correct command.\n");
//            printf("Enter 'H' for help.\n");
        }
    }


    return 0;
}