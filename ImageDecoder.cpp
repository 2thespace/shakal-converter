#include "ImageDecoder.hpp"

#include <fstream>
#include <string>

#define cimg_display 0
#define cimg_use_png
#define cimg_use_jpeg
#define cimg_use_zlib
#include "third-party/CImg.h"

namespace converter {
ImageConverter::ImageConverter(std::filesystem::path const& path_to_image) {
    if (!std::filesystem::exists(path_to_image)) {
        throw;
    }
    cimg_library::CImg<std::uint8_t> reader(path_to_image.string().c_str());
    channels         = reader.spectrum();
    auto width       = reader.width();
    auto height      = reader.height();
    auto channel_it  = std::min(channels, rgbImage.size());
    auto image_begin = reader.data();

    for (auto i = 0; i != channel_it; ++i) {
        for (auto j = 0; j != height; ++j) {
            auto rows = rgbImage[i].rows();
            Matrix<>::Row tmp;
            tmp.resize(width);
            std::copy(&reader(0, j, i), &reader(0, j, i) + width, tmp.begin());
            rgbImage[i].insert_row(rows, tmp);
        }
    }
}

ImageConverter::ImageConverter(std::span<std::uint8_t> binaryImage, std::uint32_t width, std::uint32_t height,
                               std::uint8_t collors) {
    std::string temp_filename = std::tmpnam(nullptr);
    std::ofstream temp_file(temp_filename, std::ios::binary);
    temp_file.write((char*)binaryImage.data(), binaryImage.size());
    temp_file.close();

    *this = ImageConverter(temp_filename.c_str());
    std::remove(temp_filename.c_str());
}

void ImageConverter::AddPadding(std::size_t size, PaddingType type) {
    // TO DO: swap algorithm with using PAddingType
}

void ImageConverter::SaveImage(std::string_view path_to_image) {
    std::vector<type> result;
    for (auto i = 0; i != channels; ++i) {
        auto const& mutableData = rgbImage[i].mutable_data();
        for (auto const& row : mutableData) {
            result.insert(result.end(), row.begin(), row.end());
        }
    }
    auto height = rgbImage[0].rows();
    auto width  = rgbImage[0].collums();

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
        if (rgbImage[j].collums() == 0 || rgbImage[j].rows() == 0) {
            continue;
        }
        auto result = averagePixelResizer(rgbImage[j], averageKernel);
        rgbImage[j] = result;
    }
}

void ImageConverter::bilinearFiltration(std::size_t kernelSize) {
    Channel<3> newRgbImage;
    for (auto i = 0; i != newRgbImage.size(); ++i) {
        newRgbImage[i].default_init(rgbImage[i].rows() * kernelSize, rgbImage[i].collums() * kernelSize);
    }
    for (auto j = 0; j != rgbImage.size(); ++j) {
        auto result    = bilinearInterpolation(rgbImage[j], kernelSize);
        newRgbImage[j] = result;
    }
    rgbImage = std::move(newRgbImage);
}

void ImageConverter::ShakalImage(std::size_t shakal_depth) {
    auto downSizer = [this](std::size_t shakal_depth) { averageFiltration(shakal_depth); };
    auto upscaler  = [this](std::size_t shakal_depth) { bilinearFiltration(shakal_depth); };

    ShakalImage(shakal_depth, downSizer, upscaler);
}

}  // namespace converter
