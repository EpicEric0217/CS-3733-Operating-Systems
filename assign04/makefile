JCC = javac
JFLAGS = -g

all: parallel_sort.c ParallelSort.java 
	gcc  parallel_sort.c -pthread
	javac ParallelSort.java


clean:
	$(RM) a.out
	$(RM) *.class
