/*
 * File: driver1.c
 * Eric Samuel Huddleston
 */

#include <ctype.h>
#include <fcntl.h> //used to read and write files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "myio.h"

int main(int argc, char *argv[]) 
{
  //here I declare the largest variables
  int int_largest = 0; 
  double double_largest = 0.0;
  char *string_largest = (char*) malloc(1024 * sizeof(char));
  //here I convert to ints.
  int intgrs = atoi(argv[1]);
  int dbls = atoi(argv[2]);
  int lines = atoi(argv[3]);
  
  
  
   int x; //counter variable for all 3 loops
   int numb; //represent the integer type, instantiate to respective myio.h function.
   for (numb = 0; numb < intgrs; numb++){ //one loop for each myio.h function, each prints repective values for each. 
	   printf("please input an integer:\n");
	   x = ReadInteger(); //calling the ReadInteger func. from myio.h file
	   if(x > int_largest) {
		   int_largest = x;
	   }
   }
   
   double d; //represent the double type, instantiate to respective myio.h function.
    for (numb = 0; numb < dbls; numb++){
	   printf("please input a double:\n");
	   d = ReadDouble();
	   if(d > double_largest) {
		   double_largest = d; 
	   }
   }
   
   char *r; //represent the char. pointer type for strings, instantiate to respective myio.h function.
   for (numb = 0; numb < lines; numb++){
	 printf("please input a string:\n");
	   r = ReadLine();
	   if(strlen(r) > strlen(string_largest)){
		 strcpy(string_largest,r);  
	      free(r); //have to free r since in ReadLine the newline character is not stored as part of the string. 
		}
    }
	/*Here I will print the final result of the largest values*/
	printf("The largest integer is: %d\n", int_largest);
	printf("The largest double is: %f\n", double_largest);
	printf("The longest string is: %s\n", string_largest);

}


