#ifndef A3_PCB.H
#define A3_PCB.H

#define SUCCESS 0
#define FAILURE -1


enum pcb_states {
    RUNNING,
    READY,
    BLOCKED
};

typedef struct pcb_s PCB;
struct pcb_s{
    int pid;                    // a process ID (pid)
    int priority;               // priority (0, 1, or 2)
    enum pcb_states state;      // state of the process (running, ready, blocked)
    char* msg;                  // place to store any msgs another process "sends"s or "reply"s to this process
};

void pcb_initialize();

#endif //A3_PCB.H
