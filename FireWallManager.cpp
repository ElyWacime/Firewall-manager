#include "FireWallManager.hpp"

// constractor / distractor
FirewallManager::FirewallManager() { loadRulesFromFile(); }

FirewallManager::~FirewallManager() { saveRulesToFile(); }

// configuration functions
void FirewallManager::loadRulesFromFile() {
  std::string currentIP;
  std::ifstream rulesFiles("rules.txt");
  while (getline(rulesFiles, currentIP)) {
    FirewallManager::addRule(currentIP);
  }
  rulesFiles.close();
}

void FirewallManager::saveRulesToFile() {
  std::ofstream rulesFile("rules.txt");
  int i = 0;
  for (auto it = this->blockedIPs.begin(); it != this->blockedIPs.end(); ++it) {
    addRule(*it);
  }
  i = 0;
  while (this->blokedPorts.at(i)) {
    addRule(this->blokedPorts.at(i));
  }
}

void FirewallManager::addRule(std::string ip) {
  this->blockedIPs.push_back(ip);
}

void FirewallManager::addRule(int port) { this->blokedPorts.push_back(port); }

void FirewallManager::removeRule(std::string ip) {
  this->blockedIPs.erase(
      std::remove(this->blockedIPs.begin(), this->blockedIPs.end(), ip),
      this->blockedIPs.end());
}
