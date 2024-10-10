#include "hanoi.hh"
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>
#include <nlohmann/json.hpp>
#include <ncurses.h>

using json = nlohmann::json;

// Function to save the rod configuration to a JSON file
void saveConfigToJson(const Rod& sourceRod, const Rod& targetRod, const Rod& auxRod) {
    json config;

    // Convert rod states into JSON format
    config["rods"]["Source"] = sourceRod.getDisks();
    config["rods"]["Target"] = targetRod.getDisks();
    config["rods"]["Auxiliary"] = auxRod.getDisks();

    // Count the total number of disks
    config["num_disks"] = sourceRod.getDisks().size() + targetRod.getDisks().size() + auxRod.getDisks().size();

    // Write JSON to file
    std::ofstream file("config.json");
    file << config.dump(4);  // Pretty-print JSON
    file.close();
}

// Function to load the rod configuration from a JSON file
bool loadConfigFromJson(Rod& sourceRod, Rod& targetRod, Rod& auxRod, int& n) {
    std::ifstream file("config.json");
    if (!file.is_open()) {
        std::cerr << "Error opening config.json.\n";
        return false;
    }

    json config;
    file >> config;

    // Load disk data from JSON into the rods
    for (int disk : config["rods"]["Source"]) {
        sourceRod.addDisk(disk);
    }
    for (int disk : config["rods"]["Target"]) {
        targetRod.addDisk(disk);
    }
    for (int disk : config["rods"]["Auxiliary"]) {
        auxRod.addDisk(disk);
    }

    // Get the number of disks
    n = config["num_disks"];
    return true;
}

// Function to log each move to log.txt
void logMoveToFile(int disk, const std::string& fromRod, const std::string& toRod) {
    std::ofstream file("log.txt", std::ios::app);  // Open in append mode
    file << "Moved disk " << disk << " from " << fromRod << " to " << toRod << "\n";
    file.close();
}

// Function to display the contents of the log.txt file after the program finishes
void displayLogFile() {
    std::ifstream file("log.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening log.txt\n";
        return;
    }

    std::cout << "\n--- Log of Moves ---\n";
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
    }
    file.close();
}

// TUI setup function
void setupTUI() {
    initscr();  // Initialize the screen
    cbreak();   // Disable line buffering
    noecho();   // Don't echo the input
    curs_set(0); // Hide the cursor
}

// Optimized function to update the TUI after each move, with better spacing
void updateTUI(const Rod& sourceRod, const Rod& targetRod, const Rod& auxRod) {
    mvprintw(0, 0, "Tower of Hanoi TUI");
    mvprintw(1, 0, "-------------------");

    // Only update the Source rod display
    mvprintw(3, 0, "Source: ");
    clrtoeol();  // Clear to end of the line to overwrite only the relevant part
    for (int disk : sourceRod.getDisks()) {
        printw("[%d]   ", disk);  // Add extra spacing between each disk
    }

    // Only update the Auxiliary rod display
    mvprintw(5, 0, "Auxiliary: ");
    clrtoeol();  // Clear to end of the line to overwrite only the relevant part
    for (int disk : auxRod.getDisks()) {
        printw("[%d]   ", disk);  // Add extra spacing between each disk
    }

    // Only update the Target rod display
    mvprintw(7, 0, "Target: ");
    clrtoeol();  // Clear to end of the line to overwrite only the relevant part
    for (int disk : targetRod.getDisks()) {
        printw("[%d]   ", disk);  // Add extra spacing between each disk
    }

    refresh(); // Refresh the screen to show updates
}

// Recursive Tower of Hanoi function with logging and TUI updates
void towerOfHanoiRecursive(int n, Rod& sourceRod, Rod& targetRod, Rod& auxRod) {
    if (n == 1) {
        int disk = sourceRod.getTopDisk();
        sourceRod.moveTopDiskTo(targetRod);
        logMoveToFile(disk, sourceRod.getName(), targetRod.getName());

        // Update TUI after every move
        updateTUI(sourceRod, targetRod, auxRod);

        // Print the move log at the bottom of the screen
        mvprintw(10, 0, "Moved disk %d from %s to %s", disk, sourceRod.getName().c_str(), targetRod.getName().c_str());
        clrtoeol();  // Clear to end of the line
        refresh();
        return;
    }

    towerOfHanoiRecursive(n - 1, sourceRod, auxRod, targetRod);
    int disk = sourceRod.getTopDisk();
    sourceRod.moveTopDiskTo(targetRod);
    logMoveToFile(disk, sourceRod.getName(), targetRod.getName());

    // Update TUI after every move
    updateTUI(sourceRod, targetRod, auxRod);

    // Print the move log at the bottom of the screen
    mvprintw(10, 0, "Moved disk %d from %s to %s", disk, sourceRod.getName().c_str(), targetRod.getName().c_str());
    clrtoeol();  // Clear to end of the line
    refresh();

    towerOfHanoiRecursive(n - 1, auxRod, targetRod, sourceRod);
}

// Main Tower of Hanoi function with multithreading support
void towerOfHanoi(int n, Rod& sourceRod, Rod& targetRod, Rod& auxRod, int threadLimit) {
    std::vector<std::thread> threads;
    std::atomic<int> moveCounter(0);

    // Set up the TUI
    setupTUI();

    // Run the Tower of Hanoi algorithm
    towerOfHanoiRecursive(n, sourceRod, targetRod, auxRod);

    // End the ncurses session
    endwin();
}
