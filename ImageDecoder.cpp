#include "ImageDecoder.hpp"

#include <filesystem>
#include <string>

#define cimg_display 0
#include "third-party/CImg.h"

namespace converter {
ImageConverter::ImageConverter(std::string_view path_to_image) {
    cimg_library::CImg<std::uint8_t> reader(std::string{path_to_image}.c_str());
    auto channels    = reader.spectrum();
    auto width       = reader.width();
    auto height      = reader.height();
    auto channel_it  = std::min(channels, 3);
    auto image_begin = reader.data();

    for (auto i = 0; i != channel_it; ++i) {
        for (auto j = 0; j != height; ++j) {
            auto rows               = rgbImage[i].rows();
            auto currentRowPosition = i * width * height;
            Matrix<>::Row tmp;
            tmp.resize(width);
            std::copy(&reader(0, j, i), &reader(0, j, i) + width, tmp.begin());
            rgbImage[i].insert_row(rows, tmp);
        }
    }
}

void ImageConverter::AddPadding(std::size_t size, PaddingType type) {}
void ImageConverter::SaveImage(std::string_view path_to_image) {
    std::vector<type> result;
    for (auto i = 0; i != 3; ++i) {
        for (auto j = 0; j != rgbImage[i].rows(); ++j) {
            auto& row = rgbImage[i].getRow(j);
            result.insert(result.end(), row.begin(), row.end());
        }
    }
    auto channels = 3;
    auto height   = rgbImage[0].rows();
    auto width    = rgbImage[0].collums();

    cimg_library::CImg<std::uint8_t> tmp(result.data(), width, height, 1, channels);
    tmp.save_png(std::string{path_to_image}.c_str());
}
void ImageConverter::ShakalImage(std::size_t shakal_depth) {
    resizeImage(shakal_depth);
    upsckaleImage(shakal_depth);
}
void ImageConverter::resizeImage(std::size_t kDivided) {
    averageFiltration(kDivided);
}

void ImageConverter::upsckaleImage(std::size_t kUpscale) {}

void ImageConverter::averageFiltration(std::size_t skipPixel, std::size_t kernelSize) {
    auto averageFiltration = [skipPixel, kernelSize](Matrix<> const& input) -> Matrix<> {
        auto raw = input.mutable_data();
        Matrix<> new_matrix;
        for (auto i = skipPixel; i != input.rows();) {
            // auto newPixel = input.Submatrix() * BlurMatrix();
        }

        return new_matrix;
    }
}

void ImageConverter::bilinearFiltration(std::size_t skipPixel, std::size_t kernelSize) {}

}  // namespace converter