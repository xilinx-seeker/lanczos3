#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif
// Old Image dimensions
#define OLD_WIDTH 640
#define OLD_HEIGHT 480

// New Image dimensions (Upsampling by 2)
#define NEW_WIDTH 1280
#define NEW_HEIGHT 960

// Lanczos-3 kernel calculations
// Lanczos-3 kernel is sinc(x) * sinc(x/3)
// sinc(x) = sin (pi * x )/(pi * x)
// while sinc (x/3) = sin (pi *x/3)/(pi * x/3)
// so we have to multiply both sinc and the result is below in return statement of lanczos3 function 
float lanczos3(float x) {
    if (x == 0.0) {
        return 1.0;
    }
    if (x > -3.0 && x < 3.0) {
        return (3.0 * sin(x * M_PI) * sin(x * M_PI / 3.0)) / (x * x * M_PI * M_PI);
    }
    return 0.0;
}


void imageRandomMatrix_GreyScale(unsigned char old_img[480][640], int width, int height) {
    for (int k = 0; k < height; k++) {
        for (int l = 0; l < width; l++) {
            // Generate a random value between 0 and 255
            old_img[k][l] = rand() % 256;
            // printf ("image array is for K=%d and L=%d value= %d\n", k,l,old_img[k][l]);
        }
    }
}



// Upsample the image using Lanczos-3 algorithm
void lanczos3_upscale(const unsigned char old_img[OLD_HEIGHT][OLD_WIDTH], unsigned char new_img[NEW_HEIGHT][NEW_WIDTH]) {
    for (int y = 0; y < NEW_HEIGHT; y++) {
        for (int x = 0; x < NEW_WIDTH; x++) {
            float upscaledX = (float)x * (OLD_WIDTH - 1) / (NEW_WIDTH - 1); // upscaledX value will never exceed OLD_WIDTH value
            float upscaledY = (float)y * (OLD_HEIGHT - 1) / (NEW_HEIGHT - 1); // upscaledY value will never exceed OLD_HEIGHT value
            float sum = 0.0;
            float weightSum = 0.0;

            for (int j = -2; j <= 2; j++) {   // here the range is from -2 to 2 which constitute 5x5 kernel mask. For 3x3 kernel we can set the range from -1 to 1 and so on
                for (int i = -2; i <= 2; i++) {
                    int oldX = (int)upscaledX + i; //to calculate coloumn index of original image that will contribute in upscaling
                    int oldY = (int)upscaledY + j; //to calculate row index of original image that will contribute in upscaling

                    if (oldX >= 0 && oldX < OLD_WIDTH && oldY >= 0 && oldY < OLD_HEIGHT) {
                        float weight = lanczos3(upscaledX - oldX) * lanczos3(upscaledY - oldY);
                        sum += old_img[oldY][oldX] * weight;
                        weightSum += weight;
                    }
                }
            }

            new_img[y][x] = (unsigned char)(sum / weightSum); //to calculate the weighted average of the neighbouhood pixels
            // if (y > 480 && x > 640) {
            // printf ("the new image pixel value is %d and y is %d while x is %d\n", new_img[y][x], y, x);}
        }
    }
}

int main() {
    unsigned char old_img[OLD_HEIGHT][OLD_WIDTH];  // Static memory for old image *unsigned char is used to accomodate the 8 bit value, and greyscale image is presumed here for simplicity
    unsigned char new_img[NEW_HEIGHT][NEW_WIDTH];  // Static memory for new image

    // loading the old_img 2-D array with random values from 0 to 255 (grey scale)
    int width = OLD_WIDTH;
    int height = OLD_HEIGHT;
    imageRandomMatrix_GreyScale(old_img, width, height);
    // Perform the Lanczos-3 upscaling
    lanczos3_upscale(old_img, new_img);

    // Save the new_img array as the new image
     
    return 0;
}
