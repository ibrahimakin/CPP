#if !defined(TERMINALWRITER_HPP_INCLUDED)
#define TERMINALWRITER_HPP_INCLUDED
#include "AnyWriter.hpp"

class TerminalWriter: public AnyWriter
{
public:
    void putchar(int);
};

#endif // TERMINALWRITER_HPP_INCLUDED