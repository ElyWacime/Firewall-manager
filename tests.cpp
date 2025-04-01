#include <algorithm>
#include <iostream>
#include <vector>

int main() {
  std::vector<int> vec = {1, 2, 3, 4, 3, 5, 6, 3};

  auto new_end = std::remove(vec.begin(), vec.end(), 3);

  std::cout << "After std::remove, before erase: ";
  for (int num : vec)
    std::cout << num << " "; // Output: 1 2 4 5 6 6 3 3
  std::cout << std::endl;

  vec.erase(new_end, vec.end());

  std::cout << "After erase: ";
  for (int num : vec)
    std::cout << num << " "; // Output: 1 2 4 5 6
  std::cout << std::endl;

  return 0;
}
