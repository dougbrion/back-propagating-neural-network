// Include the necessary header files
#include "network.hpp"
#include <iostream>
#include <vector>
#include <cmath>

// Create a network consisting of a single empty layer
Network::Network(){
  m_layer.push_back(Network_layer());
}

// Constructor to fill the network up with multiple layers;
// in these layers are multiple neurons, both of these are accessed
// using vectors. In nested for loop calls the Neuron constructor
// to set the number of outputs for each neuron and its index.
Network::Network(const std::vector<int> &shape){
  unsigned num_layers = shape.size();
  m_pass_number = 0;
  for (unsigned current_layer = 0; current_layer < num_layers; ++current_layer){
    // Push back an empty layer into the network
    m_layer.push_back(Network_layer());
    unsigned num_outputs;
    if (current_layer == shape.size() - 1){
      num_outputs = 0;
    } else {
      num_outputs = shape[current_layer + 1];
    }
    for (int current_neuron = 0; current_neuron <= shape[current_layer]; ++current_neuron){
      // Push back a neuron with index and number of outputs in to the layer
      m_layer.back().push_back(Neuron(num_outputs, current_neuron));
    }
  }
}

// Function in class network for inputting values first into the first layer
// of the neural network, and then pushing these values through the network
// by calling the respective feed_forward() function in class neuron.
void Network::feed_forward(const std::vector<double> &input){
  // Assign the values of the inputs to the neurons in the first layer
  for (unsigned i = 0; i < input.size(); ++i){
    m_layer[0][i].output_set(input[i]);
  }
  ++m_pass_number;
  // Assign all the bias neurons to be 1.0
  for (unsigned i = 0; i < m_layer.size(); ++i){
    m_layer[i][m_layer[i].size() - 1].output_set(1.0);
  }

  // Feeding the values inputted forward to the following layer
  for (unsigned current_layer = 1; current_layer < m_layer.size(); ++current_layer){
    // Copy from the current layer up by using the address
    Network_layer &previous_layer = m_layer[current_layer - 1];
    for (unsigned j = 0; j < m_layer[current_layer].size() - 1; ++j){
      m_layer[current_layer][j].feed_forward(previous_layer);
    }
  }
}

// Function in class network to update the weights in the network to minimise
// the cost function/error in the network. This is done by calculating the gradient
// of this cost function with respect to all the different weights in the network.
// Input to this function the desired output of the network for the network to compare
// against in order to calculate the gradients.
void Network::back_propagate(const std::vector<double> &target){
  // Copy the final layer into new Network_layer output_layer by using the address
  Network_layer &output_layer = m_layer.back();
  double delta_error = 0;
  m_error = 0;

  // Calculate the error between the target and the current output
  for (unsigned i = 0; i < output_layer.size(); ++i){
    delta_error = target[i] - output_layer[i].output_get();
    m_error += delta_error * delta_error;
  }

  // Normalise the error with the number of output neurons
  m_error /= output_layer.size() - 1;
  m_error = sqrt(m_error);

  m_avg_error = (m_avg_error * m_avg_smooth + m_error) / (m_avg_smooth + 1);

  // Calculate the values of the output layer gradients
  for (unsigned i = 0; i < output_layer.size() - 1; ++i) {
    output_layer[i].calc_output_gradients(target[i]);
  }

  // Hidden layers gradients
  for (unsigned current_layer = m_layer.size() - 2; current_layer > 0; --current_layer) {
    Network_layer &hidden_layer = m_layer[current_layer];
    Network_layer &next_layer = m_layer[current_layer + 1];

    for (unsigned i = 0; i < hidden_layer.size(); ++i) {
      hidden_layer[i].calc_hidden_gradients(next_layer);
    }
  }

  // Updating the weights of the network
  for (unsigned current_layer = m_layer.size() - 1; current_layer > 0; --current_layer){
    Network_layer &reference  = m_layer[current_layer];
    Network_layer &previous_layer = m_layer[current_layer - 1];

    for (unsigned i = 0; i < reference.size() - 1; ++i){
      reference[i].update_weights(previous_layer);
    }
  }
  log_level_out(target);
}

// Retrieves the result as a vector from the neurons
void Network::receive_result(std::vector<double> &result) const {
  // Clear the current values in result
  result.clear();
  // Iterate through the neurons getting their output and storing in result
  for (unsigned i = 0; i < m_layer.back().size() - 1; ++i){
    result.push_back(m_layer.back()[i].output_get());
  }
}

// Function to print out to teh console the values of all of the neurons in
// the network layer by layer. This is useful to see what exactly is happening
// in the hidden layers of the network.
void Network::print_neuron_vals() const {
  for (unsigned i = 0; i < m_layer.size(); ++i){
    std::cout << "Layer " << i << " : ";
    for (unsigned j = 0; j < m_layer[i].size(); ++j){
      std::cout << " " << m_layer[i][j].output_get() << " ";
    }
    std::cout << std::endl;
  }
}

// Returns the avg error in class network
double Network::avg_error_get() const {
  return m_avg_error;
}

void Network::set_debug_level(int level){
  if (level >= 0 && level <= 5){
    m_log_level = level;
  } else {
    std::cout << "Error! Please set a valid log level (0-5)" << std::endl;
  }
}

int Network::log_level_get() const {
  return m_log_level;
}

void Network::log_level_out(const std::vector<double> &target){
    if (m_log_level == 1){
      std::cout << "Pass Number: " << m_pass_number << std::endl;
      std::cout << "Average error: " << m_avg_error << std::endl << std::endl;
    } else if (m_log_level == 2){
      std::cout << "Pass Number: " << m_pass_number << std::endl;
      std::cout << "Target: " << target[0] << std::endl;
      std::cout << "Result: " << m_layer.back()[0].output_get() << std::endl;
      std::cout << "Average error: " << m_avg_error << std::endl << std::endl;
    } else if (m_log_level == 3){
      std::cout << "Pass Number: " << m_pass_number << std::endl;
      print_neuron_vals();
      std::cout << "Target: " << target[0] << std::endl;
      std::cout << "Result: " << m_layer.back()[0].output_get() << std::endl;
      std::cout << "Average error: " << m_avg_error << std::endl << std::endl;
    }
}
