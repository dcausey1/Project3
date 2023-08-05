#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <set>
#include <algorithm>
#include <map>
#include <queue>
#include <stack>
#include <chrono>

class Airline {
public:
    Airline(std::string name) : name(name), totalMinutesDelayed(0) {}

    std::string getName() const {
        return name;
    }

    void addMinutesDelayed(int minutes) {
        totalMinutesDelayed += minutes;
    }

    int getTotalMinutesDelayed() const {
        return totalMinutesDelayed;
    }

private:
    std::string name;
    int totalMinutesDelayed;
};

class AirportBST {
private:
    struct AirportNode {
        AirportNode(std::string airportCode) : code(airportCode) {}

        std::string getCode() const {
            return code;
        }

        void addAirline(std::string airlineName, int statMinutesDelayedTotal) {
            // Check if the airline already exists in the map
            auto it = airlines.find(airlineName);
            if (it == airlines.end()) {
                // If the airline does not exist, create a new entry
                airlines.emplace(airlineName, Airline(airlineName));
                it = airlines.find(airlineName); // Update the iterator after emplacing the new entry
            }

            // Update the total minutes delayed for the airline
            it->second.addMinutesDelayed(statMinutesDelayedTotal);
        }

        int getTotalMinutesDelayed() const {
            int totalMinutesDelayed = 0;
            for (const auto &airlinePair: airlines) {
                totalMinutesDelayed += airlinePair.second.getTotalMinutesDelayed();
            }
            return totalMinutesDelayed;
        }

        std::map<std::string, Airline> &getAirlines() {
            return airlines;
        }


        std::shared_ptr<AirportNode> left;
        std::shared_ptr<AirportNode> right;

    private:
        std::string code;
        std::map<std::string, Airline> airlines;
    };

public:
    AirportBST() : root(nullptr) {}

    void insertAirport(std::string airportCode) {
        root = insertRecursive(root, airportCode);
    }

    void addAirlineToAirport(std::string airportCode, std::string airlineName, int statMinutesDelayedTotal) {
        addAirlineToAirportRecursive(root, airportCode, airlineName, statMinutesDelayedTotal);
    }

    void printInOrder() const {
        printInOrderRecursive(root);
    }

    void printCodes() const {
        printCodesRecursive(root);
    }

    std::map<std::string, int> getTotalMinutesDelayedByAirline() const {
        std::map<std::string, int> airlineTotalMinutes;

        getTotalMinutesDelayedRecursive(root, airlineTotalMinutes);

        return airlineTotalMinutes;
    }

    void DFS(const std::string &code) {
        helperDFS(code);
    }

    // Public member function of class AirportBST
    void BFS(const std::string &code) {
        helperBFS(code);
    }


private:
    std::shared_ptr<AirportNode> root;

    // Private helper functions for tree operations
    std::shared_ptr<AirportNode> insertRecursive(std::shared_ptr<AirportNode> currentNode, std::string airportCode) {
        if (currentNode == nullptr) {
            return std::make_shared<AirportNode>(airportCode);
        }

        if (airportCode < currentNode->getCode()) {
            currentNode->left = insertRecursive(currentNode->left, airportCode);
        } else if (airportCode > currentNode->getCode()) {
            currentNode->right = insertRecursive(currentNode->right, airportCode);
        }

        return currentNode;
    }

    void addAirlineToAirportRecursive(std::shared_ptr<AirportNode> currentNode, std::string airportCode,
                                      std::string airlineName, int statMinutesDelayedTotal) {
        if (currentNode != nullptr) {
            if (currentNode->getCode() == airportCode) {
                currentNode->addAirline(airlineName, statMinutesDelayedTotal);
            } else if (airportCode < currentNode->getCode()) {
                addAirlineToAirportRecursive(currentNode->left, airportCode, airlineName, statMinutesDelayedTotal);
            } else {
                addAirlineToAirportRecursive(currentNode->right, airportCode, airlineName, statMinutesDelayedTotal);
            }
        }
    }

    void printInOrderRecursive(std::shared_ptr<AirportNode> currentNode) const {
        if (currentNode != nullptr) {
            printInOrderRecursive(currentNode->left);
            std::cout << "Airport: " << currentNode->getCode() << std::endl << "Airlines: ";
            for (const auto &airlinePair: currentNode->getAirlines()) {
                std::string str;
                // Do not print any leading or trailing quote marks
                if (airlinePair.first[0] == '"') {
                    str = airlinePair.first.substr(1);
                    std::cout << "\"" << str << "\"" << ", ";
                }
                else if (airlinePair.first[airlinePair.first.size() - 1] == '"') {
                    str = airlinePair.first.substr(0, airlinePair.first.size() - 1);
                    std::cout << "\"" << str << "\"" << ", ";
                }
                else {
                    std::cout << "\"" << airlinePair.first << "\"" << ", ";
                }
            }
            int airportTotalMinutedDelayed = 0;
            for (const auto &airlinePair: currentNode->getAirlines()) {
                airportTotalMinutedDelayed += airlinePair.second.getTotalMinutesDelayed();
            }
            std::cout << std::endl;
            std::cout << "Total Minutes Delayed for the Airport: " << airportTotalMinutedDelayed << " minutes"
                      << std::endl;
            std::cout << std::endl;
            printInOrderRecursive(currentNode->right);
        }
    }

    void printCodesRecursive(std::shared_ptr<AirportNode> currentNode) const {
        if (currentNode != nullptr) {
            printCodesRecursive(currentNode->left);
            std::cout << currentNode->getCode() << " ";
            printCodesRecursive(currentNode->right);
        }
    }

    void getTotalMinutesDelayedRecursive(std::shared_ptr<AirportNode> currentNode,
                                         std::map<std::string, int> &airlineTotalMinutes) const {
        if (currentNode != nullptr) {
            const std::map<std::string, Airline> &airlines = currentNode->getAirlines();
            for (const auto &airlinePair: airlines) {
                std::string airlineName = airlinePair.first;
                int minutesDelayed = airlinePair.second.getTotalMinutesDelayed();

                // Check if the airline already exists in the map
                auto it = airlineTotalMinutes.find(airlineName);
                if (it == airlineTotalMinutes.end()) {
                    // If the airline does not exist, create a new entry with total minutes delayed set to 0
                    airlineTotalMinutes[airlineName] = 0;
                    it = airlineTotalMinutes.find(airlineName); // Update the iterator after emplacing the new entry
                }

                // Update the total minutes delayed for the airline
                it->second += minutesDelayed;
            }

            getTotalMinutesDelayedRecursive(currentNode->left, airlineTotalMinutes);
            getTotalMinutesDelayedRecursive(currentNode->right, airlineTotalMinutes);
        }
    }

    void helperDFS(const std::string &code) {
        // Check that there is an existing root
        if (root == nullptr) {
            return;
        }

        // Create stack for visiting tree nodes
        std::stack<std::shared_ptr<AirportNode>> stack;
        // Add the root to the stack
        stack.push(root);

        while (!stack.empty()) {
            // Current node set to node at top of stack (peek)
            std::shared_ptr<AirportNode> currentNode = stack.top();
            // Remove current node from the stack
            stack.pop();

            // Print data if code is found and return
            if (currentNode->getCode() == code) {
                std::cout << "Airport: " << currentNode->getCode() << " found!" << std::endl;
                std::cout << "Total minutes delayed: " << currentNode->getTotalMinutesDelayed() << " minutes"
                          << std::endl;
                return;
            }

            // Push the right child node of the current node if it exists
            if (currentNode->right != nullptr) {
                stack.push(currentNode->right);
            }

            // Push the left child node of the current node if it exists
            if (currentNode->left != nullptr) {
                stack.push(currentNode->left);
            }
        }

        std::cout << code << " not found!" << std::endl;
    }

    // Private member function of class AirportBST
    void helperBFS(const std::string &code) {
        // Check that there is an existing root
        if (root == nullptr) {
            return;
        }
        // Create queue for visiting tree nodes
        std::queue<std::shared_ptr<AirportNode>> q;
        // Add the root to the queue
        q.push(root);

        // Iterate through q until empty
        while (!q.empty()) {
            // Current node set to node at front of queue
            std::shared_ptr<AirportNode> currentNode = q.front();
            // Print data if code is found and return
            if (currentNode->getCode() == code) {
                std::cout << "Airport: " << currentNode->getCode() << " found!" << std::endl;
                std::cout << "Total minutes delayed: " << currentNode->getTotalMinutesDelayed() << " minutes"
                          << std::endl;
                return;
            }
            // Push the left and right child nodes of current node if they exist
            if (currentNode->left != nullptr) {
                q.push(currentNode->left);
            }
            if (currentNode->right != nullptr) {
                q.push(currentNode->right);
            }
            // Remove current node from the queue
            q.pop();
        }
        std::cout << code << " not found!" << std::endl;
    }
};

int main() {
    AirportBST airportBST;

    // Read data from CSV file and populate the AirportBST
    std::ifstream inputFile("airlines.csv");
    if (!inputFile.is_open()) {
        std::cerr << "Error opening the CSV file." << std::endl;
        return 1;
    }

    std::string line;
    std::getline(inputFile, line); // Skip the header line
    std::vector<std::string> elements;
    std::vector<int> late;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string airportCode, carriers, lateAircraftDelays, timeMonth;
        std::string timeMonthName, timeYear, statDelaysCarrier, statDelaysLateAircraft;
        std::string statDelaysNAS, statDelaysSecurity, statDelaysWeather, statCarriersName, statCarriersTotal;
        std::string statFlightsCancelled, statFlightsDelayed, statFlightsDiverted, statFlightsOnTime;
        std::string statFlightsTotal, statMinutesDelayedCarrier, statMinutesDelayedLateAircraft;
        std::string statMinutesDelayedNAS, statMinutesDelayedSecurity, statMinutesDelayedTotal, statMinDelayedWeather;

        // Read the columns from the CSV line
        while (std::getline(iss, airportCode, ',')) {
            while (std::getline(iss, carriers, ',')) {
                if (std::all_of(carriers.begin(), carriers.end(), ::isdigit)) {
                    break;
                } else {
                    elements.push_back(carriers);
                }
            }
            break;
        }
        std::getline(iss, timeMonth, ',');
        std::getline(iss, timeMonthName, ',');
        std::getline(iss, timeYear, ',');
        std::getline(iss, statDelaysCarrier, ',');
        std::getline(iss, statDelaysLateAircraft, ',');
        std::getline(iss, statDelaysNAS, ',');
        std::getline(iss, statDelaysSecurity, ',');
        std::getline(iss, statDelaysWeather, ',');
        while (std::getline(iss, statCarriersName, ',')) {
            if (std::all_of(statCarriersName.begin(), statCarriersName.end(), ::isdigit)) {
                break;
            }
        }
        std::getline(iss, statCarriersTotal, ',');
        std::getline(iss, statFlightsCancelled, ',');
        std::getline(iss, statFlightsDelayed, ',');
        std::getline(iss, statFlightsDiverted, ',');
        std::getline(iss, statFlightsOnTime, ',');
        std::getline(iss, statFlightsTotal, ',');
        std::getline(iss, statMinutesDelayedCarrier, ',');
        std::getline(iss, statMinutesDelayedLateAircraft, ',');
        std::getline(iss, statMinutesDelayedNAS, ',');
        std::getline(iss, statMinutesDelayedSecurity, ',');
        std::getline(iss, statMinutesDelayedTotal, ',');
        std::getline(iss, statMinDelayedWeather, ',');
        statMinutesDelayedTotal = statMinutesDelayedSecurity;
        // Add data into the tree
        airportBST.insertAirport(airportCode);
        for (size_t i = 0; i < elements.size(); ++i) {
            airportBST.addAirlineToAirport(airportCode, elements[i], std::stoi(statMinutesDelayedTotal));
        }

        // Clear the vectors for the next iteration
        elements.clear();
        late.clear();
    }


    // Initialize variables for interface and execution timer
    std::string strIn;
    bool stop = true;
    auto startTime =std::chrono::high_resolution_clock::now();
    auto endTIme=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::microseconds>(endTIme - startTime);
    int user_input;

    // UI menu
    while(stop) {
        std::cout << "****************************************************" << std::endl;
        std::cout << "****************************************************" << std::endl;
        std::cout << "Do You Want To Find the Best Airport For Less Delays?" << std::endl;
        std::cout << "****************************************************" << std::endl;
        std::cout << "****************************************************" << std::endl;
        std::cout << "Menu:" << std::endl;
        std::cout << "1: Print inorder traversal of tree" << std::endl;
        std::cout << "2: BFS" << std::endl;
        std::cout << "3: DFS" << std::endl;
        std::cout << "4: DFS & BFS" << std::endl;
        std::cout << "5: Quit" << std::endl;

        std::cout << "Please Enter a selection 1-4:" << std::endl;
        std::cin>> user_input;

        // Print inorder traversal
        if (user_input == 1) {
            airportBST.printInOrder();
        }

        // Execute BFS and show time recorded
        if(user_input == 2) {
            std::cout << "Airport codes:" << std::endl;
            airportBST.printCodes();
            std::cout << "\nEnter code from list above: " << std::endl;
            std::cin >> strIn;
            std::cout << "BFS:" << std::endl;
            startTime = std::chrono::high_resolution_clock::now();
            airportBST.BFS(strIn);
            endTIme = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(endTIme - startTime);
            std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
            std::cout << std::endl;

        }

        // Execute DFS and show time recorded
        if(user_input == 3){
            std::cout << "Airport codes:" << std::endl;
            airportBST.printCodes();
            std::cout << "\nEnter code from list above: " << std::endl;
            std::cin >> strIn;
            std::cout << "DFS:" << std::endl;
            startTime = std::chrono::high_resolution_clock::now();
            airportBST.DFS(strIn);
            endTIme = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(endTIme - startTime);
            std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
            std::cout << std::endl;
        }

        // Execute both BFS and DFS and show times recorded for each
        if (user_input == 4){
            std::cout << "Airport codes:" << std::endl;
            airportBST.printCodes();
            std::cout << "\nEnter code from list above: " << std::endl;
            std::cin >> strIn;
            std::cout << "BFS:" << std::endl;
            startTime = std::chrono::high_resolution_clock::now();
            airportBST.BFS(strIn);
            endTIme = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(endTIme - startTime);
            std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
            std::cout << std::endl;
            std::cout << "DFS:" << std::endl;
            startTime = std::chrono::high_resolution_clock::now();
            airportBST.DFS(strIn);
            endTIme = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::microseconds>(endTIme - startTime);
            std::cout << "Execution time: " << duration.count() << " microseconds" << std::endl;
            std::cout << std::endl;
        }

        // Exit
        if(user_input == 5){
            std::cout << "Thank You For Using Our Application" << std::endl;
            stop = false;
            break;}
    }

    // Close program
    return 0;
}
