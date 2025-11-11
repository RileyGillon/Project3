#include <iostream>
#include <vector>
#include <string>
using namespace std;

class BankersAlgorithm {
private:
    int numResources;
    int numProcesses;
    vector<int> available;
    vector<vector<int>> max;
    vector<vector<int>> allocation;
    vector<vector<int>> need;

    // Calculate the Need matrix (Need = Max - Allocation)
    void calculateNeed() {
        need.resize(numProcesses, vector<int>(numResources));
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                need[i][j] = max[i][j] - allocation[i][j];
            }
        }
    }

    // Safety Algorithm: Check if the system is in a safe state
    bool isSafe(vector<int> work, vector<vector<int>> currentAllocation) {
        vector<bool> finish(numProcesses, false);

        // Calculate current need based on the allocation passed in
        vector<vector<int>> currentNeed(numProcesses, vector<int>(numResources));
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                currentNeed[i][j] = max[i][j] - currentAllocation[i][j];
            }
        }

        int count = 0;
        while (count < numProcesses) {
            bool found = false;

            // Find a process that can finish
            for (int i = 0; i < numProcesses; i++) {
                if (!finish[i]) {
                    bool canAllocate = true;

                    // Check if all needed resources are available
                    for (int j = 0; j < numResources; j++) {
                        if (currentNeed[i][j] > work[j]) {
                            canAllocate = false;
                            break;
                        }
                    }

                    if (canAllocate) {
                        // Simulate process finishing and releasing resources
                        for (int j = 0; j < numResources; j++) {
                            work[j] += currentAllocation[i][j];
                        }
                        finish[i] = true;
                        found = true;
                        count++;
                    }
                }
            }

            // If no process could finish, system is unsafe
            if (!found) {
                return false;
            }
        }

        return true; // All processes can finish - safe state
    }

public:
    // Read input from stdin
    void readInput() {
        string temp;

        // Read number of resources (R n)
        cin >> temp >> numResources;

        // Read number of processes (P n)
        cin >> temp >> numProcesses;

        // Read Available vector
        cin >> temp; // "Available"
        available.resize(numResources);
        for (int i = 0; i < numResources; i++) {
            cin >> available[i];
        }

        // Read Max matrix
        cin >> temp; // "Max"
        max.resize(numProcesses, vector<int>(numResources));
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                cin >> max[i][j];
            }
        }

        // Read Allocation matrix
        cin >> temp; // "Allocation"
        allocation.resize(numProcesses, vector<int>(numResources));
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                cin >> allocation[i][j];
            }
        }

        // Calculate Need matrix
        calculateNeed();
    }

    // Print the current state
    void printState() {
        cout << "R " << numResources << endl;
        cout << "P " << numProcesses << endl;
        cout << "Available" << endl;
        for (int i = 0; i < numResources; i++) {
            cout << available[i];
            if (i < numResources - 1) cout << " ";
        }
        cout << endl;

        cout << "Max" << endl;
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                cout << max[i][j];
                if (j < numResources - 1) cout << " ";
            }
            cout << endl;
        }

        cout << "Allocation" << endl;
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                cout << allocation[i][j];
                if (j < numResources - 1) cout << " ";
            }
            cout << endl;
        }
    }

    // Resource-Request Algorithm: Process a resource request
    void processRequest() {
        string processId;
        cin >> processId;

        // Extract process number from process ID (e.g., "P1" -> 1)
        int processNum = processId[1] - '0';

        vector<int> request(numResources);
        for (int i = 0; i < numResources; i++) {
            cin >> request[i];
        }

        // Print the request
        cout << processId;
        for (int i = 0; i < numResources; i++) {
            cout << " " << request[i];
        }
        cout << endl;

        // First check if the current system is safe
        if (!isSafe(available, allocation)) {
            cout << "The current system is in unsafe state." << endl;
            return;
        }

        cout << "Before granting the request of " << processId
             << ", the system is in safe state." << endl;

        // Check if request exceeds need
        bool exceedsNeed = false;
        for (int i = 0; i < numResources; i++) {
            if (request[i] > need[processNum][i]) {
                exceedsNeed = true;
                break;
            }
        }

        // Check if request exceeds available
        bool exceedsAvailable = false;
        for (int i = 0; i < numResources; i++) {
            if (request[i] > available[i]) {
                exceedsAvailable = true;
                break;
            }
        }

        if (exceedsNeed || exceedsAvailable) {
            cout << processId << "'s request cannot be granted." << endl;
            return;
        }

        // Simulate granting the request
        cout << "Simulating granting " << processId << "'s request." << endl;

        vector<int> newAvailable = available;
        vector<vector<int>> newAllocation = allocation;

        for (int i = 0; i < numResources; i++) {
            newAvailable[i] -= request[i];
            newAllocation[processNum][i] += request[i];
        }

        // Print new Need matrix
        cout << "New Need" << endl;
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                int newNeed = max[i][j] - newAllocation[i][j];
                cout << newNeed;
                if (j < numResources - 1) cout << " ";
            }
            cout << endl;
        }

        // Check if the new state is safe
        if (isSafe(newAvailable, newAllocation)) {
            cout << processId << "'s request can be granted. The system will be in safe state." << endl;
        } else {
            cout << processId << "'s request cannot be granted. The system will be in unsafe state." << endl;
        }
    }
};

int main() {
    BankersAlgorithm banker;
    banker.readInput();
    banker.printState();
    banker.processRequest();

    return 0;
}
