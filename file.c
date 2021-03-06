#include <stdio.h>


const unsigned char buffer[] = {
'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','.','-','-','-','-','-','-','-','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','-','-','-','-','-','-','-','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','-','-','-','-','.','-','-','.','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','.','-','-','-','.','-','-','-','.','.','.','.','\n','-','.','.','.','.','.','-','.','.','.','.','.','-','-','.','.','.','.','.','.','.','.','.','.','-','.','.','.','.','.','.','.','-','-','.','.','.','.','.','.','.','.','\n','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','\n','-','-','-','-','-','-','-','-','-','*','*','-','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','-','-','.','.','\n','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',

};


int main() {
    const size_t bufferSize = sizeof(buffer);

    // Extract the header from the compressed frame buffer
    const unsigned char styleCode = 0;
    const unsigned short frameWidth = 74;
    const unsigned short frameHeight = 21;

    // Calculate the size of the uncompressed frame
    const size_t frameSize = (frameWidth + 1) * frameHeight;

    // Create the buffer to store the uncompressed frame
    // +1 to store the null termination character
    char uncompressedFrame[frameSize + 1];
    uncompressedFrame[frameSize] = '\0';

    // Start from bufferIndex HEADER_SIZE because the header is already extracted
    size_t bufferIndex = 0;
    size_t frameIndex = 0;

    while (frameIndex < frameSize) {
        const unsigned char byte = buffer[bufferIndex];

        printf("Byte: %d, index: %ld\n", byte, frameIndex);

        if (byte == 0) {
            // Extract the count and the character from the buffer
            const size_t count = buffer[bufferIndex + 1];
            const unsigned char character = buffer[bufferIndex + 2];

            // Add <count> characters to the buffer
            for (unsigned char i = 0; i < count; i++) {
                uncompressedFrame[frameIndex + i] = character;
            }
            bufferIndex += 3;
            frameIndex += count;
        }
        else {
            // Add the character to the buffer
            uncompressedFrame[frameIndex] = byte;
            bufferIndex ++;
            frameIndex ++;
        }

    }

    // Print the uncompressed frame
    printf("%s\n", uncompressedFrame);

}

