#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>
using namespace std;

struct Process {
    int processID;
    int arrivalTime;
    int waitTime;
    int serviceTime;
    int interArrivalTime;
};

int main () {
    srand(time(0));
    //creating a fixed array to store the 1000 processes
    const int MAX_PROCESSES = 1000;
    Process processArray[MAX_PROCESSES];

    double result = static_cast<double>(rand()) / RAND_MAX;

    //using inverse-transform formula to get exponential inter-arrival times
    double const lambda = 2.0;
    double const serviceLambda = 1.0;
    double interTime = ((log(1 - result)) / lambda) / -1;

    //converting inter-arrival time to int ms
    interTime *= 1000;
    interTime = static_cast<int>(round(interTime));

    //creating a for-loop to store interArrivalTime, processID, serviceTime, arrivalTime data for each Process
    for (int i = 0; i < MAX_PROCESSES; i++) {
        double result = static_cast<double>(rand()) / RAND_MAX;
        double interTime = ((log(1 - result)) / lambda) / -1;
        interTime *= 1000;
        interTime = static_cast<int>(round(interTime));
        processArray[i].interArrivalTime = interTime;
        processArray[i].processID = i + 1;

        //calculating service time for the current process
        double resultService = static_cast<double>(rand()) / RAND_MAX;
        double servTime = ((log(1 - resultService)) / serviceLambda) / -1;
        servTime *= 1000;
        servTime = static_cast<int>(round(servTime));

        //condtion so every process has a service time
        if (servTime < 1) {
            servTime = 1;
        }
        processArray[i].serviceTime = servTime;

        //calculating arrival time
        if (i == 0) {
            processArray[i].arrivalTime = processArray[i].interArrivalTime;
        }
        else {
            processArray[i].arrivalTime = processArray[i-1].arrivalTime + processArray[i].interArrivalTime;
        }
    }

    //calculating final arrival time and the arrival rate
    double lastArrivalTime = static_cast<double>(processArray[MAX_PROCESSES - 1].arrivalTime) / 1000;
    double avgArrivalRate = static_cast<double>(MAX_PROCESSES) / lastArrivalTime;

    //calculating total Inter-Arrival Time and total Service Time
    long long int totalInterTime = 0;
    long long int totalServTime = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        totalInterTime += processArray[i].interArrivalTime;
        totalServTime += processArray[i].serviceTime;
    }
    //calculating average service time and converting it to seconds
    double avgServiceTime = static_cast<double>(totalServTime) / MAX_PROCESSES;
    double avgServiceTimeSecs = avgServiceTime / 1000;

    //calculating average total Inter-Arrival Time and Total Service Time in ms
    double avgTotalInterTime = static_cast<double>(totalInterTime) / MAX_PROCESSES;
    double avgTotalServTime = static_cast<double> (totalServTime) / MAX_PROCESSES;

    //creating array to hold processes waiting to begin
    int queueLine[MAX_PROCESSES];
    int queueFront = 0;
    int queueRear = 0;

    //variables used to track state of simulation
    int currentTime =0;
    int nextProcess = 0;
    int currentProcess = -1;
    int serviceTimeLeft = 0;

    //variables for detecting changes in running process
    int previousProcess = -1;
    int rangeStart = 0;

    //array for storing beginning and end range of processes
    const int MAX_RANGES = (MAX_PROCESSES * 2) + 1;
    int rangeBegin[MAX_RANGES];
    int rangeEnd [MAX_RANGES];
    int rangeProcess [MAX_RANGES];
    int rangeCount = 0;

    //while loop to run simulation
    while (nextProcess < MAX_PROCESSES || queueFront < queueRear || currentProcess != -1) {
        //adding
        while (nextProcess < MAX_PROCESSES && processArray[nextProcess].arrivalTime <= currentTime) {
            queueLine[queueRear] = nextProcess;
            queueRear++;
            nextProcess++;
        }
        //If cpu IDLE, start next process
        if (currentProcess == -1 && queueFront < queueRear) {
            currentProcess = queueLine[queueFront];
            queueFront++;

            processArray[currentProcess].waitTime = currentTime - processArray[currentProcess].arrivalTime;
            serviceTimeLeft = processArray[currentProcess].serviceTime;
        }
        //used for detecting change in current process or cpu state "BUSY/IDLE"
        if (currentProcess != previousProcess) {
            //storing previous cpu range
            if (rangeStart < currentTime) {
            rangeBegin[rangeCount] = rangeStart;
            rangeEnd[rangeCount] = currentTime;
            rangeProcess[rangeCount] = previousProcess;
            rangeCount++;
            }

            //new cpu range at the current simulation time
            rangeStart = currentTime;
            previousProcess = currentProcess;
        }
        if (currentProcess != -1) {
            serviceTimeLeft--;
        }
        //setting to IDLE when current process finishes and no new process needs to be started yet
        if (currentProcess != -1 && serviceTimeLeft == 0) {
            currentProcess = -1;
        }
        currentTime++;

        //storing CPU state
        if (currentProcess != previousProcess) {
            rangeBegin[rangeCount] = rangeStart;
            rangeEnd[rangeCount] = currentTime;
            rangeProcess[rangeCount] = previousProcess;
            rangeCount++;

            rangeStart = currentTime;
            previousProcess = currentProcess;
        }
    }
    //storing any cpu range that may remain after simulation is done
    if (rangeStart < currentTime) {
        rangeBegin[rangeCount] = rangeStart;
        rangeEnd[rangeCount] = currentTime;
        rangeProcess[rangeCount] = previousProcess;
        rangeCount++;
    }

    //calculating average wait tim for all processes
    long long int totalWaitTime = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        totalWaitTime += processArray[i].waitTime;
    }
    double avgWaitTime = static_cast<double>(totalWaitTime) / MAX_PROCESSES;

    //calculating the total turn around time
    long long int totalTurnAroundTime = 0;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        totalTurnAroundTime += (processArray[i].waitTime + processArray[i].serviceTime);
    }
    double avgTotalTurnAroundTime = static_cast<double>(totalTurnAroundTime) / MAX_PROCESSES;

    //calculating overall CPU utilization
    double cpuUtil = (static_cast<double>(totalServTime) / currentTime) * 100;

    //calculating throughput
    double totalSimulationTime = static_cast<double>(currentTime) / 1000;
    double throughPut = static_cast<double>(MAX_PROCESSES) / totalSimulationTime;

    //printing the Generated Statistics
    cout << "Time (in ms)" << setw(11) << " | " << setw(10) << "CPU Status" << " | " << setw(4) << "PID" << endl;
    cout << "---------------------|------------|-----" << endl;

    for (int i = 0; i < rangeCount; i++) {
        //cout << right;
        cout << "[" << setw(8) << rangeBegin[i] << ", " << setw(8) << rangeEnd[i] << ") | ";
        //cout << left;
        if(rangeProcess[i] == -1) {
            cout << setw(10) << "IDLE" << " | " << setw(4) << "" << endl;
        }
        else {
            cout << setw(10) << "BUSY" << " | " << setw(4) << processArray[rangeProcess[i]].processID <<endl;
        }
    }

    //printing general statistics
    cout << "\n**Generation Statistics**\n";
    cout << left << setw(32) << "Statistics" << "| Results" << endl;
    cout << "--------------------------------" << "|------------------------------" << endl;
    cout << left << setw(32) << "Expected Average Arrival Rate" << "| " << lambda << " processes per second" << endl;
    cout << left << setw(32) << "Computed Average Arrival Rate" << "| " << avgArrivalRate << " processes per second" << endl;
    cout << left << setw(32) << "Expected Average Service Time" << "| " << 1.0 << " seconds" << endl;
    cout << left << setw(32) << "Computed Average Service Time" << "| " << avgServiceTimeSecs << " seconds" << endl;



    //printing averages and Overall Statistics
    cout << "\n**Simulation Statistics**\n";
    cout << left << setw(32) << "Statistics" << "| Results" << endl;
    cout << "--------------------------------" << "|------------------------------" << endl;
    cout << left << setw(32) << "Total Complete Time" << "| " << currentTime << " ms" << endl;
    cout << left << setw(32) << "Average Turnaround Time" << "| " << avgTotalTurnAroundTime << " ms" << endl;
    cout << left << setw(32) << "Average Waiting Time" << "| " << avgWaitTime << " ms" << endl;
    cout << left << setw(32) << "Overall CPU Utilization" << "| " << cpuUtil << " %" << endl;
    cout << left << setw(32) << "Overall System Throughput" << "| " << throughPut << " processes/second" << endl;

    return 0;
}
