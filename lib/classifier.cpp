#include "classifier.hpp"

namespace th {

Classifier::Classifier(int input_size) : m_input_size(input_size), m_opt(0.001) {
  dnn::Layer* first_layer = new dnn::FullyConnected<dnn::Sigmoid>(input_size, 1);
  dnn::Output* output_layer = new dnn::MultiClassEntropy();

  m_network.add_layer(first_layer);
  m_network.set_output(output_layer);

  m_network.init();
}

BodyPart Classifier::operator()(const eig::VectorXd& input)
{
  return BodyPart(m_network.predict(input)(0, 0));
}

void Classifier::feed_data(BodyPart body_part, const eig::VectorXd& input)
{
  m_data.push_back(input);
  m_labels.push_back(body_part);
}

void Classifier::train()
{
  eig::MatrixXd x(m_input_size, m_data.size());
  eig::MatrixXd y(1, m_labels.size());

  for (int col = 0; col < m_data.size(); ++col) {
    x.col(col) = m_data[col];
  }
  for (int label = 0; label < m_labels.size(); ++label) {
    y(1, label) = static_cast<double>(m_labels[label]);
  }

  m_network.fit(m_opt, x, y, 100, 10);
}

}
