#include "media/video_builder.hpp"

#include <cmath>

#define RED (4 * (y * width + x) + 0)
#define GREEN (4 * (y * width + x) + 1)
#define BLUE (4 * (y * width + x) + 2)

int main()
{
    constexpr int width = 1080;
    constexpr int height = 720;
    constexpr int fps = 60;

    media::video_builder builder{ "video.mp4", width, height, 12800000, fps, 12, 2 };

    std::unique_ptr<uint8_t[]> buffer{ new uint8_t[width * height * 4]{ 0 } };

    for (int i = 0; i < fps * 10; ++i)
    {
        for (int x = 0; x < width; ++x)
        {
            for (int y = 0; y < height; ++y)
            {
                double a = (std::sin((x / (y + 1) + i) / 32) + 1) / 2;
                double b = (std::cos((x / (y + 1) + i) / 32) + 1) / 2;
                double c = (a + b) / 2;

                buffer[RED] = (uint8_t) (a * 255);
                buffer[GREEN] = (uint8_t) (b * 255);
                buffer[BLUE] = (uint8_t) (c * 255);
            }
        }
        builder.push_frame(buffer.get());
    }
}