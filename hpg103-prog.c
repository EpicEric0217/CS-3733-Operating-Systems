//main.c
//compile with gcc hpg103-prog.c -pthread -o  hpg103-prog -std=c99
//program written by - Eric Samuel Huddleston, 7/26/2020, CS 3733 Section 01T
#include <stdio.h> 
#include <pthread.h> 
#include <errno.h> 
#include <limits.h> 
#include <stdlib.h> 
double BALANCE = 0.0; //global variable, should be volatile?
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
double convertToDouble(void *arg) {
	double numb;
	char *p;
	errno = 0;
	double convrt = strtod(arg, &p); //covert user-inputed argument to string, if address of p is not NULL, store in location referenced by &p.
	if (errno != 0 || *p != '\0' || convrt > INT_MAX) { } else { numb = convrt; return numb; }
}
int convertToInt(void *arg) {
	int numb;
	char *p;
	errno = 0;
	long convrt = strtol(arg, &p, 10); //covert initial part of string in arg to long int according to base of 10.
	if (errno != 0 || *p != '\0' || convrt > INT_MAX) { } else { numb = convrt; return numb; }
}
void *deposit(void *arg) {
	char **args = (char**)arg; //create an array of pointers to char pointers.
	int a = convertToInt(args[1]); //the # of deposits
	double b = convertToDouble(args[2]); //the amount of deposits
	pthread_mutex_lock( &mutex1 ); 
	for (int i = 0; i < a; i++) { BALANCE += b; }
	pthread_mutex_unlock( &mutex1 );
	return NULL;
}
void *withdraw(void *arg) {
	pthread_mutex_lock( &mutex1 );
	char **args = (char**)arg; //create an array of pointers to char pointers.
	int c = convertToInt(args[3]); //# of withdrawals
	double d = convertToDouble(args[4]); //the amount of withdrawals
	for (int i = 0; i < c; i++) { BALANCE -= d;}
	pthread_mutex_unlock( &mutex1 );
	return NULL;
}
void main (int argc, char *argv[]) {
	pthread_t v, w;
	pthread_create (&v, NULL, deposit,(void *) argv);
	pthread_create (&w, NULL, withdraw,(void *) argv);
	pthread_join(v, NULL);
	pthread_join(w, NULL);
	double totalData = convertToInt(argv[1]) * convertToDouble(argv[2]) - convertToInt(argv[3]) * convertToDouble(argv[4]); //to print a*b-c*d
	printf("BALANCE %lf %lf \n", BALANCE, totalData);
}
/* * * BALANCE == a*b-c*d since to get the total balance we need to multiply the amount of each deposit b by how many 
 * * * deposits in order to get the total dollar amount of deposits made then subtract the total dollar amount of withdrawals
 * * * (how many withdrawals, c, times the dollar amount of each withdrawal, d)
 * * * from that in order to yield the total remaining balance. Additionally it is worthy to note they way we actually
 * * * represent the progam data of the balance following this same formula except using the user inputed string arguments, which
 * * * had previously been converted by my functions to integers for a and c and to doubles for b and d, respectively. 
 * * * Testcase #1: 
 * * ./hpg103-prog 1000 3.5 500 2.8 
 * * BALANCE 2100.000000 2100.000000 
 * * * Testcase #2: 
 * * ./hpg103-prog 10000 10.8 5000 5.7 
 * BALANCE 79500.000000 79500.000000 */ 
