#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<pthread.h>
//#include<sys/wait.h>
int main(int argc, char** argv) 

{

int Pipe1[2];

int Pipe2[2];

int Pipe3[2];

int Pipe4[2];

char msg[] = "abcd";

char strg_input[100];

pid_t p,q;

if (pipe(Pipe1)==-1) //using the pipe() system call, if -1 is returned indicates an error.
	
	{
		
		fprintf(stderr, "Pipe was failed..");
		
		return 1;
		
	}
	

if (pipe(Pipe2)==-1)
	
	{
		
		fprintf(stderr, "Pipe was failed..");
		
		return 1;
		
	}	
	
	
if (pipe(Pipe3)==-1)
	
	{
		
		fprintf(stderr, "Pipe was failed..");
		
		return 1;
		
	}


if (pipe(Pipe4)==-1)
	
	{
		
		fprintf(stderr, "Pipe was failed..");
		
		return 1;
		
	}
	
	p = fork();
	
	if (p < 0) { //if the call to "fork()" function fails
		
		fprintf(stderr, "the fork Failed.");
		
		return 1;
		
	}
	
	
	else if (p == 0) {

		char concat_strg[100]; //to represemt the concatenated string later
		
		char fixed_strg[100];
		
		close(Pipe1[0]); //close reading; end of the 1st pipe
		
		printf("\n\tPlease enter message:");
		
		scanf("%s", concat_strg); //read in user input for the concatenated string
		
		write(Pipe1[1], concat_strg, strlen(concat_strg)+1);
		
		//Here we will concatenate a fixed string  along with it 
		
		int q = strlen(concat_strg);
		
		int i;
		
		for (i=0; i<strlen(fixed_strg); i++) {
			
			concat_strg[q++] = fixed_strg[i];
			
		}
		
		concat_strg[q] = '\0'; //if the string ends with '\0'
		
		//Then we will close both of the writing ends 
		
		close(Pipe1[1]);
		
		wait(NULL); //wait until any child of the process terminates
		
//*************************************************************************************

close(Pipe2[1]);

read(Pipe2[0], concat_strg, 100);

if(strcmp(concat_strg,"invalid")==0) {
	
	printf("\n\tmessage not sent");
	
}

else {
	
	printf("\n\tmessage was sent to prog_2(child_2).");
	
}

close(Pipe2[0]); //closing the reading end of the second pipe

exit(0); //exit success

	}
	
	else {
		
		close(Pipe1[1]); //clsoing the writing end of the 1st pipe
		
		char concat_strg[100];
		
		read(Pipe1[0], concat_strg, strlen(concat_strg)+1);
		
		close(Pipe1[0]);
		
		close(Pipe2[0]);//close the writing end of the 2nd pipe
		
		if(strcmp(concat_strg,"invalid")==1) {
			
			//if not then...
			
			write(Pipe2[1], "invalid", sizeof(concat_strg));
			
			return 0; //then we return zero
			
		}
		
		else {
			
			//if yes then...
			
			write(Pipe2[1], "valid", sizeof(concat_strg));
			
			close(Pipe2[1]);
			
			pid_t k;
			
			k=fork();//create child process #2
			
			if(k>0) {
				
				close(Pipe3[0]); /*close read head offd3[] */
				
				write(Pipe3[1], concat_strg, sizeof(concat_strg)); //here we write message by monitor(main process) by usikng fd3[1]
				
				close(Pipe3[1]);
				
				wait(NULL); //wait until child_process_2 send the ACK
				
				//*****************************************************************************
				
				close(Pipe4[1]);
				
				read(Pipe4[0], concat_strg, 100);
				
				close(Pipe4[0]);
				
				if(strcmp(concat_strg,"ack")==0) {
					
					printf("The message of child process_1 is recieved by child process_2");
					
				}
				
				else {
					
					printf("The message of child process_1 is NOT recieved by child process_2");
					
				}
				
			}
			
			else {
				
				if(p<0)
					
					{
						
						printf("Child_Process_2 not created...");
						
					}
					
					else {
						
						close(Pipe3[1]);//close the writing end of the 1st pipe 
						
						char concat_strg[100];
						
						read(Pipe3[0], concat_strg, strlen(concat_strg)+1);
						
						close(Pipe3[0]);
						
						close(Pipe4[0]);//now we close the writing end of the second pipe
						
						write(Pipe4[1], "ack", sizeof(concat_strg));
						
					}
					
			}
			
		}
		
		close(Pipe2[1]); //finally we close the writing end of the second pipe.
		
	}
	
	
}
