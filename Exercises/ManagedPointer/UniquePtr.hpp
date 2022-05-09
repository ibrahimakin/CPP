#if !defined(UNIQUE_PTR_HPP_INCLUDED)
#define UNIQUE_PTR_HPP_INCLUDED

template <class T>
class unique_ptr
{
private:
    T* data;
public:
    static int allocated_memory;
    unique_ptr(T*);
    ~unique_ptr();
    T* get();
    T* operator ->();
    T& operator *();
};
#include "UniquePtr.cpp"
#endif // UNIQUE_PTR_HPP_INCLUDED