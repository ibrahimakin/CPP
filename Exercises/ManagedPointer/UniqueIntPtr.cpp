#include "UniqueIntPtr.hpp"

unique_int_ptr::unique_int_ptr(int* x)
{
    data = x;
}

unique_int_ptr::~unique_int_ptr()
{
    delete data;
}

int* unique_int_ptr::get()
{
    return data;
}