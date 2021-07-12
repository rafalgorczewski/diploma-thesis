#ifndef HQR_LIB_CLASSIFIER_HPP
#define HQR_LIB_CLASSIFIER_HPP

#include <vector>
#include <MiniDNN.h>
#include "bodypart.hpp"

namespace eig = Eigen;
namespace dnn = MiniDNN;

namespace th {
class Classifier
{
public:
  Classifier(int input_size);

  BodyPart operator()(const eig::VectorXd& input);

  void feed_data(BodyPart body_part, const eig::VectorXd& input);
  void train();

  void save_data();

private:
  int m_input_size;

  std::vector<eig::VectorXd> m_data;
  std::vector<BodyPart> m_labels;
  dnn::Network m_network;
  dnn::RMSProp m_opt;
};
}

#endif // HQR_LIB_CLASSIFIER_HPP
