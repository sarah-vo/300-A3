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
    int id;
    int val;

};

List* list_ready_high;
List* list_ready_norm;
List* list_ready_low;

List* list_waiting_send;
List* list_waiting_receive;

PCB* pcb_init;
PCB* pcb_curr;

int init();

int create(int priority);

int fork();

int kill(int pid);

void terminate();

void quantum();

int send(int pid, char* msg);

int receive();

int reply(int pid, char* msg);

int create_semaphore(int sid, int init);

int P(int sid);

int V(int sid);

void procinfo(int pid);

void totalinfo();

void pcb_next();

#endif //A3_PCB.H
