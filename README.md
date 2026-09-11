# assignent1
## Synthetic Process Workload Generation and Simulation
The purpose of this program is to implement a discrete workload generator that synthesizes the execution of <ins>**_1000 distinct processes_**</ins>. 
### Process Generation
Every created process is assigned a process ID, inter-arrival time, arrival time, and service time. The inter-arrival and service times are generated using an exponential distribution with inverse transform method.

The program uses an average arrival rate of <ins>**_2.0 processes per second_**</ins> and an average service time of <ins>**_1.0 second_**</ins>. The generated times are then converted to integer milliseconds.

### FIFO CPU Scheduling
This program will use a <ins>**_FIFO_**</ins> scheduling method to keep track of which process was received first and will be handled by the CPU next.

IF the CPU is busy and a process arrives and the waiting queue is empty, it will be put at the front of the queue. Any process that arrives with a process already in the waiting queue, it is put in the back of the queue behind the previous process. If the CPU is idle and there are processes in the queue, the process at the front of the queue is selected to run.

### CPU Simulation
Simulation starts at 0 milliseconds(ms), with the CPU initially idle, it will advance 1ms at a time. When a process is selected from the waiting queue, its service time will begin. This will keep track of how long the process used the CPU.

If another process is waiting in the FIFO queue, the process at the front of the queue will then be selected to use the CPU next. If no processes are waiting, the CPU will remain idle until another process arrives.

### Simulation Output
The program will display the status of the CPU in the simulation. Each output range will show the beginning and ending time in ms, whether the CPU was <ins>**_BUSY_**</ins> or <ins>**_IDLE_**</ins>, and the process ID (PID) of the process using the CPU.

When the CPU is busy, the PID of the process currently using the CPU will be displayed. When the CPU is idle, no PID will be displayed.



