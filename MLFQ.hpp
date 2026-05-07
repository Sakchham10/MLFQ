#ifndef MLFQ_MLFQ_HPP
#define MLFQ_MLFQ_HPP
#include "Queue.hpp"
#include <vector>

class MLFQ {
  std::vector<Queue *> queues;
  int currentTime;
  int priorityBoostInterval;
  int lastPriorityBoost;
  std::queue<Process *> ioQueue;
  void downgradeProcess(int, Process &);
  void priorityBoost();
  void addToIoQueue(Process &);
  void completeProcess(Process &);

public:
  void run();
  void enQueueProcess(Process *);
  void addQueue(Queue &);
  MLFQ();
};

#endif // MLFQ_MLFQ_HPP
