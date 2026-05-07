#ifndef MLFQ_QUEUE_HPP
#define MLFQ_QUEUE_HPP
#include "Process.hpp"
#include <map>
#include <queue>

class Queue {
  std::deque<Process> processes;
  std::map<int, int> processToCpuTime;
  int timeSlice;
  int priority;
  int maxTimeInQueue;
  void downgradeProcess(Process &);
  void completeProcess(Process &);
  void addToIoQueue(Process &);
  void priorityBoost();

public:
  Queue(int, int, int);
  void addToQueue(Process &);
  void runQueue(int &, int &, const int);
  std::deque<Process> getProcesses();
  void resetQueue();
};

#endif // MLFQ_QUEUE_HPP
