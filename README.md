# SANA-DSA-ASSIGNMENT1
DSA Assignment - Linked List Implementation
TASK 1 
1. Approach:
- The code simulates round-robin scheduling using a circular linked list to manage processes.
- Each process is represented by a `Process` class with fields for ID, execution time, remaining time, and a pointer to the next process.
- The `ProcessScheduler` class manages:
  - Adding processes to the list.
  - Running CPU cycles, each process gets 3 units of CPU time per cycle.
  - Removing completed processes from the list and keeping track of them.
  - Displaying process states before and after each cycle.

2. Assumptions:
- Round-robin scheduling is used with a fixed time slice (3 units).
- Processes are managed in a circular linked list.
- Completed processes are dynamically removed from memory.

3. Challenges:
- Deleting processes from a circular linked list without breaking the structure.
- Maintaining circular traversal after removing processes.
- Proper memory management to avoid leaks after process completion.

TASK 2
