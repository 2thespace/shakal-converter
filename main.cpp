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
    converter::ImageConverter tmp("../tests/input.png");
    tmp.ShakalImage(4);
    tmp.SaveImage("../tests/shakal.png");
}

void Tests() {
    MatrixTest();
    ImageReadTest();
}

int main() {
    Tests();
    return 0;
}