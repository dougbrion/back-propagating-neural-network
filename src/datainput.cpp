#include "datainput.hpp"

Data_input::Data_input(const std::string filename){
  m_input_file.open(filename);
  if (!m_input_file.is_open()){
    std::cout << "Error! Could not open the input file" << std::endl;
    exit(EXIT_FAILURE);
  } else {
    std::cout << "Success! File open..." << std::endl;
  }
}

void Data_input::shape_get(std::vector<int> &shape){
  std::string first_line, check;
  int n;
  getline(m_input_file, first_line);
  std::stringstream ss(first_line);
  ss >> check;
  if (check.compare("Shape:") != 0){
    std::cout << "Strings do not match, need to have 'shape:' at top of input file" << std::endl;
    return;
  }
  while(!ss.eof()){
    ss >> n;
    shape.push_back(n);
  }
  m_num_inputs = shape[0];
  m_num_targets = shape[shape.size() - 1];
  return;
}

//CIRCUIT SIM INPUT
void Data_input::input_get(std::vector<double> &input){
  input.clear();
  std::string line, check;
  double n;
  getline(m_input_file, line);
  std::stringstream ss(line);
  ss >> check;
  if (check.compare("Input:") == 0){
    for (int i = 0; i < m_num_inputs; ++i){
      ss >> n;
      input.push_back(n);
    }
  }
  return;
}


//CIRCUIT SIM INPUT
void Data_input::target_get(std::vector<double> &target){
  target.clear();
  std::string line, check;
  double n;
  getline(m_input_file, line);
  std::stringstream ss(line);
  ss >> check;
  if (check.compare("Target:") == 0){
    for (int i = 0; i < m_num_targets; ++i){
      ss >> n;
      target.push_back(n);
    }
  }
  return;
}
//
// void Data_input::input_target_get(std::vector<double> &input, std::vector<double> &target){
//   input.clear();
//   target.clear();
//   std::string line;
//   double n;
//   getline(m_input_file, line);
//   std::stringstream ss(line);
//   ss >> n;
//   target.push_back(n);
//   if (n == 0 || n == 1){
//     // Sorry for the cancer
//     if (n == 1){
//       std::cout << "ITS AN A!!!!!" << std::endl;
//     }
//     for (int i = 0; i < m_num_inputs; ++i){
//       ss >> n;
//       input.push_back(n);
//     }
//   }
//   return;
// }
//
// void Data_input::input_get(std::vector<double> &input){
//   input.clear();
//   std::string line;
//   double n;
//   getline(m_input_file, line);
//   std::cout << "INPUT" << line << std::endl;
//   std::stringstream ss(line);
//   ss >> n;
//   if (n == 0 || n == 1){
//     for (int i = 0; i < m_num_inputs; ++i){
//       ss >> n;
//       input.push_back(n);
//     }
//   }
//   return;
// }
//
//
// void Data_input::target_get(std::vector<double> &target){
//   target.clear();
//   std::string line;
//   double n;
//   getline(m_input_file, line);
//   std::cout << "OUTPUT" << line << std::endl;
//   std::stringstream ss(line);
//   ss >> n;
//   target.push_back(n);
//   return;
// }
//
bool Data_input::eof() {
  return m_input_file.eof();
}
