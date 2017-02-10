// Include the necessary header files
#include <cmath>
#include <cstdlib>
#include <iostream>
#include "neuron.hpp"
#include <ctime>

double Neuron::sigma = 0.01;
double Neuron::tau = 0.9;

// Constructor for creating an empty neuron currently only prints to console
Neuron::Neuron(){
  std::cout << "Created an unconnected empty neuron" << std::endl;
}

// Neuron constructor, input the number of outputs the neuron should have and
// the index of the neuron. Calculates and random weight and assigns it to the
// weight of the arc structure. It also pushes back the same number of these arcs
// to the number of outputs the neuron has.
Neuron::Neuron(unsigned num_outputs, int index){
  for (unsigned i = 0; i < num_outputs; ++i){
    m_weight_out.push_back(arc());
    m_weight_out.back().weight = rand_weight();
  }
  m_index = index;
}

// Set the value of the output of the neuron
void Neuron::output_set(double value){
  m_output = value;
}

// Retrieve the value of the output of the neuron
double Neuron::output_get() const{
  return m_output;
}

// Function for returning a random double using cstdlib
double Neuron::rand_weight(){
   return (rand() / double(RAND_MAX));
}

// The feed forward function for neuron called by feed forward in class network.
// Calculate the sum to feed forward in the network by multiplying the weight of
// the arc and the output of the neuron
void Neuron::feed_forward(const Network_layer &previous_layer){
  double sum = 0;
  // Iterate through all the neurons in the previous layer
  // The value sum is the output of the neuron multiplied by the weight
  // of the arc at that indexed neuron.
  for (unsigned i = 0; i < previous_layer.size(); ++i){
    sum += previous_layer[i].output_get() * previous_layer[i].m_weight_out[m_index].weight;
  }
  m_output = transfer_fn(sum);
}

// Called during back propogation and calculates the gradient of the cost function
// between the target val of the network and the current output
void Neuron::calc_output_gradients(double x){
  double delta = x - m_output;
  m_gradient = delta * transfer_d_fn(m_output);
}

// Called during back propogation and calculates hte gradient of the cost function
// between the outputs of the neurons in the hidden layers of the network
void Neuron::calc_hidden_gradients(const Network_layer &following_layer){
  double weight_sum = sum_of_weights(following_layer);
  m_gradient = weight_sum * transfer_d_fn(m_output);
}

// Function to return the sum of the weight of the arcs and the gradients of the cost functions
// between the neurons is different layers.
double Neuron::sum_of_weights(const Network_layer &following_layer){
  double sum = 0;

  for (unsigned i = 0; i < following_layer.size() - 1; ++ i){
    sum += m_weight_out[i].weight * following_layer[i].m_gradient;
  }

  return sum;
}

// Function to update the weights in the network, this is called at the end of back
// propogation. Using values definied for sigma and tau and the cost function gradients
// calcucates a new delta weight. The arc is updated with this new delta weight and this
// delta weight is added to the arcs current weight.
void Neuron::update_weights(Network_layer &previous_layer){
  for (unsigned i = 0; i < previous_layer.size(); ++i){
    Neuron &reference = previous_layer[i];
    double old_d_weight = reference.m_weight_out[m_index].d_weight;
    double new_d_weight = (sigma * reference.output_get() * m_gradient) + (tau * old_d_weight);

    reference.m_weight_out[m_index].weight += new_d_weight;
    reference.m_weight_out[m_index].d_weight = new_d_weight;
  }
}

// Transfer function is tanh(x)
double Neuron::transfer_fn(double x){
  return tanh(x);
}

double Neuron::transfer_d_fn(double x){
  // Derivative of tanh(x) is sech^2(x) we can
  // use the identity sech^2(x) = 1 - tanh^2(x)
  return (1.0 - (tanh(x) * tanh(x)));
}
