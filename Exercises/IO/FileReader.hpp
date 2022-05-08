#if !defined(FILEREADER_HPP_INCLUDED)
#define FILEREADER_HPP_INCLUDED
#include <stdio.h>
#include "AnyReader.hpp"

class FileReader: public AnyReader
{
private:
    FILE* file;
public:
    FileReader(const char*);
    ~FileReader();
    int getchar();
};

#endif // FILEREADER_HPP_INCLUDED