#include "FireWallManager.hpp"
#include <stdio.h>
#include <string>

// constractor / distractor
FirewallManager::FirewallManager() {
  std::cout << "constractor called;" << std::endl;
  loadRulesFromFile();
}

FirewallManager::~FirewallManager() {
  std::cout << "Destractor called" << std::endl;
  saveRulesToFile();
}

// configuration functions
void FirewallManager::loadRulesFromFile() {
  std::string currentRule;
  std::ifstream rulesFiles("rules.txt");
  while (getline(rulesFiles, currentRule)) {
    if (currentRule.find(".") != std::string::npos)
      this->blockedIPs.push_back(currentRule);
    else
      this->blockedPorts.push_back(currentRule);
  }
  rulesFiles.close();
}

void FirewallManager::saveRulesToFile() {
  std::fstream rulesFile;
  rulesFile.open("rules.txt", std::ios::out);
  if (rulesFile.is_open()) {
    for (std::vector<std::string>::iterator it = this->blockedIPs.begin();
         it != this->blockedIPs.end(); ++it)
      rulesFile << *it << std::endl;
    for (std::vector<std::string>::iterator iter = this->blockedPorts.begin();
         iter != this->blockedPorts.end(); ++iter)
      rulesFile << *iter << std::endl;
    rulesFile.close();
  }
}

void FirewallManager::addRule(std::string rule) {
  for (std::vector<std::string>::iterator it = this->blockedIPs.begin();
       it != this->blockedIPs.end(); ++it) {
    if (*it == rule)
      throw FirewallManager::RuleAlreadyExists();
  }
  for (std::vector<std::string>::iterator it = this->blockedPorts.begin();
       it != this->blockedPorts.end(); ++it) {
    if (*it == rule)
      throw FirewallManager::RuleAlreadyExists();
  }
  if (rule.find(".") != std::string::npos)
    this->blockedIPs.push_back(rule);
  else
    this->blockedPorts.push_back(rule);
}

void FirewallManager::removeRule(std::string ip) {
  for (std::vector<std::string>::iterator iter = this->blockedIPs.begin();
       iter != this->blockedIPs.end();) {
    if (*iter == ip)
      iter = this->blockedIPs.erase(iter);
    else
      ++iter;
  }
}

// exception
const char *FirewallManager::RuleAlreadyExists::what() const throw() {
  return "Rule already exist it's being ingored!";
}
