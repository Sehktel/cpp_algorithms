#pragma once

// Текстура
class Texture
{
private:
    unsigned char* data;
    int width;
    int height;
    int channels;
    
public:
    Texture();
    Texture(int width, int height, int channels = 3);
    ~Texture();
    
    bool loadFromFile(const char* filename);
    bool createSolidColor(int width, int height, unsigned char r, unsigned char g, unsigned char b);
    
    // Получить цвет в точке (u, v) где u, v в диапазоне [0, 1]
    void sample(float u, float v, unsigned char& r, unsigned char& g, unsigned char& b) const;
    
    // Билинейная фильтрация
    void sampleBilinear(float u, float v, unsigned char& r, unsigned char& g, unsigned char& b) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    unsigned char* getData() { return data; }
};

