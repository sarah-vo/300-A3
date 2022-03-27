#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "pcb.h"
#include "list.h"

List *list_ready_high;
List *list_ready_norm;
List *list_ready_low;
List *list_waiting_send;
List *list_waiting_receive;
List *list_msg_send;
PCB* pcb_init;
PCB* pcb_curr;

int totalPID = 0;
int total_proc = 0;
Semaphore* semaphore[5];



List* priorityList(int priority){
    List* returnVal = NULL;
    switch (priority){
        case 0:
            return list_ready_high;
        case 1:
            return list_ready_norm;
        case 2:
            return list_ready_low;
        default:
            printf("Failed to get list. (List* priorityList) \n");
            exit(FAILURE);
    }
}

char* priorityChar(int priority){
    switch (priority){
        case 0:
            return "High";
        case 1:
            return "Normal";
        case 2:
            return "Low";
        default:
            return "Un-initialized";
    }
}

bool list_comparator(void* pcb, void* receiver){
    return(((PCB*)pcb)->pid == *((int*)receiver));
}
PCB* pcb_search(int pid){
    COMPARATOR_FN pComparatorFn = &list_comparator;
    PCB* returnPCB = NULL;
    //if List_search fails, the pointer would point NULL
    List_first(priorityList(0));
    List_first(priorityList(1));
    List_first(priorityList(2));
    List_first(list_waiting_send);
    List_first(list_waiting_receive);

    returnPCB = List_search(priorityList(0), pComparatorFn, &pid);
    if(returnPCB != NULL) return returnPCB;
    returnPCB = List_search(priorityList(1), pComparatorFn, &pid);
    if(returnPCB != NULL) return returnPCB;
    returnPCB = List_search(priorityList(2), pComparatorFn, &pid);
    if(returnPCB != NULL) return returnPCB;
    returnPCB = List_search(list_waiting_receive, pComparatorFn, &pid);
    if(returnPCB != NULL) return returnPCB;
    returnPCB = List_search(list_waiting_send, pComparatorFn, &pid);
    if(returnPCB != NULL) return returnPCB;
    //if search cannot find List then NULL will be returned
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
    list_msg_send = List_create();
    termination_val = 0;

    if( list_ready_high == NULL ||
        list_ready_norm == NULL ||
        list_ready_low == NULL ||
        list_waiting_send == NULL ||
        list_waiting_receive == NULL){
            printf("Initializing list failed!\n");
            return(FAILURE);
        }

    for(int i = 0; i < SEM_MAX; i++){
        semaphore[i] = NULL;
    }

    //initialize init PCB
    pcb_init = malloc(sizeof(PCB));
    pcb_init->state = RUNNING;
    pcb_init->pid = 0;
    pcb_init->priority = 0;
    pcb_init->msgPCBItem = NULL;
    totalPID++;

    pcb_curr = pcb_init;
    printf("The initial process (pid: %d) has been created.\n");
    return SUCCESS;

}

int pcb_create(int priority){
    //if totalPID is 0, initialize
    if(totalPID == 0){
        printf("init has not run yet! Cannot create new process\n");
        return FAILURE;
    }

    //initialize new PCB
    PCB* newPCB = malloc(sizeof(PCB));
    newPCB->priority = priority;
    newPCB->pid = totalPID;
    newPCB->state = READY;
    newPCB->msgPCBItem = NULL;
    newPCB->listWaitState = NONE;

    totalPID++;
    total_proc++;


    // if the currently executing pcb == initial pcb,
    // set pcb_curr = newPCB
    if(pcb_curr == pcb_init){
        pcb_curr = newPCB;
        printf("Process created successfully. Its PID is: %d with %s\n", newPCB->pid, priorityChar(newPCB->priority));
        return SUCCESS;
    }

    // check priority number
    // if the pcb_curr != initial pcb, put the new PCB into the correct ready_queue
    bool appendSuccess = false;
    switch (priority) {
        case 0: //high priority
            appendSuccess = (List_prepend(list_ready_high, (void*)newPCB) == 0);
            break;
        case 1: //normal priority
            appendSuccess = (List_prepend(list_ready_norm, (void*)newPCB) == 0);
            break;
        case 2: //low priority
            appendSuccess = (List_prepend(list_ready_low, (void*)newPCB) == 0);
            break;
    }
    if(!appendSuccess){
        printf("Append new process to list failed!\n");
        return(FAILURE);
    }
    printf("Process created successfully. Its PID is: %d with %s\n", newPCB->pid, priorityChar(newPCB->priority));
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
    newProcess->pid = totalPID;
    totalPID++;
    total_proc++;

    List* list = priorityList(pcb_curr->priority);
    List_prepend(list, newProcess);
    //TODO: I'm not sure if we can call pcb_next(), it's not in the specifications
    //    pcb_next();
    printf("Process forked successfully. The new PID is: %d with priority: %s (%d)\n", newProcess->pid, priorityChar(newProcess->priority), newProcess->priority);
    return SUCCESS;
}

int pcb_kill(int pid){
    //search for the PCB corresponding to the pid, remove it from the list and free that pcb's memory
    COMPARATOR_FN pComparatorFn = &list_comparator;



    if(pid == 0){
        if(total_proc == 0){
            printf("No more process except the initial process.\n");
            printf("Terminating the simulation...\n");
            termination_val = -1;
            return EXIT_SIGNAL;
        }
        printf("Cannot kill initial process.\n");
        return EXIT_FAILURE;
    }
    PCB* PCB_kill = pcb_search(pid);
    if(pid == pcb_curr->pid){
        PCB_kill = pcb_curr;
    }
    if(PCB_kill == NULL){
        printf("Process with PID #%d cannot be found.\n" ,pid);
        return EXIT_FAILURE;
    }
    List* listItemRemove = priorityList(PCB_kill->priority);
    List_first(listItemRemove);
    List_search(listItemRemove, pComparatorFn, &pid);
    List_remove(listItemRemove);
    free(PCB_kill);
    total_proc--;
    printf("PCB PID# %d removed.\n", pid);
    pcb_next();
    return EXIT_SUCCESS;
}

int pcb_exit() {
    int curr_pid = pcb_curr->pid;
    if(pcb_curr == pcb_init){
        free(pcb_init);
        printf("Killing the initial process. Exiting the simulation...\n");
        termination_val = -1;
        return EXIT_SIGNAL;
    }
    free(pcb_curr);
    total_proc--;
    pcb_next();
    printf("Current process (pid: %d) is killed.\n", curr_pid);
    printf("The new current running process is: PID #%d\n",pcb_curr->pid);
    return EXIT_SUCCESS;
}

void list_pcb_free(void* pcb){
    free((PCB*)pcb);
}
void pcb_terminate(){
    FREE_FN pFreeFn = &list_pcb_free;
    List_free(list_ready_high, pFreeFn);
    List_free(list_ready_norm, pFreeFn);
    List_free(list_ready_low, pFreeFn);
    List_free(list_waiting_send, pFreeFn);
    List_free(list_waiting_receive, pFreeFn);
}

int pcb_quantum(){

    int currPID = pcb_curr->pid;
    int currPriority = pcb_curr->priority;
    PCB* preQuantumPCB = pcb_curr;
    pcb_next();
    int nextPID = pcb_curr->pid;
    if(currPID == 0){
        printf("Exiting from initial process.\nThe current active process is PCB #%d", nextPID);

    }
    else{
        List_prepend(priorityList(currPriority), preQuantumPCB);
        printf("PCB #%d is put back into %s. \nThe current active process is PCB #%d", currPID, priorityChar(currPriority), nextPID);

    }
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

    //search for the PCB we want to send msg to
    PCB* receiver = pcb_search(pid);
    if(receiver == NULL){
        printf("Cannot find process.\n");
        return FAILURE;
    }

    if(receiver->listWaitState == SEND){
        printf("PID chosen is in wait send queue! Send failed.\n");
        return FAILURE;
    }

    msgPCB* item = malloc(sizeof(msgPCB));
    item->msg = msg;
    item->PID = pcb_curr->pid;
    receiver->msgPCBItem = realloc(item, sizeof(msgPCB));
    printf("Message sent to PCB PID #%d. ", receiver->pid);

    if(receiver->listWaitState == RECEIVE){
        receiver->state = READY;
        List_prepend(priorityList(receiver->priority), receiver);
        List_first(list_waiting_receive);
        if(List_search(list_waiting_receive, pComparatorFn, &receiver->pid) == NULL){
            printf("Error searching for the PCB in waiting receive queue!\n");
            return FAILURE;
        }
        List_remove(list_waiting_receive);
        printf("Its state is currently: RUNNING,");
        printf("\n");
    }

    if(pcb_curr->pid != 0){
        pcb_curr->state = BLOCKED;
        pcb_curr->listWaitState = SEND;
        List_prepend(list_waiting_send, pcb_curr);
        printf("PCB PID #%d is now in waiting send queue.\n", pcb_curr->pid);
        pcb_next();
    }
    return SUCCESS;

}

int pcb_receive(){
    if(pcb_curr == pcb_init){
        printf("Initial PCB cannot receive messages.\n");
        return FAILURE;
    }
    if(pcb_curr->msgPCBItem == NULL){
        pcb_curr->state = BLOCKED;
        pcb_curr->listWaitState = RECEIVE;
        List_prepend(list_waiting_receive, pcb_curr);
        printf("PCB PID #%d is in BLOCKED state waiting to receive a message.\n",pcb_curr->pid);
        pcb_next();
        return SUCCESS;
    }
    pcb_curr->listWaitState = NONE;
    pcb_curr->state = READY;
    printf("Message sent from PID #%d to PID #%d: %s. Message will be cleared after this command.\n",
           pcb_curr->msgPCBItem->PID, pcb_curr->pid, pcb_curr->msgPCBItem->msg);
    free(pcb_curr->msgPCBItem);
    pcb_curr->msgPCBItem = NULL;
    return SUCCESS;
}

int pcb_reply(int pid, char* msg){
    COMPARATOR_FN pComparatorFn = &list_comparator;

    //search for the PCB we want to send msg to
    PCB* receiver = pcb_search(pid);
    if(receiver == NULL){
        printf("Cannot find process.\n");
        return FAILURE;
    }

    if(receiver->listWaitState == RECEIVE){
        printf("PID chosen is in wait receive queue! Reply failed.\n");
        return FAILURE;
    }

    msgPCB* item = malloc(sizeof(msgPCB));
    item->msg = msg;
    item->PID = pcb_curr->pid;
    receiver->msgPCBItem = realloc(item, sizeof(msgPCB));
    printf("\nMessage replied to PCB PID #%d. ", receiver->pid);

    if(receiver->listWaitState == SEND){
        receiver->state = READY;
        List_prepend(priorityList(receiver->priority), receiver);
        List_first(list_waiting_send);
        if(List_search(list_waiting_send, pComparatorFn, &receiver->pid) == NULL){
            printf("Error searching for the PCB in waiting send queue!\n");
            return FAILURE;
        }
        List_remove(list_waiting_send);
        printf("Its state is currently: RUNNING,");
        printf("\n");
    }

    return SUCCESS;

}
int pcb_create_semaphore(int sid, int init){
    if(sid < 0 || sid >= SEM_MAX){
        printf("Invalid sid. The value sid will be from 0 to 4.\n");
        return FAILURE;
    }

    if(semaphore[sid] == NULL){
        semaphore[sid] = malloc(sizeof(Semaphore));
        if(semaphore[sid] == NULL){
            printf("Failed to allocate memory for semaphore of the given sid: %d\n", sid);
            return FAILURE;
        }

        semaphore[sid]->val = init;
        semaphore[sid]->plist = List_create();
    }else{
        printf("Semaphore with the given sid (%d) already exists. Try again.\n", sid);
        return FAILURE;
    }
    return SUCCESS;
}

int pcb_P(int sid){
    // Check for the validity of sid
    if(sid < 0 || sid >= SEM_MAX){
        printf("Invalid sid. The value sid will be from 0 to 4.\n");
        return FAILURE;
    }

    // Implementation based on the lecture note
    semaphore[sid]->val--;
    if(semaphore[sid]->val < 0){
        List_prepend(semaphore[sid]->plist, pcb_curr);
        pcb_next(); // switch the currently running process to the next process available
    }

}

int pcb_V(int sid){
    // Check for the validity of sid
    if(sid < 0 || sid >= SEM_MAX){
        printf("Invalid sid. The value sid will be from 0 to 4.\n");
        return FAILURE;
    }

    PCB* pcb_wake;

    semaphore[sid]->val++;
    if(semaphore[sid]->val <= 0){
        pcb_wake = List_trim(semaphore[sid]->plist);
        if(pcb_wake == NULL){
            printf("There is no process to unblock for the semaphore of sid: %d\n", sid);
            return FAILURE;
        }
        List_prepend(priorityList(pcb_wake->priority), pcb_wake);   // wake up a process in pList and add it to the correct ready list
    }
}

char* stateChar(enum pcb_states states){
    switch(states){
        case RUNNING:
            return "Running";
        case BLOCKED:
            return "Blocked";
        case READY:
            return "Ready";
        default:
            perror("Error in parsing state in character form!");
            exit(FAILURE);
    }
}

void pcb_procinfo(int pid){
    PCB* temp = pcb_search(pid);
    printf("\nProcess PID#%d is in state: '%s' and is in queue %s(%d).\n",
           temp->pid, stateChar(temp->state), priorityChar(temp->priority), temp->priority);
    if(temp->listWaitState == NONE){
        printf("It is not in a wait/receive state.\n");
    }
    if(temp->listWaitState == SEND){
        printf("It is in SEND state.\n");
    }
    if(temp->listWaitState == RECEIVE){
        printf("It is in RECEIVE state.\n");
    }


}

void totalinfo_helper(List* list, int priority){
    char* priorityCharOutput = priorityChar(priority);
    PCB* pcb_temp = List_first(list);
    printf("Processes with priority %s: ", priorityCharOutput);
    for(int i = 0; pcb_temp != NULL; i++){
    printf("%d ", pcb_temp->pid);
    pcb_temp = List_next(list);
}
    printf(".\n");
}

void pcb_totalinfo(){
    printf("\nTotal information about the system:\n");
    if(pcb_curr != pcb_init){
        printf("The current active process: PID# %d, priority: %s(%d).\n",
               pcb_curr->pid, priorityChar(pcb_curr->pid), pcb_curr->priority);
    }
    else{
        printf("The initial process is currently running.\n");
    }
    totalinfo_helper(list_ready_high, 0);
    totalinfo_helper(list_ready_norm, 1);
    totalinfo_helper(list_ready_low, 2);


    PCB* pcb_waitingSend = List_first(list_waiting_send);
    printf("Processes in waiting send list: ");
    for(int i = 0; pcb_waitingSend != NULL; i++){
        printf("%d ", pcb_waitingSend->pid);
        pcb_waitingSend = List_next(list_waiting_send);
    }
    printf(".\n");

    PCB* pcb_waitingReceive = List_first(list_waiting_receive);
    printf("Processes in waiting send list: ");
    for(int i = 0; pcb_waitingReceive != NULL; i++){
        printf("%d ", pcb_waitingReceive->pid);
        pcb_waitingReceive = List_next(list_waiting_receive);
    }
    printf(".\n");

    //need to add semaphore
    if(semaphore == NULL)
        return;
    for (int i = 0; i < SEM_MAX; i++){
        if(semaphore[i] != NULL){
            PCB* sem_blocked = List_first(semaphore[i]->plist);
            while(sem_blocked != NULL){
                printf("Process (pid: %d) is currently blocked by semaphore (sid: %d)\n", sem_blocked->pid, i);
                sem_blocked = List_next(semaphore[i]->plist);
            }
        }
    }
}
