#ifndef MLFQ_PROCESS_HPP
#define MLFQ_PROCESS_HPP

enum class ProcessStatus { Ready, Blocked, Completed };

class Process {
  int id;
  int arrivalTime;
  float cpuTimePercent;
  int totalTime;
  int cpuTimeUsed;
  ProcessStatus status;
  void generateCpuTime();
  int nextCPUDuration;
  int ioTimeUsed;
  int nextIODuration;
  int maxWindowDuration;
  int minWindowDuration;
  int getRemainingCPUTime();
  int getRemainingIOTime();
  int timeWhenReady;
  bool isCompleted();

public:
  Process(int id, int arrivalTime, float cpuTimePercent, int totalTime);
  int runProcess(int timeSlice, int currentTime);
  int getCpuTimeUsed();
  int getTimeWhenReady();
  int getID();
  ProcessStatus getStatus();
};

#endif // MLFQ_PROCESS_HPP
