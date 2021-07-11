#include <iostream>
#include <string>
#include <chrono>

using namespace std;
string password = "password123";
string warmPrefix = "gqwegq";
string allChars = "abcdefghijklmnopqrstuvwxyz0123456789";
string possibleChars = warmPrefix + allChars;
string randomChars = "wqegiwqjgeoiwqegjow";

bool login(const std::string& input) {
  if (input.size() != password.size()) {
    // std::cout << "size mismatch:: input = " << input.size() << "password = " << password.size() << std::endl;
    return false;
  }
  // std::cout << "input size is " << input.size () << "\t password size is " << password.size() << endl;
  for (int i = 0; i < input.size(); i++) {
      if (password[i] != input[i]) {
        return false;
      }
    }
  return true;
}


int getPasswordSize() {
  int mostTime = -1;
  int mostTimeTakingSize = 0;
  for (int testLength = 1; testLength < 30; testLength++) {
    int time = 0;
    for (int j = 0; j < allChars.size(); j++) {
      string input = allChars[j] + randomChars.substr(0, testLength - 1);
      auto start = std::chrono::high_resolution_clock::now();
      for (int k = 0; k < 100; k++)
        login(input);
      auto end = std::chrono::high_resolution_clock::now();
      time += std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    }

    std::cout << "testLength\t" << testLength << "\ttime\t" << time <<endl;

    if (time > mostTime) {
      mostTime = time;
      mostTimeTakingSize = testLength;
    }
  }
  
  return mostTimeTakingSize;
}


int getNextChar(std::string prefix, std::string suffix) {
  int timesTaken[possibleChars.size()];
  int mostTime = -1;
  char mostTimeChar;
  int mostTimeCharIndex;
  for (int i = 0; i < possibleChars.size(); i++) {
        char candidate = possibleChars[i];
        auto input = prefix + candidate + suffix;
        auto start = std::chrono::high_resolution_clock::now();
        // std::cout << "input::\t" << input << endl;
        for (int j = 0; j < 1000; j++) {
          login(input);
        }
        auto finish = std::chrono::high_resolution_clock::now();
        int time = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count();
        timesTaken[i] = time;
        if (i < warmPrefix.size())
          continue;
        // std::cout << "time taken for" << candidate << "\t" << time << endl;
        if (time > mostTime) {
          mostTimeChar = candidate;
          mostTimeCharIndex = i - warmPrefix.size();
          mostTime = time;
        }
    }
  
  return mostTimeCharIndex;
}

std::string hackPassword() {
  int passwordSize = getPasswordSize();
   string password = "";
   for (int i = 0; i < passwordSize; i++) {
     int frequencies[36] = {};
     for (int j = 0; j < 100; j++) {
       int index = getNextChar(password, randomChars.substr(0, passwordSize - i  - 1));
       frequencies[index]++;
     }
     int highestFreq = -1;
     char mostFrequentChar;
     for (int k = 0; k < 36; k++) {
       if (frequencies[k] > highestFreq) {
         highestFreq = frequencies[k];
         mostFrequentChar = allChars[k];
       }
     }
     password += mostFrequentChar;
     std::cout << i << "\t" << password << std::endl;
   }
   return password;
};

int main() {
  hackPassword();
  // std::cout << "password length is\n" << getPasswordSize() << std::endl;
}
