#ifndef A3_PCB_H
#define A3_PCB_H
#include "list.h"

#define SUCCESS 0
#define FAILURE -1
#define MAX_LEN 40
#define SEM_MAX 5



enum pcb_states {
    RUNNING,
    READY,
    BLOCKED
};

enum wait_receive {
    SEND,
    RECEIVE,
    NONE
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
    enum wait_receive waitReceiveState;
    char* msg;                  // place to store any msgs another process "sends"s or "reply"s to this process
};

typedef struct Semaphore_s Semaphore;

struct Semaphore_s{
    int val;            // integer representing a value of semaphore
    List* plist;     // a list of processes waiting for the semaphore
};

int pcb_initialize();

int pcb_create(int priority);

int pcb_fork();

int pcb_kill(int pid);

int pcb_exit();

void pcb_terminate();

int pcb_quantum();

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
