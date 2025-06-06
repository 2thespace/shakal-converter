#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "ImageDecoder.hpp"

std::filesystem::path inputImageForTest = "../tests/input.png";

void MatrixTest() {
    converter::Matrix tmp({{0, 0, 0, 0}});
    assert(tmp.collums() == 4);
    assert(tmp.rows() == 1);

    converter::Matrix<>::Row row{1, 1, 1, 1};
    tmp.insert_row(1, row);
    assert(tmp.collums() == 4);
    assert(tmp.rows() == 2);
    converter::Matrix<>::Collumn col{2, 2};
    tmp.insert_collumn(1, col);
    assert(tmp.collums() == 5);
    assert(tmp.rows() == 2);
}

void ImageReadTest() {
    converter::ImageConverter tmp(inputImageForTest);
    tmp.SaveImage("../tests/output.png");
}

void ImageShakalTest(std::size_t shakal_depth = 4) {
    converter::ImageConverter tmp(inputImageForTest);
    tmp.ShakalImage(shakal_depth);
    tmp.SaveImage("../tests/shakal.png");
}

void MultiplyTest() {
    converter::Matrix first({{1, 2}, {3, 4}, {5, 6}});
    converter::Matrix second({{7, 8, 9}, {10, 11, 12}});
    converter::Matrix expected({{27, 30, 33}, {61, 68, 75}, {95, 106, 117}});
    auto third = first * second;
    // TO DO make == operator and specialization for floating types
    // assert(std::equal(expected.mutable_data().begin(), expected.mutable_data().end(), third.mutable_data().begin()));
}

void TestSummarize() {
    // zeros
    {
        converter::Matrix tmp({{0, 0, 0, 0}});
        double res = tmp.sumarize();
        assert(res == 0.0);
    }
    // ones
    {
        converter::Matrix tmp({{1, 1, 1, 1}});
        double res = tmp.sumarize();
        assert(res == 4.0);
    }
    // ones
    {
        converter::Matrix tmp({{1, 1, 1, 1}});
        double res = tmp.sumarize<true>();
        assert(res == 1.0);
    }
}
void SubmatrixTest() {
    converter::Matrix tmp({{0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}, {0, 1, 2, 3}});
    auto res = tmp.subMatrix(1, 1, 2, 2);
    assert(res.collums() == 1);
    assert(res.rows() == 1);
}

void ImageFromVectorTest() {
#include "anton.hpp"
    converter::ImageConverter tmp(antonPng, 268, 140, 3);
    tmp.SaveImage("../tests/output_vector.png");
    tmp.ShakalImage(4);

    tmp.SaveImage("../tests/shakal_vector.png");
}

void Tests() {
    MatrixTest();
    TestSummarize();
    MultiplyTest();

    ImageReadTest();
    SubmatrixTest();
    ImageShakalTest();
    ImageFromVectorTest();
}

int main(int argc, char** argv) {
    if (argc > 2) {
        inputImageForTest       = argv[1];
        std::size_t shakalDepth = std::stoi(argv[2]);
        ImageShakalTest(shakalDepth);
        return 0;
    }
    if (!std::filesystem::exists(inputImageForTest)) {
        std::cerr << "File does not exist!\n";
        return 1;
    }

    if (!std::filesystem::is_regular_file(inputImageForTest)) {
        std::cerr << "Path is not a regular file!\n";
        return 1;
    }
    Tests();
    std::cout << "All tests passed" << std::endl;
    return 0;
}
