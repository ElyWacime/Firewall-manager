#pragma once

#include <exception>
#include <fstream>
#include <iostream>
#include <vector>

class FirewallManager {
private:
  std::vector<std::string> blockedIPs;
  std::vector<std::string> blockedPorts;

public:
  // constractor
  FirewallManager();
  ~FirewallManager();
  // configuration functions
  void addRule(std::string ip);
  void removeRule(std::string ip);
  void removeRule(int port);
  void listRules() const;
  bool isBlockedIP(std::string ip);
  bool isBlockedPort(std::string port);
  void loadRulesFromFile();
  void saveRulesToFile();

  // execute
  void executeRules() const;

  // overloading operator
  FirewallManager &operator+(const std::string rule);
  FirewallManager &operator-(const std::string rule);

  // getters
  std::vector<std::string> getBlockedIPs() { return blockedIPs; }
  std::vector<std::string> getBlockedPorts() { return blockedPorts; }

  class RuleAlreadyExists : public std::exception {
  public:
    virtual const char *what() const throw();
  };
  class RuleDoNotExist : public std::exception {
  public:
    virtual const char *what() const throw();
  };
};
std::ostream &operator<<(std::ostream &out, const FirewallManager &obj);
void undoChanges(const std::string &rulesFile);
