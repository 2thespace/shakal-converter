#include "ImageDecoder.hpp"

#include <string>

#define cimg_display 0
#include "third-party/CImg.h"

namespace converter {
ImageConverter::ImageConverter(std::filesystem::path const& path_to_image) {
    if (!std::filesystem::exists(path_to_image)) {
        throw;
    }
    cimg_library::CImg<std::uint8_t> reader(path_to_image.string().c_str());
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

void ImageConverter::AddPadding(std::size_t size, PaddingType type) {
    // TO DO: swap algorithm with using PAddingType
}

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

void ImageConverter::resizeImage(std::size_t kDivided) {}

void ImageConverter::upsckaleImage(std::size_t kUpscale) {}

void ImageConverter::averageFiltration(std::size_t kernelSize) {
    Matrix<> averageKernel;
    for (auto i = 0; i != kernelSize; ++i) {
        Matrix<>::Row singleLine(kernelSize, 1);
        averageKernel.insert_row(averageKernel.rows(), singleLine);
    }
    for (auto j = 0; j != rgbImage.size(); ++j) {
        auto result = averagePixelResizer(rgbImage[j], averageKernel);
        rgbImage[j] = result;
    }
}

void ImageConverter::bilinearFiltration(std::size_t kernelSize) {}

void ImageConverter::ShakalImage(std::size_t shakal_depth) {
    ShakalImage(shakal_depth, [this](std::size_t shakal_depth) { averageFiltration(shakal_depth); });
}

}  // namespace converter