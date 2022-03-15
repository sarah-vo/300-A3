#ifndef A3_PCB.H
#define A3_PCB.H

#define SUCCESS 0
#define FAILURE -1
#define MAX_LEN 40

List* list_ready_high;
List* list_ready_norm;
List* list_ready_low;

List* list_waiting_send;
List* list_waiting_receive;

PCB* pcb_init;
PCB* pcb_curr;

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

void pcb_initialize();

#endif //A3_PCB.H
