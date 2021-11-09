#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

struct PCB *Head = NULL;
struct PCB *Tail = NULL;
int total_Waiting_Time = 0;
int total_Turnaround_Time = 0;
int total_job = 0;
int CPUreg[8] = {0};
int CLOCK = 0;

char print;
int count = 0;
int maxcount = 0;
int arraydigits[100];

struct PCB { 
	int ProcPR;
	int ProcID;
	int Cpuburst;
	int Reg[8];
	int waitingTime;
	int queueEnterClock;
	/*other needed info.*/
	struct PCB *next;
};

void append(struct PCB** head_ref, int ProcessID, int ProcessPrior, int CPUBurstT) 
{
	struct PCB* node_new = (struct PCB*) malloc(sizeof(struct PCB));
	struct PCB *previous = *head_ref;
	node_new->ProcID = ProcessID;
	node_new->ProcPR = ProcessPrior;
	node_new->Cpuburst = CPUBurstT;
	node_new->waitingTime = 0;
	node_new->queueEnterClock = 0;
	node_new->Reg[8] = ProcessID;
	node_new->next = NULL; 
	
	if (*head_ref == NULL) { //if the pointer to head_ref, which was equivalent to the struct variable *previous, is NULL, then..
		*head_ref = node_new;
		return;	
	}
	
	while (previous->next != NULL)  // while pointer to the next structure from the struct variable *previous exists
		previous = previous -> next; //if next process is not NULL then we set the previous process to the next process, defined by a struct variable...		
	previous->next = node_new; //if next process is not NULL then we set the next process struct variable equal to the new node.
	return;
}

void printAlgo(char *fileName)
{
	printf("Student Name : Eric Huddleston \n");
	printf("Input File Name : %s \n",fileName);
}
	
void printPrefM()
{
	float AWT = (float)total_Waiting_Time / (float)total_job; //to store the average waiting time we divide the total waiting time by the total job time.
	float AvgTT = (float)total_Turnaround_Time / (float)total_job; //to store the average turnaround time we divide the total turnaround time by the total job time.
	float Time = (float)total_job / (float)CLOCK; //the time it takes to execute process = time take to execute the total job / clock time
	printf("\nAverage Waiting time = %.2f ms (%d/%d)\n", AWT, total_Waiting_Time, total_job);
	printf("Average Turnaround time = %.2f ms (%d/%d)\n", AvgTT, total_Turnaround_Time, total_job);
	printf("Throughput = %.2f jobs per ms	(%d/%d)\n", Time, total_job, CLOCK);
}
	
void removeLinkedList(struct PCB **head_ref, int PID)
{
	struct PCB* tmp = *head_ref, *prev;
	if(tmp != NULL && tmp->ProcID == PID)
	{
		*head_ref = tmp->next;	
		free(tmp);
		return;
	}
	while (tmp != NULL && tmp->ProcID != PID)
	{
		prev = tmp;	
		tmp = tmp->next;
	}
	if (tmp == NULL) return;
	prev->next = tmp->next;
	free(tmp);
}
	
void postProcessing(struct PCB *pcb){
	total_Waiting_Time =  total_Waiting_Time + pcb -> waitingTime; //set the total waiting time equal to the total waiting time plus the value of the pcb struct variable, the value of the PCB,
	//then set to the waiting time struct variable using the c arrow operator.
	CLOCK = CLOCK + pcb -> Cpuburst; //the clock time for processor is equal to the clock time plus the struct variable pcb, value of the PCB, which is then set to the Cpuburst. 
	//This makes sense since the cpu burst is the "speed" of the cpu and also the time it takes to execute programs, a.ka. the process control board. 
	total_Turnaround_Time = total_Turnaround_Time + CLOCK;
	total_job =  total_job + 1; 
}

void FIFO_Scheduling()
{
	struct PCB *pcb = Head;//set the pcb strucure variable to the head of the linked list, the pointer to the first node.
	int calcltburst = 0; //represents the calculation of the cpu burst
	while(pcb != NULL) { //while the pcb struct variable is not NULL, in other words while their is active process (PCB).	
		pcb->waitingTime = calcltburst;
		pcb->queueEnterClock = CLOCK;
		calcltburst = calcltburst + pcb->Cpuburst; //the calcltburst variable is equal to the value of the pcb plus the calcltburst set to the Cpuburst struct variable, while PCB is active
		printf("Process %d is completed at %d ms\n", pcb->ProcID, calcltburst); //print respective process # and the cpu burst time it is completed at.
		postProcessing(pcb);
		removeLinkedList(&Head, pcb->ProcID); 			 //then we remove the value of the address of the head of the linked list and the process #, ensuring the first process is the first to be out, 
		pcb = pcb->next; //then we set the pcb to the pcb struct variable set to the next pcb process (through the struct), using the c arrow operator.
		//then loop through.
	}
}
	
void SJF_Scheduling()
{		
	struct PCB *pcb = Head; 
	int totalProcess = 0;
	while(pcb != NULL){
		totalProcess++;
		pcb = pcb->next;
	}
	int calcltburst = 0;
	while(totalProcess>0){
		struct PCB *it = Head;
		int minimumBurst = it->Cpuburst;
		int shortestJobProcId = it->ProcID;
		struct PCB *min = it;
		while(it != NULL){
			if(minimumBurst > it->Cpuburst){
				min = it;
				minimumBurst = it->Cpuburst;
				shortestJobProcId = it->ProcID;
			}
			it = it->next;
		}
		min->waitingTime = calcltburst;
		min->queueEnterClock = CLOCK;
		calcltburst = calcltburst + min->Cpuburst;
		printf("Process %d is completed at %d ms\n", min->ProcID, calcltburst);
		postProcessing(min);
		removeLinkedList(&Head, min->ProcID);	
		totalProcess--;	
	}
}	

void PR_Scheduling()
{
	struct PCB *pcb = Head; 
	int totalProcess = 0;
	while(pcb != NULL){
		totalProcess++;
		pcb = pcb->next;
	}
	int calcltburst = 0;
	while(totalProcess>0){
		struct PCB *it = Head;
		int maxPriority = it->ProcPR;
		int maxPriorityProcId = it->ProcID;
		struct PCB *max = it;
		while(it != NULL){
			if(maxPriority < it->ProcPR){
				max = it;
				maxPriority = it->ProcPR;
				maxPriorityProcId = it->ProcID;
			}
			it = it->next;
		}
		max->waitingTime = calcltburst;
		max->queueEnterClock = CLOCK;
		calcltburst = calcltburst + max->Cpuburst;
		printf("Process %d is completed at %d ms\n", max->ProcID, calcltburst);
		postProcessing(max);
		removeLinkedList(&Head, max->ProcID);	
		totalProcess--;	
	}
}

int RR_Scheduling(int quantum)
{
	if(quantum <= 0){ // If quantum is 0 or negative, infinite loop so it wont finish executing ever.
		printf("Error (Quantum must be greater than 0.)\n");
		return -1;
	}
	struct PCB *pcb = Head; //initialize pcb structure variable 
	int totalProcess = 0; //this variable will hold the number of processes in the queue
	while(pcb != NULL){
		pcb->waitingTime = 0; //Set waiting time for each process initially as 0. 
		totalProcess++; //increment variable totalProcess for each process in linked list.
		pcb = pcb->next; //increment process to next element in linked list.
	}
	int calcltburst = 0; //represents the calculation of the cpu burst
	while(totalProcess>0) {
		if(pcb == NULL){ //Reset pcb to Head of linked list when we reach end of queue in round robin manner.
			pcb = Head;
		}
		// Debugging print statements. These were used to pinpoint the error while writing code.
		// printf("Initial waiting time: %d\n",pcb->waitingTime);
		// printf("Initial calcBurst: %d\n",calcltburst);
		// printf("Initial queueEnterClock: %d\n", pcb->queueEnterClock);

		if(pcb->Cpuburst <= quantum){ //When Cpuburst left in process is less than quantun then execute these statements.
			pcb->waitingTime = pcb->waitingTime + calcltburst - pcb->queueEnterClock; // Update waiting time of process. 
			pcb->queueEnterClock = CLOCK;
			calcltburst = calcltburst + pcb->Cpuburst; // CpuBurst calculations.
			//printf("Waiting time : %d\n",pcb->waitingTime); // Debugging print statement.
			printf("Process %d is completed at %d ms\n", pcb->ProcID, calcltburst); 
			postProcessing(pcb); // Postprocessing as process has finished executing.
			removeLinkedList(&Head, pcb->ProcID); //Remove process from queue as this process has finished executing.			 
			pcb = pcb->next; // Point to next process
			totalProcess--; // Since this process has finished, decrement totalProcess by 1.
		}
		else{
			pcb->waitingTime = pcb->waitingTime + calcltburst - pcb->queueEnterClock; // Update waiting time of process.
			//printf("Waiting time for process %d is now: %d\n",pcb->ProcID,pcb->waitingTime); // Debugging print statement.
			pcb->queueEnterClock = calcltburst + quantum; // This is used to store the time at which process finished executing last time.
			CLOCK = CLOCK + quantum; //Update Clock to increment by size of quantum.
			pcb->Cpuburst = pcb->Cpuburst - quantum; // Reduce CpuBurst by quantum since this much process has already executed right now.
			calcltburst = calcltburst + quantum; // Cpu burst calculation. Increase by quantum as process just executed.
			pcb = pcb->next; // Point to next process in the queue
		}
	}
}

int main(int argc, char *argv[])
{
	if(argc < 4) { //if there are two few arguments...
		printf("Error (Not enough arguments)\n");
		return -1;
	}
	// Index to keep track of input arguments	
	int i = 0;
	// Flags to keep track of scheduling algo being used
	int runFIFO = 0;
	int runSJF = 0;
	int runPR = 0;
	int runRR = 0; 
	// Variable to store input file name.
	char *inputptr;
	int inputspc = 0;
	int algospc = 0;
	// Variable to store size of quanta in case of Round robin.
	int quantum = 0;
		
	while(i < argc) {
			
		if(strcmp("-alg", argv[i]) == 0)
		{
			algospc = 1;
			i++;
			if(strcmp("SJF", argv[i]) == 0) 
			//if the user inputed arguments is equal to 'SJF' we set to 0, from strncmp() function. If this happens then we set the variable to run the SJF 
			//function to 1, in order to trigger the if statement to call the SJF_Scheduling() function.
			{
				runSJF = 1;
			}
			else if(strcmp("PR", argv[i]) == 0) 
			//if the user inputed arguments is equal to 'PR' we set to 0, from strncmp() function. If this happens then we set the variable to run the PR
			//function to 1, in order to trigger the if statement to call the PR_Scheduling() function.
			{
				runPR = 1;
			}
			else if(strcmp("FIFO", argv[i]) == 0) 
			//if the user inputed arguments is equal to 'FIFO' we set to 0, from strncmp() function. If this happens then we set the variable to run the FIFO 
			//function to 1, in order to trigger the if statement to call the FIFO_Scheduling() function.
			{
				runFIFO = 1;
			}
			else if(strcmp("RR", argv[i]) == 0) 
			//if the user inputed arguments is equal to 'RR' we set to 0, from strncmp() function. If this happens then we set the variable to run the RR
			//function to 1, in order to trigger the if statement to call the RR_Scheduling() function.
			{
				runRR = 1;
			}
			else{
				printf("Error (Invalid algo)\n");
				return -1;
			}
		}
		else if (strcmp("-quantum", argv[i]) == 0)
		{
			i++;
			quantum = atoi(argv[i]);
		}
		else if(strcmp("-input", argv[i]) == 0)
		{
			inputspc = 1;
			i++;
			inputptr = argv[i];
		}
		i++;
	}
	
	if(algospc == 0)
	{
		printf("Error (algo not specified)\n");
		return -1;
	}

	if(inputspc == 0)
	{
		printf("Error (input file not specified)\n");
		return -1;
	}
		
	FILE *fp; //file pointer
	fp=fopen(inputptr, "r");
	print = fgetc(fp);
	while(print != EOF) {
		if(isdigit(print))
		{
			int x = print - '0';
			arraydigits[count] = x; 		
			count++;
		}			
		print = fgetc(fp);
	}
	fclose(fp);
		
	maxcount = count; 
	count = 0;
		
	while(count+2 < maxcount) {	
		append(&Head,arraydigits[count],arraydigits[count + 1],arraydigits[count + 2]);	
		count += 3;
	}
		
	printAlgo(inputptr);
		
	if(runSJF == 1) { 
		//if the runSJF variable was increased previously, then we run the SJF scheduling function.
		printf("CPU Scheduling Algo : SJF \n");
		SJF_Scheduling();		
	}
	else if(runFIFO == 1) { 
		//if the runFIFO variable was increased previously, then we run the FIFO scheduling function.
		printf("CPU Scheduling Algo : FIFO \n");
		FIFO_Scheduling();
	}
	else if(runPR == 1) { 
		//if the runPR variable was increased previously, then we run the PR scheduling function.
		printf("CPU Scheduling Algo : PR \n");
		PR_Scheduling();			
	}
	else if(runRR == 1) {
        //if the runRR variable was increased previously, then we run the RR scheduling function.
		printf("CPU Scheduling Algo : RR \n");
        RR_Scheduling(quantum);
    }

	printPrefM();
	return 0;
}
