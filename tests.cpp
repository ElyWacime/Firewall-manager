#include "FireWallManager.hpp"

#include "FireWallManager.hpp"
#include <iostream>

void clearRulesFile() {
  std::ofstream rulesFile("rules.txt", std::ios::trunc);
  rulesFile.close();
}

void testAddRule() {
  std::cout << "\n=== Test: Adding Rules ===\n";
  clearRulesFile();
  FirewallManager fw;
  try {
    fw.addRule("10.0.0.1");
    fw.addRule("8081");
    fw.addRule("5000");
    std::cout << "✅ Rules added successfully.\n";
  } catch (const std::exception &e) {
    std::cout << "❌ Exception: " << e.what() << std::endl;
  }
}

void testDuplicateRule() {
  std::cout << "\n=== Test: Adding Duplicate Rule ===\n";
  clearRulesFile();
  FirewallManager fw;
  try {
    fw.addRule("172.16.0.1");
    fw.addRule("172.16.0.1"); // Should throw exception
    std::cout << "❌ Failed (Duplicate not detected)\n";
  } catch (const FirewallManager::RuleAlreadyExists &e) {
    std::cout << "✅ Exception Caught: " << e.what() << std::endl;
  }
}

void testRemoveRule() {
  std::cout << "\n=== Test: Removing Rules ===\n";
  clearRulesFile();
  FirewallManager fw;
  try {
    fw.addRule("192.168.2.1");
    fw.addRule("9090");

    fw.removeRule("192.168.2.1");
    fw.removeRule("9090");

    std::cout << "✅ Rules removed successfully.\n";
  } catch (const std::exception &e) {
    std::cout << "❌ Exception: " << e.what() << std::endl;
  }
}

void testRemoveNonExistingRule() {
  std::cout << "\n=== Test: Removing Non-Existing Rule ===\n";
  clearRulesFile();
  FirewallManager fw;
  try {
    fw.removeRule("7777"); // Should throw exception
    std::cout << "❌ Failed (Non-existent rule not detected)\n";
  } catch (const FirewallManager::RuleDoNotExist &e) {
    std::cout << "✅ Exception Caught: " << e.what() << std::endl;
  }
}

void testOperatorOverloading() {
  std::cout << "\n=== Test: Operator Overloading ===\n";
  clearRulesFile();
  FirewallManager fw;
  try {
    fw + "203.0.113.1" + "6000";
    if (fw.isBlockedIP("203.0.113.1") && fw.isBlockedPort("6000")) {
      std::cout << "✅ Operator `+` works correctly.\n";
    } else {
      std::cout << "❌ Operator `+` failed.\n";
    }

    fw - "203.0.113.1" - "6000";
    if (!fw.isBlockedIP("203.0.113.1") && !fw.isBlockedPort("6000")) {
      std::cout << "✅ Operator `-` works correctly.\n";
    } else {
      std::cout << "❌ Operator `-` failed.\n";
    }
  } catch (const std::exception &e) {
    std::cout << "❌ Exception: " << e.what() << std::endl;
  }
}

void testSaveAndLoad() {
  std::cout << "\n=== Test: Save and Load Rules ===\n";
  clearRulesFile();
  {
    FirewallManager fw;
    fw + "198.51.100.1" + "7000";
  } // Destructor should save the rules

  FirewallManager fw2;
  if (fw2.isBlockedIP("198.51.100.1") && fw2.isBlockedPort("7000")) {
    std::cout << "✅ Save and Load works correctly.\n";
  } else {
    std::cout << "❌ Save and Load failed.\n";
  }
}
void testListRules() {
  std::cout << "\n" << std::endl;
  std::cout << "=== Test: list rules ===" << std::endl;
  FirewallManager fw;
  // Adding test IPs and Ports
  try {
    fw + "192.168.1.1"; // Add an IP
    fw + "8080";        // Add a port
    fw + "10.0.0.1";    // Add another IP
    fw + "443";         // Add another port
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  // Now test the listRules function
  std::cout << "Listing Rules: " << std::endl;
  fw.listRules();
}

int main() {
  testAddRule();
  testDuplicateRule();
  testRemoveRule();
  testRemoveNonExistingRule();
  testOperatorOverloading();
  testSaveAndLoad();
  testListRules();
  return 0;
}
