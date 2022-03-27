#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "pcb.h"
#include "list.h"

void pcb_command_list(){
    printf("C: Create\n");
    printf("F: Fork\n");
    printf("K: Kill\n");
    printf("E: Exit\n");
    printf("Q: Quantum\n");
    printf("S: Send\n");
    printf("R: Receive\n");
    printf("Y: Reply\n");
    printf("N: New Semaphore\n");
    printf("P: Semaphore P\n");
    printf("V: Semaphore V\n");
    printf("I: Procinfo\n");
    printf("T: Totalinfo\n");
}

int main(){
    char command;

    printf("Initial process initialized...\n");
    pcb_initialize();

    //except for exit and help, all command should run pcb_totalinfo() after executing
    while(true){
        printf("Enter a command: \n");
        fflush(stdin);
        scanf("%c", &command);

        if(command == 'C' || command == 'c'){ //create
            printf("Type in the PCB input: ");
            int input;
            char* endptr;
            int priority;
            //fflush(stdin);
            scanf("%d", &priority);
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
            pcb_reply(PID, pMessage);

        }else if(command == 'N'){
            int sid;
            printf("Enter the sid (from 0 to 4): ");
            scanf("%d", &sid);

            int val;
            printf("Enter the semaphore value: ");
            scanf("%d", &val);
            pcb_create_semaphore(sid, val);

        }else if(command == 'P'){
            int sid;
            printf("Enter the sid (from 0 to 4): ");
            scanf("%d", &sid);
            pcb_P(sid);

        }else if(command == 'V'){
            int sid;
            printf("Enter the sid (from 0 to 4): ");
            scanf("%d", &sid);
            pcb_V(sid);

        }else if(command == 'I'){
            printf("Enter the pid of the process you want to check: ");
            int pid;
            scanf("%d", &pid);
            pcb_procinfo(pid);

        }else if(command == 'T'){
            pcb_totalinfo();

        }else if(command == 'H'){
            pcb_command_list();
        }else{
            printf("Invalid command: please enter the correct command.\n");
            printf("Enter 'H' for help.\n");
        }

        if(termination_val == -1){
            return 0;
        }
    }


    return 0;
}
