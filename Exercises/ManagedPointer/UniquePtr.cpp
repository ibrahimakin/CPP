#include <iostream>
#include "UniquePtr.hpp"

template <class T>
int unique_ptr<T>::allocated_memory = 0;

template <class T>
unique_ptr<T>::unique_ptr(T* x)
{
    allocated_memory += sizeof(T);
    std::cout << "Allocated Memory: " << allocated_memory << std::endl;
    data = x;
}

template <class T>
unique_ptr<T>::~unique_ptr()
{
    allocated_memory -= sizeof(T);
    std::cout << "Allocated Memory: " << allocated_memory << std::endl;
    delete data;
}

template <class T>
T* unique_ptr<T>::get()
{
    return data;
}

template <class T>
T* unique_ptr<T>::operator ->()
{
    return data;
}

template <class T>
T& unique_ptr<T>::operator *()
{
    return *data;
}