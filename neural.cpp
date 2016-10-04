// Include the packages needed for this neural network project
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <cassert>
#include <sstream>

// Use the standard namespace
using namespace std;

class neuron;
class network;

// Structure for the arcs linking the different nodes
struct link {
  double weight;
  double deltaWeight;
};

// A layer in the neural network consists of an array of neurons which can be easily accessed using a vector
typedef vector<neuron> layer;

// Class for inputing and the training data and training the artificial neural network
class training {
public:
  training(const string filename);
  bool isEof(void) { return trainingDataFile.eof(); }
  void getTopology(vector<unsigned> &topology);

  int nextInputs(vector<double> &input);
  int getTargetOutput(vector<double> &targetOut);

private:
  ifstream trainingDataFile;
}

void training::getTopology(vector<unsigned> &topology) {
  string line;
}

// Class for the neuron object
class neuron {
public:
  neuron(unsigned numberOutputs, int index);
  void setOutput(double val) { output = val; }
  double getOutput(void) const { return output; }
  void feedForward(const layer &precedingLayer);
  void calcOutputGrads(double target);
  void calcHiddenGrads(const layer &nextLayer);
  void updateInputWeights(layer &precedingLayer);

private:
  static double eta; // in the range 0 - 1
  static double alpha; // in the range 0 -1
  static double randomWeightGen(void) { return rand() / double(RAND_MAX); }
  static double transferFunction(double x);
  static double transferFunctionDerivative(double x);
  vector<link> outputWeight;
  double output;
  double dowSum(const layer &nextLayer) const;
  int myIndex;
  double gradient;
};

neuron::neuron(unsigned numberOutputs, int index) {
  for (int i = 0; i < numberOutputs; ++i) {
    outputWeight.push_back(link());
    outputWeight.back().weight = randomWeightGen();
  }

  myIndex = index;
}

double neuron::eta = 0.15; // initialise eta within the given range 0 - 1
double neuron::alpha = 0.5; // initiallise alpha within the given range 0 - 1

// Function within the object neuron for updating the weights of the different arcs between layers
void neuron::updateInputWeights(layer &precedingLayer) {
  for (int i = 0; i < precedingLayer.size(); ++i) {
    neuron &refNeuron = precedingLayer[i];
    double deltaWeightOld = refNeuron.outputWeight[myIndex].deltaWeight;

    double deltaWeightNew = eta * refNeuron.getOutput() * gradient + alpha * deltaWeightOld;

    refNeuron.outputWeight[myIndex].deltaWeight = deltaWeightNew;
    refNeuron.outputWeight[myIndex].weight += deltaWeightNew;
  }
}

// Function in neuron for calculating the sum of the weights and gradients between the layers
double neuron::dowSum(const layer &nextLayer) const {
  double sum = 0.0;

  for (int i = 0; i < nextLayer.size() - 1; ++i) {
    sum += outputWeight[i].weight * nextLayer[i].gradient;
  }

  return sum;
}

// Calculate the gradient of the outputs using the transfer function derivative
void neuron::calcOutputGrads(double target) {
  double delta = target - output;
  gradient = delta * neuron::transferFunctionDerivative(output);
}

// Calculate the gradient of the hidden arcs between the layers using the transfer function derivative
void neuron::calcHiddenGrads(const layer &nextLayer) {
  double dow = dowSum(nextLayer);
  gradient = dow * neuron::transferFunctionDerivative(output);
}

// The Transfer function
double neuron::transferFunction(double x) {
  return tanh(x);
}


double neuron::transferFunctionDerivative(double x) {
  return 1.0 - (tanh(x) * tanh(x));
}

void neuron::feedForward(const layer &precedingLayer) {
  double sum = 0.0;

  // Sum the outputs from the preceding layer, these now are our inputs
  for (int i = 0; i < precedingLayer.size(); ++i) {
    sum += precedingLayer[i].getOutput() * precedingLayer[i].outputWeight[myIndex].weight;
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
  void receiveResults(vector<double> &results) const;
  double getLatestAvgError(void) const { return averageError; }

// Private members consits of the array of layers in the network itself
private:
  vector<layer> layers;
  double error;
  double averageError;
  double averageSmoothFactor;
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

void network::receiveResults(vector<double> &results) const {
  results.clear();

  for (int i = 0; i < layers.back().size() - 1; ++i) {
    results.push_back(layers.back()[i].getOutput());
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
  // Calculate net error
  layer &outputLayer = layers.back();
  error = 0.0;

  for (int i = 0; i < outputLayer.size() - 1; ++i) {
    double delta = targets[i] - outputLayer[i].getOutput();
    error += delta * delta;
  }

  error /= outputLayer.size() - 1;
  error = sqrt(error);

  averageError = (averageError * averageSmoothFactor + error) / (averageSmoothFactor + 1.0);

  // Output layer gradient
  for (int i = 0; i < outputLayer.size() - 1; ++i) {
    outputLayer[i].calcOutputGrads(targets[i]);
  }

  // Hidden layers gradients
  for (int layerNumber = layers.size() - 2; layerNumber > 0; --layerNumber) {
    layer &hiddenLayer = layers[layerNumber];
    layer &nextLayer = layers[layerNumber + 1];

    for (int i = 0; i < hiddenLayer.size(); ++i) {
      hiddenLayer[i].calcHiddenGrads(nextLayer);
    }
  }

  for (int layerNumber = layers.size() - 1; layerNumber > 0; --layerNumber) {
    layer &refLayer = layers[layerNumber];
    layer &precedingLayer = layers[layerNumber - 1];

    for (int i = 0; i < refLayer.size() - 1; ++i) {
      refLayer[i].updateInputWeights(precedingLayer);
    }
  }
}

void showVals(string label, vector<double> &vec) {
  cout << label << " ";
  for (int i = 0; i < vec.size(); ++i) {
    cout << vec[i] << " ";
  }

  cout << endl;
}

// Main running of the network - creating the topology and inputing training data
int main () {
  training trainingData("*/tmp/training.txt");

  vector<unsigned> topology;

  trainingData.getTopology(topology);
  network myNetwork(topology);

  vector<double> input, target, result;
  int trainingPass = 0;

  while (!trainingData.isEof()) {
    ++trainingPass;
    cout << endl << "Pass " << trainingPass;

    if (trainingData.nextInputs(input) != topology[0]) {
      break;
    }
    showVals(": Inputs:", input);
    myNetwork.feedForward(input);

    myNetwork.receiveResults(result);
    showVals(": Outputs: ", result);

    trainingData.getTargetOutput(target);
    showVals(" Targets: ", target);

    myNetwork.backPropagation(target);

    cout << "Network latest average error: " << myNetwork.getLatestAvgError() << endl;

  }

  cout << "DONE!" << endl;

  return 0;
}
