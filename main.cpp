#include <iostream>
#include <array>
#include <bitset>
#include <random>
#include <cstdint>

// number of rounds.
// how many games, where 1 game means one first choice.
const uint64_t N = 10000000;

/**
 * Converts an int to a string with decimals after ever 3n position for readability.
 * 123456789 -> 123,456,789
 * 44 -> 44
 * 1234 -> 1,234
 *
 * @param n
 * @return
 */
std::string prettyNum(const uint64_t n)
{
  std::string num = std::to_string(n);
  std::vector<char> results{};
  results.resize(num.size() + num.size() / 3);

  uint8_t offset = 0;
  uint8_t comma = 0;
  uint8_t start = 0;
  uint8_t end = 0;
  for (long i = num.size() - 1; i >= 0; i--, start++, end++) {
    results.at(start + offset) = num.at(i);

    if (++comma == 3 && i - 1 >= 0) {
      comma = 0;
      offset++;
      end++;
      results.at(start + offset) = ',';
    }
  }

  // convert it back to a string
  std::string result;
  for (long i = end - 1; i >= 0; i--) {
    result += results.at(i);
  }

  return result;
}

uint8_t getRandom()
{
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_int_distribution<uint8_t> dis(1, 3);

  return dis(gen);
}


int main() {
  // every set bit represents which door the car is behind
  uint64_t doors = 0; // the last 9 bits: 100 010 001
  for (int i = 0; i < 3; i++) {
    doors |= 1ull << (i + i * 3);
  }

#ifdef DEBUG
  // shows every possible game outcome.
  // either the car is behind door 1, door 2 or door 3.
  std::cout << std::bitset<32>(doors) << std::endl;
#endif

  // Display progress in case of longer runs
  std::cout << "Progress meter for " << prettyNum(N) << " rounds\n";
  std::cout << "[ 0%            100% ]\n";
  std::cout << "[";

  // get the chances of winning
  uint64_t firstChoice = 0;
  uint8_t progress = 0;
  for (int i = 0; i < N; i++) {
    // select one of the three game outcomes
    const uint8_t outcome = i % 3;

    // select the first choice of the contestant
    const uint8_t choice = getRandom();

    // in the game show, they now open one of the two other doors, where we know the car isn't.
    // so here we first check if the first choice was correct.
    if ((doors & (1ull << (choice + outcome * 3))) > 0) {
      firstChoice += 1;
    }

    if (progress < (1.0 * i / N) * 100) {
      progress += 5;
      std::cout << "." << std::flush;
    }
  }
  std::cout << "]\n\n";

  // results
  std::cout << "His first choice was correct " << (1.0 * firstChoice / N) * 100 << "% of the time \n";    // 22.x%
  std::cout << "The switch was correct " << (1.0 * (N - firstChoice) / N) * 100 << "% of the time \n\n";  // 77.x%

  // exit
  return 0;
}
