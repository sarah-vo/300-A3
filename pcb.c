#include <stdlib.h>
#include <stdio.h>

#include "pcb.h"
// #include "list.h"

void pcb_initialize(){
    
    // Initialize the initial process
    pcb_init->pid = 0;
    pcb_init->priority = HIGH;
    pcb_init->state = RUNNING;
    pcb_init->msg = NULL;

    pcb_curr = pcb_init;

    list_ready_high = List_create();
    if(list_ready_high = NULL){
        perror("Failed to create a list for high priority.\n");
        exit(FAILURE);
    }

    list_ready_norm = List_create();
    if(list_ready_norm = NULL){
        perror("Failed to create a list for norm priority.\n");
        exit(FAILURE);
    }

    list_ready_low = List_create();
    if(list_ready_low = NULL){
        perror("Failed to create a list for low priority.\n");
        exit(FAILURE);
    }

    list_waiting_send = List_create();
    if(list_waiting_send = NULL){
        perror("Failed to create a list of processes waiting on send.\n");
        exit(FAILURE);
    }

    list_waiting_receive = List_create();
    if(list_waiting_receive = NULL){
        perror("Failed to create a list of processes waiting on receive.\n");
        exit(FAILURE);
    }
}

// wake up the next blocked process, according to its priority, and set it as the curr_pcb
void pcb_next(){

    // highest priority first
    if(List_count(list_ready_high) != 0){
        List_first(list_ready_high);
        pcb_curr = List_remove(list_ready_high);
    }else if(List_count(list_ready_norm) != 0){
        List_first(list_ready_norm);
        pcb_curr = List_remove(list_ready_norm);
    }else if(List_count(list_ready_low) != 0){
        List_first(list_ready_low);
        pcb_curr = List_remove(list_ready_low);
    }else{
        pcb_curr = pcb_init;
    }

}