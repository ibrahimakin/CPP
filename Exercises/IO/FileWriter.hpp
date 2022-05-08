#if !defined(FILEWRITER_HPP_INCLUDED)
#define FILEWRITER_HPP_INCLUDED
#include <stdio.h>
#include "AnyWriter.hpp"

class FileWriter: public AnyWriter
{
private:
    FILE* file;
public:
    FileWriter(const char*);
    ~FileWriter();
    void putchar(int);
};

#endif // FILEWRITER_HPP_INCLUDED