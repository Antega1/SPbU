#include <iostream>
#include <fstream>
#include <cmath>
#include <string>


struct BMP
{
    int width;
    int height;
    unsigned char header[54];
    unsigned char *pixels;
    int size;
    int row_padded;
    long long int size_padded;
};

BMP readBMP(const std::string &filename)
{
    BMP image;
    FILE *in = fopen(filename.c_str(), "rb");
    if (in == nullptr)
    {
        throw "Error: unable to open file.";
    }
    fread(image.header, sizeof(unsigned char), 54, in);
    image.width = *(int *)&image.header[18];
    image.height = *(int *)&image.header[22];
    image.row_padded = (image.width * 3 + 3) & (~3);
    image.size_padded = image.row_padded * image.height;
    image.pixels = new unsigned char[image.size_padded];
    if (fread(image.pixels, sizeof(unsigned char), image.size_padded, in) != image.size_padded)
    {
        std::cout << "Error: all bytes couldn't be read" << std::endl;
    }
    fclose(in);
    return image;
}

void writeBMP(const std::string &filename, const BMP &image)
{
    FILE *out = fopen(filename.c_str(), "wb");
    fwrite(image.header, sizeof(unsigned char), 54, out);
    unsigned char tmp;
    for (int i = 0; i < image.height; i++)
    {
        for (int j = 0; j < image.width * 3; j += 3)
        {
            tmp = image.pixels[j];
            image.pixels[j] = image.pixels[j + 2];
            image.pixels[j + 2] = tmp;
        }
    }
    fwrite(image.pixels, sizeof(unsigned char), image.size_padded, out);
    fclose(out);
}

BMP resizeImage(const BMP &image, double scale)
{
    BMP newImage;
    newImage.width = static_cast<int>(image.width * scale);
    newImage.height = static_cast<int>(image.height * scale);

    cv::Mat inputImage(image.height, image.width, CV_8UC3, image.pixels);
    cv::Mat outputImage;
    cv::resize(inputImage, outputImage, cv::Size(newImage.width, newImage.height));

    newImage.pixels = new unsigned char[newImage.size_padded];
    newImage.row_padded = (newImage.width * 3 + 3) & (~3);
    newImage.size_padded = newImage.row_padded * newImage.height;

    std::memcpy(newImage.pixels, outputImage.data, newImage.size_padded);

    return newImage;
}

BMP rotateImage(const BMP &image, double angle)
{
    BMP newImage = image;
    unsigned char *pixels = new unsigned char[newImage.size_padded];

    double radians = (angle * M_PI) / 180;
    int sinf = (int)sin(radians);
    int cosf = (int)cos(radians);

    for (int y = 0; y < newImage.height; y++)
    {
        for (int x = 0; x < newImage.width; x++)
        {
            int xx = (int)((x - newImage.width / 2) * cosf + (y - newImage.height / 2) * sinf + newImage.width / 2);
            int yy = (int)(-(x - newImage.width / 2) * sinf + (y - newImage.height / 2) * cosf + newImage.height / 2);

            if (xx >= 0 && xx < newImage.width && yy >= 0 && yy < newImage.height)
            {
                for (int c = 0; c < 3; c++)
                {
                    pixels[(y * newImage.row_padded + x * 3) + c] = newImage.pixels[(yy * newImage.row_padded + xx * 3) + c];
                }
            }
        }
    }

    newImage.pixels = pixels;
    return newImage;
}

BMP gaussBlur(const BMP &image, float sigma)
{
    BMP newImage = image;
    unsigned char *pixels = new unsigned char[newImage.size_padded];

    newImage.pixels = pixels;
    return newImage;
}

int main()
{
    BMP image = readBMP("/media/andrew/Sklad/Study/programm/bmp_24.bmp");

    // Уменьшение размера изображения
    image = resizeImage(image, 0.5); // Уменьшить изображение вдвое

    // Поворот на 90 градусов
    image = rotateImage(image, 90);

    // Поворот на -90 градусов
    image = rotateImage(image, -90);

    // Размытие Гаусса
    image = gaussBlur(image, 3.0);

    writeBMP("output.bmp", image);

    // Освобождение памяти
    delete[] image.pixels;

    return 0;
}