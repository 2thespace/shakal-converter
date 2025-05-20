#ifndef IMAGEDECODER_HPP_
#define IMAGEDECODER_HPP_

#include <array>
#include <cstdint>
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
                       std::size_t collumnPositionSecond);

    Matrix<> operator*(Matrix<T> const& rhs);
    Matrix<> operator*=(Matrix<T> const& rhs);

    template <bool isNorm = false>
    double sumarize();

    auto& mutable_data() const;

    Matrix() = default;
    Matrix(Type const& new_data);

private:
    Type data;
    std::size_t n = 0, m = 0;  // collums, rows
};

template <typename T>
inline std::size_t converter::Matrix<T>::collums() const {
    return n;
}

template <typename T>
inline std::size_t converter::Matrix<T>::rows() const {
    return m;
}

template <typename T>
inline void Matrix<T>::default_init(std::size_t rowsCnt, std::size_t collumnsCnt)
{
    data.resize(collumnsCnt);
    for(auto it = data.begin(); it != data.end(); ++it) {
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
                                     std::size_t rowPositionSecond, std::size_t collumnPositionSecond) {
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
    auto rhsData = rhs.mutable_data();
    
    for(auto i=0; i != collums(); ++i) {
        for(auto j=0; j != rows(); ++j) {
           // element [i][j] in result need to be <rows_lhs, collumns_rhs>
           for(auto k = 0; k != collums(); ++k) {
                resultData[i][j] += data[i][k]*rhsData[k][i];
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
    ImageConverter(std::string_view path_to_image);

    void AddPadding(std::size_t size, PaddingType type = PaddingType::SIMPLE);
    void SaveImage(std::string_view path_to_image);
    void ShakalImage(std::size_t shakal_depth);
    void resizeImage(std::size_t kDivided);
    void upsckaleImage(std::size_t kUpscale);

private:
    Channel<3> rgbImage;

    void averageFiltration(std::size_t skipPixel = 0, std::size_t kernelSize = 3);
    void bilinearFiltration(std::size_t skipPixel = 0, std::size_t kernelSize = 3);
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

}  // namespace converter

#endif  // IMAGEDECODER_HPP_