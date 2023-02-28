     
#include <iomanip>
#include <iostream>
#include <list>
#include <queue>
#include <vector>



using namespace std;

//-----------------------------------------------------------------------------
//--------------------------Create Process Class ------------------------------
//-----------------------------------------------------------------------------

class Process {
public:
  int processNumber;
  list<int> processData;
  int readyWaitQueue = 0;
  int ioBurstCompletedSoFar = 0;
  int cpuBurstCompletedSoFar = 0;
  int ioBurst = 0;
  int turnAroundTime = 0;
  int totalCPU = 0;
  int totalIO = 0;
  bool firstRun = true;
  int ResponseTime = 0;
};

class testNumbers {
public:
int processNumber;
int smallestNumber;
Process process;
int levelInQueue;
};

//-----------------------------------------------------------------------------
//------------------------------Process List-----------------------------------
//-----------------------------------------------------------------------------
//{CPU burst, I/O time, CPU burst, I/O time, CPU burst, I/O time,....., last CPU burst}
Process process1 = Process{1, list<int>{5, 27, 3, 31, 5, 43, 4, 18, 6, 22, 4, 26, 3, 24, 4}};
Process process2 = Process{2, list<int>{4, 48, 5, 44, 7, 42, 12, 37, 9, 76, 4, 41, 9, 31, 7, 43, 8}};
Process process3 = Process{3, list<int>{8, 33, 12, 41, 18, 65, 14, 21, 4, 61,15, 18, 14, 26, 5, 31, 6}};
Process process4 = Process{4, list<int>{3, 35, 4, 41, 5, 45, 3, 51, 4, 61, 5, 54, 6, 82, 5, 77, 3}};
Process process5 = Process{5, list<int>{16, 24, 17, 21, 5, 36, 16, 26, 7, 31, 13, 28, 11, 21, 6, 13, 3, 11, 4}};
Process process6 = Process{6, list<int>{11, 22, 4, 8, 5, 10, 6, 12, 7, 14, 9, 18, 12, 24, 15, 30, 8}};
Process process7 = Process{7, list<int>{14, 46, 17, 41, 11, 42, 15, 21, 4, 32, 7, 19, 16, 33, 10}};
Process process8 = Process{8, list<int>{4, 14, 5, 33, 6, 51, 14, 73, 16, 87, 6}};


//-----------------------------------------------------------------------------
//------------------------------QUEUE CREATION---------------------------------
//-----------------------------------------------------------------------------

vector<Process> QueueOne;
vector<Process> QueueTwo;
vector<Process> QueueThree;

vector<Process> IOQueue;
vector<Process> FinishedQueue;
vector<Process> tempVector;


//-----------------------------------------------------------------------------
//------------------------------INIT FUNCTIONS---------------------------------
//-----------------------------------------------------------------------------
void addAllProcessesToReadyQueue(){
        QueueOne.push_back(process1);
        QueueOne.push_back(process2);
        QueueOne.push_back(process3);
        QueueOne.push_back(process4);
        QueueOne.push_back(process5);
        QueueOne.push_back(process6);
        QueueOne.push_back(process7);
        QueueOne.push_back(process8);
}


//-----------------------------------------------------------------------------
//------------------------------OTHER VARIABLES--------------------------------
//-----------------------------------------------------------------------------

  double totalCPUUsage = 0;
  double cpuUtil = 0.0;
  double averageWaitingTime = 0.0;
  double totalWaitingTime = 0.0;
  double totalTurnAround = 0.0;
  double averageTurnAroundTime = 0.0;
  double totalResponseTime = 0.0;
  double averageResponseTime = 0.0;
//-----------------------------------------------------------------------------
//------------------------------OTHER FUNCTION--------------------------------
//-----------------------------------------------------------------------------

// Increments all the data in a queue up by 1
void q_inc_by_one(vector<Process>& inputQueue) {
    for (int i = 0, size = inputQueue.size(); i < size; i++) {
        Process grabProcess = inputQueue.front();
        grabProcess.readyWaitQueue = grabProcess.readyWaitQueue + 1;
        inputQueue.erase(inputQueue.begin());
        inputQueue.push_back(grabProcess);
    }
}

// Prints all the data in a queue
void q_print_all(queue<Process>& inputQueue) {
  cout << "INPUT QUEUE SIZE: " << inputQueue.size() << endl;
    for (int i = 0; i < inputQueue.size(); i++) {
        Process grabProcess = inputQueue.front();
        inputQueue.pop();
        //cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "Process #" << grabProcess.processNumber << right << setw(10) << "" << endl;
        cout << "Response Time: " << grabProcess.ResponseTime << right<< setw(10) << "" << endl;
        cout << "Time in Ready Queue: " << grabProcess.readyWaitQueue<< right << setw(10) << "" << endl;
        cout << "totalIO (totalIO): " << grabProcess.totalIO << right << setw(10) << ""<< endl;
        cout << "totalCPU (totalCPU): " << grabProcess.totalCPU << right << setw(10)<< "" << endl;
        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << endl;
        inputQueue.push(grabProcess);
    }
  }
// Prints all the data in a vector
void printVector(vector<Process> inputProcess){
      for (int i=0; i<inputProcess.size(); i++) {
        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << "Process #" << inputProcess[i].processNumber << right << setw(10) << "" << endl;
        cout << "Response Time: " << inputProcess[i].ResponseTime << right<< setw(10) << "" << endl;
        cout << "Time in Ready Queue: " << inputProcess[i].readyWaitQueue<< right << setw(10) << "" << endl;
        cout << "totalIO (totalIO): " << inputProcess[i].totalIO << right << setw(10) << ""<< endl;
        cout << "totalCPU (totalCPU): " << inputProcess[i].totalCPU << right << setw(10)<< "" << endl;
        cout << endl;
        cout << "-----------------------------------------------------" << endl;
        cout << endl;
        }
      }

// Clears the Screen
 void clearScreen() {
    cout << string(100, '\n');
  } 



//-----------------------------------------------------------------------------
//------------------------------START FUNCTIONS--------------------------------
//-----------------------------------------------------------------------------
void startMLFQ() {

//-----------------------------------------------------------------------------
//------------------------------QUEUE TWO --- TQ 5----------------------------
//-----------------------------------------------------------------------------

  
if(QueueOne.size() != 0){
  Process currentProcess = QueueOne.front();	// Gets A Process From The Front, Put's it in CurrentProcess
	QueueOne.erase(QueueOne.begin());	// Deletes From Front of ProcessOne Queue. 

  
  //--------------------- FIND RESPONSE TIME -----------------------
    if (currentProcess.firstRun == true) {
      currentProcess.ResponseTime = currentProcess.readyWaitQueue;
      currentProcess.firstRun = false;
      }
 //---------------------------------------------------------------     
  

  //----------------------- GET CPU VALUE -------------------------
    int cpuBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst

    int getRunTime=0;
  if (cpuBurst < 5){
    getRunTime = cpuBurst;
  } else if (cpuBurst >= 5) {
    getRunTime = 5;
  }

 //--------------------------------------------------------------- 
  for (int i=0; i<getRunTime; i++){                                      // Runs the CPU Burst
               currentProcess.totalCPU++;                     // Increments the Total Time The Program Is Running
               currentProcess.cpuBurstCompletedSoFar++;        //Keeps track of the CPU Bursts that have ran so far

    // ------------- While CPU is running, you must account for IO Queue waiting. -------------
    if(IOQueue.size() > 0){
            for (int b=0; b<IOQueue.size(); b++){              
                IOQueue[b].ioBurstCompletedSoFar = IOQueue[b].ioBurstCompletedSoFar + 1;
                IOQueue[b].totalIO = IOQueue[b].totalIO + 1;
               // If the IO Burst Completes while CPU is running, it will be added back to the ready queue.
              if (IOQueue[b].ioBurstCompletedSoFar == IOQueue[b].ioBurst){
                        IOQueue[b].ioBurst = 0;
                        IOQueue[b].ioBurstCompletedSoFar = 0;
                        QueueOne.push_back(IOQueue[b]);
                        IOQueue.erase(IOQueue.begin() + b );
                 }
               }
             } 
              q_inc_by_one(QueueOne);     // Increments all process in queue one wait time by one
              q_inc_by_one(QueueTwo);     // Increments all process in queue two wait time by one
              q_inc_by_one(QueueThree);   // Increments all process in queue three wait time by one
}
if (currentProcess.cpuBurstCompletedSoFar < cpuBurst){ // If the burst was not finished in queue one, then it must be pushed down to a lower queue (queue two)
  QueueTwo.push_back(currentProcess);
} else  { 
  // else, it finished it's burst and needs to go to IO
  currentProcess.processData.pop_front();              //I have to delete it from the list after I get it 
  currentProcess.cpuBurstCompletedSoFar = 0;
  if(currentProcess.processData.size()!=0){            //if the current process data size doesn't equal 0 
            currentProcess.ioBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst
            currentProcess.processData.pop_front();              //I have to delete it from the list after I get it (it will run)
            IOQueue.push_back(currentProcess);                    //push it into the IQ queue
           } else {                                            //Else, the entire process is finished, and it must be put into finished queue.
              FinishedQueue.push_back(currentProcess); 
        for(int i=0; i<FinishedQueue.size(); i++){
          FinishedQueue[i].turnAroundTime = FinishedQueue[i].totalIO + FinishedQueue[i].readyWaitQueue + FinishedQueue[i].totalCPU;
        }   
      }
    } 
  } 



//-----------------------------------------------------------------------------
//------------------------------QUEUE TWO --- TQ 10----------------------------
//-----------------------------------------------------------------------------
  
else if(QueueTwo.size() != 0 ) {

  Process currentProcess = QueueTwo.front();	// Gets A Process From The Front, Put's it in CurrentProcess
	QueueTwo.erase(QueueTwo.begin());	// Deletes From Front of Process Two Queue. 

    if (currentProcess.firstRun == true) {
      currentProcess.ResponseTime = currentProcess.readyWaitQueue;
      currentProcess.firstRun = false;
      }

  //----------------------- GET CPU VALUE -------------------------
    int cpuBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst
  int getRunTime=0;
  if (cpuBurst < 10){
    getRunTime = cpuBurst;
  } else if (cpuBurst >= 10) {
    getRunTime = 10;
  }
//--------------------------------------------------------------- 
  for (int i=0; i<getRunTime; i++){                                      // Runs the CPU Burst
               currentProcess.totalCPU++;                       // Increments the Total Time The Program Is Running
               currentProcess.cpuBurstCompletedSoFar++;         //Keeps track of the CPU Bursts that have ran so far

    
// ------------- While CPU is running, you must account for IO Queue waiting. -------------
    if(IOQueue.size() > 0){
            for (int b=0; b<IOQueue.size(); b++){              
                IOQueue[b].ioBurstCompletedSoFar = IOQueue[b].ioBurstCompletedSoFar + 1;
                IOQueue[b].totalIO = IOQueue[b].totalIO + 1;
               // If the IO Burst Completes while CPU is running, it will be added back to the ready queue.
              if (IOQueue[b].ioBurstCompletedSoFar == IOQueue[b].ioBurst){
                        IOQueue[b].ioBurst = 0;
                        IOQueue[b].ioBurstCompletedSoFar = 0;
                        QueueOne.push_back(IOQueue[b]);
                        IOQueue.erase(IOQueue.begin() + b );
                 }
               }
             } 
              q_inc_by_one(QueueTwo);     // Increments all process in queue two wait time by one
              q_inc_by_one(QueueThree);   // Increments all process in queue three wait time by one
}
if (currentProcess.cpuBurstCompletedSoFar < cpuBurst){ // if the cpu burst isn't done here, add it to the lower queue
  QueueThree.push_back(currentProcess);
} else  { // else, it's done, and it should be sent to IO
  currentProcess.processData.pop_front();              //I have to delete it from the list , as the CPU burst is done
    currentProcess.cpuBurstCompletedSoFar = 0;
  if(currentProcess.processData.size()!=0){  //if the current process data size doesn't equal 0 
            currentProcess.ioBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst
            currentProcess.processData.pop_front();              //I have to delete it from the list after I get it (it will run)
            IOQueue.push_back(currentProcess);                    //push it into the IO queue
    } else {  
          //else it's done, and it will go into the Finished Queue
              FinishedQueue.push_back(currentProcess); 
        for(int i=0; i<FinishedQueue.size(); i++){
          FinishedQueue[i].turnAroundTime = FinishedQueue[i].totalIO + FinishedQueue[i].readyWaitQueue + FinishedQueue[i].totalCPU;
        }   
      }
    } 

} 


//-----------------------------------------------------------------------------
//-------------------------- QUEUE THREE ---- FCFS ----------------------------
//-----------------------------------------------------------------------------
else if(QueueThree.size() != 0) {
    Process currentProcess = QueueThree.front();	// Gets A Process From The Front, Put's it in CurrentProcess
	    QueueThree.erase(QueueThree.begin());;	// Deletes From Front of Ready Queue. 

   //--------------------- FIND RESPONSE TIME -----------------------
    if (currentProcess.firstRun == true) {
      currentProcess.ResponseTime = currentProcess.readyWaitQueue;
      currentProcess.firstRun = false;
      }
 //------------------------------------------------------------------

  
  //----------------------- GET CPU VALUE -------------------------
    int cpuBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst
    currentProcess.processData.pop_front();              //I have to delete it from the list , as the CPU burst is done
    int cpuToRun = cpuBurst - currentProcess.cpuBurstCompletedSoFar;
//--------------------------------------------------------------- 
  for (int i=0; i<cpuToRun; i++){                                      // Runs the CPU Burst
               currentProcess.totalCPU++;                     // Increments the Total Time The Program Is Running
               currentProcess.cpuBurstCompletedSoFar++;
    if(IOQueue.size() > 0){
            for (int b=0; b<IOQueue.size(); b++){              
                IOQueue[b].ioBurstCompletedSoFar = IOQueue[b].ioBurstCompletedSoFar + 1;
                IOQueue[b].totalIO = IOQueue[b].totalIO + 1;
              if (IOQueue[b].ioBurstCompletedSoFar == IOQueue[b].ioBurst){
                        IOQueue[b].ioBurst = 0;
                        IOQueue[b].ioBurstCompletedSoFar = 0;
                        QueueOne.push_back(IOQueue[b]);
                        IOQueue.erase(IOQueue.begin() + b );
                 }
               }
             } 
              q_inc_by_one(QueueThree);   // Increments all process in queue three wait time by one
}
    currentProcess.cpuBurstCompletedSoFar = 0;
  if(currentProcess.processData.size()!=0){  //if the current process data size doesn't equal 0 
            currentProcess.ioBurst = currentProcess.processData.front();   //I want to get the value for the CPU first burst
            currentProcess.processData.pop_front();              //I have to delete it from the list after I get it (it will run)
            IOQueue.push_back(currentProcess);                    //push it into the IO queue
        
           } else { 
        // Else it's finished it's bursts and will go into the Finished Queue.
              FinishedQueue.push_back(currentProcess); 
        for(int i=0; i<FinishedQueue.size(); i++){
          FinishedQueue[i].turnAroundTime = FinishedQueue[i].totalIO + FinishedQueue[i].readyWaitQueue + FinishedQueue[i].totalCPU;
        }   
      }
    } 

  	// This only runs if the CPU isn't running, but it has IO bursts that are running
else if (IOQueue.size() != 0){ 
                    int IOQueueNumber = 0;
   // Get the IO Burst that has already completed and run if it's not equal to the original IO burst.
                    while (IOQueue[IOQueueNumber].ioBurstCompletedSoFar != IOQueue[IOQueueNumber].ioBurst){
                        IOQueue[IOQueueNumber].ioBurstCompletedSoFar = IOQueue[IOQueueNumber].ioBurstCompletedSoFar + 1;
                        IOQueue[IOQueueNumber].totalIO = IOQueue[IOQueueNumber].totalIO + 1;
                      // Goes through and itterates through IO 
                        if (IOQueueNumber == IOQueue.size()-1){
                          IOQueueNumber = 0; 
                          } else {
                            IOQueueNumber++; 
                        }                
                    }
                       //  Put it back in ready queue when done.
                      QueueOne.push_back(IOQueue[IOQueueNumber]);
                      IOQueue.erase(IOQueue.begin() + IOQueueNumber);
                  }
  }


/*

void printCurrentQueue()
Description: Prints out the current queue status (what location is every process in).

*/
void printCurrentQueue() {
    cout << endl;
    cout << "==============Queue One LIST================" << endl;
    printVector(QueueOne);
    cout << "==============================================" << endl;
    cout << endl;
    cout << endl;
      cout << "==============Queue Two LIST================" << endl;
    printVector(QueueTwo);
    cout << "==============================================" << endl;
    cout << endl;
    cout << endl;
      cout << "==============Queue Three LIST================" << endl;
    printVector(QueueThree);
    cout << "==============================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "============IO WAITING QUEUE LIST=============" << endl;
    printVector(IOQueue);
    cout << "==============================================" << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << endl;
    cout << "============FinishedProcesses LIST============" << endl;

    for(int i=0; i<FinishedQueue.size(); i++){
      cout << "Process: " << FinishedQueue[i].processNumber << " finished at total time: " << FinishedQueue[i].turnAroundTime << endl;
    }

    printVector(FinishedQueue);
    cout << "==============================================" << endl;
    cout << endl;
}

/*

double getCPUUtilization()
Description: Returns a CPU Utilization Number.

*/

double getCPUUtilization() {
  for(int i=0; i<8; i++){
     totalCPUUsage += FinishedQueue[i].totalCPU;
    }
    int getTotalTime = FinishedQueue[FinishedQueue.size()-1].turnAroundTime;
    cpuUtil = 582/totalCPUUsage;
    return cpuUtil;
}

/*

void printDataChartAndAverages()
Description: Prints a chart of the result numbers and get's the averages.

*/

void printDataChartAndAverages (){

  cout << "--------------------------------------------------------------" << endl;
  cout << "| Process  #     |      Tw      |      Ttr     |     Tr      |      " << endl;
  cout << "--------------------------------------------------------------" << endl;
    for(int i=0; i<FinishedQueue.size(); i++){

    totalWaitingTime += FinishedQueue[i].readyWaitQueue;
    totalTurnAround += FinishedQueue[i].turnAroundTime;
    totalResponseTime += FinishedQueue[i].ResponseTime;
      
      cout << "| Process: "<<FinishedQueue[i].processNumber << "     |      " 
           << FinishedQueue[i].readyWaitQueue<< "     |      "    
           << FinishedQueue[i].turnAroundTime<< "     |      "    << left << setw(2)
           << FinishedQueue[i].ResponseTime  << "     |"    
        <<endl;
      cout << "--------------------------------------------------------------" << endl;
    }
  averageWaitingTime = totalWaitingTime/FinishedQueue.size();
  averageTurnAroundTime = totalTurnAround/FinishedQueue.size();
  averageResponseTime = totalResponseTime/FinishedQueue.size();

  
  cout << "| Average        |      " << averageWaitingTime << "   |    " << averageTurnAroundTime <<"   |    "<< averageResponseTime <<"   |    " << endl;
  cout << "--------------------------------------------------------------" << endl;

}

//-----------------------------------------------------------------------------
//------------------------------MAIN FUNCTION----------------------------------
//-----------------------------------------------------------------------------

int main() {

    //---------------- Add All Processes To The Ready Queue -----------------------
    addAllProcessesToReadyQueue();  
  
  //-------- While The Finished Queue Size is Not 8 (Total Processes) -----------
//---------------------- repeat the MLFQ Algorithm ----------------------------
  
while(FinishedQueue.size() != 8){
    startMLFQ();
  }
  
	// ----- Print Out Data -----------
 
    printCurrentQueue();
	printDataChartAndAverages();
	cout << endl;
	cout << "CPU Utilization: " << getCPUUtilization();
	cout << endl;




   
  }

  
