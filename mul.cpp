#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>

void multiply(const uint8_t * a, const uint8_t * b, uint8_t * c, int a_rows, int a_cols, int b_rows, int b_cols) {
    for (int i = 0; i < a_rows; ++i) {
        for (int j = 0; j < b_cols; ++j) {
            for (int k = 0; k < a_cols; ++k) {
                c[i * b_cols + j] += a[i * a_cols + k] * b[k * b_cols + j];
            }
        }
    }
}

uint8_t * readBlock(std::fstream &fin, const int blockRows, const int blockCols,
                 const int rows, const int cols, const int rowsOffset, const int colsOffset, const int offset) {
    uint8_t * buffer = new uint8_t [blockRows * blockCols];
    for (int i = 0; i < blockRows; i++) {
        if (cols != 1 && rows != 1 || i == 0) {
            int seek = (rowsOffset + i) * cols + colsOffset + offset;
            fin.seekg(seek, std::ios_base::beg);
        }
        fin.read((char *)(buffer + i * blockCols), blockCols);
    }
    return buffer;
}

void writeBlock(std::fstream &fout, const uint8_t *buffer, const int blockRows, const int blockCols,
                const int rows, const int cols,
                const int rowsOffset, const int colsOffset, const int offset) {
    for (int i = 0; i < blockRows; i++) {
        if (cols != 1 && rows != 1 || i == 0) {
            int seek = (rowsOffset + i) * cols + colsOffset + offset;
            fout.seekp(seek, std::ios_base::beg);
        }
        fout.write((char *) (buffer + i * blockCols), blockCols);
    }
}

int main() {
    std::fstream fin("input_3x3.bin", std::fstream::in | std::fstream::binary);
    std::fstream fout("output.bin", std::fstream::out | std::fstream::binary);
    fout.clear();
    uint32_t size = 0;
    uint32_t blockSize = 2;

    fin.read((char *)&size, sizeof(size));
    fout.write((char *)&size, sizeof(size));
    fout.write((char *)&size, sizeof(size));

    if (size < blockSize) {
        blockSize = size;
    }

    for (int i = 0; i < size;  i += blockSize) {
        int a_rows = std::min(blockSize, size - i);
        for (int j = 0; j < size;  j += blockSize) {
            int b_cols = std::min(blockSize, size - j);
            uint8_t * c_block = new uint8_t [a_rows * b_cols];
            std::fill(c_block, c_block + a_rows * b_cols, 0);

            for (int k = 0; k < size;  k += blockSize) {
                int a_cols = std::min(blockSize, size - k);
                int b_rows = a_cols;
                uint8_t * a_block = readBlock(fin, a_rows, a_cols, size, size, i, k, 8);
                uint8_t * b_block = readBlock(fin, b_rows, b_cols, size, size, k, j, 16 + size * size);
                multiply(a_block, b_block, c_block, a_rows, a_cols, b_rows, b_cols);

                delete a_block;
                delete b_block;
            }

            writeBlock(fout, c_block, a_rows, b_cols, size, size, i, j, 8);
            delete c_block;
        }
    }

    fin.close();
    fout.close();
    return 0;
}