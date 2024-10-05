//ASSIGNMENT 1 DSA QUESTION 1
#include <iostream>
#include <string>
using namespace std;

class Process {
public:
    int process_id; // Unique ID of the process
    int execution_time; // Total time required for the process to complete
    int remaining_time; // Time left for the process to complete
    Process* next; // Pointer to the next process in the circular list

    // Constructor
    Process(int id, int time) : process_id(id), execution_time(time), remaining_time(time), next(nullptr) {}
};

// Class to manage the process scheduling and simulate the CPU execution cycles
class ProcessScheduler {
private:
    Process* head;            
    const int cpu_time_per_cycle = 3; // CPU time assigned to each process per cycle

public:
    // Constructor 
    ProcessScheduler() : head(nullptr) {}

    // Function to add a new process to the circular list
    void addProcess(int process_id, int execution_time) {
        Process* newProcess = new Process(process_id, execution_time);  // Create a new process
        if (!head) {  // If the list is empty, make the new process the head and point it to itself
            head = newProcess;
            newProcess->next = head;
        } else {  // Otherwise, insert the new process at the end of the circular list
            Process* current = head;
            while (current->next != head) {
                current = current->next;
            }
            current->next = newProcess;
            newProcess->next = head; // Make the list circular
        }
    }

    // Function to simulate a single CPU cycle where time is assigned to processes
    void runCycle(int& cycleCount) {
        if (!head) {  // If there are no processes, exit
            return;
        }
        cout << "Cycle " << cycleCount << ":" << endl;
        cout << "Before: ";
        displayState();  // Display the state of all processes before running this cycle

        Process* current = head;       // Pointer to track the current process in the list
        Process* prev = nullptr;       // Pointer to track the previous process (for deletion)
        Process* completedHead = nullptr;  // Temporary list for completed processes

        // Loop through the circular list of processes and assign CPU time
        do {
            // Display which process is running and for how much time
            cout << "Running process P" << current->process_id << " for " << min(cpu_time_per_cycle, current->remaining_time) << " units." << endl;

            // Reduce the remaining time of the current process
            current->remaining_time -= cpu_time_per_cycle;

            // Check if the process has completed its execution
            if (current->remaining_time <= 0) {
                // Add completed process to a temporary list
                Process* completed = new Process(current->process_id, 0);
                completed->next = completedHead;
                completedHead = completed;

                // Remove the process from the circular linked list
                if (current->next == current) { // If it's the only process in the list
                    delete current;
                    head = nullptr; // Set head to null since there are no more processes
                } else {
                    if (prev) {
                        prev->next = current->next;  // Skip over the current process
                    } else {
                        head = current->next;        // Move head to the next process
                    }
                    Process* temp = current;         // Delete the current process
                    current = current->next;
                    delete temp;
                    continue;  // Continue to the next process
                }
            } else {
                prev = current;  // Update prev to the current process
            }

            if (head == nullptr) break;  // If no processes remain, stop the loop
            current = current->next;     // Move to the next process in the list
        } while (current != head);

        cout << "After:  ";
        displayState();  // Display the state of processes after the cycle
        
        displayCompleted(completedHead);  // Display completed processes
        freeCompletedList(completedHead); // Free the memory for the completed process list
        
        cycleCount++;  // Increment the cycle counter
        cout << endl;
    }

    // Function to display the state of all processes in the system
    void displayState() {
        if (!head) {  // If there are no processes
            cout << "No processes in the system." << endl;
            return;
        }

        Process* current = head;
        do {
            cout << "P" << current->process_id << " (Remaining: " << current->remaining_time << ") ";  // Display remaining time of each process
            current = current->next;
        } while (current != head);
        cout << endl;
    }

    // Function to display the list of completed processes after a cycle
    void displayCompleted(Process* completed) {
        if (!completed) {  // If no processes have completed, return
            return;
        }
        cout << "Completed processes: ";
        Process* current = completed;
        while (current) {  // Loop through the list of completed processes and display their IDs
             cout << "P" << current->process_id;
            if (current->next)  cout << ", ";
            current = current->next;
        }
        cout << endl;
    }

    // Function to free the memory of the completed process list
    void freeCompletedList(Process* completedHead) {
        while (completedHead) {
            Process* temp = completedHead;  // Free each completed process node
            completedHead = completedHead->next;
            delete temp;
        }
    }

    // Function to simulate the entire scheduling process, running cycles until all processes are completed
    void simulate() {
        int cycleCount = 1;  // Initialize cycle counter
        while (head) {       // Keep running cycles as long as there are processes
            runCycle(cycleCount);
        }
    }

    // Destructor to free the memory of any remaining processes in the list
    ~ProcessScheduler() {
        if (!head) return;
        Process* current = head->next;
        while (current != head) {
            Process* temp = current;
            current = current->next;
            delete temp;
        }
        delete head;
    }
};

int main() {
    ProcessScheduler scheduler;
    
    // Create initial processes
    scheduler.addProcess(1, 10);  // Process P1 with 10 units of execution time
    scheduler.addProcess(2, 5);   // Process P2 with 5 units of execution time
    scheduler.addProcess(3, 8);   // Process P3 with 8 units of execution time

    // Simulate the process scheduling
    cout << "Initial Processes:" << endl;
    scheduler.simulate();

    // Adding a new process P4 after the initial simulation
    scheduler.addProcess(4, 9);   // Process P4 with 9 units of execution time
    cout << "New process P4 added." << endl;
    cout << "Simulation:" << endl;
    scheduler.simulate();

    return 0;
}
