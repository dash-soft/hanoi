
#include "rod.hh"
#include <algorithm>
#include <iostream>

// Constructor: Initializes the rod with the given name
Rod::Rod(const std::string &name) : name(name) {}

// Adds a disk to the rod and ensures they remain sorted by size (larger at the
// bottom)
void Rod::addDisk(int disk) {
  disks.push_back(disk);
  std::sort(disks.begin(), disks.end(),
            std::greater<int>()); // Ensure disks are sorted by size
}

// Moves the top disk from this rod to the target rod and prints the move with
// the move count
void Rod::moveTopDiskTo(Rod &targetRod, int moveCount) {
  if (!disks.empty()) {
    int topDisk = disks.back();
    disks.pop_back();
    targetRod.addDisk(topDisk);
    std::cout << "Move " << moveCount << ": Moved disk " << topDisk << " from "
              << name << " to " << targetRod.getName() << std::endl;
  }
}

// Returns the name of the rod
const std::string &Rod::getName() const { return name; }

// Displays the current disks on the rod
void Rod::displayRod() const {
  std::cout << "Rod " << name << ": ";
  for (int disk : disks) {
    std::cout << disk << " ";
  }
  std::cout << std::endl;
}

// Displays the initial state of the rod, visualizing the disk sizes with
// asterisks
void Rod::displayInitialState() const {
  std::cout << name << ": ";
  for (int disk : disks) {
    std::cout << "[" << std::string(disk, '*')
              << "] "; // Visualize disk sizes with asterisks
  }
  std::cout << std::endl;
}

// Returns a reference to the vector of disks
const std::vector<int> &Rod::getDisks() const { return disks; }

// Returns the top disk on the rod, or -1 if the rod is empty
int Rod::getTopDisk() const {
  if (!disks.empty()) {
    return disks.back();
  }
  return -1; // Return -1 if the rod is empty
}
