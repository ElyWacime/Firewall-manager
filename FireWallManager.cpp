#include "FireWallManager.hpp"
#include <algorithm>
#include <cstdio>
#include <stdio.h>
#include <string>
#include <vector>

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

bool FirewallManager::isBlockedIP(std::string ip) {
  for (std::vector<std::string>::iterator it = this->blockedIPs.begin();
       it != this->blockedIPs.end(); ++it) {
    if (*it == ip)
      return true;
  }
  return false;
}

bool FirewallManager::isBlockedPort(std::string port) {
  for (std::vector<std::string>::iterator it = this->blockedPorts.begin();
       it != this->blockedPorts.end(); ++it) {
    if (*it == port)
      return true;
  }
  return false;
}

void FirewallManager::addRule(std::string rule) {
  if (isBlockedIP(rule))
    throw FirewallManager::RuleAlreadyExists();
  if (isBlockedPort(rule))
    throw FirewallManager::RuleAlreadyExists();
  if (rule.find(".") != std::string::npos) {
    std::cout << "adding this IP: " << rule << std::endl;
    this->blockedIPs.push_back(rule);
  } else {
    std::cout << "adding this port: " << rule << std::endl;
    this->blockedPorts.push_back(rule);
  }
}

void FirewallManager::removeRule(std::string rule) {
  std::vector<std::string>::iterator iter =
      std::find(this->blockedIPs.begin(), this->blockedIPs.end(), rule);
  std::vector<std::string>::iterator itPort =
      std::find(this->blockedPorts.begin(), this->blockedPorts.end(), rule);
  if (iter != this->blockedIPs.end()) {
    std::cout << "removing this IP: " << *iter << std::endl;
    this->blockedIPs.erase(iter);
  } else if (itPort != this->blockedPorts.end()) {
    std::cout << "removing this port: " << *itPort << std::endl;
    this->blockedPorts.erase(itPort);
  } else
    throw FirewallManager::RuleDoNotExist();
}

void FirewallManager::listRules() const {
  std::cout << "blockedIPs: " << std::endl;
  for (std::vector<std::string>::const_iterator it = this->blockedIPs.begin();
       it != this->blockedIPs.end(); ++it)
    std::cout << *it << "\t";
  std::cout << std::endl;
  std::cout << "blockedPorts: " << std::endl;
  for (std::vector<std::string>::const_iterator it = this->blockedPorts.begin();
       it != this->blockedPorts.end(); ++it)
    std::cout << *it << "\t";
  std::cout << std::endl;
}

// operators
FirewallManager &FirewallManager::operator+(const std::string rule) {
  addRule(rule);
  return *this;
}

FirewallManager &FirewallManager::operator-(const std::string rule) {
  removeRule(rule);
  return *this;
}

// exception
const char *FirewallManager::RuleAlreadyExists::what() const throw() {
  return "Rule already exist it's being ingored!";
}
const char *FirewallManager::RuleDoNotExist::what() const throw() {
  return "Rule do not exist can't remove it;";
}
