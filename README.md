# neural-network
Testing neural network in C++

## What is a Neural network
A Neural Network (ANN) is a paradigm inspired by the way biological systems process information, such as animal brains, working by using loads of interconnected neurons forming a giant network. The key element of this paradigm, like the brain it is imitating, is the novel structure of the information processing system - composed of many interconnected 'neurons' working in unison to solve specific problems. Computer based (Artificial) Neural Networks, like the brain, learn by example. A NN is specialised for a specific application, such as image processing or data classification. The brain learns by adjusting synaptic connexions between the neurons, this is also how artificial neural networks work.

## About this ANN
This ANN is implemented in C++ using just standard libraries and no NN libraries. The two main structures of this program consist of the class *neuron* and the class *network*. In these objects we have the necessary variables and functions in order to form most of our network.

## My Tests

So far I have used this back prop neural net to identify solutions to digital circuits, as the network will learn what an *and gate* and *or gate* etc will do, plus also for letter recognition of multiple fonts. In future I plan to use it for more applications and you should be able to do so as well with some minor tweaking.

### Download and Setup

Download the files with

```bash
git clone git@github.com:dougbrion/back-propagating-neural-network.git
```
Enter the directory and make using

```bash
make bin/test
```

Also if you want make other binarys for changing data format and also creating the data.

```bash
make bin/change_char
make bin/creation
```
## Use

Network is currently setup for basic training using ones and zeros. We can run the program using the following.

```bash
bin/test data.txt
```

Notice the training data is input as an argument.
