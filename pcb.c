#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "pcb.h"
#include "list.h"

int totalPID = 0;
Semaphore* semaphore[5];

// wake up the next blocked process, according to its priority, and set it as the curr_pcb
void pcb_next() {

    // highest priority first
    if (List_count(list_ready_high) != 0) {
        List_first(list_ready_high);
        pcb_curr = List_remove(list_ready_high);
    } else if (List_count(list_ready_norm) != 0) {
        List_first(list_ready_norm);
        pcb_curr = List_remove(list_ready_norm);
    } else if (List_count(list_ready_low) != 0) {
        List_first(list_ready_low);
        pcb_curr = List_remove(list_ready_low);
    } else {
        pcb_curr = pcb_init;
    }
}
char* priorityChar(int priority){
    switch (priority){
        case '0':
            return "High";
        case '1':
            return "Normal";
        case '2':
            return "Low";
        default:
            return "Un-initialized";
    }
}

List* priorityList(int priority){
    List* returnVal = NULL;
    switch (priority){
        case '0':
            return list_ready_high;
        case '1':
            return list_ready_norm;
        case '2':
            return list_ready_low;
        default:
            printf("Failed to get list. (List* priorityList) \n");
            exit(FAILURE);
    }
}

int init(){
    //initialize list

    list_ready_high = List_create();
    if(list_ready_high == NULL){
        perror("Failed to create a list for high priority.\n");
        exit(FAILURE);
    }

    list_ready_norm = List_create();
    if(list_ready_norm == NULL){
        perror("Failed to create a list for norm priority.\n");
        exit(FAILURE);
    }

    list_ready_low = List_create();
    if(list_ready_low == NULL){
        perror("Failed to create a list for low priority.\n");
        exit(FAILURE);
    }

    list_waiting_send = List_create();
    if(list_waiting_send == NULL){
        perror("Failed to create a list of processes waiting on send.\n");
        exit(FAILURE);
    }

    list_waiting_receive = List_create();
    if(list_waiting_receive == NULL){
        perror("Failed to create a list of processes waiting on receive.\n");
        exit(FAILURE);
    }

    //initialize init PCB
    pcb_init = malloc(sizeof(PCB));
    pcb_init->state = RUNNING;
    pcb_init->pid = 0;
    pcb_init->priority = 0;
    pcb_init->msg = NULL;
    totalPID++;
    pcb_curr = pcb_init;

    return SUCCESS;

}

int create(int priority){
    //if totalPID is 0, initialize
    if(totalPID == 0){
        printf("init has not run yet! Cannot create new process");
        return FAILURE;
    }

    //initialize new PCB
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->priority = priority;
    newPCB->pid = totalPID;
    newPCB->state = READY;

    totalPID++;

    //check priority number
    bool appendSuccess;
    switch (priority) {
        case '0': //high priority
            appendSuccess = List_prepend(list_ready_high, newPCB) == 0;
            break;
        case '1': //high priority
            appendSuccess = List_prepend(list_ready_norm, newPCB) == 0;
            break;
        case '2': //high priority
            appendSuccess = List_prepend(list_ready_low, newPCB) == 0;
            break;
    }
    if(!appendSuccess){
        printf("Append new process to list failed!\n");
        return(FAILURE);
    }
    printf("Process created successfully. The PID is: %d with %s\n", newPCB->pid, priorityChar(newPCB->priority));
    return SUCCESS;
}

int fork(){
    if(pcb_curr == pcb_init){
        printf("Current process is init! Fork() cannot be executed,\n");
        return FAILURE;
    }
    if(pcb_curr == NULL){
        printf("Current PCB is null. Cannot fork.\n");
        return(FAILURE);
    }
    PCB *newProcess = malloc(sizeof(PCB));
    memcpy(newProcess, pcb_curr, sizeof(PCB));
    List* list = priorityList(pcb_curr->priority);
    List_prepend(list, newProcess);
    pcb_next();
    printf("Process forked successfully. The new PID is: %d with %s\n", newProcess->pid, priorityChar(newProcess->priority));
    return SUCCESS;
}

int kill(int pid){

}

void terminate(){

}

void quantum(){

}

/**
 * @brief
 *
 * @param pid (pid of process to send msg to)
 * @param msg (null-terminated msg string, 40 char max)
 * @return int
 */
int send(int pid, char* msg){

}

int receive(){

}

int reply(int pid, char* msg){

}

int create_semaphore(int sid, int init){

}

int P(int sid){

}

int V(int sid){

}

void procinfo(int pid){

}

void totalinfo(){

}