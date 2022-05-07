#include "String.hpp"

#include <stdio.h>
#include <cstring>

#define TEST_TRUE(x) printf("Test '%s' %s\n", #x, (x) ? "passed" : "failed!")

/**
 * @brief 
 * 
 * @return int 
 */

int main()
{
    String empty_string;
    TEST_TRUE(empty_string.empty());

    String empty_string2(NULL);
    TEST_TRUE(empty_string2.empty());

    String empty_string3("");
    TEST_TRUE(empty_string3.empty());

    String empty_string4 = "";
    TEST_TRUE(empty_string4.empty());

    TEST_TRUE(empty_string.equals(empty_string2));
    TEST_TRUE(empty_string2.equals(empty_string3));
    TEST_TRUE(empty_string3.equals(empty_string4));

    char const* hello_world_str = "Hello World";
    String string1 = hello_world_str;
    TEST_TRUE(string1.length() == 11);
    TEST_TRUE(string1.equals("Hello World"));

    char const* char_ptr = string1.charArray();
    TEST_TRUE(char_ptr != hello_world_str);

    string1.setCharAt(0, 'h');
    TEST_TRUE(string1.equals("hello World"));
    TEST_TRUE(string1.charArray() != hello_world_str);
    TEST_TRUE(strcmp(hello_world_str, "Hello World") == 0);

    string1.clear();
    TEST_TRUE(string1.empty());

    String string2("Foo");
    TEST_TRUE(string2.length() == 3);

    char const* bar = "_bar";
    string2.append(bar);
    TEST_TRUE(string2.equals("Foo_bar"));

    String string3("Foo_");
    TEST_TRUE(string3.length() == 4);

    string3.append("bar");
    TEST_TRUE(string3.equals("Foo_bar"));

    String string4 = "Hello";
    String string5 = string4;
    TEST_TRUE(string4.equals(string5));
    TEST_TRUE(string4.charArray() != string5.charArray());

    String string6("not");
    String string7("equal");
    TEST_TRUE(!string6.equals(string7));

    return 0;
}