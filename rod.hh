
#ifndef ROD_HH
#define ROD_HH

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

class Rod {
public:
  Rod(const std::string &name);

  void addDisk(int disk);
  void moveTopDiskTo(Rod &targetRod);
  const std::string &getName() const;
  void displayRod() const;
  void displayInitialState() const;

  const std::vector<int> &getDisks() const; // Returns the list of disks
  int getTopDisk() const;                   // Returns the top disk on the rod

private:
  std::string name;
  std::vector<int> disks; // Larger numbers represent larger disks
};

#endif // ROD_HH
