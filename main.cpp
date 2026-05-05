#include "MLFQ.hpp"
#include "Queue.hpp"
#include <iostream>
#include <vector>

int main() {
  Process processA = {0, 0, 10, 100, 0, ProcessStatus::Ready};
  Process processB = {1, 2, 30, 100, 0, ProcessStatus::Ready};
  Process processC = {2, 2, 80, 100, 0, ProcessStatus::Ready};
  Queue queue1 = Queue();
  Queue queue2 = Queue();
  Queue queue3 = Queue();
  Queue queue4 = Queue();
  MLFQ mlfq = MLFQ();
}

