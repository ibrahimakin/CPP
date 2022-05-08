#include "FileReader.hpp"

FileReader::FileReader(const char* name) {
    file = fopen(name, "r");
    if (file == NULL)
    {
        printf("Error while reading the file.\n");
    }
}

FileReader::~FileReader() {
    if (fclose(file) != 0)
    {
        printf("Error while closing the file. \n");
    }
}

int FileReader::getchar() {
    return getc(file);
}