#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "pcb.h"
#include "list.h"

List *list_ready_high = NULL;
List *list_ready_norm = NULL;
List *list_ready_low = NULL;

List *list_waiting_send = NULL;
List *list_waiting_receive = NULL;

PCB* pcb_init = NULL;
PCB* pcb_curr = NULL;

int totalPID = 0;
Semaphore* semaphore[5];

bool list_comparator(void* pcb, void* receiver){
    return(((PCB*)pcb)->pid == *((int*)receiver));
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


PCB* pcb_search(int pid){
    COMPARATOR_FN pComparatorFn = &list_comparator;
    PCB* returnPCB = NULL;
    //if List_search fails, the pointer would point NULL
    while(returnPCB == NULL){
        List_search(priorityList(0), pComparatorFn, &pid);
            returnPCB = List_curr(priorityList(0));
        List_search(priorityList(1), pComparatorFn, &pid);
            returnPCB = List_curr(priorityList(1));
        List_search(priorityList(2), pComparatorFn, &pid);
            returnPCB = List_curr(priorityList(2));
        List_search(list_waiting_receive, pComparatorFn, &pid);
            returnPCB = List_curr(list_waiting_receive);
        List_search(list_waiting_send, pComparatorFn, &pid);
            returnPCB = List_curr(list_waiting_receive);
    }
    return returnPCB;
}

// wake up the next blocked process, according to its priority, and set it as the curr_pcb
void pcb_next() {

    // highest priority first
    if (List_count(list_ready_high) != 0) {
        List_first(list_ready_high);
        pcb_curr = List_trim(list_ready_high);
    } else if (List_count(list_ready_norm) != 0) {
        List_first(list_ready_norm);
        pcb_curr = List_trim(list_ready_norm);
    } else if (List_count(list_ready_low) != 0) {
        List_first(list_ready_low);
        pcb_curr = List_trim(list_ready_low);
    } else {
        pcb_curr = pcb_init;
    }
    pcb_curr->state = RUNNING;
}




int pcb_initialize(){
    //initialize list


    list_ready_high = List_create();
    list_ready_norm = List_create();
    list_ready_low = List_create();
    list_waiting_send = List_create();
    list_waiting_receive = List_create();

    if( list_ready_high == NULL ||
        list_ready_norm == NULL ||
        list_ready_low == NULL ||
        list_waiting_send == NULL ||
        list_waiting_receive == NULL){
            printf("Initializing list failed!\n");
            return(FAILURE);
        }

    //initialize init PCB
    pcb_init = malloc(sizeof(PCB));
    pcb_init->state = RUNNING;
    pcb_init->pid = 0;
    pcb_init->priority = HIGH;
    pcb_init->msg = NULL;
    totalPID++;
    pcb_curr = pcb_init;

    return SUCCESS;

}

int pcb_create(int priority){
    //if totalPID is 0, initialize
    if(pcb_init == NULL){
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

int pcb_fork(){
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
    printf("Process forked successfully. The new PID is: %d with %s\n", newProcess->pid, priorityChar(newProcess->priority));
    return SUCCESS;
}

int pcb_kill(int pid){
    //search for the PCB corresponding to the pid, remove it from the list and free that pcb's memory
    COMPARATOR_FN pComparatorFn = &list_comparator;
    PCB* PCB_kill = pcb_search(pid);
    if(pcb_curr == pcb_init){
        return pcb_exit();
    }
    if(pcb_curr == NULL){
        printf("Error: Current PCB is null!\n");
        return FAILURE;
    }
    List* listItemRemove =priorityList(PCB_kill->priority);
    List_search(listItemRemove, pComparatorFn, &pid);
    List_remove(listItemRemove);
    free(PCB_kill);
    //TODO is this necessary (going to next PCB)
    pcb_next();
    int pid_next = pcb_curr->pid;
    printf("PCB PID# %d removed. Current PCB is PID# %d", pid, pid_next);
    return SUCCESS;
}

int pcb_exit(){

}

void list_pcb_free(void* pcb){
    free((PCB*)pcb);
}
//TODO is this exit()?
void pcb_terminate(){
    FREE_FN pFreeFn = &list_pcb_free;
    List_free(list_ready_high, pFreeFn);
    List_free(list_ready_norm, pFreeFn);
    List_free(list_ready_low, pFreeFn);
    List_free(list_waiting_send, pFreeFn);
    List_free(list_waiting_receive, pFreeFn);
}

void pcb_quantum(){
    if(pcb_curr == pcb_init){
        printf("Cannot quantum initital PCB.\n");
        return FAILURE;
    }
    int currPID = pcb_curr->pid;
    int currPriority = pcb_curr->priority;
    List_prepend(priorityList(currPriority), pcb_curr);
    pcb_next();
    int nextPID = pcb_curr->pid;
    printf("PCB #%d is put back into %c. The current active process is PCB #%d", currPID, priorityChar(currPriority), nextPID);
    return SUCCESS;


}

/**
 * @brief
 *
 * @param pid (pid of process to send msg to)
 * @param msg (null-terminated msg string, 40 char max)
 * @return int
 */
int pcb_send(int pid, char* msg){
    COMPARATOR_FN pComparatorFn = &list_comparator;
    PCB* receiver;
    
    // check if there's any process waiting for msg sent to it
    if(List_count(list_waiting_receive) != 0){
        if(List_search(list_waiting_receive, pComparatorFn, &pid) != NULL){
            receiver = List_trim(list_waiting_receive);
            receiver->msg = msg;
            receiver->state = READY;
            List_prepend(priorityList(receiver->priority), receiver);

            printf("The message has been successfully sent.\n");
            printf("Sender pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
            printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
            printf("Message sent:\t%s", msg);

            if(pcb_curr->pid != 0){
                pcb_curr->state = BLOCKED;
                List_prepend(list_waiting_send, pcb_curr);
                pcb_next();
            }
            
            return SUCCESS;
        }
    }

    // still need to be fixed  ************
    if(pid == pcb_curr->pid){   // if curr process sends msg to itself
        pcb_curr->msg = msg;
        pcb_curr->state = BLOCKED;
        List_prepend(list_waiting_send, pcb_curr);

        printf("The message has been successfully sent.\n");
        printf("Sender pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
        printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
        printf("Message sent:\t%s", msg);

        pcb_next();
        return SUCCESS;
    }

    // search all three ready queues
    // 1. high priority
    if(List_count(list_ready_high) != 0){
        if(List_search(list_ready_high, pComparatorFn, &pid) != NULL){
            receiver = List_trim(list_ready_high);
            receiver->msg = msg;
            receiver->state = READY;
            List_prepend(priorityList(receiver->priority), receiver);

            printf("The message has been successfully sent.\n");
            printf("Sender pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
            printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
            printf("Message sent:\t%s", msg);


            if(pcb_curr->pid == 0){
                pcb_curr->state = BLOCKED;
                List_prepend(list_waiting_send, pcb_curr);
                pcb_next();
            }

            return SUCCESS;
        }
    }

    // 2. normal priority
    if(List_count(list_ready_norm) != 0){
        if(List_search(list_ready_norm, pComparatorFn, &pid) != NULL){
            receiver = List_trim(list_ready_norm);
            receiver->msg = msg;
            receiver->state = READY;
            List_prepend(priorityList(receiver->priority), receiver);

            printf("The message has been successfully sent.\n");
            printf("Sender pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
            printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
            printf("Message sent:\t%s", msg);

            if(pcb_curr->pid == 0){
                pcb_curr->state = BLOCKED;
                List_prepend(list_waiting_send, pcb_curr);
                pcb_next();
            }
            return SUCCESS;
        }
    }

    // 3. low priority
    if(List_count(list_ready_low) != 0){
        if(List_search(list_ready_low, pComparatorFn, &pid) != NULL){
            receiver = List_trim(list_ready_low);
            receiver->msg = msg;
            receiver->state = READY;
            List_prepend(priorityList(receiver->priority), receiver);
            
            printf("The message has been successfully sent.\n");
            printf("Sender pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
            printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
            printf("Message sent:\t%s", msg);
           
            if(pcb_curr->pid == 0){
                pcb_curr->state = BLOCKED;
                List_prepend(list_waiting_send, pcb_curr);
                pcb_next();
            }
            return SUCCESS;
        }
    }
    
    // no match:
    printf("Failed to send the message:\t'%s'.\n", msg);
    printf("No receiver (pid = %d)\n", pid);
    printf("Try again with a different pid.\n");
    return FAILURE;
}

// Check if the currently executing process has received any messages
int pcb_receive(){
    if(pcb_curr->msg == NULL){
        printf("Current pcb (pid = %d, priority = %s) hasn't received any messages.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
        
        if(pcb_curr->pid != 0){
            pcb_curr->state = BLOCKED;
            List_prepend(list_waiting_receive, pcb_curr);
            
            pcb_next();
        }

        printf("New pcb:\tpid = %d, priority = %s\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
        return FAILURE;
    }else{
        printf("Current pcb (pid = %d, priority = %s) has successfully received the message.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
        printf("Message received:\t'%s'\n", pcb_curr->msg);

        // reset the msg and partner_pid
        pcb_curr->msg = NULL;

        return SUCCESS;
    }
}

int pcb_reply(int pid, char* msg){
    COMPARATOR_FN pComparatorFn = &list_comparator;
    PCB* receiver;
    
    if(pid == 0){
        printf("The reply message has been successfully sent.\n");
        printf("Replier pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
        printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
        printf("Reply sent:\t%s", msg); 
    }

    // check if there's any process waiting for msg sent to it
    if(List_count(list_waiting_send) != 0){
        
        if(List_search(list_waiting_send, pComparatorFn, &pid) != NULL){
            receiver = List_trim(list_waiting_send);
            receiver->msg = msg;
            receiver->state = READY;
            List_prepend(priorityList(receiver->priority), receiver);

            printf("The reply message has been successfully sent.\n");
            printf("Replier pid:\t%d with %s priority.\n", pcb_curr->pid, priorityChar(pcb_curr->priority));
            printf("Receiver pid:\t%d with %s priority.\n", pid, priorityChar(receiver->priority));
            printf("Reply sent:\t%s", msg);

            return SUCCESS;
        }
    }else{
        printf("There's no pcb waiting for replies.\n");
        return FAILURE;
    }

    printf("Failed to find a send_blocked pcb with pid = %d.\n", pid);
    printf("Try another pid.\n");

    return FAILURE;
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
