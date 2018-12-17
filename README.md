# Operating_Systems_Projects
This repository consists of projects that are based on operating systems

The projects included in this repository are:
1. System Functions: This project involves using LINUX system functions such as fork(), getpid(), getppid(), wait() and system().
2. Pipe and Fork: This project involves using the LINUX system function pipe() as well as fork(). We will have three processes which communicate with each other using pipes.  The three processes will be a parent, child, and grandchild. Their communication will run in a ring.
3. Microshell simulation: In this project a microshell has been implemented with the ability to use a pipe. We have a pipe symbolized by a '||'. The program uses pipes, forks, dup, execvp function.
4. Process Scheduling: In this project we simmulate priority scheduling of processes on a single-processor system, without preemption. The main principle of the project is that when a process ends its CPU burst (or input burst or output burst), it is succeeded by the highest-priority process that is waiting.
5. Process Synchronization: In this Project, I used POSIX threads, semaphores and a mutex to illustrate the Producer-Consumer Problem. There are several consumer threads and several producer threads and one buffer. Each producer creates widgets and inserts them into the buffer, one at a time. Each consumer removes widgets from the buffer, one at a time. 
6. Memory Management: In this project, we are simulating memory management.  When a process is loaded into memory or a process requests a block of memory dynamically, the system must allocate memory, and when a process terminates or frees memory previously requested, the system must  deallocate memory. For the sake of this project, I assumed that I have a small computer with only 16 MB of memory. Also assuming that the operating system uses the first 3 MB, leaving 13 MB for applications.  Starting at that point, we will process several kinds of transactions:
  a.  Load a process into memory
  b.  Allocate a block of memory dynamically
  c.  Deallocate a block obtained earlier
  d.  Terminate a process, freeing all its memory
7. File Allocation: This project involves maintaining a File Allocation Table. I simulated "FAT-12" on a (very small) disk. Each block will be 512 bytes in size. With FAT-12, I have 4096 blocks, so the disk is only 2 MB in size. Starting with an empty root directory, a series of transactions were carried out. From time to time I printed the contents of the directory and (part) of the FAT.
