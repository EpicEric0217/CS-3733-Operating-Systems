/*
 * File: driver2.c
 * Eric Samuel Huddleston
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myio.h"


int main(int argc, char *argv[])
{
int ix1, ix2;
int stop;
char chr;
char *inputLine, *outputLine;
FILE *infile, *outfile;

if(argc != 3) //check for the proper number of arguments
{
	printf("usage: %s <input_file> <output_file>\n", argv[0]);
	return 1;
}

//opening the files
infile = fopen(argv[1], "r");
if(infile == NULL)
{
	printf("input file %s could not be opened\n", argv[1]);
	return 1;
}

outfile = fopen(argv[2], "w");
if(outfile == NULL)
{
	printf("output file %s couldn't be opened\n", argv[2]);
	return 1;
}

while((inputLine = ReadLineFile(infile)) != NULL)
{
	outputLine = calloc(strlen(inputLine), sizeof(char));
	ix1 = 0;
	ix2 = 0;
	stop = 0;
	while(!stop)
	{
		chr = inputLine[ix1++];
		//copy all of the none-whitespace characters
		while(chr != ' '  && chr != '\t')
		{
			if(chr == '\0')
			{
				stop = 1;
				break;
			}
			outputLine[ix2++] = chr;
			chr = inputLine[ix1++];
		}
		
		if(!stop)
		{
			//insert a single space in the output line
			outputLine[ix2++] = ' ';
			
			
			//skip over all the whitespace characters
			while(inputLine[ix1] == ' ' || inputLine[ix1] == '\t')
				ix1++;
		}
		else 
		{
			outputLine[ix2] = '\0';
			fprintf(outfile, "%s\n", outputLine);
		}
		
	}
	
	//free all the allocated space
	free(inputLine);
	free(outputLine);
}

fclose(infile);
fclose(outfile);

printf("Generated output file %s\n", argv[2]);
}
			
			
   
