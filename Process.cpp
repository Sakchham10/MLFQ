#include "Process.hpp"

#include <random>

namespace Random {
inline int between(int x, int y) {
  static std::random_device rd;
  static std::mt19937 gen(rd());

  std::uniform_int_distribution<int> dist(x, y);
  return dist(gen);
}
} // namespace Random

Process::Process(const int id, const int arrivalTime,
                 const float cpuTimePercent, const int totalTime) {
  this->arrivalTime = arrivalTime;
  this->cpuTimePercent = cpuTimePercent;
  this->totalTime = totalTime;
  status = ProcessStatus::Ready;
  nextCPUDuration = 0;
  nextIODuration = 0;
  cpuTimeUsed = 0;
  this->id = id;
  ioTimeUsed = 0;
  maxWindowDuration = ceil(static_cast<float>(totalTime) / 10);
  minWindowDuration = ceil(static_cast<float>(totalTime) / 20);
  timeWhenReady = arrivalTime;
  generateCpuTime();
};

int Process::getRemainingCPUTime() {
  return floor((cpuTimePercent / 100) * totalTime) - cpuTimeUsed;
}

int Process::getRemainingIOTime() {
  return floor(((100 - cpuTimePercent) / 100) * totalTime) - ioTimeUsed;
}

bool Process::isCompleted() {
  if (ioTimeUsed + cpuTimeUsed == totalTime) {
    return true;
  }
  return false;
}

void Process::generateCpuTime() {
  int remainingCPUTime = getRemainingCPUTime();
  int remainingIOTime = getRemainingIOTime();
  if (remainingCPUTime == 0) {
    nextCPUDuration = 0;
    if (remainingIOTime <= minWindowDuration) {
      nextIODuration = remainingIOTime;
    } else {
      nextIODuration = Random::between(
          minWindowDuration, std::min(maxWindowDuration, remainingIOTime));
    }
  } else if (remainingIOTime == 0) {
    nextIODuration = 0;
    if (remainingCPUTime <= minWindowDuration) {
      nextCPUDuration = remainingCPUTime;
    } else {
      nextCPUDuration = Random::between(
          minWindowDuration, std::min(maxWindowDuration, remainingCPUTime));
    }
  } else {
    int nextWindow = Random::between(minWindowDuration, maxWindowDuration);
    nextCPUDuration =
        Random::between(1, std::min(nextWindow, remainingCPUTime));
    nextIODuration = std::min(nextWindow - nextCPUDuration, remainingIOTime);
  }
}

int Process::runProcess(int timeSlice, int currentTime) {
  int timeRan = 0;
  if (timeSlice >= nextCPUDuration) {
    cpuTimeUsed += nextCPUDuration;
    timeRan = nextCPUDuration;
    if (nextIODuration > 0) {
      timeWhenReady = currentTime + nextIODuration;
      status = ProcessStatus::Blocked;
    } else {
      if (isCompleted()) {
        status = ProcessStatus::Completed;
      } else {
        status = ProcessStatus::Ready;
      }
    }
  } else {
    cpuTimeUsed += timeSlice;
    nextCPUDuration -= timeSlice;
    timeRan = timeSlice;
    status = ProcessStatus::Ready;
  }
  return timeRan;
}

ProcessStatus Process::getStatus() { return status; }

int Process::getCpuTimeUsed() { return cpuTimeUsed; }

int Process::getTimeWhenReady() { return timeWhenReady; }

int Process::getID() { return id; }
