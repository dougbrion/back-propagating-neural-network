#ifndef network_hpp_
#define network_hpp_

// Include the necessary headers
#include "neuron.hpp"
#include <vector>

// Class for the Network
class Network{
public:
  Network();
  Network(const std::vector<int> &shape);
  void feed_forward(const std::vector<double> &input);
  void back_propagate(const std::vector<double> &target);
  void receive_result(std::vector<double> &result) const;
  void print_neuron_vals() const;
  double avg_error_get() const;
  void set_debug_level(int level);
  void log_level_out(const std::vector<double> &target);
  int log_level_get() const;

private:
  std::vector<Network_layer> m_layer;
  double m_error;
  double m_avg_error;
  double m_avg_smooth;
  int m_log_level;
  int m_pass_number;
};

#endif
