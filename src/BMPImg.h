#pragma once

#include <iostream>
#include <string>
#include <vector>


class BMPImg{
private:
    struct BMPFileHeader {
        uint16_t file_type{0x4D42};;
        uint32_t file_size{0};
        uint16_t reserved1{0};
        uint16_t reserved2{0};
        uint32_t offset_data{0};
    };
    
     struct BMPInfoHeader {
        uint32_t size{ 0 };//размер этого заголовка
        int32_t width{ 0 };
        int32_t height{ 0 };         
        uint16_t bit_count{ 0 };
        uint32_t compression{ 0 };
        uint32_t size_image{ 0 };
        int32_t x_pixels_per_meter{ 0 };
        int32_t y_pixels_per_meter{ 0 };
    };

    BMPFileHeader FHeader;
    BMPInfoHeader IHeader;
    std::vector<unsigned char> buf;

    template<typename T>
    T bytesToInt(const std::vector<unsigned char>& data, size_t start, size_t size);
    
    void paintline(int x0,int y0,int x1,int y1);
    
    bool validateCoords(int x, int y);
   

public:
    BMPImg();
    BMPImg(std::string &filename);

    void Load(const std::string& filename);
    void PrintImage();
    void DrawCross(int x1, int y1, int x2, int y2);
    void Save(const std::string& filename);
};