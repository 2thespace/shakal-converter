#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

#include "ImageDecoder.hpp"

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
    converter::ImageConverter tmp("../tests/input.png");
    tmp.SaveImage("../tests/output.png");
}

void ImageShakalTest() {
     std::cout << "All tests passed" << std::endl;
    converter::ImageConverter tmp("../tests/input.png");
    tmp.ShakalImage(4);
    tmp.SaveImage("../tests/shakal.png");
     std::cout << "All tests passed" << std::endl;
}

void TestSummarize() {
    
    // zeros
    {
        converter::Matrix tmp({{0, 0, 0, 0}});
        assert(tmp.sumarize() == 0.0);
    }
    // ones 
    {
        converter::Matrix tmp({{1, 1, 1, 1}});
        assert(tmp.sumarize() == 4.0);
    }
    std::cout << "End ones" << std::endl;
    // ones 
    {
        converter::Matrix tmp({{1, 1, 1, 1}});
        assert(tmp.sumarize<true>() == 1.0);
    }

    std::cout << "End sumarize" << std::endl;
}

void Tests() {
    MatrixTest();
    TestSummarize();
    ImageReadTest();
}

int main() {
    Tests();
    std::cout << "All tests passed" << std::endl;
    return 0;
}