#ifndef A3_PCB_H
#define A3_PCB_H
#include "list.h"

#define SUCCESS 0
#define FAILURE -1
#define MAX_LEN 40



enum pcb_states {
    RUNNING,
    READY,
    BLOCKED
};

enum pcb_priorities{
    HIGH,
    NORM,
    LOW
};

typedef struct pcb_str PCB;
struct pcb_str{
    int pid;                    // a process ID (pid)
    int priority;               // priority (0, 1, or 2)
    enum pcb_states state;      // state of the process (running, ready, blocked)
    char* msg;                  // place to store any msgs another process "sends"s or "reply"s to this process
};

typedef struct Semaphore_s Semaphore;
struct Semaphore{
    int sid;
    int val;
};

int pcb_initialize();

List* priorityList(int priority);

int pcb_create(int priority);

int pcb_fork();

int pcb_kill(int pid);

int pcb_exit();

void pcb_terminate();

void pcb_quantum();

int pcb_send(int pid, char* msg);

int pcb_receive();

int pcb_reply(int pid, char* msg);

int pcb_create_semaphore(int sid, int init);

int pcb_P(int sid);

int pcb_V(int sid);

void pcb_procinfo(int pid);

void pcb_totalinfo();

void pcb_next();

#endif //A3_PCB.H
