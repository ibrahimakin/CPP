#if !defined(ANYWRITER_HPP_INCLUDED)
#define ANYWRITER_HPP_INCLUDED

class AnyWriter
{
public:
    virtual ~AnyWriter() {};
    virtual void putchar(int) = 0;
};

#endif // ANYWRITER_HPP_INCLUDED