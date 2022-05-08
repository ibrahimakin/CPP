#include <iostream>
#include "FileWriter.hpp"

FileWriter::FileWriter(const char* name) {
    file = fopen(name, "w+");
    if (file == NULL)
    {
        printf("Error while writing the file.\n");
    }
}

FileWriter::~FileWriter() {
    if (fclose(file) != 0)
    {
        printf("Error while closing the file. \n");
    }
}

void FileWriter::putchar(int c) {
    putc(c, file);
}