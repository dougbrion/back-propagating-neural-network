#ifndef data_input_hpp_
#define data_input_hpp_

// Include the necessary headers
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "network.hpp"
#include "neuron.hpp"

// Class for the Neuron
class Data_input{
public:
  Data_input(const std::string filename);
  void shape_get(std::vector<int> &shape);
  void input_get(std::vector<double> &input);
  void target_get(std::vector<double> &target);
  void input_target_get(std::vector<double> &input, std::vector<double> &target);
  bool eof();
private:
  std::ifstream m_input_file;
  int m_num_inputs;
  int m_num_targets;
};

#endif
