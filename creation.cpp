// Include the necessary header files
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>

// Main with command line arguments
// Run ./creation data.txt
int main(int argc, char *argv[]) {
  std::ofstream training_data;
  training_data.open(argv[1]);
  std::vector<int> topos;
  int n, hidden_layers;
  unsigned trials;
  std::cout << "Please input the shape of the network you would like to create..." << std::endl;
  std::cout << "How many neurons in the input layer?" << std::endl;
  std::cin >> n;
  topos.push_back(n);
  std::cout << std::endl << "How many hidden layers would you like?" << std::endl;
  std::cin >> hidden_layers;
  for (unsigned i = 1; i <= hidden_layers; ++i){
    std::cout << std::endl << "Number of neurons in hidden layer " << i << "?" << std::endl;
    std::cin >> n;
    topos.push_back(n);
  }
  std::cout << std::endl << "How many neurons in the output layer?" << std::endl;
  std::cin >> n;
  topos.push_back(n);
  training_data << "Shape:";
  for (unsigned i = 0; i < topos.size(); ++i){
    training_data << " " << topos[i];
  }
  training_data << std::endl;

  std::cout << std::endl << "How many trials would you like to run?" << std::endl;
  std::cin >> trials;

  int output;
  int *inputs;
  inputs = new int[topos[0]];
  std::cout << std::endl << "Generating..." << std::endl;

  for (int i = trials - 1; i > 0; --i) {
    training_data << "Input:";
    for (int j = 0; j < topos[0]; ++j){
      inputs[j] = int(((2.0 * rand()) / double(RAND_MAX)));
      training_data << " " << inputs[j] << ".0";
    }
    training_data << std::endl;
    output = inputs[0];
    for (int k = 1; k < topos[0]; ++k){
      output = output ^ inputs[k];
    }
    training_data << "Target: " << output << ".0 " << std::endl;
  }

  delete[] inputs;
  training_data.close();
  std::cout << std::endl << "Success! Data created! Program terminated." << std::endl;
}
