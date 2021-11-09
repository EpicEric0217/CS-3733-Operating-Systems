Simulator.java

import java.util.LinkedList;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


public class Simulator {

   private FileReader readerThread;
   private CPU cpuThread;
   private IO ioThread;
   private String fileName;
   private Algorithm algorithm;

   /*
   * Process queues and lists
   */
   public volatile LinkedList<Process> readyQueue = new LinkedList<>();
   public volatile LinkedList<Process> ioQueue = new LinkedList<>();
   public volatile LinkedList<Process> doneList = new LinkedList<>();

   /*
   * Synchronization info
   */
   public volatile int totalProcesses = 0;
   public volatile int finishedProcesses = 0;
   // public final Lock statLock = new ReentrantLock();
   // public final Lock readyLock = new ReentrantLock();
   // public final Lock ioLock = new ReentrantLock();
   public final Lock lock = new ReentrantLock(true);
   public final Semaphore doneFlag = new Semaphore(0);

   /*
   * Process statistics
   */

   public Simulator(String fileName, Algorithm algorithm, int quantum) {
      
       if (quantum <= 0 && algorithm == Algorithm.RR) {
           System.err.println("Invalid quantum specified for RR");
           System.exit(-1);
       }
      
       this.fileName = fileName;
       this.algorithm = algorithm;
       readerThread = new FileReader(this, fileName);
       cpuThread = new CPU(this, algorithm, quantum);
       ioThread = new IO(this);

   }


   public void startSimulation() {

       //System.err.println("Beginning sim...");

       cpuThread.start();
       ioThread.start();
       readerThread.start();

       //System.err.println("Threads started");

       try {
           doneFlag.acquire(3);
       } catch (InterruptedException e) {
           e.printStackTrace();
       }

       //System.err.println("All threads done");

       //System.err.println("Simulation complete");

       long totalCPUTime = 0;
       long totalWaitingTime = 0;
       long totalIOTime = 0;
       long totalTime = 0;

       // All threads have completed execution
       for (Process p : doneList) {
           totalCPUTime += p.cpuTime;
           totalWaitingTime += p.waitingTime;
           totalIOTime += p.ioTime;
           totalTime += p.turnaroundTime;
           //System.out.println(p);
       }

       double averageTurnaround = (double) totalTime / doneList.size();
       double throughputPerSecond = 1000 / averageTurnaround;
       double avgWait = (double) totalWaitingTime / doneList.size();

       System.out.printf("Input File Name     : %s\n", fileName);
       System.out.printf("CPU Scheduling Alg : %s\n", algorithm);
       System.out.printf("CPU utilization     : %.2f%%\n", ((double) totalCPUTime / totalTime) * 100);
       System.out.printf("Throughput          : %.2f processes per second\n", throughputPerSecond);
       System.out.printf("Turnaround time     : %.2f ms (average) %d ms (total)\n", averageTurnaround, totalTime);
       System.out.printf("Waiting time        : %.2f ms (average) %d ms (total)\n", avgWait, totalWaitingTime);

   }

}


Algorithm.java


public enum Algorithm {
  
   FIFO,
   SJF,
   PR,
   RR

}

CPU.java


public class CPU extends Thread {

   Simulator sim;
   Algorithm algorithm;
   int quantum;

   /**
   *
   * @param algorithm
   * @param quantum
   */
   public CPU(Simulator sim, Algorithm algorithm, int quantum) {
       this.sim = sim;
       this.algorithm = algorithm;
       this.quantum = quantum;
   }

   @Override
   public void run() {

       Process process;

       while (sim.readyQueue.isEmpty()) {
           try {
               Thread.sleep(1);
           } catch (InterruptedException e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
           }
       }

       for (;;) {

           if (!sim.readyQueue.isEmpty()) {

               switch (algorithm) {
               case FIFO:
                   /**
                   * Pick the first process
                   */

                   synchronized (sim.readyQueue) {
                       process = sim.readyQueue.removeFirst();
                   }
                   //System.out.println("choose process " + process.processID + " to run");

                   try {
                       Thread.sleep(process.burst());
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }

                   process.turnaroundTime += process.burst();
                   process.cpuTime += process.burst();

                   // The time that this process used should be added to the
                   // waiting time of any other processes in the readyQueue
                   synchronized (sim.readyQueue) {

                       for (Process p : sim.readyQueue) {
                           p.waitingTime += process.burst();
                           p.turnaroundTime += process.burst();
                       }

                       //System.out.println("process " + process.processID + " cpu bursted for " + process.burst());

                   }

                   // Prepare process for next io burst
                   process.nextBurst();

                   if (process.done) {
                       sim.finishedProcesses++;
                       synchronized (sim.doneList) {
                           //System.out.println("process " + process.processID + " done!");
                           sim.doneList.add(process);
                       }
                   } else {
                       synchronized (sim.ioQueue) {
                           sim.ioQueue.add(process);
                           //System.out.println("sent process " + process.processID + " to ioqueue");
                       }
                   }

                   break;
               case SJF:
                   /**
                   * Pick the shortest burst time first
                   */

                   synchronized (sim.readyQueue) {

                       process = sim.readyQueue.peekFirst();

                       for (Process p : sim.readyQueue) {
                           if (p.burst() < process.burst()) {
                               process = p;
                           }
                       }

                       sim.readyQueue.remove(process);

                   }

                   try {
                       Thread.sleep(process.burst());
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
                  
                   process.turnaroundTime += process.burst();
                   process.cpuTime += process.burst();
                  
                   // The time that this process used should be added to the
                   // waiting time of any other processes in the readyQueue
                   synchronized (sim.readyQueue) {

                       for (Process p : sim.readyQueue) {
                           p.waitingTime += process.burst();
                           p.turnaroundTime += process.burst();
                       }

                       //System.out.println("process " + process.processID + " cpu bursted for " + process.burst());

                   }

                   process.nextBurst();

                   if (process.done) {
                       // process.turnaroundTime = System.currentTimeMillis() -
                       // process.startTimestamp;
                       sim.finishedProcesses++;
                       synchronized (sim.doneList) {
                           sim.doneList.add(process);
                       }
                   } else {
                       synchronized (sim.ioQueue) {
                           sim.ioQueue.add(process);
                       }
                   }

                   break;
               case PR:
                   /**
                   * Choose highest priority process
                   */
                   synchronized (sim.readyQueue) {

                       process = sim.readyQueue.peekFirst();

                       for (Process p : sim.readyQueue) {
                           if (p.priority < process.priority) {
                               process = p;
                           }
                       }

                       sim.readyQueue.remove(process);

                   }

                   try {
                       Thread.sleep(process.burst());
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
                  
                   process.turnaroundTime += process.burst();
                   process.cpuTime += process.burst();
                  
                   // The time that this process used should be added to the
                   // waiting time of any other processes in the readyQueue
                   synchronized (sim.readyQueue) {

                       for (Process p : sim.readyQueue) {
                           p.waitingTime += process.burst();
                           p.turnaroundTime += process.burst();
                       }

                       //System.out.println("process " + process.processID + " cpu bursted for " + process.burst());

                   }

                   process.nextBurst();

                   if (process.done) {
                       // process.turnaroundTime = System.currentTimeMillis() -
                       // process.startTimestamp;
                       sim.finishedProcesses++;
                       synchronized (sim.doneList) {
                           sim.doneList.add(process);
                       }
                   } else {
                       synchronized (sim.ioQueue) {
                           sim.ioQueue.add(process);
                       }
                   }
                  
                   break;
               case RR:
                   /**
                   * First-come-first-serve, however each burst is limited to
                   * min(quantum, p.burst()). If there is still burst left, we
                   * keep the process here and add it back to the end of the
                   * ready queue
                   */
                  
                   synchronized (sim.readyQueue) {
                       process = sim.readyQueue.removeFirst();
                   }
                  
                   int burst = Math.min(quantum, process.burst());

                   try {
                       Thread.sleep(burst);
                   } catch (InterruptedException e) {
                       e.printStackTrace();
                   }
                  
                   process.turnaroundTime += burst;
                   process.cpuTime += burst;
                  
                   // The time that this process used should be added to the
                   // waiting time of any other processes in the readyQueue
                   synchronized (sim.readyQueue) {

                       for (Process p : sim.readyQueue) {
                           p.waitingTime += burst;
                           p.turnaroundTime += burst;
                       }

                       //System.out.println("process " + process.processID + " cpu bursted for " + process.burst());

                   }

                   process.bursts[process.burstIndex] -= burst;
                  
                   if (process.burst() < 0) {
                       System.err.println("Internal error: below 0 burst time");
                       System.exit(-1);
                   } else if (process.burst() == 0) {          
                       // CPU burst is done, send process to ioQueue
                      
                       process.nextBurst();
                      
                       if (process.done) {
                           // process.turnaroundTime = System.currentTimeMillis() -
                           // process.startTimestamp;
                           sim.finishedProcesses++;
                           synchronized (sim.doneList) {
                               sim.doneList.add(process);
                           }
                       } else {
                           synchronized (sim.ioQueue) {
                               sim.ioQueue.add(process);
                           }
                       }
                      
                   } else {
                       // CPU burst isn't done, put this process at the end of the queue
                      
                       synchronized (sim.readyQueue) {
                           sim.readyQueue.add(process);
                       }
                      
                   }      
                  
                   break;
               }

           }

           if (sim.finishedProcesses == sim.totalProcesses) {
               break;
           }

       }

       sim.doneFlag.release();

   }

}

CPUScheduler.java

import java.io.IOException;

public class CPUScheduler {
  
   public static void main(String[] args) throws IOException {
      
       /*
       * Possible command line flags:
       *
       * required: -alg [FIFO|SJF|PR|RR] (2)
       * required: -input [file name] (2)
       * optional: -quantum [int milliseconds] (2)
       *
       * We could do some more robust command line argument parsing here
       * but that's not the point of this assignment...
       */
      
       if (args.length < 4 || args.length > 6) {
           usageExit("Invalid arguments");
       }
      
       String algorithmStr = null;
       Algorithm algorithm;
       String fileName = null;
       int quantum = -1;
      
       for (int i = 0; i < args.length; i++) {
           switch (args[i]) {
           case "-alg":
               if (i + 1 == args.length) {
                   usageExit("No algorithm specified. Options: [FIFO|SJF|PR|RR]");
               }
               algorithmStr = args[i + 1];
               i++;
               break;
           case "-input":
               if (i + 1 == args.length) {
                   usageExit("No input file name specified");
               }
               fileName = args[i + 1];
               i++;
               break;
           case "-quantum":
               if (i + 1 == args.length) {
                   usageExit("No quantum time specified");
               }
               try {
                   quantum = Integer.parseInt(args[i + 1]);
               } catch (NumberFormatException e) {
                   usageExit("Quantum must be an integer in milliseconds");
               }
               i++;
               break;
           default:
               usageExit("Invalid arguments");
           }
       }
      
       if (algorithmStr == null) {
           usageExit("Algorithm flag is required");
       }
      
       algorithm = Algorithm.valueOf(algorithmStr);
      
       if (fileName == null) {
           usageExit("Input flag is required");
       }
      
       Simulator simulator = new Simulator(fileName, algorithm, quantum);
       simulator.startSimulation();
      
   }
  
   public static void usageExit(String message) {
       System.err.println(message);
       System.err.println("Usage: java CPUScheduler -alg [FIFO|SJF|PR|RR] [-quantum [integer(ms)]] -input [file name]");
       System.exit(-1);
   }

}

FileReader.java

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.util.LinkedList;

public class FileReader extends Thread {

   File inputFile;
   Simulator sim;

   /**
   * Constructs a new FileReader to read input from the given file
   */
   public FileReader(Simulator sim, String fileName) {
       this.sim = sim;
       this.inputFile = new File(fileName);
   }

   @Override
   public void run() {

       /*
       * Input file format:
       *
       * proc <priority> <<cpu burst> <io burst>> ... (repeated) <cpu burst>
       * sleep <milliseconds> stop
       *
       */
       try {
           Files.lines(inputFile.toPath()).forEachOrdered(line -> {

               line = line.trim();

               if (!line.isEmpty()) {

                   // System.out.println(line);

                   String[] parse = line.split("\\s+");

                   switch (parse[0]) {
                   case "proc":

                       synchronized (sim.readyQueue) {

                           //System.out.println("added process " + sim.totalProcesses + " to readyqueue");
                           sim.readyQueue.add(new Process(sim.totalProcesses, line));
                           sim.totalProcesses++;

                       }

                       break;
                   case "sleep":

                       int time = Integer.parseInt(parse[1]);
                       try {
                           Thread.sleep(time);
                       } catch (InterruptedException e) {
                           System.err.println("Interrupted file reader thread while sleeping!");
                           e.printStackTrace();
                       }

                       break;
                   case "stop":
                       sim.doneFlag.release();
                       return; // stop thread by returning from run
                   default:
                       System.err.println("Unknown command in input file");
                       System.exit(-1);
                   }

               }

           });
       } catch (IOException e) {
           // TODO Auto-generated catch block
           e.printStackTrace();
       }

       sim.doneFlag.release();

   }

}

IO.java


public class IO extends Thread {

   Simulator sim;

   /**
   */
   public IO(Simulator sim) {
       this.sim = sim;
   }

   @Override
   public void run() {

       Process process;

       while (sim.ioQueue.isEmpty()) {
           try {
               Thread.sleep(1);
           } catch (InterruptedException e) {
               // TODO Auto-generated catch block
               e.printStackTrace();
           }
       }

       for (;;) {

           if (!sim.ioQueue.isEmpty()) {

               synchronized (sim.ioQueue) {

                   process = sim.ioQueue.removeFirst();
                   //System.out.println("choose process " + process.processID + " for io");

               }

               try {
                   Thread.sleep(process.burst());
               } catch (InterruptedException e) {
                   // TODO Auto-generated catch block
                   e.printStackTrace();
               }

               process.turnaroundTime += process.burst();
               process.ioTime += process.burst();

               // The time that this process used should be added to the
               // waiting time of any other processes in the ioQueue
               synchronized (sim.ioQueue) {

                   for (Process p : sim.ioQueue) {
                       p.waitingTime += process.burst();
                       p.turnaroundTime += process.burst();
                   }

               }

               //System.out.println("process " + process.processID + " io bursted for " + process.burst());

               process.nextBurst();

               synchronized (sim.readyQueue) {
                   sim.readyQueue.add(process);
               }
              
               //System.out.println("sent process " + process.processID + " to readyqueue");

           }

           if (sim.finishedProcesses == sim.totalProcesses) {
               break;
           }

       }

       sim.doneFlag.release();

   }

}


Process.java


public class Process {

   public int processID;

   /**
   * Priority of this process, where 1 is the lowest priority and 10 is the
   * highest priority.
   */
   public volatile int priority;

   /**
   * Alternating CPU and IO burst numbers, always with an odd length and
   * ending with a final CPU burst.
   */
   public volatile int[] bursts;

   /**
   * Internal index representing which burst should be processed next.
   */
   public volatile int burstIndex = 0;

   /**
   * True if this process has completed all bursts.
   */
   public volatile boolean done = false;

   public final long startTimestamp = System.currentTimeMillis();

   public volatile long turnaroundTime = 0;
   public volatile long cpuTime = 0;
   public volatile long ioTime = 0;
   public volatile long waitingTime = 0;

   /**
   * Constructs a new Process from a "proc ..." command representing a virtual
   * process for simulation by the CPU and IO threads.<br>
   * <br>
   *
   * A proc command has the following format:<br>
   *
   * proc [priority] [[cpu burst] [io burst]] ... (repeated) [cpu burst]
   *
   * @param creationString
   *            A "proc ..." command to build this process
   */
   public Process(int processID, String creationString) {

       this.processID = processID;

       String[] split = creationString.split("\\s+");

       int finalBurst = Integer.parseInt(split[split.length - 1]);
       priority = Integer.parseInt(split[1]);

       bursts = new int[split.length - 2];

       int i;
       for (i = 2; i < split.length - 1; i++) {
           bursts[i - 2] = Integer.parseInt(split[i]);
       }

       bursts[i - 2] = finalBurst;

   }

   public int burst() {
       return this.bursts[burstIndex];
   }

   public void nextBurst() {
       this.burstIndex++;
       if (burstIndex >= bursts.length) {
           this.done = true;
       }
   }

   @Override
   public String toString() {

       return String.format("[id: %d, cpu: %d, io: %d, waiting: %d, turnaround: %d]", this.processID, this.cpuTime,
               this.ioTime, this.waitingTime, this.turnaroundTime);

   }

}
