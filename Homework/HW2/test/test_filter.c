/**
 * @file    test_filter.c
 * @author  cy023 (cyyang@g.ncu.edu.tw)
 * @date    2021.12.01
 * @brief   Digital Image Processing Fall2021 @ National Central University.
 *          Homework 2
 */
#include "../lib/bmp.h"
#include "../lib/fft.h"
#include "../lib/filter.h"

int main()
{
    uint64_t i, j;
    uint64_t pixelSize;
    BMP_t bitmap;
    uint8_t *origin_img;

    /**************************************************************************/
    /*                          Read bitmap file.                             */
    /**************************************************************************/
    FILE *fd;
    fd = fopen("./Image/DIPpic1_gray.bmp", "rb");
    fread(&bitmap, sizeof(uint8_t), HEADER_SIZE, fd);

    pixelSize = bitmap.info_header.width * bitmap.info_header.height;
    bitmap.info_header.data_size = pixelSize * bitmap.info_header.bits_per_pixel / 8;
    bitmap.data = (uint8_t *)calloc(bitmap.info_header.data_size, sizeof(uint8_t));
    origin_img  = (uint8_t *)calloc(bitmap.info_header.data_size, sizeof(uint8_t));

    fread(bitmap.palette, sizeof(uint8_t), PALETTE_SIZE, fd);
    fread(bitmap.data, sizeof(uint8_t), bitmap.info_header.data_size, fd);
    memcpy(origin_img, bitmap.data, bitmap.info_header.data_size);
    fclose(fd);

    /**************************************************************************/
    /*                               FFT                                      */
    /**************************************************************************/
    Complex_t *freq = (Complex_t *)calloc(bitmap.info_header.data_size, sizeof(Complex_t));
    Image_t it;
    it.data = bitmap.data;
    it.freq = freq;
    it.height = bitmap.info_header.height;
    it.width  = bitmap.info_header.width;

    imgfft2(&it);
    saveBmpFile(&bitmap, "./Image/_filter_fft.bmp");

    /**************************************************************************/
    /*                            Low Pass Filter                             */
    /**************************************************************************/
    lowpass(&it, 150);
    saveBmpFile(&bitmap, "./Image/_filter_lp150.bmp");

    imgifft2(&it);
    saveBmpFile(&bitmap, "./Image/_filter_lp150_ifft.bmp");

    // highpass(&it, 50);
    // saveBmpFile(&bitmap, "./Image/_filter_hp150.bmp");

    // imgifft2(&it);
    // saveBmpFile(&bitmap, "./Image/_filter_hp150_ifft.bmp");




    printf("mse  = %lf\n",  mse(origin_img, bitmap.data, bitmap.info_header.data_size));
    printf("psnr = %lf dB\n", psnr(origin_img, bitmap.data, bitmap.info_header.data_size));

    free(freq);
    free(bitmap.data);
    return 0;
}
