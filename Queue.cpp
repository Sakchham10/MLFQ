#include "Queue.hpp"

Queue::Queue(int timeSlice, int priority, int maxTimeInQueue,
             std::function<void(int, Process &)> downgradeProcess,
             std::function<void()> priorityBoost,
             std::function<void(Process &)> addToIoQueue,
             std::function<void(Process &)> completeProcess) {
  this->timeSlice = timeSlice;
  this->priority = priority;
  processToCpuTime = std::map<int, int>();
  processes = std::deque<Process>();
  this->maxTimeInQueue = maxTimeInQueue;
  this->downgradeProcess = downgradeProcess;
  this->priorityBoost = priorityBoost;
  this->addToIoQueue = addToIoQueue;
  this->completeProcess = completeProcess;
}

void Queue::removeProcess(Process &process) {
  processToCpuTime.erase(process.getID());
  processes.pop_front();
}

void Queue::tick(int &currentTime) {
  currentTime++;
  return;
}

void Queue::runQueue(int &currentTime, int &lastPriorityBoost,
                     const int priorityBoostPeriod) {

  while (!processes.empty()) {
    Process processToRun = processes.front();
    int timeUsedByProcess = processToCpuTime[processToRun.getID()];
    if (timeUsedByProcess == maxTimeInQueue) {
      removeProcess(processToRun);
      downgradeProcess(priority, processToRun);
      return;
    }
    int timeRemainingInQueue =
        std::min(maxTimeInQueue - timeUsedByProcess, timeSlice);
    int timeRan = processToRun.runProcess(timeRemainingInQueue, currentTime);
    processToCpuTime[processToRun.getCpuTimeUsed()] += timeRan;
    currentTime += timeRan;
    switch (processToRun.getStatus()) {
    case ProcessStatus::Ready:
      processes.pop_front();
      if (processToCpuTime[processToRun.getCpuTimeUsed()] < maxTimeInQueue) {
        processes.push_back(processToRun);
      } else {
        removeProcess(processToRun);
        downgradeProcess(priority, processToRun);
      }
      break;
    case ProcessStatus::Blocked:
      removeProcess(processToRun);
      addToIoQueue(processToRun);
      break;
    case ProcessStatus::Completed:
      removeProcess(processToRun);
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
      lastPriorityBoost = currentTime;
      return;
    }
  }
}

void Queue::addToQueue(Process &process) {
  processes.push_back(process);
  processToCpuTime[process.getID()] = 0;
}

void Queue::resetQueue() {
  processes.clear();
  processToCpuTime.clear();
}

std::deque<Process> Queue::getProcesses() { return processes; }
