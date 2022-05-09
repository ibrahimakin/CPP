#if !defined(UNIQUE_INT_PTR_HPP_INCLUDED)
#define UNIQUE_INT_PTR_HPP_INCLUDED

class unique_int_ptr
{
private:
    int* data;
public:
    unique_int_ptr(int*);
    ~unique_int_ptr();
    int* get();
};

#endif // UNIQUE_INT_PTR_HPP_INCLUDED