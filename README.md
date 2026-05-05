# MLFQ — Multi-Level Feedback Queue Scheduler

A C++ simulation of the Multi-Level Feedback Queue (MLFQ) scheduling algorithm, as described in [OSTEP](https://pages.cs.wisc.edu/~remzi/OSTEP/) Chapter 8.

## Overview

MLFQ approximates SJF (Shortest Job First) without knowing job lengths in advance. It observes process behavior over time and adjusts priorities accordingly:

- New processes start at the highest-priority queue.
- A process that exhausts its time allotment is demoted to a lower queue.
- Periodic **priority boosts** move all processes back to the top queue, preventing starvation.

## Architecture

| File | Description |
|---|---|
| `Process.hpp/cpp` | Models a process with randomized CPU/IO burst windows based on a CPU-time percentage |
| `Queue.hpp/cpp` | A single priority level: round-robins processes within a time slice, demotes on allotment exhaustion |
| `MLFQ.hpp/cpp` | Orchestrates multiple queues, enqueues new processes, triggers priority boosts |
| `IOQueue.hpp/cpp` | Stub for tracking blocked (I/O-waiting) processes *(in progress)* |

### Process Model

Each `Process` is constructed with:

```
Process(id, arrivalTime, cpuTimePercent, totalTime)
```

- `cpuTimePercent` — fraction of total time spent on CPU vs. I/O (e.g. `80` = 80% CPU-bound).
- Burst durations are sampled randomly within a window derived from `totalTime`, mimicking realistic workloads.

### Queue Rules

Each `Queue` has:
- **`timeSlice`** — maximum CPU time per scheduling turn.
- **`maxTimeInQueue`** — total CPU allotment before a process is demoted.
- **`priority`** — queue's rank in the hierarchy.

A process is demoted when its accumulated CPU time in a queue reaches `maxTimeInQueue`.

### Priority Boost

The MLFQ checks `currentTime - lastPriorityBoost >= priorityBoostInterval` (default: every **50 time units**) and moves all processes back to the highest-priority queue, preventing indefinite starvation of long-running jobs.

## Build

Requires CMake ≥ 4.1 and a C++17-compatible compiler.

```bash
cmake -B build
cmake --build build
./build/MLFQ
```

Or open in CLion — the project includes a pre-configured `cmake-build-debug/` setup.

## Example

```cpp
// main.cpp
Process processA = {0, 0,  10, 100, ...};  // mostly I/O-bound
Process processB = {1, 2,  30, 100, ...};
Process processC = {2, 2,  80, 100, ...};  // mostly CPU-bound

// Four queues with decreasing priority
Queue q1, q2, q3, q4;
MLFQ mlfq;
```

## Status

| Feature | Status |
|---|---|
| Core MLFQ scheduling | Done |
| Process CPU/IO burst simulation | Done |
| Priority boost (anti-starvation) | Done |
| IO queue (blocked process tracking) | In progress |
