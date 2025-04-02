#include "FireWallManager.hpp"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
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
  executeRules();
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

// execute
void FirewallManager::executeRules() const {
  std::fstream rulesFile;
  std::string currentRule;
  rulesFile.open("rules.txt", std::ios::in);
  if (!rulesFile.is_open()) {
    std::cerr << "can't open rules.txt";
  }
  while (getline(rulesFile, currentRule)) {
    if (currentRule.find('.') != std::string::npos) {
      std::cout << "Blocking IP: " << currentRule << std::endl;
      std::string command =
          "sudo iptables -A INPUT -s " + currentRule + " -j DROP";
      system(command.c_str());
    } else {
      std::cout << "Blocking port: " << currentRule << std::endl;
      std::string command =
          "sudo iptables -A INPUT -p tcp --dport " + currentRule + " -j DROP";
      system(command.c_str());
    }
  }
  rulesFile.close();
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

// undo the changes from the os
void undoChanges(const std::string &rulesFile) {
  std::ifstream file(rulesFile.c_str());
  std::string rule;

  if (!file.is_open()) {
    std::cerr << "Error opening rules file: " << rulesFile << std::endl;
    return;
  }

  while (getline(file, rule)) {
    if (rule.find(".") != std::string::npos) {
      // It's an IP address, remove the rule blocking the IP
      std::string command = "sudo iptables -D INPUT -s " + rule + " -j DROP";
      if (system(command.c_str()) != 0) {
        std::cout << "Error removing IP from iptables: " + rule << std::endl;
      } else {
        std::cout << "Removed block for IP: " << rule << std::endl;
      }
    } else {
      // It's a port, remove the rule blocking the port
      std::string commandDrop =
          "sudo iptables -D INPUT -p tcp --dport " + rule + " -j DROP";
      if (system(commandDrop.c_str()) != 0) {
        std::cout << "Error removing Port from iptables: " + rule << std::endl;
      } else {
        std::cout << "Removed block for Port: " << rule << std::endl;
      }
    }
  }

  file.close();
}
