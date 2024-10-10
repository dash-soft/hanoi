
#include "rod.hh"

Rod::Rod(const std::string &name) : name(name) {}

void Rod::addDisk(int disk) {
  disks.push_back(disk);
  std::sort(disks.begin(), disks.end(),
            std::greater<int>()); // Ensure disks are sorted by size
}

void Rod::moveTopDiskTo(Rod &targetRod) {
  if (!disks.empty()) {
    int topDisk = disks.back();
    disks.pop_back();
    targetRod.addDisk(topDisk);
    std::cout << "Move disk " << topDisk << " from " << name << " to "
              << targetRod.getName() << std::endl;
  }
}

const std::string &Rod::getName() const { return name; }

void Rod::displayRod() const {
  std::cout << "Rod " << name << ": ";
  for (int disk : disks) {
    std::cout << disk << " ";
  }
  std::cout << std::endl;
}

void Rod::displayInitialState() const {
  std::cout << name << ": ";
  for (int disk : disks) {
    std::cout << "[" << std::string(disk, '*')
              << "] "; // Each disk size visualized by asterisks
  }
  std::cout << std::endl;
}

// New method: Returns the list of disks
const std::vector<int> &Rod::getDisks() const { return disks; }

// New method: Returns the top disk (the last one in the vector)
int Rod::getTopDisk() const {
  if (!disks.empty()) {
    return disks.back();
  }
  return -1; // Return -1 if the rod is empty
}
