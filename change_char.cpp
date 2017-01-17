#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

int main() {
  ifstream infile("letter-recognition.txt");
  ofstream outfile("letter.txt");
  char c;
  int comma = 0;
  string line;
  while(!infile.eof()){
    getline(infile, line);
    cout << line << endl;
    replace(line.begin(), line.end(), ',', ' ');
    cout << line << endl;
    outfile << line << endl;
  }
  infile.close();
  outfile.close();
  return 0;
}
