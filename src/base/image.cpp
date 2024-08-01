#include "base/image.hpp"

#include <cstring>

namespace base
{

    pixel::pixel() : red{ 0 }, green{ 0 }, blue{ 0 } {}

    pixel::pixel(uint8_t r, uint8_t g, uint8_t b) : red{ r }, green{ g }, blue{ b } {}

    pixel::pixel(uint32_t pix) :
    red{ static_cast<uint8_t>((pix >> 16) & 0xFF) },
    green{ static_cast<uint8_t>((pix >> 8) & 0xFF) },
    blue{ static_cast<uint8_t>((pix >> 0) & 0xFF) } {}

    pixel operator""_rgb(unsigned long long pix) { return pixel{ static_cast<uint32_t>(pix) }; }

    image::image(int width, int height) : _width{ width }, _height{ height }, _buf{ new pixel[width * height] } {}

    image::image(const image& img) : _width{ img._width }, _height{ img._height }, _buf{ new pixel[_width * _height] }
    {
        std::memcpy(_buf, img._buf, _width * _height * sizeof(pixel));
    }

    image::image(image&& img) : _width{ img._width }, _height{ img._height }, _buf{ img._buf }
    {
        img._buf = nullptr;
    }

    image& image::operator=(const image& img)
    {
        if (this == &img) return *this;

        _width = img._width;
        _height = img._height;

        delete[] _buf;
        _buf = img._buf;

        return *this;
    }

    image& image::operator=(image&& img)
    {
        if (this == &img) return *this;

        _width = img._width;
        _height = img._height;

        delete[] _buf;
        _buf = img._buf;
        img._buf = nullptr;

        return *this;
    }

    image::~image()
    {
        delete[] _buf;
    }

    int image::width() const
    {
        return _width;
    }

    int image::height() const
    {
        return _height;
    }

    pixel* image::get_buffer() const
    {
        return _buf;
    }

    pixel& image::operator[](image::coordinate coord)
    {
        return _buf[coord.y * _width + coord.x];
    }

    const pixel& image::operator[](image::coordinate coord) const
    {
        return _buf[coord.y * _width + coord.x];
    }

    void image::fill(pixel pix)
    {
        for (int i = 0; i < _width * _height; ++i)
        {
            _buf[i] = pix;
        }
    }


}