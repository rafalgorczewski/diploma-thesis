#include "classifier.hpp"

#include <fstream>
#include <ctime>
#include <string>
#include <iostream>
#include <utility>
#include <cstddef>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <numeric>

namespace th {

int Classifier::operator()(const cv::Mat& input)
{
  const auto projected_input = m_lda.project(input);

  std::unordered_map<int, std::vector<double>> class_distances;
  for (int i = 0; i < m_projected.rows; ++i) {
    const double d = cv::norm(m_projected.row(i), projected_input);
    class_distances[m_labels.at<int>(i)].push_back(d);
  }

  int best_class{};
  double best_dist = std::numeric_limits<double>::max();
  for (auto& [cls, distances] : class_distances) {
    const double mean = std::accumulate(distances.cbegin(), distances.cend(), 0) / distances.size();

    if (mean < best_dist) {
      best_dist = mean;
      best_class = cls;
    }
  }

  return best_class;
}

void Classifier::feed_data(int body_part, const cv::Mat& input)
{
  m_data.push_back(input);
  m_labels.push_back(body_part);
}

void Classifier::train()
{
  m_lda.compute(m_data, m_labels.t());
  m_projected = m_lda.project(m_data);
}

cv::Mat Classifier::projectInput(const cv::Mat& input)
{
  return m_lda.project(input);
}

const cv::Mat& Classifier::getProjectedData() const
{
  return m_projected;
}

const cv::Mat& Classifier::getLabels() const
{
  return m_labels;
}

void Classifier::save_data(const std::string& config_name)
{
  const auto now = std::time(0);
  const auto dt = std::to_string(now);

  std::ofstream output("cal_" + config_name + "_" + dt + ".csv");
  for (int i = 0; i < m_labels.rows; ++i) {
    output << m_labels.at<int>(i);
    for (int j = 0; j < m_data.cols; ++j) {
      output << "," << m_data.at<double>(i, j);
    }
    output << std::endl;
  }
}

void Classifier::load_data(const std::string& file)
{
  std::ifstream input(file);
  std::string line;

  while (std::getline(input, line)) {
    std::string chunk;
    std::stringstream line_stream(line);

    std::getline(line_stream, chunk, ',');
    int label = std::stoi(chunk);

    cv::Mat features{};
    while (std::getline(line_stream, chunk, ',')) {
      features.push_back(std::stod(chunk));
    }

    feed_data(label, features.t());
  }
  train();
}

}
