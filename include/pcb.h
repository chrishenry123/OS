// pcb.c Header File

struct pcb{
	char name[64]; // 64 max name length
	int classType; // 0 for user application, 1 for system process
	int priority;
	int exState; // 0 for ready, 1 for running, 2 for blocked
	int disState; // 0 for suspended, 1 for not suspended
	unsigned char stack[1024]; // 1024 stack area bytes
	unsigned char *stackPtr; // Pointer to current stack location
} pcb;

struct pcb* pcb_allocate(void);

int pcb_free(struct pcb*);

struct pcb* pcb_setup(const char*, int, int);

struct pcb* pcb_find(const char*);

void pcb_insert(struct pcb*);

int pcb_remove(struct pcb*);
