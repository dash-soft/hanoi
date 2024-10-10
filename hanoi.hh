#ifndef HANOI_HH
#define HANOI_HH

#include "rod.hh"
#include <nlohmann/json.hpp>
#include <string>

// Tower of Hanoi logic
void towerOfHanoi(int n, Rod &sourceRod, Rod &targetRod, Rod &auxRod,
                  int threadLimit);
void towerOfHanoiRecursive(int n, Rod &sourceRod, Rod &targetRod, Rod &auxRod,
                           int &moveCount);

// JSON handling functions
void saveConfigToJson(const Rod &sourceRod, const Rod &targetRod,
                      const Rod &auxRod);
bool loadConfigFromJson(Rod &sourceRod, Rod &targetRod, Rod &auxRod, int &n);

// Logging moves to file
void logMoveToFile(int disk, const std::string &fromRod,
                   const std::string &toRod);

// Display the moves log file
void displayLogFile();

// TUI setup function (if you need the ncurses-based terminal UI)
void setupTUI();

#endif // HANOI_HH
