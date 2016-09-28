// Include the packages needed for this neural network project
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

// Use the standard namespace
using namespace std;

class neuron;
class network;

struct link {
  double weight;
  double deltaWeight;
};

// A layer in the neural network consists of an array of neurons which can be easily accessed using a vector
typedef vector<neuron> layer;

// Class for the neuron object
class neuron {
public:
  neuron(unsigned numberOutputs, int index);
  void setOutput(double val) { output = val; }
  double getOutput(void) const { return output; }
  void feedForward(const layer &precedingLayer);

private:
  double output;
  static double randomWeightGen(void) { return rand() / double(RAND_MAX); }
  vector<link> outputWeight;
  static double transferFunction(double x);
  static double transferFunctionDerivative(double x);
  int myIndex;
};

neuron::neuron(unsigned numberOutputs, int index) {
  for (int i = 0; i < numberOutputs; ++i) {
    outputWeight.push_back(link());
    outputWeight.back().weight = randomWeightGen();
  }

  myIndex = index;
}

void neuron::transferFunction(double x) {
  return tanh(x);
}

void neuron::transferFunctionDerivative(double x) {
  return 1.0 - (tanh(x) * tanh(x));
}

void neuron::feedForward(const layer &precedingLayer) {
  double sum = 0.0;

  // Sum the outputs from the preceding layer, these now are our inputs
  for (int i = 0; i < precedingLayer.size(); ++i) {
    sum += precedingLayer[i].getOutput() * precedingLayer[i].outputWeight[index].weight;
  }

  output = neuron::transferFunction(sum);
}

// Class for the network as a whole formed of layers which contain the neurons
class network {
// Public members are the topology of the network plus functions needed to input values, check against target and output result
public:
  network(const vector<unsigned> &topology);
  void feedForward(const vector<double> &inputs);
  void backPropagation(const vector<double> &targets);
  void receiveResults(vector<double> &results) const {}

// Private members consits of the array of layers in the network itself
private:
  vector<layer> layers;
};

// Fills the network up with the layers and in the layers the neurons for the net
network::network(const vector<unsigned> &topology){
  unsigned numberLayers = topology.size();
  for (int layerNumber = 0; layerNumber < numberLayers; ++layerNumber) {
    layers.push_back(layer());
    unsigned numberOutputs;
    if (layerNumber == topology.size() - 1) {
      numberOutputs = 0;
    } else {
      numberOutputs = topology[layerNumber + 1];
    }
    for (unsigned neuronNumber = 0; neuronNumber <= topology[layerNumber]; ++neuronNumber) {
      layers.back().push_back(neuron(numberOutputs, neuronNumber));
      cout << "Made a new Neuron in layer " << layerNumber << endl;
    }
  }
}

void network::feedForward(const vector<double> &inputs) {
  // Assign input vaues to the input neurons
  for (int i = 0; i < inputs.size(); ++i) {
    layers[0][i].setOutput(inputs[i]);
  }

  // Feed forward
  for (int layerNumber = 1; layerNumber < layers.size(); ++layerNumber) {
    layer &precedingLayer = layers[layerNumber - 1];
    for (int j = 0; j < layers[layerNumber].size() - 1; ++j) {
      layers[layerNumber][j].feedForward(precedingLayer);
    }
  }
}

void network::backPropagation(const vector<double> &targets) {

}

int main () {


  vector<unsigned> topology;
  topology.push_back(3);
  topology.push_back(2);
  topology.push_back(1);

  network myNetwork(topology);

  vector<double> inputs;
  vector<double> targets;
  vector<double> results;

  myNetwork.feedForward(inputs);
  myNetwork.backPropagation(targets);
  myNetwork.receiveResults(results);

  return 0;
}
