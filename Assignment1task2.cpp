#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <climits>
#include <cstdint>
#include <random>

using namespace std;

// Constants defining node and bit sizes
const int bits_per_node = 64; // Each node can hold 64 bits (uint64_t)
const int nodes_required = 1024 / bits_per_node; // Total nodes required to represent a 1024-bit number
const uint64_t MAX_UINT64 = UINT64_MAX; // Maximum value for a 64-bit unsigned integer

// Node structure representing each part of a large number
struct Node {
    uint64_t value; // Value stored in the node
    Node* next; // Pointer to the next node in the linked list
    Node(uint64_t val = 0) : value(val), next(nullptr) {} // Constructor initializing node value
};

class LargeNumber {
private:
    Node* head; 

    // function to perform a deep copy of another LargeNumber
    void deepCopy(const LargeNumber& other) {
        Node* otherCurrent = other.head; // Pointer to traverse the other LargeNumber
        Node** current = &head; // Pointer to insert new nodes into this LargeNumber
        while (otherCurrent) {
            *current = new Node(otherCurrent->value); // Create a new node and copy the value
            current = &((*current)->next); // Move to the next pointer
            otherCurrent = otherCurrent->next; // Move to the next node in the other LargeNumber
        }
    }
public:
    // Default constructor
    LargeNumber() : head(nullptr) {}

    // Constructor to initialize with a single uint64_t value
    LargeNumber(uint64_t val) : head(nullptr) {
        head = new Node(val); // Create a new node for the value
    }
    // Copy constructor
    LargeNumber(const LargeNumber& other) : head(nullptr) {
        deepCopy(other); // Call deepCopy to copy nodes
    }
    // Assignment operator
    LargeNumber& operator=(const LargeNumber& other) {
        if (this != &other) { // Self-assignment check
            // Deallocate existing nodes
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp; // Delete the old node
            }
            deepCopy(other); // Deep copy the new nodes
        }
        return *this; // Return current object
    }
    // Generate a random 1024-bit number
    void generateRandom() {
        srand(time(nullptr)); // Seed random number generator
        for (int i = 0; i < nodes_required; ++i) {
            // Generate a random 64-bit number
            uint64_t randomValue = ((uint64_t)rand() << 32) | rand();
            if (i == nodes_required - 1) {
                randomValue |= (1ULL << 63); // Ensure the last node is set to 1024 bits
            }
            Node* newNode = new Node(randomValue); // Create a new node with the random value
            newNode->next = head; // Insert at the beginning of the linked list
            head = newNode; // Update head pointer
        }
    }
    // Check if the number is even
    bool isEven() const {
        return head && !(head->value & 1); // Check if the least significant bit is 0
    }
    // Check if the number is zero
    bool isZero() const {
        Node* current = head; // Pointer to traverse the linked list
        while (current) {
            if (current->value != 0) return false; // Return false if any value is not zero
            current = current->next; // Move to the next node
        }
        return true; // All values are zero
    }
    // Check if the number is one
    bool isOne() const {
        if (!head || head->value != 1) return false; // Check if the head is 1
        Node* current = head->next; // Pointer to traverse remaining nodes
        while (current) {
            if (current->value != 0) return false; // Return false if any subsequent value is not zero
            current = current->next; // Move to the next node
        }
        return true; // The number is one
    }
    // Divide the large number by two
    void divideByTwo() {
        uint64_t carry = 0; // Carry to keep track of division
        Node* current = head; // Pointer to traverse the linked list
        while (current) {
            uint64_t newValue = (current->value >> 1) | (carry << 63); // Shift right and include carry
            carry = current->value & 1; // Update carry with the least significant bit
            current->value = newValue; // Update current node value
            current = current->next; // Move to the next node
        }
        // Remove leading zero node
        if (head && head->value == 0) {
            Node* temp = head;
            head = head->next; // Update head to the next node
            delete temp; // Delete the old head
        }
    }
    // Subtract another LargeNumber from this one
    void subtract(const LargeNumber& other) {
        Node* thisCurrent = head; // Pointer for this number
        Node* otherCurrent = other.head; // Pointer for other number
        uint64_t borrow = 0; // Borrow for subtraction
        // Loop through both numbers
        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value >= otherCurrent->value + borrow) {
                thisCurrent->value -= otherCurrent->value + borrow; // Direct subtraction
                borrow = 0; // No borrow
            } else {
                // Need to borrow
                thisCurrent->value = (MAX_UINT64 - otherCurrent->value - borrow + 1 + thisCurrent->value);
                borrow = 1; // Set borrow for next iteration
            }
            thisCurrent = thisCurrent->next; // Move to the next node
            otherCurrent = otherCurrent->next; // Move to the next node
        }
        // Handle remaining nodes in this number
        while (thisCurrent && borrow) {
            if (thisCurrent->value >= borrow) {
                thisCurrent->value -= borrow; // Subtract borrow
                borrow = 0; // Clear borrow
            } else {
                // Need to wrap around
                thisCurrent->value = MAX_UINT64 - borrow + 1;
                borrow = 1; // Set borrow for next node
            }
            thisCurrent = thisCurrent->next; // Move to the next node
        }
        // Remove leading zeros
        while (head && head->next && head->value == 0) {
            Node* temp = head;
            head = head->next; // Update head
            delete temp; // Delete old head
        }
    }
    // Calculate modulus of this number with another LargeNumber
    LargeNumber mod(const LargeNumber& m) const {
        LargeNumber result = *this; // Copy this number
        while (result >= m) { // While result is greater than or equal to modulus
            result.subtract(m); // Subtract modulus from result
        }
        return result; // Return the result
    }
    // Perform modular multiplication of this number with another LargeNumber
    LargeNumber modMul(const LargeNumber& other, const LargeNumber& m) const {
        LargeNumber result; // Result of multiplication
        LargeNumber temp = *this; // Temporary variable for calculations

        Node* otherCurrent = other.head; // Pointer for the other LargeNumber
        while (otherCurrent) {
            for (int i = 0; i < 64; ++i) { // Loop through bits
                if (otherCurrent->value & (1ULL << i)) {
                    result = (result + temp).mod(m); // Add temp to result and take modulus
                }
                temp = (temp + temp).mod(m); // Double temp and take modulus
            }
            otherCurrent = otherCurrent->next; // Move to the next node
        }
        return result; // Return the result
    }
    // Perform modular exponentiation
    LargeNumber modPow(const LargeNumber& exponent, const LargeNumber& m) const {
        if (m.isOne()) return LargeNumber(0); // If modulus is one, return zero

        LargeNumber result(1); // Initialize result to 1
        LargeNumber base = this->mod(m); // Reduce base with modulus
        LargeNumber exp = exponent; // Copy exponent

        while (!exp.isZero()) { // While exponent is not zero
            if (!exp.isEven()) {
                result = result.modMul(base, m); // Multiply result with base and take modulus
            }
            base = base.modMul(base, m); // Square base and take modulus
            exp.divideByTwo(); // Divide exponent by two
        }
        return result; 
    }
    // Comparison operator for equality
    bool operator==(const LargeNumber& other) const {
        Node* thisCurrent = head; // Pointer for this number
        Node* otherCurrent = other.head; // Pointer for other number
        // Compare node values
        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value != otherCurrent->value) return false; // If values differ, return false
            thisCurrent = thisCurrent->next; // Move to the next node
            otherCurrent = otherCurrent->next; // Move to the next node
        }
        return !thisCurrent && !otherCurrent; // Return true if both lists are fully traversed
    }
    // Comparison operator for greater than or equal to
    bool operator>=(const LargeNumber& other) const {
        Node* thisCurrent = head; // Pointer for this number
        Node* otherCurrent = other.head; // Pointer for other number
        // Skip leading zeros
        while (thisCurrent && thisCurrent->value == 0) thisCurrent = thisCurrent->next;
        while (otherCurrent && otherCurrent->value == 0) otherCurrent = otherCurrent->next;
        // Compare number of significant nodes
        int thisNodes = 0, otherNodes = 0; // Count significant nodes
        Node* temp = thisCurrent;
        while (temp) {
            thisNodes++; // Count nodes in this number
            temp = temp->next;
        }
        temp = otherCurrent;
        while (temp) {
            otherNodes++; // Count nodes in other number
            temp = temp->next;
        }
        if (thisNodes > otherNodes) return true; // This number has more significant nodes
        if (thisNodes < otherNodes) return false; // Other number has more significant nodes
        // Compare node values from most significant to least
        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value > otherCurrent->value) return true; // This number is greater
            if (thisCurrent->value < otherCurrent->value) return false; // Other number is greater
            thisCurrent = thisCurrent->next; // Move to the next node
            otherCurrent = otherCurrent->next; // Move to the next node
        }
        return true; // Numbers are equal
    }
    // Addition operator
    LargeNumber operator+(const LargeNumber& other) const {
        LargeNumber result; // Result of addition
        Node** resultCurrent = &result.head; // Pointer to insert nodes into result
        Node* thisCurrent = head; // Pointer for this number
        Node* otherCurrent = other.head; // Pointer for other number
        uint64_t carry = 0; // Carry for addition
        // Loop through both numbers
        while (thisCurrent || otherCurrent || carry) {
            uint64_t sum = carry; // Start with carry
            if (thisCurrent) {
                sum += thisCurrent->value; // Add value from this number
                thisCurrent = thisCurrent->next; // Move to the next node
            }
            if (otherCurrent) {
                sum += otherCurrent->value; // Add value from the other number
                otherCurrent = otherCurrent->next; // Move to the next node
            }
            *resultCurrent = new Node(sum & MAX_UINT64); // Create a new node for the result
            resultCurrent = &((*resultCurrent)->next); // Move to the next pointer
            carry = sum >> 64; // Update carry for next iteration
        }
        return result; 
    }
    // Subtraction operator
    LargeNumber operator-(const LargeNumber& other) const {
        LargeNumber result = *this; // Copy this number
        result.subtract(other); // Subtract other from the copy
        return result; // Return the result
    }
    // Check if the number is prime using Miller-Rabin primality test
    bool isPrime() {
        if (isEven()) return false; // Exclude even numbers
        
        LargeNumber two(2);
        LargeNumber one(1); 

        LargeNumber d = *this - one; // d = n - 1
        int s = 0; // Count of factors of 2 in d
        while (d.isEven()) {
            d.divideByTwo(); // Divide d by 2
            s++; // Increase count of factors of 2
        }
        for (int i = 0; i < 5; i++) { // 5 iterations for Miller-Rabin
            LargeNumber a;
            a.generateRandom(); // Generate random number
            a = a.mod(*this); // Reduce it modulo n

            if (a.isZero() || a.isOne()) continue; // Skip if a is 0 or 1

            LargeNumber x = a.modPow(d, *this); // x = a^d mod n

            if (x.isOne() || x == *this - one) continue; // x is either 1 or n - 1

            bool composite = true; // Assume number is composite
            for (int j = 0; j < s - 1; j++) {
                x = x.modMul(x, *this); // Square x and take modulus
                if (x == *this - one) { // Check if x is n - 1
                    composite = false; // Found a witness, it's not composite
                    break;
                }
            }
            if (composite) return false; // If still composite, n is not prime
        }
        return true; // n is probably prime
    }
    // Display the number
    void display() const {
        Node* current = head; // Pointer to traverse the linked list
        while (current) {
            cout << current->value << " "; // Print each node value
            current = current->next; // Move to the next node
        }
        cout << endl; 
    }
    // Destructor to clean up memory
    ~LargeNumber() {
        while (head) {
            Node* temp = head; // Pointer to the current head
            head = head->next; // Move head to the next node
            delete temp; // Delete the old head
        }
    }
};
//MAIN FUNCTION
int main() {
    LargeNumber largeNum; // Create a LargeNumber object
    largeNum.generateRandom(); // Generate a random large number

    cout << "The Generated number is: ";
    largeNum.display(); 

    bool isPrime = largeNum.isPrime(); // Check if the number is prime
    cout << "Is prime: " << (isPrime ? "True" : "False") << endl; 

    return 0; 
}
