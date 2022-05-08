#if !defined(TERMINALREADER_HPP_INCLUDED)
#define TERMINALREADER_HPP_INCLUDED
#include "AnyReader.hpp"

class TerminalReader: public AnyReader
{
public:
    int getchar();
};

#endif // TERMINALREADER_HPP_INCLUDED