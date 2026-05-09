#ifndef MLFQ_QUEUE_HPP
#define MLFQ_QUEUE_HPP
#include "Process.hpp"
#include <functional>
#include <map>
#include <queue>

class Queue {
  std::deque<Process> processes;
  std::map<int, int> processToCpuTime;
  int timeSlice;
  int priority;
  int maxTimeInQueue;
  std::function<void(int, Process &)> downgradeProcess;
  std::function<void(Process &)> completeProcess;
  std::function<void(Process &)> addToIoQueue;
  std::function<void()> priorityBoost;
  std::function<Process()> removeFromIO;
  void tick(int &);
  void removeProcess(Process &);

public:
  Queue(int, int, int, std::function<void(int, Process &)>,
        std::function<void()>, std::function<void(Process &)>,
        std::function<void(Process &)>);
  void addToQueue(Process &);
  void runQueue(int &, int &, const int);
  std::deque<Process> getProcesses();
  void resetQueue();
};

#endif // MLFQ_QUEUE_HPP
