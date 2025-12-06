#include "framebuffer.h"
#include <cstring>
#include <fstream>
#include <algorithm>

Framebuffer::Framebuffer(int w, int h) : width(w), height(h)
{
    data = new unsigned char[width * height * 3];
    depthBuffer = new float[width * height];
    clear();
    clearDepth();
}

Framebuffer::~Framebuffer()
{
    delete[] data;
    delete[] depthBuffer;
}

void Framebuffer::clear(unsigned char r, unsigned char g, unsigned char b)
{
    for (int i = 0; i < width * height; i++)
    {
        data[i * 3] = r;
        data[i * 3 + 1] = g;
        data[i * 3 + 2] = b;
    }
}

void Framebuffer::clearDepth(float depth)
{
    std::fill(depthBuffer, depthBuffer + width * height, depth);
}

void Framebuffer::setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        int index = (y * width + x) * 3;
        data[index] = r;
        data[index + 1] = g;
        data[index + 2] = b;
    }
}

void Framebuffer::setPixelDepth(int x, int y, float depth)
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        depthBuffer[y * width + x] = depth;
    }
}

bool Framebuffer::testDepth(int x, int y, float depth)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    
    if (depth < depthBuffer[y * width + x])
    {
        depthBuffer[y * width + x] = depth;
        return true;
    }
    
    return false;
}

bool Framebuffer::saveToBMP(const char* filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        return false;
    
    // BMP заголовок
    int fileSize = 54 + width * height * 3;
    int dataOffset = 54;
    
    // BMP file header
    unsigned char bmpFileHeader[14] = {
        'B', 'M',           // Сигнатура
        (unsigned char)(fileSize), (unsigned char)(fileSize >> 8),
        (unsigned char)(fileSize >> 16), (unsigned char)(fileSize >> 24),
        0, 0, 0, 0,         // Зарезервировано
        (unsigned char)(dataOffset), (unsigned char)(dataOffset >> 8),
        (unsigned char)(dataOffset >> 16), (unsigned char)(dataOffset >> 24)
    };
    
    // BMP info header
    unsigned char bmpInfoHeader[40] = {
        40, 0, 0, 0,        // Размер заголовка
        (unsigned char)(width), (unsigned char)(width >> 8),
        (unsigned char)(width >> 16), (unsigned char)(width >> 24),
        (unsigned char)(height), (unsigned char)(height >> 8),
        (unsigned char)(height >> 16), (unsigned char)(height >> 24),
        1, 0,               // Плоскости
        24, 0,              // Бит на пиксель
        0, 0, 0, 0,         // Сжатие
        0, 0, 0, 0,         // Размер изображения
        0, 0, 0, 0,         // X пикселей на метр
        0, 0, 0, 0,         // Y пикселей на метр
        0, 0, 0, 0,         // Цвета в палитре
        0, 0, 0, 0          // Важные цвета
    };
    
    file.write((char*)bmpFileHeader, 14);
    file.write((char*)bmpInfoHeader, 40);
    
    // Данные изображения (BMP хранит строки снизу вверх)
    for (int y = height - 1; y >= 0; y--)
    {
        for (int x = 0; x < width; x++)
        {
            int index = (y * width + x) * 3;
            // BMP использует BGR порядок
            file.write((char*)&data[index + 2], 1);  // B
            file.write((char*)&data[index + 1], 1);  // G
            file.write((char*)&data[index], 1);      // R
        }
        // Выравнивание по 4 байта
        int padding = (4 - (width * 3) % 4) % 4;
        for (int i = 0; i < padding; i++)
            file.write("\0", 1);
    }
    
    file.close();
    return true;
}

