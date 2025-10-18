#pragma once

#include <vector>
#include <random>

#include "libs/stb/Image_Class.h"

using namespace std;

void gray_scale(Image &image);
void black_and_white(Image &image);
void invert(Image &image);
void merge(Image &image1, Image &image2, int choice, float merge_ratio);
void vertical_flip(Image &image);
void horizontal_flip(Image &image);
void rotate(Image &image);
void brightness(Image &image, int percent = 100);
void crop(Image &image, int x, int y, int width, int height);
void decorated_two(Image &image, Image &frame_image, int frame_size);
void decorated_three(Image &image, int frame_size);
void frame(Image &image, int option, int frame_size = 35, int r = 0, int g = 0, int b = 0);
void edge_detection(Image &image);
void resize(Image &image, double width, double height);
void blur(Image &image, int level = 4);
void yellow(Image &image);
void oil_paint(Image &image, int level = 5);
void retro(Image &image);
void purple(Image &image);
void infrared(Image &image);
void skew(Image &image, int angle = 20);
void emboss(Image &image);
void sepia(Image &image);
void fire(Image &image);
void neon(Image &image, int choice);
void custom_merge_with_neon(Image &image, Image &edge, double edges_ratio, double original_ratio);
void glowing_edges(Image &image, int choice, double edges_ratio, double original_ratio);
void sharpness(Image &image, int strength);
void swirl(Image &image, int swirl_factor);
uint32_t xorshift32(uint32_t &state)
{
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}
int fastRand255(uint32_t &state)
{
    return xorshift32(state) & 255; // 0–255
}

inline void gray_scale(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int avg = 0;

            for (int k = 0; k <= 2; k++)
                avg += image(i, j, k);

            for (int k = 0; k <= 2; k++)
                image(i, j, k) = avg / 3;
        }
    }
}
inline void black_and_white(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int avg = 0;

            for (int k = 0; k <= 2; k++)
                avg += image(i, j, k);

            for (int k = 0; k <= 2; k++)
                image(i, j, k) = ((avg / 3 < 127) ? 0 : 255);
        }
    }
}
inline void invert(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                image(i, j, k) = 255 - image(i, j, k);
            }
        }
    }
}
inline void merge(Image &image1, Image &image2, int choice, float merge_ratio)
{
    if (merge_ratio == 0)
        return;

    merge_ratio /= 100;

    if (image1.width == image2.width && image1.height == image2.height)
    {
        Image merged_image(image1.width, image1.height);

        for (int i = 0; i < image1.width; ++i)
        {
            for (int j = 0; j < image1.height; ++j)
            {
                merged_image(i, j, 0) = (merge_ratio * image2(i, j, 0) + (1 - merge_ratio) * image1(i, j, 0));
                merged_image(i, j, 1) = (merge_ratio * image2(i, j, 1) + (1 - merge_ratio) * image1(i, j, 1));
                merged_image(i, j, 2) = (merge_ratio * image2(i, j, 2) + (1 - merge_ratio) * image1(i, j, 2));
            }
        }
        image1 = merged_image;
    }

    else if (choice == 1)
    {
        int min_width = min(image1.width, image2.width);
        int min_height = min(image1.height, image2.height);

        Image merged_image(min_width, min_height);

        for (int i = 0; i < min_width; ++i)
        {
            for (int j = 0; j < min_height; ++j)
            {
                merged_image(i, j, 0) = (merge_ratio * image2(i, j, 0) + (1 - merge_ratio) * image1(i, j, 0));
                merged_image(i, j, 1) = (merge_ratio * image2(i, j, 1) + (1 - merge_ratio) * image1(i, j, 1));
                merged_image(i, j, 2) = (merge_ratio * image2(i, j, 2) + (1 - merge_ratio) * image1(i, j, 2));
            }
        }
        image1 = merged_image;
    }

    else if (choice == 2)
    {
        long area1 = image1.width * image1.height;
        long area2 = image2.width * image2.height;

        if (area1 > area2)
            resize(image1, image2.width, image2.height);

        else
            resize(image2, image1.width, image1.height);

        Image merged_image(image2.width, image2.height);

        for (int i = 0; i < image1.width; ++i)
        {
            for (int j = 0; j < image1.height; ++j)
            {
                merged_image(i, j, 0) = (merge_ratio * image2(i, j, 0) + (1 - merge_ratio) * image1(i, j, 0));
                merged_image(i, j, 1) = (merge_ratio * image2(i, j, 1) + (1 - merge_ratio) * image1(i, j, 1));
                merged_image(i, j, 2) = (merge_ratio * image2(i, j, 2) + (1 - merge_ratio) * image1(i, j, 2));
            }
        }
        image1 = merged_image;
    }

    else if (choice == 3)
    {
        long area1 = image1.width * image1.height;
        long area2 = image2.width * image2.height;

        if (area1 > area2)
            resize(image2, image1.width, image1.height);

        else
            resize(image1, image2.width, image2.height);

        Image merged_image(image1.width, image1.height);

        for (int i = 0; i < image1.width; ++i)
        {
            for (int j = 0; j < image1.height; ++j)
            {
                merged_image(i, j, 0) = (merge_ratio * image2(i, j, 0) + (1 - merge_ratio) * image1(i, j, 0));
                merged_image(i, j, 1) = (merge_ratio * image2(i, j, 1) + (1 - merge_ratio) * image1(i, j, 1));
                merged_image(i, j, 2) = (merge_ratio * image2(i, j, 2) + (1 - merge_ratio) * image1(i, j, 2));
            }
        }
        image1 = merged_image;
    }
}
inline void horizontal_flip(Image &image)
{
    for (int i = 0; i < image.width / 2; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                unsigned int pix = image(i, j, k);
                image(i, j, k) = image(image.width - 1 - i, j, k);
                image(image.width - 1 - i, j, k) = pix;
            }
        }
    }
}
inline void vertical_flip(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height / 2; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                unsigned int pix = image(i, j, k);
                image(i, j, k) = image(i, image.height - 1 - j, k);
                image(i, image.height - 1 - j, k) = pix;
            }
        }
    }
}
inline void rotate(Image &image)
{
    Image rotated_image(image.height, image.width);

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                rotated_image(image.height - 1 - j, i, k) = image(i, j, k);
            }
        }
    }
    image = rotated_image;
}
inline void brightness(Image &image, int percent)
{
    if (percent == 100)
        return;

    float level = (percent - 100) / 100.0f;

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                image(i, j, k) = clamp((int)(image(i, j, k) * (1 + level)), 0, 255);
            }
        }
    }
}
inline void crop_darken_and_lighten(Image &image, int x, int y, int width, int height)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                if (!(i >= x and i < width + x and j >= y and j < height + y))
                {
                    image(i, j, k) /= 4;
                }
            }
        }
    }
}
inline void crop(Image &image, int x, int y, int width, int height)
{
    if (x == 0 and y == 0 and width == image.width - 1 and height == image.height - 1)
        return;

    Image cropped_image(width, height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                cropped_image(i, j, k) = image(i + x, j + y, k);
            }
        }
    }
    image = cropped_image;
}
inline void decorated_two(Image &image, Image &frame_image, int frame_size)
{
    int s = frame_size / 4;

    // -- Top Horizontal Line --
    for (int j = frame_size - s; j < frame_size; ++j)
    {
        for (int i = 0; i < frame_image.width; ++i)
        {
            frame_image(i, j, 0) = 255;
            frame_image(i, j, 1) = 255;
            frame_image(i, j, 2) = 255;
        }
    }

    // -- Left Vertical Line --
    for (int i = frame_size - s; i < frame_size; ++i)
    {
        for (int j = 0; j < frame_image.height; ++j)
        {
            frame_image(i, j, 0) = 255;
            frame_image(i, j, 1) = 255;
            frame_image(i, j, 2) = 255;
        }
    }

    // -- Right Vertical Line --
    for (int i = image.width + frame_size; i < image.width + s + frame_size; ++i)
    {
        for (int j = 0; j < frame_image.height; ++j)
        {
            frame_image(i, j, 0) = 255;
            frame_image(i, j, 1) = 255;
            frame_image(i, j, 2) = 255;
        }
    }

    // -- Bottom Horizontal Line --
    for (int j = frame_size + image.height; j < frame_size + image.height + s; ++j)
    {
        for (int i = 0; i < frame_image.width; ++i)
        {
            frame_image(i, j, 0) = 255;
            frame_image(i, j, 1) = 255;
            frame_image(i, j, 2) = 255;
        }
    }
}
inline void decorated_three(Image &image, int frame_size)
{
    int s = frame_size / 2;

    // -- Left inner squares --
    for (int i = 0; i < min(s * 2, image.width); i++) // Thickness
    {
        for (int j = 0; j < min(s * 2, image.height); j++) // Length
            for (int k = 0; k < 3; k++)
                image(i, j, k) = 255;
        for (int j = max(0, image.height - s * 2); j < image.height; j++)
            for (int k = 0; k < 3; k++)
                image(i, j, k) = 255;
    }

    // -- Right inner squares --
    for (int i = max(0, image.width - s * 2); i < image.width; i++) // Thickness
    {
        for (int j = max(0, image.height - s * 2); j < image.height; j++) // Length
            for (int k = 0; k < 3; k++)
                image(i, j, k) = 255;
        for (int j = 0; j < min(s * 2, image.height); j++)
            for (int k = 0; k < 3; k++)
                image(i, j, k) = 255;
    }

    // -- Inner square vertical --
    for (int i = max(0, 3 * s); i < min((int)(3.5 * s), image.width); i++) // Thickness
    {
        for (int j = 0; j < min(s * 3, image.height); j++) // Length
        {
            for (int k = 0; k < 3; k++)
            {
                if (image.height - j - 1 >= 0 && image.width - i - 1 >= 0)
                {
                    image(i, j, k) = 255;
                    image(i, image.height - j - 1, k) = 255;
                    image(image.width - i - 1, j, k) = 255;
                    image(image.width - i - 1, image.height - j - 1, k) = 255;
                }
            }
        }
    }

    // -- Inner square horizontal --
    for (int j = max(0, s * 3); j < min((int)(3.5 * s), image.height); j++) // Thickness
    {
        for (int i = 0; i < min((int)(3.5 * s), image.width); i++) // Length
        {
            for (int k = 0; k < 3; k++)
            {
                if (image.height - j - 1 >= 0 && image.width - i - 1 >= 0)
                {
                    image(i, j, k) = 255;
                    image(i, image.height - j - 1, k) = 255;
                    image(image.width - i - 1, j, k) = 255;
                    image(image.width - i - 1, image.height - j - 1, k) = 255;
                }
            }
        }
    }

    // -- Outer square (vertical) --
    for (int i = max(0, (int)(4.5 * s)); i < min((int)(5.5 * s), image.width); i++) // Thickness
    {
        for (int j = 0; j < min((int)(4.5 * s), image.height); j++) // Length
        {
            for (int k = 0; k < 3; k++)
            {
                if (image.height - j - 1 >= 0 && image.width - i - 1 >= 0)
                {
                    image(i, j, k) = 255;
                    image(i, image.height - j - 1, k) = 255;
                    image(image.width - i - 1, j, k) = 255;
                    image(image.width - i - 1, image.height - j - 1, k) = 255;
                }
            }
        }
    }

    // -- Outer square horizontal --
    for (int j = max(0, (int)(4.5 * s)); j < min((int)(5.5 * s), image.height); j++) // Thickness
    {
        for (int i = 0; i < min((int)(5.5 * s), image.width); i++) // Length
        {
            for (int k = 0; k < 3; k++)
            {
                if (image.height - j - 1 >= 0 && image.width - i - 1 >= 0)
                {
                    image(i, j, k) = 255;
                    image(i, image.height - j - 1, k) = 255;
                    image(image.width - i - 1, j, k) = 255;
                    image(image.width - i - 1, image.height - j - 1, k) = 255;
                }
            }
        }
    }
}
inline void frame(Image &image, int option, int frame_size, int r, int g, int b)
{
    if (frame_size == 0)
        return;

    Image frame_image(image.width + 2 * frame_size, image.height + 2 * frame_size);

    // -- Top Part --
    for (int j = 0; j < frame_size; ++j)
    {
        for (int i = 0; i < frame_image.width; ++i)
        {
            frame_image(i, j, 0) = r;
            frame_image(i, j, 1) = g;
            frame_image(i, j, 2) = b;
        }
    }

    // -- Left-Right Part --
    for (int j = frame_size; j < image.height + frame_size; ++j)
    {
        for (int i = 0; i < frame_size; ++i)
        {
            frame_image(i, j, 0) = r;
            frame_image(i, j, 1) = g;
            frame_image(i, j, 2) = b;
        }
        for (int i = frame_size; i < image.width + frame_size; ++i)
        {
            frame_image(i, j, 0) = image(i - frame_size, j - frame_size, 0);
            frame_image(i, j, 1) = image(i - frame_size, j - frame_size, 1);
            frame_image(i, j, 2) = image(i - frame_size, j - frame_size, 2);
        }
        for (int i = image.width + frame_size; i < frame_image.width; ++i)
        {
            frame_image(i, j, 0) = r;
            frame_image(i, j, 1) = g;
            frame_image(i, j, 2) = b;
        }
    }

    // -- Bottom Part --
    for (int j = image.height + frame_size; j < frame_image.height; ++j)
    {
        for (int i = 0; i < frame_image.width; ++i)
        {
            frame_image(i, j, 0) = r;
            frame_image(i, j, 1) = g;
            frame_image(i, j, 2) = b;
        }
    }

    if (option == 1)
    {
        // -- Bottom Horizontal Outer-Line --
        int s = frame_size / 2;
        for (int j = frame_size - s; j < frame_size; ++j)
        {
            for (int i = frame_size - s; i < image.width + s + frame_size; ++i)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Left Vertical Outer-Line --
        for (int i = frame_size - s; i < frame_size; ++i)
        {
            for (int j = frame_size; j < image.height + frame_size; ++j)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Right Vertical Outer-Line --
        for (int i = image.width + frame_size; i < image.width + s + frame_size; ++i)
        {
            for (int j = frame_size; j < image.height + frame_size; ++j)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Bottom Horizontal Outer-Line --
        for (int j = frame_size + image.height; j < frame_size + image.height + s; ++j)
        {
            for (int i = frame_size - s; i < image.width + frame_size + s; ++i)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Top Horizontal Inner-Line --
        for (int j = frame_size + s; j < frame_size + 2 * s; ++j)
        {
            for (int i = frame_size + s; i < image.width + frame_size - s; ++i)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Left Vertical Inner-Line --
        for (int i = frame_size + s; i < frame_size + 2 * s; ++i)
        {
            for (int j = frame_size + 2 * s; j < image.height + frame_size - 2 * s; ++j)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Right Vertical Inner-Line --
        for (int i = image.width + frame_size - 2 * s; i < image.width - s + frame_size; ++i)
        {
            for (int j = frame_size + 2 * s; j < image.height + frame_size - 2 * s; ++j)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }

        // -- Bottom Horizontal Inner-Line --
        for (int j = frame_size + image.height - 2 * s; j < frame_size + image.height - s; ++j)
        {
            for (int i = frame_size + s; i < image.width + frame_size - s; ++i)
            {
                frame_image(i, j, 0) = 255;
                frame_image(i, j, 1) = 255;
                frame_image(i, j, 2) = 255;
            }
        }
    }
    else if (option == 2)
    {
        decorated_three(frame_image, frame_size);
        decorated_two(image, frame_image, frame_size);
    }
    else if (option == 3)
    {
        decorated_two(image, frame_image, frame_size);
        decorated_three(frame_image, frame_size);
    }
    image = frame_image;
}
inline void edge_detection(Image &image)
{
    Image detected_image = image;
    blur(detected_image, 2);
    gray_scale(detected_image);

    vector<vector<int>> Kx = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    vector<vector<int>> Ky = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    long double sum = 0;
    int count = 0;

    for (int i = 1; i < image.height - 1; i++)
    {
        for (int j = 1; j < image.width - 1; j++)
        {
            int Sx = 0;
            int Sy = 0;

            for (int l = -1; l <= 1; l++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    Sx += image(j + m, i + l, 0) * Kx[l + 1][m + 1];
                    Sy += image(j + m, i + l, 0) * Ky[l + 1][m + 1];
                }
            }

            double M = sqrt(Sx * Sx + Sy * Sy);
            sum += M;
            count++;
        }
    }

    double threshold = (sum / count) * 1.3;

    for (int i = 1; i < image.height - 1; i++)
    {
        for (int j = 1; j < image.width - 1; j++)
        {
            int Sx = 0;
            int Sy = 0;

            for (int l = -1; l <= 1; l++)
            {
                for (int m = -1; m <= 1; m++)
                {
                    Sx += image(j + m, i + l, 0) * Kx[l + 1][m + 1];
                    Sy += image(j + m, i + l, 0) * Ky[l + 1][m + 1];
                }
            }

            double M = sqrt(Sx * Sx + Sy * Sy);

            short result = ((M >= threshold) ? 255 : 0);

            detected_image(j, i, 0) = result;
            detected_image(j, i, 1) = result;
            detected_image(j, i, 2) = result;
        }
    }
    image = detected_image;
}
inline void resize(Image &image, double width, double height)
{
    Image resized_image(width, height);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                resized_image(i, j, k) = image(round(i * image.width / width), j * image.height / height, k);
            }
        }
    }
    image = resized_image;
}
inline void blur(Image &image, int level)
{
    if (level == 0)
        return;

    for (int times = 0; times < 3; times++)
    {
        vector<vector<long long>> pre_r(image.height + 1, vector<long long>(image.width + 1, 0));
        vector<vector<long long>> pre_g(image.height + 1, vector<long long>(image.width + 1, 0));
        vector<vector<long long>> pre_b(image.height + 1, vector<long long>(image.width + 1, 0));

        for (int i = 0; i < image.height; i++)
        {
            for (int j = 0; j < image.width; j++)
            {
                pre_r[i + 1][j + 1] = image(j, i, 0) + pre_r[i][j + 1] + pre_r[i + 1][j] - pre_r[i][j];
                pre_g[i + 1][j + 1] = image(j, i, 1) + pre_g[i][j + 1] + pre_g[i + 1][j] - pre_g[i][j];
                pre_b[i + 1][j + 1] = image(j, i, 2) + pre_b[i][j + 1] + pre_b[i + 1][j] - pre_b[i][j];
            }
        }

        for (int i = 1; i < image.height - 1; i++)
        {
            for (int j = 1; j < image.width - 1; j++)
            {
                int r1 = max(0, i - level);
                int c1 = max(0, j - level);
                int r2 = min(image.height - 1, i + level);
                int c2 = min(image.width - 1, j + level);

                int count = (r2 - r1 + 1) * (c2 - c1 + 1);

                image(j, i, 0) = (int)((pre_r[r2 + 1][c2 + 1] - pre_r[r1][c2 + 1] - pre_r[r2 + 1][c1] + pre_r[r1][c1]) / count);
                image(j, i, 1) = (int)((pre_g[r2 + 1][c2 + 1] - pre_g[r1][c2 + 1] - pre_g[r2 + 1][c1] + pre_g[r1][c1]) / count);
                image(j, i, 2) = (int)((pre_b[r2 + 1][c2 + 1] - pre_b[r1][c2 + 1] - pre_b[r2 + 1][c1] + pre_b[r1][c1]) / count);
            }
        }
    }
}
inline void yellow(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = min(255, (int)(image(i, j, 0) * 1.2));
            image(i, j, 1) = min(255, (int)(image(i, j, 1) * 1.2));
            image(i, j, 2) *= 0.8;
        }
    }
}
inline void oil_paint(Image &image, int level)
{
    Image oil_image = image;

    const int color_bins = 20;

    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width; j++)
        {
            int inten_count[color_bins] = {0};
            int avg_r[color_bins] = {0};
            int avg_g[color_bins] = {0};
            int avg_b[color_bins] = {0};
            int max_inten_count = 0;
            int max_idx = 0;

            int x_start = max(0, i - level);
            int x_end = min(image.height - 1, i + level);
            int y_start = max(0, j - level);
            int y_end = min(image.width - 1, j + level);

            for (int x = x_start; x <= x_end; x++)
            {
                for (int y = y_start; y <= y_end; y++)
                {
                    int r = image(y, x, 0);
                    int g = image(y, x, 1);
                    int b = image(y, x, 2);

                    double avg = (r + g + b) / 3.0;
                    int cur_inten = min(color_bins - 1, (int)(avg * color_bins / 255));
                    inten_count[cur_inten]++;
                    avg_r[cur_inten] += r;
                    avg_g[cur_inten] += g;
                    avg_b[cur_inten] += b;

                    if (inten_count[cur_inten] >= max_inten_count)
                    {
                        max_inten_count = inten_count[cur_inten];
                        max_idx = cur_inten;
                    }
                }
            }

            if (max_inten_count > 0)
            {
                oil_image(j, i, 0) = avg_r[max_idx] / max_inten_count;
                oil_image(j, i, 1) = avg_g[max_idx] / max_inten_count;
                oil_image(j, i, 2) = avg_b[max_idx] / max_inten_count;
            }
        }
    }
    image = oil_image;
}
inline void retro(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 3; j < image.height; j += 4)
        {
            for (int k = 0; k <= 2; k++)
            {
                image(i, j, k) /= 2;
                image(i, j - 1, k) /= 2;
                image(i, j - 2, k) = min(255, (int)(1.3 * image(i, j - 2, k)));
                image(i, j - 3, k) = min(255, (int)(1.3 * image(i, j - 3, k)));
            }
        }
    }
}
inline void purple(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 1) *= 0.8;
            image(i, j, 0) = min(255, (int)(image(i, j, 0) * 1.25));
            image(i, j, 2) = min(255, (int)(image(i, j, 2) * 1.25));
        }
    }
}
inline void infrared(Image &image)
{
    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = 255;
            image(i, j, 1) = 255 - image(i, j, 1);
            image(i, j, 2) = 255 - image(i, j, 2);
        }
    }
}
inline void skew(Image &image, int angle)
{
    if (angle == 0)
        return;

    double rad = angle * 3.1415 / 180;

    Image skewed_image(image.width + round((image.height * tan((rad >= 0) ? rad : -rad))), image.height);

    for (int i = 0; i < skewed_image.width; i++)
    {
        for (int j = 0; j < skewed_image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                skewed_image(i, j, k) = 0;
            }
        }
    }

    if (rad < 0)
    {
        for (int i = 0; i < image.height; i++)
        {
            for (int j = 0; j < image.width; j++)
            {
                int y = round(j + i * tan(-rad));
                int x = i;

                for (int k = 0; k <= 2; k++)
                {
                    skewed_image(y, x, k) = image(j, i, k);
                }
            }
        }
    }
    else if (rad > 0)
    {
        for (int i = image.height - 1; i >= 0; i--)
        {
            for (int j = 0; j < image.width; j++)
            {
                int y = round(j + (image.height - 1 - i) * tan(rad));
                int x = i;

                for (int k = 0; k <= 2; k++)
                {
                    skewed_image(y, x, k) = image(j, i, k);
                }
            }
        }
    }
    image = skewed_image;
}
inline void emboss(Image &image)
{
    int kernel[3][3] = {{-2, -1, 0},
                        {-1, 0, 1},
                        {0, 1, 2}};

    Image emboss_image(image.width, image.height);

    for (int i = 1; i < image.width - 1; ++i)
    {
        for (int j = 1; j < image.height - 1; ++j)
        {
            for (int k = 0; k <= 2; ++k)
            {
                int sum = 0;

                for (int r = 0; r <= 2; ++r)
                {
                    for (int c = 0; c <= 2; ++c)
                        sum += image(i + r - 1, j + c - 1, k) * kernel[r][c];

                    sum += 128;
                }
                emboss_image(i, j, k) = clamp(sum / 3, 0, 255);
            }
        }
    }
    image = emboss_image;
}
inline void sepia(Image &image)
{
    gray_scale(image);

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            image(i, j, 0) = min((int)(image(i, j, 0) * 1.1), 255);
            image(i, j, 1) = image(i, j, 1) * 0.7;
            image(i, j, 2) = image(i, j, 2) * 0.4;
        }
    }
}
inline void fire(Image &image)
{
    Image fire_image(image.width, image.height);

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int glow = 0.299 * image(i, j, 0) + 0.587 * image(i, j, 1) + 0.114 * image(i, j, 2);

            if (glow < 50)
            {
                fire_image(i, j, 0) = 0;
                fire_image(i, j, 1) = 0;
                fire_image(i, j, 2) = 0;
            }
            else if (glow < 100)
            {
                fire_image(i, j, 0) = 128;
                fire_image(i, j, 1) = 0;
                fire_image(i, j, 2) = 0;
            }
            else if (glow < 150)
            {
                fire_image(i, j, 0) = 255;
                fire_image(i, j, 1) = 64;
                fire_image(i, j, 2) = 0;
            }
            else if (glow < 200)
            {
                fire_image(i, j, 0) = 255;
                fire_image(i, j, 1) = 128;
                fire_image(i, j, 2) = 0;
            }
            else
            {
                fire_image(i, j, 0) = 255;
                fire_image(i, j, 1) = 255;
                fire_image(i, j, 2) = 128;
            }
        }
    }
    image = fire_image;
}
inline void neon(Image &image, int choice)
{
    blur(image, 1);
    Image neon_image = image;

    if (choice == 1)
    {
        int kernel[3][3] = {{-2, -1, 0},
                            {-1, 0, 1},
                            {0, 1, 2}};

        int sum = 0;

        for (int i = 1; i < image.width - 1; ++i)
        {
            for (int j = 1; j < image.height - 1; ++j)
            {
                for (int k = 0; k <= 2; ++k)
                {
                    sum = 0;
                    for (int r = 0; r <= 2; ++r) // Kernel Loops
                    {
                        for (int c = 0; c <= 2; ++c)
                            sum += image(i + c - 1, j + r - 1, k) * kernel[r][c];
                    }
                    neon_image(i, j, k) = max(0, min(255, sum));
                }
            }
        }
    }
    else if (choice == 2) //(CMYG Coloring)
    {
        int Kx[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}}; // Sobel X Kernel

        int Ky[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}}; // Sobel Y Kernel

        for (int i = 1; i < image.width - 1; ++i)
        {
            for (int j = 1; j < image.height - 1; ++j)
            {
                double Gx = 0;
                double Gy = 0;

                for (int k = 0; k < 3; ++k)
                {
                    for (int r = 0; r <= 2; ++r) // Kernel loop
                    {
                        for (int c = 0; c <= 2; ++c)
                        {
                            Gx += image(i + c - 1, j + r - 1, k) * Ky[r][c];
                            Gy += image(i + c - 1, j + r - 1, k) * Kx[r][c];
                        }
                    }
                }

                Gx /= 3;
                Gy /= 3;
                int magnitude = min(255, (int)sqrt(Gx * Gx + Gy * Gy));

                if (abs(Gx) > abs(Gy)) // Colors by the rate of change
                {
                    if (Gx > 0)
                    {
                        neon_image(i, j, 0) = magnitude;
                        neon_image(i, j, 1) = 0;
                        neon_image(i, j, 2) = magnitude;
                    }
                    else
                    {
                        neon_image(i, j, 0) = 0;
                        neon_image(i, j, 1) = magnitude;
                        neon_image(i, j, 2) = magnitude;
                    }
                }
                else if (abs(Gy) > 0)
                {
                    if (Gy > 0)
                    {
                        neon_image(i, j, 0) = magnitude;
                        neon_image(i, j, 1) = magnitude;
                        neon_image(i, j, 2) = 0;
                    }
                    else
                    {
                        neon_image(i, j, 0) = 0;
                        neon_image(i, j, 1) = magnitude;
                        neon_image(i, j, 2) = 0;
                    }
                }
                else
                    neon_image(i, j, 0) = neon_image(i, j, 1) = neon_image(i, j, 2) = 0; // if it isn't an edge at all
            }
        }
    }
    else if (choice == 3)
    {
        int Kx[3][3] = {{-1, 0, 1},
                        {-2, 0, 2},
                        {-1, 0, 1}};

        int Ky[3][3] = {{-1, -2, -1},
                        {0, 0, 0},
                        {1, 2, 1}};

        for (int i = 1; i < image.width - 1; ++i)
        {
            for (int j = 1; j < image.height - 1; ++j)
            {
                double Gx = 0;
                double Gy = 0;
                for (int k = 0; k < 3; ++k)
                {
                    for (int r = 0; r <= 2; ++r)
                    {
                        for (int c = 0; c <= 2; ++c)
                        {
                            Gx += image(i + c - 1, j + r - 1, k) * Kx[r][c];
                            Gy += image(i + c - 1, j + r - 1, k) * Ky[r][c];
                        }
                    }
                }

                Gx /= 3;
                Gy /= 3;
                int magnitude = 2 * (int)sqrt(Gx * Gx + Gy * Gy);

                if (magnitude < 50)
                {
                    neon_image(i, j, 0) = neon_image(i, j, 1) = neon_image(i, j, 2) = 0;
                    continue;
                }

                neon_image(i, j, 0) = min(255, (int)(abs(Gy) * 1.5));
                neon_image(i, j, 1) = min(255, (int)(abs(Gx) * 1.5));
                neon_image(i, j, 2) = 200; // make the image more bluish
            }
        }
    }
    image = neon_image;
}
inline void custom_merge_with_neon(Image &image, Image &edge, double edges_ratio, double original_ratio)
{
    Image merged_image(image.width, image.height);

    edges_ratio /= 100;
    original_ratio /= 100;

    for (int i = 0; i < image.width; ++i)
    {
        for (int j = 0; j < image.height; ++j)
        {
            int sum = edge(i, j, 0) + edge(i, j, 1) + edge(i, j, 2);

            if (sum > 400)
            {
                merged_image(i, j, 0) = edges_ratio * edge(i, j, 0) + (1 - edges_ratio) * image(i, j, 0);
                merged_image(i, j, 1) = edges_ratio * edge(i, j, 1) + (1 - edges_ratio) * image(i, j, 1);
                merged_image(i, j, 2) = edges_ratio * edge(i, j, 2) + (1 - edges_ratio) * image(i, j, 2);
            }
            else
            {
                merged_image(i, j, 0) = original_ratio * image(i, j, 0) + (1 - original_ratio) * edge(i, j, 0);
                merged_image(i, j, 1) = original_ratio * image(i, j, 1) + (1 - original_ratio) * edge(i, j, 1);
                merged_image(i, j, 2) = original_ratio * image(i, j, 2) + (1 - original_ratio) * edge(i, j, 2);
            }
        }
    }
    edge = merged_image;
}
inline void glowing_edges(Image &image, int choice, double edges_ratio, double original_ratio)
{
    Image edge = image;

    if (choice == 1)
    {
        neon(edge, 2);
        brightness(edge, 225);
        custom_merge_with_neon(image, edge, edges_ratio, original_ratio); // Order is very important
    }
    else if (choice == 2)
    {
        neon(edge, 3);
        brightness(edge, 225);
        custom_merge_with_neon(image, edge, edges_ratio, original_ratio); // Order is very important
    }
    image = edge;
}
inline void sharpness(Image &image, int strength)
{
    if (strength == 0)
        return;

    strength /= 5;

    Image blurred_image = image;
    Image result_image = image;
    blur(blurred_image, 1);

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            for (int k = 0; k <= 2; k++)
            {
                int sharpened_pixel = image(i, j, k) + strength * (image(i, j, k) - blurred_image(i, j, k));
                sharpened_pixel = clamp(sharpened_pixel, 0, 255);
                result_image(i, j, k) = sharpened_pixel;
            }
        }
    }
    image = result_image;
}
inline void noise(Image &image, float noise_factor)
{
    if (noise_factor == 0)
        return;

    noise_factor /= 7.7;

    uint32_t state = 123456789; // seed once

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            int random_value = fastRand255(state) / (13.001f - noise_factor);

            for (int k = 0; k <= 2; k++)
            {
                image(i, j, k) = min(255, image(i, j, k) + random_value);
            }
        }
    }
}
inline void swirl(Image &image, int swirl_factor)
{
    if (swirl_factor == 0)
        return;

    Image swirl_image(image.width, image.height);

    int radius = min(image.width, image.height);
    int mid_x = ceil(image.width / 2);
    int mid_y = ceil(image.height / 2);

    for (int i = 0; i < image.width; i++)
    {
        for (int j = 0; j < image.height; j++)
        {
            double angle = atan2(j - mid_y, i - mid_x);
            double p = sqrt((j - mid_y) * (j - mid_y) + (i - mid_x) * (i - mid_x));
            double r = log(2) * radius / 5;
            double new_angle = swirl_factor * exp(-p / r) + angle;
            double new_x = mid_x + p * cos(new_angle);
            double new_y = mid_y + p * sin(new_angle);

            if (new_x >= 0 && new_x < image.width && new_y >= 0 && new_y < image.height)
            {
                swirl_image(i, j, 0) = image(new_x, new_y, 0);
                swirl_image(i, j, 1) = image(new_x, new_y, 1);
                swirl_image(i, j, 2) = image(new_x, new_y, 2);
            }
            else
            {
                swirl_image(i, j, 0) = 0;
                swirl_image(i, j, 1) = 0;
                swirl_image(i, j, 2) = 0;
            }
        }
    }
    image = swirl_image;
}
