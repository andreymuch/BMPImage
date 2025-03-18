#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include "BMPImg.h"

template<typename T>
T BMPImg::bytesToInt(const std::vector<unsigned char>& data, size_t start, size_t size){
    T result = 0;
    memcpy(&result, data.data() + start, size);
    return result;
}
void BMPImg::PrintImage(){
    if (IHeader.bit_count == 24|| IHeader.bit_count == 32){
        
        int bytes_per_pixel = IHeader.bit_count/8;
        const int bytes_per_line = (IHeader.bit_count == 24) ? ((IHeader.width * 3 + 3) / 4 * 4) : (IHeader.width * 4);
        bool is_top_down = (IHeader.height < 0);

        for (int y=0; y < std::abs(IHeader.height); ++y){
            int32_t curent_y = is_top_down ? y : (IHeader.height - 1 - y);

            for (int32_t x = 0; x < IHeader.width; ++x) {
                size_t offset = FHeader.offset_data + x * bytes_per_pixel+ curent_y * bytes_per_line;
                int blue = buf[offset];
                int green = buf[offset + 1];
                int red = buf[offset + 2];
                if (blue + green + red == 0){
                    std::cout << ".";
                }else{
                    std::cout <<"#";
                }
            }
            std::cout << std::endl;
        }
    }
    else{
        std::cerr << "Неподдерживаемое количество бит на пиксель!" << std::endl;
    }
}
void BMPImg::paintline(int x0,int y0,int x1,int y1) {
        
    const int bytes_per_pixel = IHeader.bit_count / 8;
    const int bytes_per_line = (IHeader.bit_count == 24) ? ((IHeader.width * 3 + 3) / 4 * 4) : (IHeader.width * 4);

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx - dy;

    while(true) {
        if(validateCoords(x0, y0)){
            size_t offset = FHeader.offset_data + x0 * bytes_per_pixel + y0 * bytes_per_line;
            buf[offset] = 0;
            buf[offset + 1] = 0;
            buf[offset + 2] = 0;

        }

        if(x0 == x1 && y0 == y1) break;
        
        int e2 = 2 * err;
        if(e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if(e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
void BMPImg::Load(const std::string& filename){
    std::ifstream file("../../TestImage/" + filename + ".bmp", std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка открытия файла!");
    }
    file.seekg(0x02, std::ios::beg);

    unsigned char bytes[4];
    file.read(reinterpret_cast<char*>(bytes), 4);
    memcpy(&FHeader.file_size, bytes, sizeof(uint32_t));
    buf.resize(FHeader.file_size);

    file.seekg(0, std::ios::beg);

    if (file.read(reinterpret_cast<char*>(buf.data()), FHeader.file_size)) {
        FHeader.file_type = bytesToInt<uint16_t>(buf, 0x00, 2);
        FHeader.offset_data = bytesToInt<uint32_t>(buf, 0x0A, 4);
        IHeader.size = bytesToInt<uint32_t>(buf, 0x0E, 4);
        IHeader.width = bytesToInt<int32_t>(buf, 0x12, 4);
        IHeader.height = bytesToInt<int32_t>(buf, 0x16, 4);
        IHeader.bit_count = bytesToInt<uint16_t>(buf, 0x1C, 2);
        std::cout << "битность: " << IHeader.bit_count << std::endl;
        IHeader.compression = bytesToInt<uint32_t>(buf, 0x1E, 4);
        IHeader.size_image = bytesToInt<uint32_t>(buf, 0x22, 4);
        IHeader.x_pixels_per_meter = bytesToInt<int32_t>(buf, 0x26, 4);
        IHeader.y_pixels_per_meter = bytesToInt<int32_t>(buf, 0x2A, 4);
    } else {
        throw std::runtime_error("Ошибка чтения из файла!");
    }
}
bool BMPImg::validateCoords(int x, int y){
    return (x >= 0 && x < IHeader.width && y >= 0 && y < std::abs(IHeader.height));
}
void BMPImg::DrawCross(int x1, int y1, int x2, int y2){
    if (validateCoords(x1, y1) && (validateCoords(x2, y2))){
        paintline(x1, y1, x2, y2);
        paintline(x1, y2, x2, y1);
    }
    else{
        std::cerr << "Ошибка в координатах!" << std::endl;
    }
}
void BMPImg::Save(const std::string& filename){
    std::ofstream outfile("../../OutImage/" + filename + ".bmp", std::ios::binary);
    outfile.write(reinterpret_cast<const char*>(buf.data()), buf.size());
}
BMPImg::BMPImg(std::string &filename){
    Load(filename);
}
BMPImg::BMPImg(){}