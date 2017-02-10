#ifndef neuron_hpp_
#define neuron_hpp_

// Include the necessary headers
#include <vector>

// Create class Neuron for the typedef
class Neuron;

// The network consists of an input layer and output layer with
// multiple hidden layers. Each layer is formed of neurons and
// the layers together make up the whole network

// Define a type Network_layer which is a vector of neurons
typedef std::vector<Neuron> Network_layer;

// Struct for linking the the different neurons together
// containing both the weight of the arc and the delta weight
struct arc{
  double weight;
  double d_weight;
};

// Class for the Neuron
class Neuron{
public:
  Neuron();
  Neuron(unsigned num_outputs, int index);
  void output_set(double value);
  double output_get() const;
  void feed_forward(const Network_layer &previous_layer);
  void calc_output_gradients(double x);
  void calc_hidden_gradients(const Network_layer &following_layer);
  double sum_of_weights(const Network_layer &following_layer);
  void update_weights(Network_layer &previous_layer);
private:
  int m_index;
  double m_output;
  double m_gradient;
  std::vector<arc> m_weight_out;
  double rand_weight();
  double transfer_fn(double x);
  double transfer_d_fn(double x);
  static double sigma;
  static double tau;
};

#endif
