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
1. Approach:
The `LargeNumber` class uses a linked list of `Node` structures to represent large integers (up to 1024 bits). Each node holds a 64-bit unsigned integer, allowing dynamic growth. Key functionalities include generating random large numbers, performing arithmetic operations (addition, subtraction, and division by two), and checking primality using the Miller-Rabin test.

2. Assumptions:
- The linked list can handle large integers without overflow.
- The `rand` function provides a uniform distribution for random number generation.
- Five iterations of the Miller-Rabin test are sufficient for primality checking.

3. Challenges Faced:
- Managing dynamic memory to prevent leaks.
- Implementing precise bit manipulation for arithmetic operations.
- Ensuring efficiency and accuracy in the Miller-Rabin primality test.
- Addressing potential performance issues with large numbers in linked list format.
