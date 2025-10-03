#include <stdio.h>
#include <stdlib.h>

#pragma pack(push,1) // Ensure no padding in structs
typedef struct {
    unsigned short bfType;
    unsigned int bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

void encryptDecryptBMP(const char *inputFile, const char *outputFile, unsigned char key) {
    FILE *in = fopen(inputFile, "rb");
    if (!in) {
        printf("Error: Cannot open input file\n");
        return;
    }

    FILE *out = fopen(outputFile, "wb");
    if (!out) {
        printf("Error: Cannot open output file\n");
        fclose(in);
        return;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;

    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, in);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, in);

    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, out);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, out);

    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;
    int row_padded = (width * 3 + 3) & (~3); // Each row padded to multiple of 4 bytes

    unsigned char *row = (unsigned char *)malloc(row_padded);

    for (int i = 0; i < height; i++) {
        fread(row, 1, row_padded, in);

        // XOR operation for encryption/decryption
        for (int j = 0; j < width * 3; j++) {
            row[j] ^= key; // XOR each byte with the key
        }

        // Simple pixel swapping (reverse row order)
        fwrite(row, 1, row_padded, out);
    }

    free(row);
    fclose(in);
    fclose(out);
}
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./SCT_CS_2 <input.bmp> <output.bmp>\n");
        return 1;
    }

    encryptDecryptBMP(argv[1], argv[2], 123); // key = 123 (example)
    return 0;
}