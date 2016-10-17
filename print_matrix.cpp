#include <cstdint>
#include <iostream>
#include <fstream>
#include <cmath>

int main() {
    std::ifstream fin("output.bin", std::ifstream::in | std::ifstream::binary);

    uint32_t rows = 0;
    uint32_t cols = 0;

    fin.read((char *)&rows, sizeof(rows));
    fin.read((char *)&cols, sizeof(cols));

    for (int i = 0; i < rows; i++) {
        uint8_t * buffer = new uint8_t [cols];
        fin.read((char *)buffer, cols);
        for (int j = 0; j < cols; j++) {
            std::cout << (int)buffer[j] << " ";
        }
        std::cout << std::endl;
    }

    fin.close();
    return 0;
}

