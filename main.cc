#include "hanoi.hh"
#include <iostream>
#include <fstream>

// Function to check if the config.json file exists
bool configFileExists() {
    std::ifstream file("config.json");
    return file.good();
}

// Function to clear log.txt at the start of the program
void clearLogFile() {
    std::ofstream file("log.txt");
    file.close();
}

int main() {
    int n = 0;
    int threadLimit;
    bool useExistingConfig = false;

    // Check if config.json exists
    if (configFileExists()) {
        std::cout << "A config.json file exists. Do you want to use it? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            useExistingConfig = true;
        }
    }

    // Clear log.txt before running the program
    clearLogFile();

    // Create rods
    Rod sourceRod("Source");
    Rod targetRod("Target");
    Rod auxRod("Auxiliary");

    if (useExistingConfig) {
        // Load disk configuration from config.json if user agrees
        if (!loadConfigFromJson(sourceRod, targetRod, auxRod, n)) {
            std::cerr << "Error loading config.json file.\n";
            return 1;
        }
        std::cout << "Loaded disk configuration from config.json\n";
    } else {
        // Ask the user for the number of disks and threads
        std::cout << "Enter the number of disks: ";
        std::cin >> n;

        std::cout << "Enter the number of threads to use: ";
        std::cin >> threadLimit;

        // Let the user manually enter where each disk is
        std::cout << "Please enter the location of each disk (Source, Auxiliary, Target): \n";
        for (int i = 1; i <= n; ++i) {
            std::cout << "Where is disk " << i << "? (S/A/T): ";
            char rodChoice;
            std::cin >> rodChoice;
            switch (rodChoice) {
                case 'S':
                case 's':
                    sourceRod.addDisk(i);
                    break;
                case 'A':
                case 'a':
                    auxRod.addDisk(i);
                    break;
                case 'T':
                case 't':
                    targetRod.addDisk(i);
                    break;
                default:
                    std::cout << "Invalid input. Defaulting to Source for disk " << i << ".\n";
                    sourceRod.addDisk(i);
            }
        }

        // Save the initial configuration to the config.json file
        saveConfigToJson(sourceRod, targetRod, auxRod);
    }

    // Perform the Tower of Hanoi algorithm with TUI
    towerOfHanoi(n, sourceRod, targetRod, auxRod, threadLimit);

    // Display final state of the rods
    std::cout << "\nFinal State:\n";
    sourceRod.displayRod();
    auxRod.displayRod();
    targetRod.displayRod();

    // Display the log of all moves
    displayLogFile();

    return 0;
}
