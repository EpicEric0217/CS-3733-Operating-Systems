/*
 * File: myio.c
 * Eric Samuel Huddleston
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "myio.h"
#include<ctype.h>
#include <limits.h>
#include<sys/types.h>
#include<unistd.h>
 
char *ReadStdInput(int fd){
	char buff[BUFSIZ];
	int i;
	char *input;
	int r, ret, x;
	r = 0;
	ret = 1;
	i = 1;
	input = calloc(BUFSIZ, sizeof(char));
	while(ret > 0) {
		ret = read(fd, &buff, BUFSIZ);
		
		for (x = 0; x < BUFSIZ; x++) {
			if(buff[x] == '\n' || buff[x] == EOF) {
				ret=-1;
				break;
			}
			input[x*i] = buff[x];
			r++;
		}
		
		i++;
		
		if (ret != -1)
			input = realloc(input, BUFSIZ*i);
	}
	if (r == 0)
		return NULL;
	input[r] = '\0'; //inserting \0 instead of \n 
	input = realloc(input, r + 1); 
	return(input);
}
	
int ReadInteger()
{
  int gng, number, x;
  char *input; //pointer to char, input variable
  gng = 0;  
  do {
	  gng = 0;
	  
	  printf("Please input an integer\n");
	  input = ReadStdInput(STDIN_FILENO); //passing in default stdin file descriptor number to input function; instantiate input variable to this.
	  for (x = 0; x < INT_MAX; x++) {
		  if(x == 0 && input[x] == '-') //if x is zero and the input is a dash
			  continue;
		  if(input[x] == 0) //if the whole input array is zero..
			  break;
		  else if (input[x] < '0' || input[x] > '9') { //if the input is out of the range 0 - 9...
			  gng = 1; 
			  printf("Improper input..\n");
			  break;
		  }
	  }
  }
  while (gng == 1 );
  number = atoi(input); //using C library function 'atoi' to read in a (char* type) C-string as input parameter and return an 
  //integer.
  free(input); //deallocates the memory of input variable  
  return number; //instead of returning zero. 
}

double ReadDouble(void)
{
  char *input; //for the user input
  int gng;
  int exp, decr;
  int x;
  double number; 
  do {
	  gng = 0; //instantiate variables
	  decr = 0;
	  exp = 0; 
	  
	  printf("Please input a double\n");
	  input = ReadStdInput(STDIN_FILENO); //passing in default stdin file descriptor number to input function; instantiate input variable to this.
	  for(x = 0; x < INT_MAX; x++) {
		  if(x == 0 && input[x] == '-') //if x hasn't changed and the input is a dash 
			  continue;
			  if(input[x] == 0) //if input array as a whole is zero
				  break;
			  else if(decr == 0 && input[x] == '.')  //if decr is unchanged and the input is a dot; since doubles have '.'
			  //seperating the numbers.
				  decr = 1;
			  else if(exp == 0 && x != 0 && (input[x] == 'E' || input[x] == 'e')) {  
				  exp = 1;
				  decr = 1;
			  } 
			  else if (input[x] < '0' || input[x] > '9') { //if the numbers don't fall within the range 0 - 9...
				  gng = 1;
				  printf("Improper input\n");
				  break;
			  }
	  }
  }
  while (gng == 1);
  number = strtod(input, NULL); 
  free(input); //making sure to free dynamically allocated memory
  return number; //instead of 0.0 since number is of type double  
  
}

char *ReadLine(void)
{ 
   printf("Please input a line\n");
   return(ReadStdInput(STDIN_FILENO)); 
} 


char *ReadLineFile(FILE *infile)
{
  int x;
  char *input = calloc(BUFSIZ, sizeof(char)); //allocate and zero-initialize input array.
  if(fgets(input, BUFSIZ, infile)) //read line from infile stream, stop when BUFSIZ-1 characters read, point to input array
  {
	  for(x = 0; input[x] != '\0' && x < BUFSIZ; x++) //if we do not see '\n' char before reading the 100th character.
		  if(input[x] == '\n') //if we encounter the '\n' char
			  break;    // then break
		  input[x] = '\0';   // insert '\0' into xth position of array. 
		  return input;
  }
  else 
  {
	  return NULL;
  }
  
}
 
