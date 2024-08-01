#ifndef GPU_RAYTRACE_IMAGE_HPP
#define GPU_RAYTRACE_IMAGE_HPP

#include <cstdint>
#include <cstddef>

namespace base
{
    class alignas(4) pixel
    {
    public:
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        explicit pixel();
        explicit pixel(uint8_t r, uint8_t g, uint8_t b);
        explicit pixel(uint32_t pix);
        pixel(const pixel&) = default;
        pixel(pixel&&) = default;
        pixel& operator=(const pixel&) = default;
        pixel& operator=(pixel&&) = default;
        ~pixel() = default;
    };

    pixel operator""_rgb(unsigned long long pix);

    static_assert(sizeof(pixel) == 4 &&
                  offsetof(pixel, red) == 0 &&
                  offsetof(pixel, green) == 1 &&
                  offsetof(pixel, blue) == 2,
                  "pixel layout is not correct for compatibility with AV_PIX_FMT_RGB0");

    class image
    {
    private:
        int _width;
        int _height;
        pixel *_buf;

        struct coordinate
        {
            int x;
            int y;
        };
    public:
        image(int width, int height);
        image(const image& img);
        image(image&& img);
        image& operator=(const image& img);
        image& operator=(image&& img);
        ~image();

        int width() const;
        int height() const;
        pixel* get_buffer() const;

        pixel& operator[](coordinate coord);
        const pixel& operator[](coordinate coord) const;

        void fill(pixel pix);
    };

}

#endif //GPU_RAYTRACE_IMAGE_HPP
