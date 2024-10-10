#include "hanoi.hh"
#include <fstream>
#include <iostream>
#include <ncurses.h> // Include ncurses for the TUI functions
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Function to save the rod configuration to a JSON file
void saveConfigToJson(const Rod &sourceRod, const Rod &targetRod,
                      const Rod &auxRod) {
  json config;

  // Convert rod states into JSON format
  config["rods"]["Source"] = sourceRod.getDisks();
  config["rods"]["Target"] = targetRod.getDisks();
  config["rods"]["Auxiliary"] = auxRod.getDisks();

  // Count the total number of disks
  config["num_disks"] = sourceRod.getDisks().size() +
                        targetRod.getDisks().size() + auxRod.getDisks().size();

  // Write JSON to file
  std::ofstream file("config.json");
  file << config.dump(4); // Pretty-print JSON
  file.close();
}

// Function to load the rod configuration from a JSON file
bool loadConfigFromJson(Rod &sourceRod, Rod &targetRod, Rod &auxRod, int &n) {
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
void logMoveToFile(int disk, const std::string &fromRod,
                   const std::string &toRod) {
  std::ofstream file("log.txt", std::ios::app); // Open in append mode
  file << "Moved disk " << disk << " from " << fromRod << " to " << toRod
       << "\n";
  file.close();
}

// Function to display the contents of the log.txt file after the program
// finishes
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
  initscr();   // Initialize the screen
  cbreak();    // Disable line buffering
  noecho();    // Don’t echo the input
  curs_set(0); // Hide the cursor
  clear();     // Clear the screen
}

// Optimized function to update the TUI after each move, with better spacing
void updateTUI(const Rod &sourceRod, const Rod &targetRod, const Rod &auxRod,
               int moveCount) {
  mvprintw(0, 0, "Tower of Hanoi Simulation");
  mvprintw(1, 0, "------------------------");

  mvprintw(3, 0, "Source: ");
  clrtoeol(); // Clear to end of line
  for (int disk : sourceRod.getDisks()) {
    printw("[%d] ", disk);
  }

  mvprintw(5, 0, "Auxiliary: ");
  clrtoeol(); // Clear to end of line
  for (int disk : auxRod.getDisks()) {
    printw("[%d] ", disk);
  }

  mvprintw(7, 0, "Target: ");
  clrtoeol(); // Clear to end of line
  for (int disk : targetRod.getDisks()) {
    printw("[%d] ", disk);
  }

  // Display the move count at the bottom
  mvprintw(9, 0, "Move %d", moveCount);
  refresh(); // Refresh the screen to display the updated TUI
}

// Recursive Tower of Hanoi function with logging, move count, and TUI animation
void towerOfHanoiRecursive(int n, Rod &sourceRod, Rod &targetRod, Rod &auxRod,
                           int &moveCount) {
  if (n == 1) {
    moveCount++;                                   // Increment move count
    sourceRod.moveTopDiskTo(targetRod, moveCount); // Pass move count
    logMoveToFile(sourceRod.getTopDisk(), sourceRod.getName(),
                  targetRod.getName());
    updateTUI(sourceRod, targetRod, auxRod, moveCount); // Update TUI after move
    return;
  }

  towerOfHanoiRecursive(n - 1, sourceRod, auxRod, targetRod, moveCount);
  moveCount++;                                   // Increment move count
  sourceRod.moveTopDiskTo(targetRod, moveCount); // Pass move count
  logMoveToFile(sourceRod.getTopDisk(), sourceRod.getName(),
                targetRod.getName());
  updateTUI(sourceRod, targetRod, auxRod, moveCount); // Update TUI after move
  towerOfHanoiRecursive(n - 1, auxRod, targetRod, sourceRod, moveCount);
}

// Main Tower of Hanoi function
void towerOfHanoi(int n, Rod &sourceRod, Rod &targetRod, Rod &auxRod,
                  int threadLimit) {
  int moveCounter = 0; // Initialize move counter

  setupTUI(); // Setup ncurses TUI

  towerOfHanoiRecursive(n, sourceRod, targetRod, auxRod, moveCounter);

  endwin(); // End ncurses session

  // Print the total number of moves
  std::cout << "\nTotal moves: " << moveCounter << std::endl;
}
