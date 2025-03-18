
#include <iostream>
#include <string>
#include "BMPImg.h"

int main() {
    std::string input;
    std::cout << "Enter input BMP file name: ";
    std::cin >> input;
    BMPImg img(input);
    img.PrintImage();
    std::cout <<"введите x1 y1 x2 y2 " << std::endl;
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    img.DrawCross(x1, y1, x2, y2);
    img.PrintImage();
    std::string outfile;
    std::cout <<"Введите имя файла " << std::endl;
    std::cin >> outfile;
    img.Save(outfile);
    return 0;
}
