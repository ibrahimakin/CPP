#if !defined(ANYREADER_HPP_INCLUDED)
#define ANYREADER_HPP_INCLUDED

class AnyReader
{
public:
    virtual ~AnyReader() {};
    virtual int getchar() = 0;
};

#endif // ANYREADER_HPP_INCLUDED