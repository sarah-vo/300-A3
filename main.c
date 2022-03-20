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