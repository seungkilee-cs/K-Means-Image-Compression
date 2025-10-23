#include <gtest/gtest.h>

#include "image_preprocessor.hpp"

using namespace imgc::preprocess;

namespace {

cv::Mat make_test_image() {
    cv::Mat image(2, 2, CV_8UC3);
    image.at<cv::Vec3b>(0, 0) = {10, 20, 30};
    image.at<cv::Vec3b>(0, 1) = {40, 50, 60};
    image.at<cv::Vec3b>(1, 0) = {70, 80, 90};
    image.at<cv::Vec3b>(1, 1) = {100, 110, 120};
    return image;
}

}  // namespace

TEST(ImagePreprocessorTest, ToSamplesProducesExpectedShapeAndValues) {
    cv::Mat image = make_test_image();
    cv::Mat samples = to_samples(image);

    ASSERT_EQ(samples.rows, image.total());
    ASSERT_EQ(samples.cols, 3);
    ASSERT_EQ(samples.type(), CV_32F);

    for (int i = 0; i < samples.rows; ++i) {
        const cv::Vec3b& expected = image.at<cv::Vec3b>(i / image.cols, i % image.cols);
        EXPECT_FLOAT_EQ(samples.at<float>(i, 0), static_cast<float>(expected[0]));
        EXPECT_FLOAT_EQ(samples.at<float>(i, 1), static_cast<float>(expected[1]));
        EXPECT_FLOAT_EQ(samples.at<float>(i, 2), static_cast<float>(expected[2]));
    }
}

TEST(ImagePreprocessorTest, SamplesToImageReconstructsOriginal) {
    cv::Mat image = make_test_image();
    cv::Mat samples = to_samples(image);

    cv::Mat reconstructed = samples_to_image(samples, image.rows, image.cols);

    ASSERT_EQ(reconstructed.rows, image.rows);
    ASSERT_EQ(reconstructed.cols, image.cols);
    ASSERT_EQ(reconstructed.type(), CV_8UC3);

    for (int r = 0; r < image.rows; ++r) {
        for (int c = 0; c < image.cols; ++c) {
            cv::Vec3b expected = image.at<cv::Vec3b>(r, c);
            cv::Vec3b actual = reconstructed.at<cv::Vec3b>(r, c);
            EXPECT_EQ(actual, expected);
        }
    }
}

TEST(ImagePreprocessorTest, SamplesToImageRejectsMismatchedShape) {
    cv::Mat image = make_test_image();
    cv::Mat samples = to_samples(image);

    EXPECT_THROW(samples_to_image(samples, 1, 1), std::invalid_argument);
}
