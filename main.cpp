#include "FireWallManager.hpp"
#include <exception>

int main() {
  FirewallManager instance;
  try {
    instance + "127.0.0.1" + "80";
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
  }
  executeRules(instance.getFilePath().c_str());

  undoChanges(instance.getFilePath().c_str());
  return 0;
}
