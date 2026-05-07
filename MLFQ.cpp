//
// Created by Sakchham Sangroula on 5/2/26.

#include "MLFQ.hpp"
#include "Queue.hpp"

void storeStats(Process &process) {}

MLFQ::MLFQ() {
  queues = std::vector<Queue *>();
  currentTime = 0;
  priorityBoostInterval = 50;
  lastPriorityBoost = 0;
}

void MLFQ::enQueueProcess(Process *process) {
  Queue *firstQueue = queues.front();
  firstQueue->addToQueue(process);
}

void MLFQ::run() {
  for (int i = 0; i < queues.size(); i++) {
    Queue *queueToRun = queues[i];
    queueToRun->runQueue(currentTime, lastPriorityBoost, priorityBoostInterval);
  }
}

void MLFQ::downgradeProcess(int currentPriority, Process &process) {
  int finalQueue = queues.size();
  if (currentPriority == finalQueue) {
    queues[currentPriority - 1]->addToQueue(&process);
  } else {
    queues[currentPriority]->addToQueue(&process);
  }
}

void MLFQ::completeProcess(Process &process) { storeStats(process); }

void MLFQ::priorityBoost() {
  std::vector<Process *> processes = std::vector<Process *>();
  for (int i = 0; i < queues.size(); i++) {
    Queue *currentQueue = queues[i];
    std::deque<Process *> currentQueueProcesses = currentQueue->getProcesses();
    for (int j = 0; j < currentQueueProcesses.size(); i++) {
      Process *currentProcess = currentQueueProcesses[j];
      processes.push_back(currentProcess);
    }
    currentQueue->resetQueue();
  }
  for (int i = 0; i < processes.size(); i++) {
    Queue *firstQueue = queues[0];
    firstQueue->addToQueue(processes[i]);
  }
}
