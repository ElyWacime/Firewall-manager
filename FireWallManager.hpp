#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class FirewallManager {
private:
  std::vector<std::string> blockedIPs;
  std::vector<int> blokedPorts;

public:
  // constractor
  FirewallManager();
  ~FirewallManager();
  // configuration functions
  void addRule(std::string ip);
  void addRule(int port);
  void removeRule(std::string ip);
  void removeRule(int port);
  void listRules() const;
  bool isBlocked(std::string ip);
  bool isBlocked(int port);
  void loadRulesFromFile();
  void saveRulesToFile();

  // overloading operator
  FirewallManager &operator+(const std::string ip);
  FirewallManager &operator+(const int port);
  FirewallManager &operator-(const std::string ip);
  FirewallManager &operator-(const int port);

  // getters
  std::vector<std::string> getBlockedIPs() { return blockedIPs; }
  std::vector<int> getBlockedPorts() { return blokedPorts; }
};
std::ostream &operator<<(std::ostream &out, const FirewallManager &obj);
