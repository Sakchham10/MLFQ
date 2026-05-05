#ifndef MLFQ_MLFQ_HPP
#define MLFQ_MLFQ_HPP
#include "Queue.hpp"
#include <vector>

class MLFQ {
  std::vector<Queue *> queues;
  void priorityBoost();
  int currentTime;
  int priorityBoostInterval;
  int lastPriorityBoost;

public:
  void run();
  void enQueueProcess(Process *);
  void addQueue(Queue &queue);
  MLFQ();
};

#endif // MLFQ_MLFQ_HPP
