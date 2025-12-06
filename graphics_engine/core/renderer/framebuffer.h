#pragma once

// Буфер кадра (framebuffer)
class Framebuffer
{
private:
    unsigned char* data;
    float* depthBuffer;  // Z-buffer
    int width;
    int height;
    
public:
    Framebuffer(int width, int height);
    ~Framebuffer();
    
    void clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
    void clearDepth(float depth = 1.0f);
    
    void setPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b);
    void setPixelDepth(int x, int y, float depth);
    
    bool testDepth(int x, int y, float depth);
    
    unsigned char* getData() { return data; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    // Сохранение в файл (BMP)
    bool saveToBMP(const char* filename);
};

