#include "Queue.hpp"

Queue::Queue(int timeSlice, int priority, int maxTimeInQueue) {
  this->timeSlice = timeSlice;
  this->priority = priority;
  processToCpuTime = std::map<int, int>();
  processes = std::deque<Process *>();
  this->maxTimeInQueue = maxTimeInQueue;
}

void Queue::runQueue(int &currentTime, int lastPriorityBoost,
                     int priorityBoostPeriod) {
  while (!processes.empty()) {
    Process *processToRun = processes.front();
    int timeUsedByProcess = processToCpuTime[processToRun->getID()];
    if (timeUsedByProcess == maxTimeInQueue) {
      processes.pop_front();
      downgradeProcess(processToRun);
    }
    int timeRemainingInQueue =
        std::min(maxTimeInQueue - timeUsedByProcess, timeSlice);
    int timeRan = processToRun->runProcess(timeRemainingInQueue, currentTime);
    processToCpuTime[processToRun->getCpuTimeUsed()] += timeRan;
    currentTime += timeRan;
    switch (processToRun->getStatus()) {
    case ProcessStatus::Ready:
      processes.pop_front();
      if (processToCpuTime[processToRun->getCpuTimeUsed()] < maxTimeInQueue) {
        processes.push_back(processToRun);
      } else {
        downgradeProcess(processToRun);
      }
      break;
    case ProcessStatus::Blocked:
      processes.pop_front();
      addToIoQueue(processToRun);
      break;
    case ProcessStatus::Completed:
      processes.pop_front();
      completeProcess(processToRun);
      break;
    }
    // edge case 1, if currentTime > priority boost, then priority boost happens
    // later than expected.
    // e.g. currentTime becomes 51 after timeRan but priority boost should have
    // happened at 50. Don't think this affects how the process functions, but
    // affects the lastPriorityBoostPeriod
    if (currentTime - lastPriorityBoost >= priorityBoostPeriod) {
      priorityBoost();
    }
  }
}

void Queue::addToQueue(Process *process) {
  processes.push_back(process);
  processToCpuTime[process->getID()] = 0;
}

void Queue::resetQueue() {
  processes.clear();
  processToCpuTime.clear();
}

std::deque<Process *> Queue::getProcesses() { return processes; }
