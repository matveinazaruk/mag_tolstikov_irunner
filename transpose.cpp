#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>

char * transpose(const char * array, const int rows, const int cols) {
    char * result = new char [rows * cols];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[rows * j + i] = array[i * cols + j];
        }
    }
    return result;
}

char * readBlock(std::ifstream &fin, const int blockRows, const int blockCols,
                 const int rows, const int cols, const int rowsOffset, const int colsOffset) {
    char * buffer = new char [blockRows * blockCols];
    if (cols == 1 || rows == 1) {
        fin.read(buffer, blockRows * blockCols);
    } else {
        for (int i = 0; i < blockRows; i++) {
            int seek = (rowsOffset + i) * cols + colsOffset + 8;
            fin.seekg(seek, std::ios_base::beg);
            fin.read(buffer + i * blockCols, blockCols);
        }
    }
    
    return buffer;
}

void writeBlock(std::ofstream &fout, const char *buffer, const int blockRows, const int blockCols,
                const int rows, const int cols,
                const int rowsOffset, const int colsOffset) {
    if (cols == 1 && rows == 1) {
        fout.write(buffer, blockCols * blockRows);
    } else {
        for (int i = 0; i < blockRows; i++) {
            int seek = (rowsOffset + i) * cols + colsOffset + 8;
            fout.seekp(seek, std::ios_base::beg);
            fout.write(buffer + i * blockCols, blockCols);
        }
    }
}

int main() {
    std::ifstream fin("input.bin", std::ifstream::in | std::ifstream::binary);
    std::ofstream fout("output.bin", std::ofstream::out | std::ofstream::binary);

    uint32_t rows = 0;
    uint32_t cols = 0;
    uint32_t blockRows = 300;
    uint32_t blockCols = 300;
    uint32_t blockSize = blockRows * blockCols;

    fin.read((char *)&rows, sizeof(rows));
    fin.read((char *)&cols, sizeof(cols));

    fout.write((char *)&cols, sizeof(cols));
    fout.write((char *)&rows, sizeof(rows));

    if (rows < blockRows) {
        blockRows = rows;
        blockCols = std::min(blockSize / blockRows, cols);
    }

    if (cols < blockCols) {
        blockCols = cols;
        blockRows = std::min(blockSize / blockCols, rows);
    }

    for (int i = 0; i < rows / (double) blockRows; i++) {
        int rowsOffset = i * blockRows;
        for (int j = 0; j < cols / (double) blockCols; j++) {
            int colsOffset = j * blockCols;
            int adjustedBlockRows = (rowsOffset + blockRows) < rows ? blockRows : rows - rowsOffset;
            int adjustedBlockCols = (colsOffset + blockCols) < cols ? blockCols : cols - colsOffset;
            char * buffer = readBlock(fin, adjustedBlockRows, adjustedBlockCols,
                                      rows, cols, rowsOffset, colsOffset);
            auto transposed = transpose(buffer, adjustedBlockRows, adjustedBlockCols);
            writeBlock(fout, transposed, adjustedBlockCols, adjustedBlockRows, cols, rows, colsOffset, rowsOffset);
            delete buffer;
            delete transposed;
        }
    }

    fin.close();
    fout.close();
    return 0;
}
