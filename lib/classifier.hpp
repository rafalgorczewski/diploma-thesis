#ifndef HQR_LIB_CLASSIFIER_HPP
#define HQR_LIB_CLASSIFIER_HPP

#include <vector>
#include <opencv2/opencv.hpp>

#include "bodypart.hpp"

namespace th {
class Classifier
{
public:
  Classifier(int feature_vector_size);

  int operator()(const cv::Mat& input);

  void feed_data(int body_part, const cv::Mat& input);
  void train();

  void save_data();

private:
  static constexpr int CLASSES_COUNT = 3;

  int m_feature_vector_size = 0;

  cv::LDA m_lda;
  cv::Mat m_projected;

  cv::Mat m_data;
  cv::Mat m_labels;
};
}

#endif // HQR_LIB_CLASSIFIER_HPP
