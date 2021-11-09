//Java implementation of multithreaded app.: ParallelSort.java. Compiled with javac ParallelSort.java. Ran as 'java ParallelSort 5000' etc..
import java.util.*;

	class ThreadMerger extends Thread {
		private double [] arrin; //input arrayin
		
		private double [] arrout; //output array 
		
		private int midl; //used as dividing point into the sublists.
		
		public ThreadMerger(double [] i, double [] o, int m) {
			arrin = i;
			
			arrout = o;
			
			midl = m;
		}
		
		public void runThread() {
			int i, j, k;
			
			//System.out.println("Merged sublists " + arrin.length + " : ");
			
			i = k = 0; //initialize i and k to zero.
			
			j = midl;
			
			while(i < midl && j < arrin.length) { //use java string length method to return length of arrin's length
				if(arrin[i] < arrin[j])
					arrout[k++] = arrin[i++];
				else
					arrout[k++] = arrin[j++];
			}
			
			while(i < midl) 
				arrout[k++] = arrin[i++];
			while(j < arrin.length)
				arrout[k++] = arrin[j++];
			/*for(i = 0; i < arrin.length; i++)
            System.out.print(" " + arrout[i]);
            System.out.println(""); */
		}
	};
	
	class ThreadSorter extends Thread {
		private double [] arr; //the array which is to be sorted
		
		private int arrbeg, arrend;//starting and ending points in the array to be sorted.
		
		public ThreadSorter(double [] a, int b, int n) { //class constructor for ThreadSorter class, initialize the array objects.
			arr = a;
			
			arrbeg = b;
			
			arrend = n;
		}
		
		public void runThread() {
			double temp; //temporary variable 
			
			int i, j, minim;
			
			arr = arr;
			
			//System.out.println("Sort array from  " + arrbeg + " to " + arrend);
			
			for(i = arrbeg; i <arrend; i++) { //from the beggining to the end of the array
				minim = i;
				for(j = i + 1; j < arrend; j++) {
					if(arr[j] < arr[minim])
						minim = j;
				}
				temp = arr[i]; //the temp variable is set to the ith position of the sublist
				
				arr[i] = arr[minim]; //the ith position of the sublist is set to the 'minimum' element of the sublist
				
				arr[minim] = temp; //the 'minimum' element of the sublist is then set to the temporary variable.
			}
			/*for(i = arrbeg; i < arrend; i++)
            System.out.print(" " + arr[i]);
            System.out.println("\nEnd of sort"); */
		}
	};
	
	public class ParallelSort {
		
		public static void main(String []arg) {
			int i, N, midl;
			
			double [] arr; //first array
			
			double [] arrcop; //Used for a copy of array
			
			double [] mergedarr; //stores the third merged array.
			
			long start, end;
			
			if(arg.length != 1) {
				System.out.println("Usage: java ParallelSort <N>");
				
				return;
			}
			
			N = Integer.parseInt(arg[0]); //user Integer.parseInt java method to convert the user's input string to an integer and store in 'N'.
			
			arr = new double[N];
			
			arrcop = new double[N];
			
			mergedarr = new double[N];
			
			Random rand = new Random(System.currentTimeMillis());
			
			//System.out.println ("Array is: ");
			
			for(i = 0; i < N; i++) { /* generate  sequence of random numbers */
            arr[i] = arrcop[i] = rand.nextDouble() * 99.0 + 1.0; /* range is from 1.0 - 100.0 */
			
            //System.out.print(" "+ arr[i]);
			}
			//System.out.println("");

			//Now I am going to create the threads, first the two sorting threads, then the 'merging' thread 
			ThreadSorter sort1 = new ThreadSorter(arr, 0, N/2);
			
			ThreadSorter sort2 = new ThreadSorter(arr, N/2, arr.length);
			
			ThreadMerger merge1 = new ThreadMerger(arr, mergedarr, N/2);
			
			start = System.nanoTime(); //The start time; initialize start to value of most precise 'system' timer.
			
			/* Now I am going to 'start' the threads to begin their work on the sublists(sub-arrays).*/
			sort1.start();
			
			sort2.start();
			
			//Waiting for the sublist sorting to complete...
			try {
				sort1.join();
				
				sort2.join();
			} catch (Exception e) {
				e.printStackTrace(); //used to print Java.lang.Throwable class and other details, in this case of the exception 'e'.
			}
			
			/*Now I am going to start the thread for merging...*/
			merge1.start();
			/*Wait for the merger thread to finish...*/
		try {
			merge1.join();
		} catch (Exception e) {
				e.printStackTrace();
		}
		
		end = System.nanoTime() - start; //This will represent the ending time.
		
		System.out.println("Sorting is done within " + end/1000000.0f + "ms when two threads are utilized");
		
		ThreadSorter completeSort = new ThreadSorter(arrcop, 0, N); //For running along the entirety of the array.
		
		start = System.nanoTime(); //the start time..
		
		completeSort.start();
		
		try {
			completeSort.join();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		end = System.nanoTime() - start; //the end time...
		
		System.out.println("Sorting is done within " + end/1000000.0f + "ms when one single thread is utilized");
	}
} //compile with javac ParallelSort.java. run as 'java ParallelSort 5000' etc..
