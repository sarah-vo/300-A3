#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

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

}

int main(){
    char command;
    bool run = true;

    printf("Initial process initialized...\n");
    pcb_initialize();

    //except for exit and help, all command should run pcb_totalinfo() after executing
    while(run){
        printf("Enter a command: \n");
        scanf("%c", &command);

        if(command == 'C'){ //create
            printf("Type in the PCB priority: ");
            int priority;
            scanf("%d", &priority);
            if(priority != (0 || 1 || 2)){
                printf("\nPriority value must be either 0, 1, or 2. Please try again.\n");
            }
            else{
                pcb_create(priority);
                pcb_totalinfo();
            }
        }
        else if(command == 'F'){ //fork
            pcb_fork();
            pcb_totalinfo();    
        }
        else if(command == 'K'){ //kill
            printf("Type in the PID#: ");
            int PID;
            scanf("%d", &PID);
            if(PID != (0 || 1 || 2)){
                printf("\nPriority value must be either 0, 1, or 2. Please try again.\n");
            }
            else{
                pcb_kill(PID);
                pcb_totalinfo();
            }

        }else if(command == 'E'){//exit
            pcb_exit();
            run = false;

        }else if(command == 'Q'){
            pcb_quantum();
            pcb_totalinfo();

        }else if(command == 'S'){
            pcb_totalinfo();


        }else if(command == 'R'){
            pcb_totalinfo();

        }else if(command == 'Y'){
            pcb_totalinfo();

        }else if(command == 'N'){
                
        }else if(command == 'P'){
            pcb_totalinfo();

        }else if(command == 'V'){
            pcb_totalinfo();

        }else if(command == 'I'){
            pcb_totalinfo();

        }else if(command == 'T'){
            pcb_totalinfo();

        }else if(command == 'H'){
            pcb_command_list();
        }else{
            printf("Invalid command: please enter the correct command.\n");
            printf("Enter 'H' for help.\n");
        }
    }
   
    
    return 0;
}