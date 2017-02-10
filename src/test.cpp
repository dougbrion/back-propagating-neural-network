#include "network.hpp"
#include "datainput.hpp"
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){

  // Init all the vectors needed for the network
  vector<int> shape;
  vector<double> input_vals;
  vector<double> target_vals;
  vector<double> result_vals;

  // Open the data file to train the network
  Data_input data(argv[1]);
  data.shape_get(shape);

  // Create the network itself
  Network network(shape);

  // Set log level between 0-3
  // Log level of 0 -> Nothing printed to console
  // Log level of 1 -> Pass Number and Avg Error
  // Log level of 2 -> Pass Number, Result, Target and Avg Error
  // Log level of 3 -> Pass Number, Layers, Result, Target and Avg Error
  network.set_debug_level(2);

// Loop until the whole file has been read
  while(!data.eof()){

    // Retrieve the input values
    //data.input_get(input_vals);
    data.input_target_get(input_vals, target_vals);

    // Feed the input values forward
    network.feed_forward(input_vals);

    // Receive the results from the network output
    network.receive_result(result_vals);

    // Retrieve the target values
    //data.target_get(target_vals);

    // Compare results to target and update network
    network.back_propagate(target_vals);

  }

  cout << "NETWORK TRAINED!" << endl;

  return 0;
}
