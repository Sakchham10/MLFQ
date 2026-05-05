#ifndef MLFQ_QUEUE_HPP
#define MLFQ_QUEUE_HPP
#include "Process.hpp"
#include <map>
#include <queue>

class Queue {
  std::deque<Process *> processes;
  std::map<int, int> processToCpuTime;
  int timeSlice;
  int priority;
  int maxTimeInQueue;
  void downgradeProcess(Process *process);
  void completeProcess(Process *process);
  void addToIoQueue(Process *process);
  void priorityBoost();

public:
  Queue(int timeSlice, int priority, int maxTimeInQueue);
  void addToQueue(Process *process);
  void runQueue(int &currentTime, int lastPriorityBoost,
                int priorityBoostPeriod);
  std::deque<Process *> getProcesses();
  void resetQueue();
};

#endif // MLFQ_QUEUE_HPP
