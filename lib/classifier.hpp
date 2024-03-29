#ifndef HQR_LIB_CLASSIFIER_HPP
#define HQR_LIB_CLASSIFIER_HPP

#include <vector>
#include <opencv2/opencv.hpp>
#include <string>

#include "bodypart.hpp"

namespace th {
class Classifier
{
public:
  int operator()(const cv::Mat& input);

  void feed_data(int body_part, const cv::Mat& input);
  void train();

  cv::Mat projectInput(const cv::Mat& input);
  const cv::Mat& getProjectedData() const;
  const cv::Mat& getLabels() const;

  void save_data(const std::string& config_name);
  void load_data(const std::string& file);

private:
  cv::LDA m_lda{2};
  cv::Mat m_projected{};

  cv::Mat m_data{};
  cv::Mat m_labels{};
};
}

#endif // HQR_LIB_CLASSIFIER_HPP
