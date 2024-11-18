# Task Scheduling System with Visual LED Representation
This project demonstrates a task scheduling system using LEDs to visualize task execution and signaling. The implementation uses FreeRTOS semaphores to manage task priorities and execution. This system builds upon prior work to address challenges in identifying task release times, particularly for lower-priority tasks.

## Project Overview
The system consists of three tasks:

1. **taskA (Blue LED):**

- Executes for 300ms of CPU time.
- Only starts execution after receiving a signal from taskACtrl.
- Has a controlled period of 3000ms, determined by taskACtrl.
**taskB (Green LED):**

- Executes for 100ms every 1000ms.
- Has higher priority than taskA according to Rate Monotonic Scheduling (RM).
**taskACtrl (Red LED):**

- The highest priority task in the system.
- Executes every 3000ms.
- Signals taskA to start its execution.
## Key Features
- **Semaphore-based synchronization:** Ensures that taskA only begins execution when released by taskACtrl.
- **Visualization of task execution:** LEDs indicate when tasks are running.
- **Interrupt-handling analogy:** High-priority task (taskACtrl) mimics an interrupt to release lower-priority tasks.
## How It Works
- **taskACtrl:**

- Runs every 3000ms and signals taskA to begin execution.
- Controls the release timing of taskA.
- **taskA:**

- Waits for a semaphore signal from taskACtrl.
- Runs for 300ms once released, visualized with the Blue LED.
- **taskB:**

- Runs periodically every 1000ms for 100ms.
- Operates independently, with a higher priority than taskA.
## Implementation Details
### Required FreeRTOS Functions
- The following FreeRTOS APIs are used to implement semaphore-based task synchronization:

- **vSemaphoreCreateBinary(xSemaphoreHandle xS):**

Creates a binary semaphore. If xS is not NULL, the semaphore was created successfully.
- **xSemaphoreTake(xSemaphoreHandle xS, portTickType xBlockTime):**

- Attempts to acquire the semaphore.
- If unavailable, blocks until it becomes available or xBlockTime elapses.
- Returns pdPASS on success and pdFALSE otherwise.
- **xSemaphoreGive(xSemaphoreHandle xSemaphore):**

Releases the semaphore, allowing the waiting task to proceed.
### Execution Flow
- taskACtrl signals taskA via semaphore.
- taskA starts execution for 300ms and waits for the next signal.
- Concurrently, taskB executes every 1000ms for 100ms, preempting taskA if necessary.
## Setup and Usage
### Prerequisites:

Configure FreeRTOS on the STM32 Board using CubeMX with three LEDs for task visualization.
Clone the Repository:

```bash

git clone https://github.com/RayenNacef/FreeRTOS-with-STM32/tree/main
cd <repository-folder>
```
### Compile and Flash:

Compile the project using your preferred IDE (Keil, CubeIDE).
Flash the binary to the STM32 Board.
Run the System:

- Observe the LEDs for task execution patterns:
    - Red LED (taskACtrl): Indicates taskA’s release.
    - Blue LED (taskA): Lights up during taskA's execution.
    - Green LED (taskB): Indicates taskB's periodic execution.
### Results and Observations
The addition of taskACtrl resolves ambiguity in identifying taskA's release time.
taskB’s execution minimally impacts taskA, as taskACtrl ensures timely signaling.
