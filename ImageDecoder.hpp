#ifndef IMAGEDECODER_HPP_
#define IMAGEDECODER_HPP_

#include <array>
#include <cstdint>
#include <filesystem>
#include <string_view>
#include <vector>

namespace converter {
template <typename T = std::uint8_t>
class Matrix {
public:
    using value_type = T;
    using Row        = std::vector<T>;
    using Collumn    = Row;
    using Type       = std::vector<Row>;

    std::size_t collums() const;
    std::size_t rows() const;

    void default_init(std::size_t rowsCnt, std::size_t collumnsCnt);

    void insert_row(std::size_t position, Row const& new_row);
    void insert_collumn(std::size_t position, Collumn const& new_coll);
    auto& getRow(std::size_t position) const;
    // TODO make span realisation
    Matrix<> subMatrix(std::size_t rowPositionFirst, std::size_t collumnPositionFirst, std::size_t rowPositionSecond,
                       std::size_t collumnPositionSecond) const;

    Matrix<> operator*(Matrix<T> const& rhs);
    Matrix<> operator*=(Matrix<T> const& rhs);

    template <bool isNorm = false>
    double sumarize();

    auto& mutable_data() const;
    auto& mutable_data();

    Matrix() = default;
    Matrix(Type const& new_data);

private:
    Type data;
    std::size_t n = 0, m = 0;  // collums, rows
};

template <typename T>
Matrix<T> averagePixelResizer(Matrix<T> const& inputMatrix, Matrix<T> const& kernelMatrix) {
    typename converter::Matrix<T> resultData;
    auto kX = kernelMatrix.rows();
    auto kY = kernelMatrix.collums();

    auto inputRow = inputMatrix.rows();
    auto inputCol = inputMatrix.collums();
    for (auto i = 0; i <= (inputRow - kX); i = i + kX) {
        typename converter::Matrix<T>::Row pixelLine;
        for (auto j = 0; j <= (inputCol - kY); j = j + kY) {
            auto inputWindow = inputMatrix.subMatrix(i, j, i + kX, j + kY);
            inputWindow *= kernelMatrix;
            T pixel = static_cast<T>(inputWindow.template sumarize<true>());
            pixelLine.emplace_back(pixel);
        }
        resultData.insert_row(resultData.rows(), pixelLine);
    }
    return resultData;
}

template <typename T>
Matrix<T> bicubicInterpolation(Matrix<T> const& input, std::size_t upscaleK = 2) {
    Matrix<T> result;
    return result;
}

template <typename T>
Matrix<T> bilinearInterpolation(Matrix<T> const& input, std::size_t upscaleK = 2) {
    Matrix<T> result;

    const auto& inputRaw = input.mutable_data();  // Assuming this returns const reference

    auto ySize   = inputRaw.size();
    auto resultY = upscaleK * ySize;
    for (std::size_t y_new = 0; y_new < resultY; ++y_new) {
        double y       = double(y_new) / upscaleK;
        std::size_t y1 = static_cast<std::size_t>(y);
        y1             = std::min(y1, ySize - 1);
        std::size_t y2 = std::min(y1 + 1, ySize - 1);
        double b       = y - y1;  // Fractional part
        auto xSize     = inputRaw[y1].size();
        auto resultX   = upscaleK * xSize;
        typename Matrix<T>::Row line;
        for (std::size_t x_new = 0; x_new < resultX; ++x_new) {
            double x       = double(x_new) / upscaleK;
            std::size_t x1 = static_cast<std::size_t>(x);
            x1             = std::min(x1, xSize - 1);
            std::size_t x2 = std::min(x1 + 1, xSize - 1);
            double a       = x - x1;  // Fractional part

#if 1
            // Bilinear interpolation
            auto newValue = (1 - a) * (1 - b) * inputRaw[y1][x1] + a * (1 - b) * inputRaw[y1][x2] +
                            (1 - a) * b * inputRaw[y2][x1] + a * b * inputRaw[y2][x2];

#else
            auto newValue = inputRaw[y1][x1];
#endif
            line.push_back(newValue);
        }
        result.insert_row(result.rows(), line);
    }
    return result;
}

template <typename T>
inline std::size_t converter::Matrix<T>::collums() const {
    return n;
}

template <typename T>
inline std::size_t converter::Matrix<T>::rows() const {
    return m;
}

template <typename T>
inline void Matrix<T>::default_init(std::size_t rowsCnt, std::size_t collumnsCnt) {
    data.resize(collumnsCnt);
    for (auto it = data.begin(); it != data.end(); ++it) {
        it->resize(rowsCnt);
    }
    n = collumnsCnt;
    m = rowsCnt;
}

template <typename T>
inline void Matrix<T>::insert_row(std::size_t position, Row const& new_row) {
    if (position > m) {
        return;
    }
    data.insert(data.begin() + position, new_row);
    m = data.size();
    n = data[0].size();
}

template <typename T>
inline void Matrix<T>::insert_collumn(std::size_t position, Collumn const& new_coll) {
    if (position > n) {
        return;
    }
    for (std::size_t j = 0; j != m; ++j) {
        data[j].insert(data[j].begin() + position, new_coll[j]);
    }
    n = data[0].size();
    m = data.size();
}

template <typename T>
inline auto& Matrix<T>::getRow(std::size_t position) const {
    if (position > m) {
        throw;
    }
    return data[position];
}

template <typename T>
inline Matrix<> Matrix<T>::subMatrix(std::size_t rowPositionFirst, std::size_t collumnPositionFirst,
                                     std::size_t rowPositionSecond, std::size_t collumnPositionSecond) const {
    Matrix<> subMatrix;
    for (auto i = rowPositionFirst; i != rowPositionSecond; ++i) {
        Row subRow;
        subRow.insert(subRow.end(), data.at(i).begin() + collumnPositionFirst,
                      data.at(i).begin() + collumnPositionSecond);
        subMatrix.insert_row(subMatrix.rows(), subRow);
    }
    return subMatrix;
}

template <typename T>
inline Matrix<> Matrix<T>::operator*(Matrix<T> const& rhs) {
    if (this->rows() != rhs.collums()) {
        throw;
    }
    Matrix<T> result;
    result.default_init(rows(), rhs.collums());
    auto resultData = result.mutable_data();
    auto rhsData    = rhs.mutable_data();

    for (auto i = 0; i != collums(); ++i) {
        for (auto j = 0; j != rows(); ++j) {
            // element [i][j] in result need to be <rows_lhs, collumns_rhs>
            for (auto k = 0; k != collums(); ++k) {
                resultData[i][j] += data[i][k] * rhsData[k][i];
            }
        }
    }

    return result;
}

template <typename T>
inline Matrix<> Matrix<T>::operator*=(Matrix<T> const& rhs) {
    return this->operator*(rhs);
}

template <typename T>
inline auto& Matrix<T>::mutable_data() const {
    return data;
}
template <typename T>
inline auto& Matrix<T>::mutable_data() {
    return data;
}

template <typename T>
inline converter::Matrix<T>::Matrix(Type const& new_data) {
    data = new_data;
    m    = data.size();
    n    = data[0].size();
}

template <std::uint8_t channelNumber>
using Channel = std::array<Matrix<>, channelNumber>;
using type    = Matrix<>::value_type;

enum class PaddingType {
    SIMPLE = 0x0,
};

class ImageConverter {
public:
    ImageConverter(std::filesystem::path const& path_to_image);
    ImageConverter(std::span<std::uint8_t> binearyImage, std::uint32_t width, std::uint32_t height,
                   std::uint8_t collors);

    void AddPadding(std::size_t size, PaddingType type = PaddingType::SIMPLE);
    void SaveImage(std::string_view path_to_image);

    template <typename ShakalDownSizer, typename ShakalUpscaler>
    void ShakalImage(std::size_t shakal_depth, ShakalDownSizer converter, ShakalUpscaler upscaler);

    void ShakalImage(std::size_t shakal_depth);

    void resizeImage(std::size_t kDivided);
    void upsckaleImage(std::size_t kUpscale);

    void averageFiltration(std::size_t kernelSize = 3);
    void bilinearFiltration(std::size_t kernelSize = 3);

private:
    Channel<3> rgbImage;
    std::size_t channels;
};

template <typename T>
template <bool isNorm>
inline double Matrix<T>::sumarize() {
    double kNorm = isNorm ? static_cast<double>(collums() * rows()) : 1.0f;
    double sum{};
    for (auto i = 0; i != collums(); ++i) {
        for (auto j = 0; j != rows(); ++j) {
            sum += data[j][i] / kNorm;
        }
    }
    return sum;
}

template <typename ShakalDownSizer, typename ShakalUpscaler>
inline void ImageConverter::ShakalImage(std::size_t shakal_depth, ShakalDownSizer converter, ShakalUpscaler upscaler) {
    // AddPadding(padding);
    converter(shakal_depth);
    upscaler(shakal_depth);
}

}  // namespace converter

#endif  // IMAGEDECODER_HPP_
