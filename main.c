#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "pcb.h"
#include "list.h"

bool list_comparator(PCB* pItem, PCB* pComparisonArg){
    return(pItem->pid = pComparisonArg->pid);
}

int pcb_create(int priority){

}

int pcb_fork(){

}

int pcb_kill(int pid){

}

void pcb_exit(){

}

void pcb_quantum(){

}

/**
 * @brief 
 * 
 * @param pid (pid of process to pcb_send msg to)
 * @param msg (null-terminated msg string, 40 char max)
 * @return int 
 */
int pcb_send(int pid, char* msg){
    if(pid == pcb_curr->pid){
        pcb_curr->msg = msg;
        pcb_curr->state = BLOCKED;

        // List_insert(list_waiting_pcb_send, pcb_curr);
        pcb_next();
    }
}

int pcb_receive(){

}

int pcb_reply(int pid, char* msg){

}

int pcb_create_semaphore(int sid, int init){

}

int pcb_P(int sid){

}

int pcb_V(int sid){

}

void pcb_procinfo(int pid){

}

void pcb_totalinfo(){

}

void pcb_commands(){
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

    while(run){
        printf("Enter a command: \n");
        scanf("%c", &command);

        if(command == 'C'){

        }else if(command == 'F'){
            
        }else if(command == 'K'){

        }else if(command == 'E'){

        }else if(command == 'Q'){

        }else if(command == 'S'){

        }else if(command == 'R'){

        }else if(command == 'Y'){

        }else if(command == 'N'){

        }else if(command == 'P'){

        }else if(command == 'V'){

        }else if(command == 'I'){

        }else if(command == 'T'){

        }else if(command == 'H'){
            pcb_command_list();
        }else if(command == 'X'){
            printf("Exiting the program...\n");
            printf("Goodbye!");
            run = false;
        }else{
            printf("Invalid command: please enter the correct command.\n");
            printf("Enter 'H' for help.\n");
        }
    }
   
    
    return 0;
}