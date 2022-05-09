#include <stdio.h>
#include "UniqueIntPtr.hpp"
#include "UniquePtr.hpp"

#define TEST_TRUE(x) printf("Test '%s' %s\n", #x, x ? "passed" : "failed")

struct SimpleData
{
    int first;
    char second;
};

int main(int argc, char const *argv[])
{
    printf("Part 1\n");
    int* data1 = new int(55);    // allocate raw pointer

    // create a managed pointer and let it manage the data
    unique_int_ptr data_ptr1(data1);

    // Make sure that both pointers show the same value
    TEST_TRUE(*data1 == *(data_ptr1.get()));

    // Both ways
    *(data_ptr1.get()) = 99;
    TEST_TRUE(*data1 == *(data_ptr1.get()));


    printf("\nPart 2\n");
    int* data2 = new int(55);    // allocate raw pointer

    // create a managed pointer and let it manage the data
    unique_ptr<int> data_ptr2(data2);

    // Make sure that both pointers show the same value
    TEST_TRUE(*data2 == *(data_ptr2.get()));

    // Both ways
    *(data_ptr1.get()) = 99;
    TEST_TRUE(*data2 == *(data_ptr2.get()));


    printf("\nPart 3\n");
    SimpleData* data3 = new SimpleData;    // allocate raw pointer
    data3->first = 55;
    data3->second = 'C';

    // create a managed pointer and let it manage the data
    unique_ptr<SimpleData> data_ptr3(data3);

    // Make sure that both pointers show the same value
    TEST_TRUE(data3->first == data_ptr3->first);
    TEST_TRUE(data3->second == data_ptr3->second);

    // Both ways
    data_ptr3->first = 99;
    data_ptr3->second = 'D';
    TEST_TRUE(data3->first == data_ptr3->first);
    TEST_TRUE(data3->second == data_ptr3->second);

    unique_ptr<int> int_ptr(new int);
    *int_ptr = 199;

    // both allocations are released here

    return 0;
}