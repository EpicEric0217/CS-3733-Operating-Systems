/*
 * File: wordcount.c
 * Eric Samuel Huddleston
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int count_of_words(char *filename,int ppid);
int main(int argc, char *argv[])
{
//in order to store the process ID of the process 
int *pid_arr;
int i, count, status, pid, pid_parent, childcount;
if(argc < 2) //if the number of arguments is less than 2
{
printf("Usage: ./exename file1 file2 etc.. \n"); 
return -1;
}
//now we create the process which is equal to argc-1
pid =(int*)malloc((argc-1)*sizeof(int));
count = argc; //keeps track of the count of the number of arguments
pid_parent=getpid();
for(i = 1; i < count; i++) {
pid = fork(); //pid set equal to fork()
if(pid == 0) 
{
childcount++;
int word = count_of_words(argv[i], pid_parent); //making a variable to represent the calling of the count_of_words function
//with the given parameters
if(word > 0)
{
	if ( WIFEXITED(status) ) { //if wait() exits, pass status as a parameter...
	const int ex = WEXITSTATUS(status);
	printf("\nMain process created %d child processes to count words ", ex);
	printf("in %d files", count-1);
		}
	printf("\nChild process for %s :number of words is %d\n", argv[i], word);
	
	}
}
else
{
    
if(pid_parent == getpid()) //the parent process
wait(&status); //here we wait until all the child processes exit or return
//print statement to indicate the files have been counted successfully
printf("\n%d files have been counted successfully!", count-1);
		}

	}
// return 0;
}
int count_of_words(char *filename,int ppid)
{
FILE *fp;
int count = 0; 
char strg[20];
//we will execute this function only if child process of parent, 
//no grandchild of the parent is allowed to execute this function!!!
if(ppid == getppid())
{
	//printf("Main process created %d child processes to count words ", count);
	fp = fopen(filename, "r");
	if(fp == NULL){  //if the file pointer is NULL it means no file is present 
	printf("%s :does not exists \n", filename);
	return -1;
	}
	while(fscanf(fp,"%s",strg)!=EOF){ //until we reach the end of the file
		count++;
	}
	return count; //exit code for the count of the number of files
	
	}
	else{
		return -1;
	}
}
