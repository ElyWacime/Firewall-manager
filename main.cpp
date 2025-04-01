#include "FireWallManager.hpp"

int main() {
  FirewallManager instance;
  try {
    instance.addRule("127.0.0.1");
  } catch (const std::exception &e) {
    std::cout << "Caught Exception: " << e.what() << std::endl;
  }
  try {
    instance.addRule("80");
  } catch (const std::exception &e) {
    std::cout << "Caught Exception: " << e.what() << std::endl;
  }
  std::cout << "printing rules..." << std::endl;
  std::vector<std::string> blockedIPs = instance.getBlockedIPs();
  std::vector<std::string> blockedPorts = instance.getBlockedPorts();
  for (long unsigned int i = 0; i < blockedIPs.size(); i++) {
    std::cout << blockedIPs[i] << std::endl;
  }
  for (long unsigned int i = 0; i < blockedIPs.size(); i++) {
    std::cout << blockedPorts[i] << std::endl;
  }
  return 0;
}
