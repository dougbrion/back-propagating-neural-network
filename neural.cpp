// Include the packages needed for this neural network project
#include <iostream>
#include <vector>

// Use the standard namespace
using namespace std;

// Class for the neuron object
class neuron {
public:

private:

};

// A layer in the neural network consists of an array of neurons which can be easily accessed using a vector
typedef vector<neuron> layer;

// Class for the network as a whole formed of layers which contain the neurons
class network {
// Public members are the topology of the network plus functions needed to input values, check against target and output result
public:
  network(const vector<unsigned> &topology);
  void feedForward(const vector<double> &inputs);
  void backPropagation(const vector<double> &targets);
  void receiveResults(vector<double> &results) const;

// Private members consits of the array of layers in the network itself
private:
  vector<layer> layers;
};

// Fills the network up with the layers and in the layers the neurons for the net
network::network(const vector<unsigned> &topology){
  unsigned numberLayers = topology.size();
  for (unsigned layerNumber = 0; layerNumber < numberLayers; ++layerNumber) {
    layers.push_back(layer());

    for (unsigned neuronNumber = 0; neuronNumber <= topology[layerNumber]; ++neuronNumber) {
      layers.back().push_back(neuron());
    }
  }
}

int main () {

  
  vector<unsigned> topology;
  network myNetwork(topology);

  vector<double> inputs;
  vector<double> targets;
  vector<double> results;

  myNetwork.feedForward(inputs);
  myNetwork.backPropagation(targets);
  myNetwork.receiveResults(results);

  return 0;
}
