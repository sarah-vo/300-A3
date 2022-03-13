#include <stdlib.h>
#include <stdio.h>
#include "pcb.h"
#include "list.h"

List* list_ready_high;
List* list_ready_norm;
List* list_ready_low;
List* list_waiting_send;
List* list_waiting_receive;


void pcb_initialize(){
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