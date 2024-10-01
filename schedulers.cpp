#include "schedulers.h"

//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}

int ShortestProcess(const int& curTime, const vector<Process>& procList){
    int idx = -1;
    static deque<int> ready;

    for (int i = 0, i_end = procList.size(); i < i_end; ++i) {
        if (procList[i].startTime == curTime && !procList[i].isDone) {
            ready.push_back(i);
        }
    }

    while (!ready.empty() && procList[ready.front()].isDone) {
        ready.pop_front();
    }

    if (!ready.empty()) {
        idx = ready.front();  
        for (int i : ready) {
            if (procList[i].totalTimeNeeded < procList[idx].totalTimeNeeded && !procList[i].isDone) {
                idx = i;  
            }
        }
    }

    return idx; 
}

int ShortestRemainingTime(const int& curTime, const vector<Process>& procList){
    int idx = -1; 
    static deque<int> ready; 

    for (int i = 0, i_end = procList.size(); i < i_end; ++i) {
        if (procList[i].startTime == curTime && !procList[i].isDone) {
            ready.push_back(i);
        }
    }

    while (!ready.empty() && procList[ready.front()].isDone) {
        ready.pop_front();
    }

    if (!ready.empty()) {
        idx = ready.front();  
        for (int i : ready) {
            int remainingTime = procList[i].totalTimeNeeded - procList[i].timeScheduled;
            int currentShortestRemainingTime = procList[idx].totalTimeNeeded - procList[idx].timeScheduled;

            if (remainingTime < currentShortestRemainingTime && !procList[i].isDone) {
                idx = i; 
            }
        }
    }

    return idx; 
}

int HighestResponseRatio(const int& curTime, const vector<Process>& procList){
    int idx = -1;
    static deque<int> ready;

    for (unsigned int i = 0; i < procList.size(); ++i) {
        if (procList[i].startTime <= curTime && !procList[i].isDone) { 
            ready.push_back(i);
        }
    }

    while (!ready.empty() && procList[ready.front()].isDone) {
        ready.pop_front();
    }

    if (ready.empty()) {
        return -1; 
    }

    double maxResponseRatio = -1.0;  

    for (int i : ready) {
        int waitingTime = curTime - procList[i].startTime - procList[i].timeScheduled; 
        int serviceTime = procList[i].totalTimeNeeded;

        if (serviceTime > 0 && procList[i].timeScheduled < serviceTime) {
            double responseRatio = (waitingTime + serviceTime) / static_cast<double>(serviceTime);

            if (responseRatio > maxResponseRatio) {
                maxResponseRatio = responseRatio;
                idx = i;  
            }
        }
    }

    return idx; 
}

