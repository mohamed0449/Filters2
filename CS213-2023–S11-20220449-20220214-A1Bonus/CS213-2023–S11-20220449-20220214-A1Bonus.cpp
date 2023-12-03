// Mohamed Gomaa 20220449 mhmd2004221@gmail.com

#include <bits/stdc++.h>
#define fudou cin.tie(0),cout.tie(0),cin.sync_with_stdio(0),cout.sync_with_stdio(0);
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#define E '\n'

using namespace std;
const int SIZE = 256;
const int RGB = 3;
const int IMAGE_SIZE = 256;
unsigned char imgRGB[SIZE][SIZE][RGB];
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;

//#define BMP_BIG_ENDIAN
#define BYTE_SWAP(num) (((num>>24)&0xff) | ((num<<8)&&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000))

typedef struct {
    uint8    bfType1;
    uint8    bfType2;
    uint32   bfSize;
    uint16    bfReserved1;
    uint16    bfReserved2;
    uint32   bfOffBits;
    uint32   biSize;          // size of structure, in bytes
    uint32   biWidth;         // bitmap width, in pixels
    uint32   biHeight;        // bitmap height, in pixels
    uint16    biPlanes;        // see below
    uint16    biBitCount;      // see below
    uint32   biCompression;   // see below
    uint32   biSizeImage;     // see below
    uint32   biXPelsPerMeter; // see below
    uint32   biYPelsPerMeter; // see below
    uint32   biClrUsed;       // see below
    uint32   biClrImportant;  // see below
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct {
    uint8 rgbBlue;
    uint8 rgbGreen;
    uint8 rgbRed;
} RGBTRIPLE;
void write_hdr(uint8 *hdr, int *hdr_idx, uint32 data, uint32 size);
uint8 tempImage[SIZE][SIZE][RGB];
uint8 tempGSImage[SIZE][SIZE];
int readRGBBMP(const char* filename, unsigned char inputImage[][SIZE][RGB])
{
    uint8 type[2];
    int headersize = 0;
    int i,j,k;
    //BITMAPFILEHEADER bfh;
    /* file pointer */
    FILE *file;
    /* read input bmp into the data matrix */
    if (!(file=fopen(filename,"rb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        return(1);
    }
    fread(type, sizeof(unsigned char), 0x2, file);
    if(type[0] != 'B' and type[1] != 'M'){
        cout << "Not a BMP file" << endl;
        return(1);
    }
    fseek(file, 8, SEEK_CUR);
    fread(&headersize, sizeof(uint32), 1, file);
#ifdef BMP_BIG_ENDIAN
    headersize = BYTE_SWAP(headersize);
#endif
    //cout << "Header size is " << headersize << endl;
    fseek(file, headersize, SEEK_SET);
    fread(tempImage, sizeof(uint8), SIZE*SIZE*RGB, file);
    // cout << (int)tempImage[0][0][0] << "," << (int)tempImage[0][0][1] << "," << (int)tempImage[0][0][2] << endl;
    fclose(file);
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            for(k=0; k < RGB; k++){
                inputImage[SIZE-1-i][j][RGB-1-k] = tempImage[i][j][k];
                //cerr << tempImage[i][j][k] << ",";
            }
        }
        //cerr << endl;
    }
    return 0;
}

int writeRGBBMP(const char* filename, unsigned char outputImage[][SIZE][RGB])
{
    uint8 hdr[54];
    int hdr_idx = 0;
    int i,j,k;
    BITMAPFILEHEADER bfh;
    // file pointer
    FILE *file;
    bfh.bfType1 = 'B';
    bfh.bfType2 = 'M';
    bfh.bfSize = 0x36;
    bfh.bfReserved1 = 0x0;
    bfh.bfReserved2 = 0x0;
    bfh.bfOffBits = 0x36;
    bfh.biSize = 0x28;
    bfh.biWidth = SIZE;
    bfh.biHeight = SIZE;
    bfh.biPlanes = 1;
    bfh.biBitCount = 24;
    bfh.biCompression  = 0;
    bfh.biSizeImage = sizeof(RGBTRIPLE)*SIZE*SIZE;
    bfh.biXPelsPerMeter = 2400;
    bfh.biYPelsPerMeter = 2400;
    bfh.biClrUsed = 0;
    bfh.biClrImportant = 0;
    write_hdr(hdr, &hdr_idx, bfh.bfType1, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfType2, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved1, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved2, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfOffBits, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biWidth, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biHeight, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biPlanes, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biBitCount, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biCompression, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSizeImage, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biXPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biYPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrUsed, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrImportant, sizeof(uint32));
    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            for(k=0; k < RGB; k++){
                tempImage[SIZE-1-i][j][RGB-1-k] = outputImage[i][j][k];
            }
        }
    }
    // write result bmp file
    if (!(file=fopen(filename,"wb")))
    {
        cout << "Cannot open file: " << filename << endl;
        return(1);
    }
    fwrite(&hdr, sizeof(unsigned char), 0x36, file);
    fwrite(tempImage, sizeof(unsigned char), SIZE*SIZE*RGB, file);
    fclose(file);

    return 0;
}

void write_hdr(uint8 *hdr, int *hdr_idx, uint32 data, uint32 size)
{
    if(size == 1){
        hdr[*hdr_idx] = (uint8) data;
        (*hdr_idx) += 1;
    }
    else if(size == 2){
        hdr[*hdr_idx] = (uint8) (data & 0x00ff);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0xff00) >> 8);
        (*hdr_idx) += 1;
    }
    else if(size == 4){
        hdr[*hdr_idx] = (uint8) (data & 0x000000ff);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0x0000ff00) >> 8);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0x00ff0000) >> 16);
        (*hdr_idx) += 1;
        hdr[*hdr_idx] = (uint8) ((data & 0xff000000) >> 24);
        (*hdr_idx) += 1;
    }
    else {
        printf("Illegal size in write_hdr...consult your instructor\n");
    }
}

int readGSBMP(const char* filename, unsigned char inputImage[][SIZE])
{
    uint8 type[2];
    int headersize = 0;
    int i,j;
    //BITMAPFILEHEADER bfh;
    /* file pointer */
    FILE *file;
    /* read input bmp into the data matrix */
    if (!(file=fopen(filename,"rb")))
    {
        cout << "Cannot open file: " << filename <<endl;
        return(1);
    }
    fread(type, sizeof(unsigned char), 0x2, file);
    if(type[0] != 'B' and type[1] != 'M'){
        cout << "Not a BMP file" << endl;
        return(1);
    }
    fseek(file, 8, SEEK_CUR);
    fread(&headersize, sizeof(uint32), 1, file);
#ifdef BMP_BIG_ENDIAN
    headersize = BYTE_SWAP(headersize);
#endif
    //cout << "Header size is " << headersize << endl;
    fseek(file, headersize, SEEK_SET);
    fread(tempGSImage, sizeof(uint8), SIZE*SIZE, file);
    //  cout << (int)tempGSImage[0][0][0] << "," << (int)tempGSImage[0][0][1] << "," << (int)tempGSImage[0][0][2] << endl;
    fclose(file);

    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            inputImage[SIZE-1-i][j] = tempGSImage[i][j];
            //	inputImage[SIZE-1-i][SIZE-1-j] = tempGSImage[i][j];
            //cerr << tempGSImage[i][j][k] << ",";
        }
        //cerr << endl;
    }
    return 0;
}

int writeGSBMP(const char* filename, unsigned char outputImage[][SIZE])
{
    uint8 hdr[54];
    int hdr_idx = 0;
    int i,j;
    BITMAPFILEHEADER bfh;
    // file pointer
    FILE *file;
    bfh.bfType1 = 'B';
    bfh.bfType2 = 'M';
    // 0x10436 = 2^16 + 1024 for color def + 0x36 for header
    bfh.bfSize = 0x010436; //0x36;
    bfh.bfReserved1 = 0x0;
    bfh.bfReserved2 = 0x0;
    // 0x0436
    bfh.bfOffBits = 0x436; //0x36;
    bfh.biSize = 0x28;
    bfh.biWidth = SIZE;
    bfh.biHeight = SIZE;
    bfh.biPlanes = 1;
    bfh.biBitCount = 8;
    bfh.biCompression  = 0;
    bfh.biSizeImage = SIZE*SIZE;
    bfh.biXPelsPerMeter = 0; //2400;
    bfh.biYPelsPerMeter = 0; //2400;
    bfh.biClrUsed = SIZE; // 0;
    bfh.biClrImportant = SIZE; // 0;
    write_hdr(hdr, &hdr_idx, bfh.bfType1, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfType2, sizeof(uint8));
    write_hdr(hdr, &hdr_idx, bfh.bfSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved1, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfReserved2, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.bfOffBits, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSize, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biWidth, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biHeight, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biPlanes, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biBitCount, sizeof(uint16));
    write_hdr(hdr, &hdr_idx, bfh.biCompression, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biSizeImage, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biXPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biYPelsPerMeter, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrUsed, sizeof(uint32));
    write_hdr(hdr, &hdr_idx, bfh.biClrImportant, sizeof(uint32));


    for(i=0; i < SIZE; i++){
        for(j=0; j < SIZE; j++){
            //tempGSImage[SIZE-1-i][SIZE-1-j] = outputImage[i][j];
            tempGSImage[SIZE-1-i][j] = outputImage[i][j];
        }
    }

    // write result bmp file
    if (!(file=fopen(filename,"wb")))
    {
        cout << "Cannot open file: " << filename << endl;
        return(1);
    }
    uint8 z = 0;
    fwrite(&hdr, sizeof(unsigned char), 0x36, file);
    for(i=0; i < SIZE; i++){
        uint8 x = (uint8) i;
        //cout << (int)x << endl;
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&x, sizeof(uint8), 0x01, file);
        fwrite(&z, sizeof(uint8), 0x01, file);

    }
    fwrite(tempGSImage, sizeof(unsigned char), SIZE*SIZE, file);
    fclose(file);


    return 0;
}

int shows = 0;
void show() {
    string command = "\\tmp\\bmplib.bmp";
    char cwd[PATH_MAX];
    system(strcat(getcwd(cwd, sizeof(cwd)), command.c_str()));
    usleep(200000);
    shows++;
}

void showRGBBMP(unsigned char inputImage[][SIZE][RGB]) {
    writeRGBBMP("./tmp/bmplib.bmp", inputImage);
    show();
}

void showGSBMP(unsigned char inputImage[][SIZE]) {
    writeGSBMP("./tmp/bmplib.bmp", inputImage);
    show();
}

//This is our functions:
int displayFilterMenu(){
    int choose;
    cout << "Choose from these filters:" <<E;
    cout << "1. Filter 1: Black and White Image" << E;
    cout << "2. Filter 2: Invert Image" << E;
    cout << "3. Filter 3: Merge Images" << E;
    cout << "4. Filter 4: Flip" << E;
    cout << "5. Filter 5: Rotate Image" << E;
    cout << "6. Filter 6: Darken and Lighten Image" << E;
    cout << "7. Filter 7: Detect Image Edges" << E;
    cout << "8. Filter 8: Enlarge Image" << E;
    cout << "9. Filter 9: Shrink Image" << E;
    cout << "10. Filter 10: Mirror Image" << E;
    cout << "11. Filter 11: Shuffle Image" << E;
    cout << "12. Filter 12: Blur Image" << E;
    cout << "13. Filter 13: Crop Image" << E;
    cout << "14. Filter 14: Skew Horizontally / Vertically" << E;
    cout << "Enter your choice (1-14): "<<E;
    cout.flush();
    cin >> choose;
    return choose;
}

void BlackAndWhiteFilter(unsigned char (*inputImage)[256][3]) {
    int sumR = 0;
    int sumG = 0;
    int sumB = 0;
    int numPixels = SIZE * SIZE;
    // Calculate the average gray level for each channel (R, G, B)
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sumR += inputImage[i][j][0]; // Red channel
            sumG += inputImage[i][j][1]; // Green channel
            sumB += inputImage[i][j][2]; // Blue channel
        }
    }
    int averageGrayLevelR = sumR / numPixels;
    int averageGrayLevelG = sumG / numPixels;
    int averageGrayLevelB = sumB / numPixels;
    // Threshold the image for each channel separately
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            inputImage[i][j][0] = (inputImage[i][j][0] > averageGrayLevelR) ? 255 : 0; // Red channel
            inputImage[i][j][1] = (inputImage[i][j][1] > averageGrayLevelG) ? 255 : 0; // Green channel
            inputImage[i][j][2] = (inputImage[i][j][2] > averageGrayLevelB) ? 255 : 0; // Blue channel
        }
    }
}

void InvertFilter(unsigned char inputImage[][SIZE][RGB]) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            // Invert each color channel (R, G, B) separately
            inputImage[i][j][0] = 255 - inputImage[i][j][0]; // Red channel
            inputImage[i][j][1] = 255 - inputImage[i][j][1]; // Green channel
            inputImage[i][j][2] = 255 - inputImage[i][j][2]; // Blue channel
        }
    }
}

void merge(unsigned char inputImage1[][SIZE][RGB], unsigned char inputImage2[][SIZE][RGB], unsigned char outputImage[][SIZE][RGB]) {
    // Loop through each pixel in the images
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            // Merge the color channels (R, G, B) separately
            for (int k = 0; k < RGB; k++) {
                outputImage[i][j][k] = (inputImage1[i][j][k] + inputImage2[i][j][k]) / 2;
            }
        }
    }
}

void flipHorizontal(unsigned char inputImage[][SIZE][RGB], unsigned char outputImage[][SIZE][RGB]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < RGB; k++) {
                outputImage[i][j][k] = inputImage[i][width - 1 - j][k];
            }
        }
    }
}

void flipVertical(unsigned char inputImage[][SIZE][RGB], unsigned char outputImage[][SIZE][RGB]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < RGB; k++) {
                outputImage[i][j][k] = inputImage[height - 1 - i][j][k];
            }
        }
    }
}

void rotate90(unsigned char (*inputImage)[256][3], unsigned char (*outputImage)[256][3]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                outputImage[j][height - 1 - i][k] = inputImage[i][j][k];
            }
        }
    }
}

void rotate180(unsigned char (*inputImage)[256][3], unsigned char (*outputImage)[256][3]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                outputImage[height - 1 - i][width - 1 - j][k] = inputImage[i][j][k];
            }
        }
    }
}

void rotate270(unsigned char (*inputImage)[256][3], unsigned char (*outputImage)[256][3]) {
    int height = SIZE;
    int width = SIZE;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; k++) {
                outputImage[width - 1 - j][i][k] = inputImage[i][j][k];
            }
        }
    }
}

void DarkenandLightenImage(unsigned char (*inputImage)[256][RGB], unsigned char (*outputImage)[256][RGB], float factor) {
    int height = SIZE;
    int width = SIZE;
    // Ensure the factor is between -1.0 and 1.0
    if (factor < -1.0) {
        factor = -1.0;
    } else if (factor > 1.0) {
        factor = 0.5;
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < RGB; k++) {
                // Adjust the brightness of each color channel
                int newValue = inputImage[i][j][k] + factor * 255;
                // Ensure the new value is within the valid range [0, 255]
                if (newValue < 0) {
                    newValue = 0;
                } else if (newValue > 255) {
                    newValue = 255;
                }
                outputImage[i][j][k] = static_cast<unsigned char>(newValue);
            }
        }
    }
}

void detectEdges(unsigned char inputImage[][256][3], unsigned char outputImage[][256][3]) {
    int height = SIZE;
    int width = SIZE;
    // Apply the Sobel operator for edge detection to each color channel
    for (int c = 0; c < 3; c++) { // Loop through color channels (R, G, B)
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                int gx = -inputImage[i - 1][j - 1][c] + inputImage[i - 1][j + 1][c] -
                         2 * inputImage[i][j - 1][c] + 2 * inputImage[i][j + 1][c] -
                         inputImage[i + 1][j - 1][c] + inputImage[i + 1][j + 1][c];
                int gy = -inputImage[i - 1][j - 1][c] - 2 * inputImage[i - 1][j][c] - inputImage[i - 1][j + 1][c] +
                         inputImage[i + 1][j - 1][c] + 2 * inputImage[i + 1][j][c] + inputImage[i + 1][j + 1][c];
                int gradient = abs(gx) + abs(gy);
                // Threshold the gradient to detect edges
                if (gradient > 100) {
                    outputImage[i][j][c] = 255; // Edge pixel
                } else {
                    outputImage[i][j][c] = 0; // Non-edge pixel
                }
            }
        }
    }
}

void enlargeQuarter(const unsigned char inputImage[][256][3], unsigned char outputImage[][256][3], int quarter) {
    int startX = 0, startY = 0;

    if (quarter == 1) {
        startX = 0;
        startY = 0;
    } else if (quarter == 2) {
        startX = SIZE / 2;
        startY = 0;
    } else if (quarter == 3) {
        startX = 0;
        startY = SIZE / 2;
    } else if (quarter == 4) {
        startX = SIZE / 2;
        startY = SIZE / 2;
    } else {
        std::cout << "Invalid quarter selection." << std::endl;
        return;
    }
    for (int i = 0; i < SIZE / 2; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            // Copy the color values from the corresponding pixel in the input image
            unsigned char pixelR = inputImage[startY + i][startX + j][0]; // Red channel
            unsigned char pixelG = inputImage[startY + i][startX + j][1]; // Green channel
            unsigned char pixelB = inputImage[startY + i][startX + j][2]; // Blue channel
            // Apply the color values to four pixels in the output image
            outputImage[i * 2][j * 2][0] = pixelR;
            outputImage[i * 2][j * 2][1] = pixelG;
            outputImage[i * 2][j * 2][2] = pixelB;
            outputImage[i * 2 + 1][j * 2][0] = pixelR;
            outputImage[i * 2 + 1][j * 2][1] = pixelG;
            outputImage[i * 2 + 1][j * 2][2] = pixelB;
            outputImage[i * 2][j * 2 + 1][0] = pixelR;
            outputImage[i * 2][j * 2 + 1][1] = pixelG;
            outputImage[i * 2][j * 2 + 1][2] = pixelB;
            outputImage[i * 2 + 1][j * 2 + 1][0] = pixelR;
            outputImage[i * 2 + 1][j * 2 + 1][1] = pixelG;
            outputImage[i * 2 + 1][j * 2 + 1][2] = pixelB;
        }
    }
}

void shrinkImage(const unsigned char inputImage[][SIZE][3], unsigned char outputImage[][SIZE][3], int scaleFactor) {
    int shrunkWidth = SIZE / scaleFactor;
    int shrunkHeight = SIZE / scaleFactor;
    for (int i = 0; i < shrunkHeight; i++) {
        for (int j = 0; j < shrunkWidth; j++) {
            // Calculate the average pixel values in a block based on the scaleFactor for each color channel
            int sumR = 0, sumG = 0, sumB = 0;
            for (int x = 0; x < scaleFactor; x++) {
                for (int y = 0; y < scaleFactor; y++) {
                    sumR += inputImage[i * scaleFactor + x][j * scaleFactor + y][0]; // Red channel
                    sumG += inputImage[i * scaleFactor + x][j * scaleFactor + y][1]; // Green channel
                    sumB += inputImage[i * scaleFactor + x][j * scaleFactor + y][2]; // Blue channel
                }
            }
            // Calculate the average color values
            outputImage[i][j][0] = static_cast<unsigned char>(sumR / (scaleFactor * scaleFactor));
            outputImage[i][j][1] = static_cast<unsigned char>(sumG / (scaleFactor * scaleFactor));
            outputImage[i][j][2] = static_cast<unsigned char>(sumB / (scaleFactor * scaleFactor));
        }
    }
}

void mirrorLeftHalf(unsigned char img[][SIZE][RGB]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            for (int k = 0; k < RGB; k++) {
                unsigned char temp = img[i][j][k];
                img[i][j][k] = img[i][j + SIZE / 2][k];
                img[i][j + SIZE / 2][k] = temp;
            }
        }
    }
}




void mirrorRightHalf(unsigned char img[][SIZE][RGB]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE / 2; j++) {
            for (int k = 0; k < RGB; k++) {
                unsigned char temp = img[i][j][k];
                img[i][j][k] = img[i][j + SIZE / 2][k];
                img[i][j + SIZE / 2][k] = temp;
            }
        }
    }
}


void mirrorUpperHalf(unsigned char img[][SIZE][RGB]) {
    int halfHeight = SIZE / 2;
    for (int i = 0; i < halfHeight; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < RGB; k++) {
                img[i][j][k] = img[SIZE - 1 - i][j][k];
            }
        }
    }
}

void mirrorLowerHalf(unsigned char img[][SIZE][RGB]) {
    int halfHeight = SIZE / 2;
    for (int i = halfHeight; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < RGB; k++) {
                img[i][j][k] = img[SIZE - 1 - i][j][k];
            }
        }
    }
}

void shuffleQuartersColor(unsigned char (*inputImage)[SIZE][RGB], unsigned char (*outputImage)[SIZE][RGB], const int order[4]) {
    // Check for valid input order (1 to 4, no duplicates)
    bool validOrder = true;
    bool seen[4] = {false};
    for (int i = 0; i < 4; i++) {
        if (order[i] < 1 || order[i] > 4 || seen[order[i] - 1]) {
            validOrder = false;
            break;
        }
        seen[order[i] - 1] = true;
    }
    if (!validOrder) {
        cout << "Error: Invalid order. Please provide a valid order (1 to 4 with no duplicates)." << std::endl;
        return;
    }
    // Calculate the dimensions of each quarter
    int quarterWidth = SIZE / 2;
    int quarterHeight = SIZE / 2;
    // Copy quarters based on the user's input order
    for (int i = 0; i < 4; i++) {
        int srcRowStart, srcColStart, destRowStart, destColStart;
        switch (order[i]) {
            case 1:
                srcRowStart = 0;
                srcColStart = 0;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 2:
                srcRowStart = 0;
                srcColStart = quarterWidth;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 3:
                srcRowStart = quarterHeight;
                srcColStart = 0;
                destRowStart = 0;
                destColStart = 0;
                break;
            case 4:
                srcRowStart = quarterHeight;
                srcColStart = quarterWidth;
                destRowStart = 0;
                destColStart = 0;
                break;
            default:
                break;
        }
        // Copy the quarter to the output image (for each color channel)
        for (int row = 0; row < quarterHeight; row++) {
            for (int col = 0; col < quarterWidth; col++) {
                for (int channel = 0; channel < RGB; channel++) {
                    outputImage[destRowStart + row][destColStart + col][channel] =
                            inputImage[srcRowStart + row][srcColStart + col][channel];
                }
            }
        }
    }
}

void blurImage(unsigned char inputImage[][SIZE][3], unsigned char outputImage[][SIZE][3]) {
    // Define a Gaussian kernel for blurring
    float kernel[3][3] = {
            {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
            {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
            {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
    };
    // Iterate over each pixel in the image
    for (int y = 1; y < SIZE - 1; y++) {
        for (int x = 1; x < SIZE - 1; x++) {
            // Apply the Gaussian blur separately to each color channel
            for (int channel = 0; channel < 3; channel++) {
                float sum = 0.0;
                // Apply the Gaussian kernel to the pixel and its neighbors
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        sum += kernel[ky + 1][kx + 1] * inputImage[y + ky][x + kx][channel];
                    }
                }
                // Store the blurred pixel value in the output image
                outputImage[y][x][channel] = static_cast<unsigned char>(sum);
            }
        }
    }
}

void cropImage(const unsigned char inputImage[][SIZE][3], unsigned char outputImage[][SIZE][3], int x, int y, int l, int w) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < l; j++) {
            for (int k = 0; k < 3; k++) { // Loop through each color channel
                outputImage[i][j][k] = inputImage[y + i][x + j][k];
            }
        }
    }
}

void skewImage(unsigned char inputImage[IMAGE_SIZE][IMAGE_SIZE][RGB], unsigned char outputImage[IMAGE_SIZE][IMAGE_SIZE][RGB], float skewDegree, char direction) {
    // Calculate the tangent of the skew angle
    float skewTan = std::tan(skewDegree);

    for (int i = 0; i < IMAGE_SIZE; i++) {
        for (int j = 0; j < IMAGE_SIZE; j++) {
            // Calculate the source position based on skew direction
            float srcX, srcY;

            if (direction == 'e') {  // Horizontal skew (right)
                srcX = i;
                srcY = j - skewTan * i;
            } else if (direction == 'f') {  // Vertical skew (up)
                srcX = i - skewTan * j;
                srcY = j;
            } else {
                std::cerr << "Invalid direction. Please use 'e' for horizontal or 'f' for vertical skewing." << std::endl;
                return;
            }

            // Bilinear interpolation (you'll need to implement this part)
            int x0 = static_cast<int>(srcX);
            int x1 = x0 + 1;
            int y0 = static_cast<int>(srcY);
            int y1 = y0 + 1;

            float xFraction = srcX - x0;
            float yFraction = srcY - y0;

            for (int k = 0; k < RGB; k++) {
                // Perform bilinear interpolation
                float top = inputImage[x0][y0][k] * (1 - xFraction) + inputImage[x1][y0][k] * xFraction;
                float bottom = inputImage[x0][y1][k] * (1 - xFraction) + inputImage[x1][y1][k] * xFraction;

                // Interpolate between top and bottom
                outputImage[i][j][k] = top * (1 - yFraction) + bottom * yFraction;
            }
        }
    }
}

int main() {
    fudou
    string inputname, path = "\\imgs\\";
    cout << "Enter image name : ";
    cout.flush();
    cin >> inputname;
    path += inputname;
    char cwd[PATH_MAX];
    const char *outputFilename = "output.bmp";
    string path2 = "\\imgs\\";
    int userChoice = displayFilterMenu();
    string secondImagePath;
    float brightnessFactor = 1.5; // Adjust this factor as needed
    readRGBBMP(strcat(getcwd(cwd, sizeof(cwd)), path.c_str()), imgRGB);
    switch (userChoice) {
        case 1:
            //// Apply the Black and White Filter
            BlackAndWhiteFilter(imgRGB);
//// Save the filtered image
            writeRGBBMP("black_and_white.bmp", imgRGB);
            showRGBBMP(imgRGB);
            break;
        case 2:
            //// Apply the Invert Filter
            InvertFilter(imgRGB);
//// Save the inverted image
            writeRGBBMP("inverted.bmp", imgRGB);
            showRGBBMP(imgRGB);
            break;
        case 3:
            cout << "Enter the path of the second image: ";
            cout.flush();
            cin >> inputname;
            path2 += inputname;
// Load the second image using readRGBBMP
            unsigned char img2[SIZE][SIZE][RGB];
            if (readRGBBMP(strcat(getcwd(cwd, sizeof(cwd)), path2.c_str()), img2) != 0) {
                cout << "Error reading the second image." << endl;
                return 1;
            }
// Create the output image array
            unsigned char mergedImage[SIZE][SIZE][RGB];
// Merge the two images
            merge(imgRGB, img2, mergedImage);
// Display or save the merged image
            showRGBBMP(mergedImage);
            break;
        case 4:
            // Create a copy for flipping
            unsigned char imgFlippedHorizontal[SIZE][SIZE][RGB];
// Call the horizontal flip function
            flipHorizontal(imgRGB, imgFlippedHorizontal);
// Show the horizontally flipped image
            showRGBBMP(imgFlippedHorizontal);
            unsigned char imgFlippedVertical[SIZE][SIZE][RGB];
// Call the vertical flip function
            flipVertical(imgRGB, imgFlippedVertical);
// Show the vertically flipped image
            showRGBBMP(imgFlippedVertical);
            break;
        case 5:
            // Create copies for rotation
            unsigned char imgRotated90[SIZE][SIZE][RGB];
            unsigned char imgRotated180[SIZE][SIZE][RGB];
            unsigned char imgRotated270[SIZE][SIZE][RGB];
            cout << "which degree do you want? (choose 90 or 180 or 270)" << E;
            cout.flush();
            int degree;
            cin >> degree;
            // Rotate the image by 90 degrees and display
            if (degree == 90) {
                rotate90(imgRGB, imgRotated90);

                // Display or save the rotated image as needed
                showRGBBMP(imgRotated90);
            }
//    // Rotate the image by 180 degrees and display
            else if (degree == 180) {
                rotate180(imgRGB, imgRotated180);
                // Display or save the rotated image as needed
                showRGBBMP(imgRotated180);
            }
//    // Rotate the image by 270 degrees and display
            else if (degree == 270) {
                rotate270(imgRGB, imgRotated270);
                // Display or save the rotated image as needed
                showRGBBMP(imgRotated270);
            } else {
                cout << "does not exist" << E;
                cout.flush();
            }
            break;
        case 6:
//            Create a copy for adjusting brightness
            unsigned char imgAdjustedBrightness[SIZE][SIZE][RGB];
            // Adjust the brightness of the image by a factor (e.g., 0.5 for darker, 1.5 for lighter)
            DarkenandLightenImage(imgRGB, imgAdjustedBrightness, brightnessFactor);
            // Display or save the adjusted brightness image
            showRGBBMP(imgAdjustedBrightness);
            break;
        case 7:
            // Create an output image for edge detection
            unsigned char edgeImage[SIZE][SIZE][RGB];
            // Apply edge detection to the color image
            detectEdges(imgRGB, edgeImage);
            // Display or save the edge-detected image
            showRGBBMP(edgeImage);
            break;
        case 8:
            int selectedQuarter;
            cout << "Select a quarter to enlarge (1, 2, 3, or 4): ";
            cout.flush();
            cin >> selectedQuarter;
            // Ensure the selected quarter is valid
            if (selectedQuarter >= 1 && selectedQuarter <= 4) {
                // Create an output image array for the enlarged quarter
                unsigned char enlargedQuarter[SIZE][SIZE][3];
                // Enlarge the selected quarter of the color image
                enlargeQuarter(imgRGB, enlargedQuarter, selectedQuarter);
                // Display or save the enlarged quarter image
                showRGBBMP(enlargedQuarter);
            } else {
                cout << "Invalid quarter selection." <<  endl;
                cout.flush();
            }
            break;
        case 9:
            int scaleFactor;
            unsigned char shrunkImg[SIZE][SIZE][RGB];
            cout << "Enter the scale factor (2, 3, or 4): ";
            cout.flush();
            cin >> scaleFactor;
            // Ensure that the scaleFactor is valid
            if (scaleFactor != 2 && scaleFactor != 3 && scaleFactor != 4) {
                cout << "Invalid scale factor. Please enter 2, 3, or 4." <<  endl;
                cout.flush();
                return 1; // Exit with an error code
            }
            // Call the shrinkImage function for color images
            shrinkImage(imgRGB, shrunkImg, scaleFactor);
            // Display or save the shrunk color image
            showRGBBMP(shrunkImg);
            break;
        case 10:
            int choice;
            cout << "Choose a mirror filter (1: Left 1/2, 2: Right 1/2, 3: Upper 1/2, 4: Lower 1/2): ";
            cout.flush();
            cin >> choice;
            switch (choice) {
                case 1:
                    mirrorLeftHalf(imgRGB); // Apply mirror filter to color image
                    break;
                case 2:
                    mirrorRightHalf(imgRGB); // Apply mirror filter to color image
                    break;
                case 3:
                    mirrorUpperHalf(imgRGB); // Apply mirror filter to color image
                    break;
                case 4:
                    mirrorLowerHalf(imgRGB); // Apply mirror filter to color image
                    break;
                default:
                    cout << "Invalid choice." << endl;
                    cout.flush();
                    return 1;
            }
            showRGBBMP(imgRGB); // Display the modified color image
            break;
        case 11:
            unsigned char imgColor[SIZE][SIZE][RGB]; // Replace with your color image data
            int order[4];
            // Prompt the user to enter the order for shuffling
            cout << "Enter the order for shuffling (e.g., 1 2 3 4): ";
            cout.flush();
            for (int i = 0; i < 4; i++) {
                cin >> order[i];
            }
            // Create an output array for the shuffled image
            unsigned char shuffledImage[SIZE][SIZE][RGB];
            // Shuffle the quarters based on user-defined order
            shuffleQuartersColor(imgColor, shuffledImage, order);
            // Display the shuffled image using showRGBBMP
            showRGBBMP(shuffledImage);
            break;
        case 12:
            unsigned char blurredImage[SIZE][SIZE][3]; // Create an array for the blurred image.
            // Call the blurImage function to apply Gaussian blur to the color image.
            blurImage(imgRGB, blurredImage);
            // Display or save the blurred image.
            showRGBBMP(blurredImage);
        case 13:
            int x, y, l, w;
            cout << "Enter the x-coordinate of the top-left corner of the square: ";
            cout.flush();
            cin >> x;
            cout << "Enter the y-coordinate of the top-left corner of the square: ";
            cout.flush();
            cin >> y;
            cout << "Enter the length of the square: ";
            cout.flush();
            cin >> l;
            cout << "Enter the width of the square: ";
            cout.flush();
            cin >> w;
//    // Create an output image for the cropped result
            unsigned char croppedImage[SIZE][SIZE][RGB];
//    // Apply the crop filter to the image
            cropImage(imgRGB, croppedImage, x, y, l, w);
//    // Now you can display or save the cropped image as needed
//    // For displaying using your image library's function, refer to its documentation
//    // For example, to display using your library's function:
            showRGBBMP(croppedImage);
//    // Optionally, you can save the cropped image using your library's function
            writeRGBBMP(outputFilename, croppedImage);
            break;
        case 14:
            unsigned char inputImage[IMAGE_SIZE][IMAGE_SIZE][RGB];
            unsigned char outputImage[IMAGE_SIZE][IMAGE_SIZE][RGB];
// ... Initialize inputImage with your color image data ...
// Degree of skewing
            float skewDegree;
// Choose to skew horizontally (e) or vertically (f)
            char choic;
            cout << "Choose skew direction (e for horizontal, f for vertical): ";
            cout.flush();
            cin >> choic;
// Check if the user's choice is valid
            if (choic != 'e' && choic != 'f') {
                cout << "Invalid choice. Please select 'e' for horizontal or 'f' for vertical skewing." << endl;
                cout.flush();
                return 1;
            }
// Ask the user for the degree of skewing
            cout << "Enter the degree of skewing (positive for right/up, negative for left/down): ";
            cout.flush();
            cin >> skewDegree;
// Apply the chosen skewing
            skewImage(inputImage, outputImage, skewDegree, choic);
// Display the skewed image using showRGBBMP
            showRGBBMP(outputImage);
    }
}