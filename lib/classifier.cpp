#include "classifier.hpp"

#include <fstream>
#include <ctime>
#include <string>
#include <iostream>
#include <utility>
#include <cstddef>
#include <limits>

namespace th {

Classifier::Classifier(int feature_vector_size) : m_feature_vector_size(feature_vector_size)
{

}

int Classifier::operator()(const cv::Mat& input)
{
  const auto projected_input = m_lda.project(input);
  std::clog << "Projected input rows: " << projected_input.rows << ", Projected input cols: " << projected_input.cols << std::endl;

  int best_class = 0;
  double best_dist = std::numeric_limits<double>::max();
  for (int i = 0; i < m_projected.rows; ++i) {
    const double d = cv::norm(m_projected.row(i), projected_input);
    if (d < best_dist) {
      best_dist = d;
      best_class = i;
    }
  }

  return m_labels.at<int>(best_class);
}

void Classifier::feed_data(int body_part, const cv::Mat& input)
{
  std::cout << "INPUT: ";
  for (int i = 0; i < input.cols; ++i) {
    std::cout << input.at<double>(0, i) << ", ";
  }
  std::cout << std::endl;

  m_data.push_back(input);
  std::cout << "M_DATA: ";
  for (int i = 0; i < m_data.cols; ++i) {
    std::cout << m_data.at<double>(m_data.rows - 1, i) << ", ";
  }
  std::cout << std::endl;

  m_labels.push_back(body_part);

  std::clog << "Data rows: " << m_data.rows << ", Data cols: " << m_data.cols << std::endl;
  std::clog << "Labels rows: " << m_labels.rows << ", Labels cols: " << m_labels.cols << std::endl;
}

void Classifier::train()
{
  m_lda.compute(m_data, m_labels.t());
  m_projected = m_lda.project(m_data);
  std::clog << "Projected rows: " << m_projected.rows << ", Projected cols: " << m_projected.cols << std::endl;
}

void Classifier::save_data()
{
  const auto now = std::time(0);
  const auto dt = std::to_string(now);

  std::ofstream output("cal_" + dt + ".csv");
  for (int i = 0; i < m_labels.rows; ++i) {
    output << m_labels.at<int>(i);
    for (int j = 0; j < m_data.cols; ++j) {
      output << "," << m_data.at<double>(i, j);
    }
    output << std::endl;
  }
}

}
