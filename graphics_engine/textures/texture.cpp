#include "texture.h"
#include <algorithm>
#include <cmath>

Texture::Texture() : data(nullptr), width(0), height(0), channels(0) {}

Texture::Texture(int w, int h, int ch) : width(w), height(h), channels(ch)
{
    data = new unsigned char[width * height * channels];
}

Texture::~Texture()
{
    delete[] data;
}

bool Texture::loadFromFile(const char* filename)
{
    // TODO: Реализовать загрузку из файла (BMP, PNG и т.д.)
    return false;
}

bool Texture::createSolidColor(int w, int h, unsigned char r, unsigned char g, unsigned char b)
{
    width = w;
    height = h;
    channels = 3;
    delete[] data;
    data = new unsigned char[width * height * channels];
    
    for (int i = 0; i < width * height; i++)
    {
        data[i * 3] = r;
        data[i * 3 + 1] = g;
        data[i * 3 + 2] = b;
    }
    
    return true;
}

void Texture::sample(float u, float v, unsigned char& r, unsigned char& g, unsigned char& b) const
{
    // Нормализуем координаты
    u = std::fmod(u, 1.0f);
    v = std::fmod(v, 1.0f);
    if (u < 0) u += 1.0f;
    if (v < 0) v += 1.0f;
    
    int x = (int)(u * width) % width;
    int y = (int)(v * height) % height;
    
    int index = (y * width + x) * channels;
    r = data[index];
    g = data[index + 1];
    b = data[index + 2];
}

void Texture::sampleBilinear(float u, float v, unsigned char& r, unsigned char& g, unsigned char& b) const
{
    // Нормализуем координаты
    u = std::fmod(u, 1.0f);
    v = std::fmod(v, 1.0f);
    if (u < 0) u += 1.0f;
    if (v < 0) v += 1.0f;
    
    float fx = u * width - 0.5f;
    float fy = v * height - 0.5f;
    
    int x0 = (int)std::floor(fx);
    int y0 = (int)std::floor(fy);
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    float fracX = fx - x0;
    float fracY = fy - y0;
    
    // Обрабатываем границы
    x0 = std::max(0, std::min(x0, width - 1));
    y0 = std::max(0, std::min(y0, height - 1));
    x1 = std::max(0, std::min(x1, width - 1));
    y1 = std::max(0, std::min(y1, height - 1));
    
    // Билинейная интерполяция
    auto getColor = [&](int x, int y) -> std::tuple<unsigned char, unsigned char, unsigned char> {
        int index = (y * width + x) * channels;
        return {data[index], data[index + 1], data[index + 2]};
    };
    
    auto [r00, g00, b00] = getColor(x0, y0);
    auto [r10, g10, b10] = getColor(x1, y0);
    auto [r01, g01, b01] = getColor(x0, y1);
    auto [r11, g11, b11] = getColor(x1, y1);
    
    // Интерполяция по X
    float r0 = r00 * (1 - fracX) + r10 * fracX;
    float g0 = g00 * (1 - fracX) + g10 * fracX;
    float b0 = b00 * (1 - fracX) + b10 * fracX;
    
    float r1 = r01 * (1 - fracX) + r11 * fracX;
    float g1 = g01 * (1 - fracX) + g11 * fracX;
    float b1 = b01 * (1 - fracX) + b11 * fracX;
    
    // Интерполяция по Y
    r = (unsigned char)(r0 * (1 - fracY) + r1 * fracY);
    g = (unsigned char)(g0 * (1 - fracY) + g1 * fracY);
    b = (unsigned char)(b0 * (1 - fracY) + b1 * fracY);
}

