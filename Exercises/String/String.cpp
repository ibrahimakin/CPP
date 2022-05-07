#include <stdlib.h>
#include "String.hpp"

String::String(const char* s) {
    m_size = length(s);
    m_string = new char[m_size + 1];

    for (int i = 0; i < m_size; i++)
    {
        m_string[i] = s[i];
    }

    m_string[m_size] = '\0';
}

String::String(const String& s) {
    int length = s.m_size;
    m_string = new char[length + 1];
    for (int i = 0; i < length; i++)
    {
        m_string[i] = s.m_string[i];
    }
    m_string[length] = '\0';
    m_size = length;
}

bool String::empty() {
    return m_size == 0;
}

int String::length() {
    return m_size;
}

int String::length(const char* s) {
    int i = 0;
    if (s != NULL)
    {
        while (s[i])
        {
            i++;
        }
    }
    return i;
}

void String::setCharAt(int i, char c) {
    if (-1 < i && i < m_size)
    {
        m_string[i] = c;
    }
}

char String::getCharAt(int i) {
    if (-1 < i && i < m_size)
    {
        return m_string[i];
    }
    return '\0';
}

bool String::equals(String s) {
    if (m_size != s.length())
    {
        return false;
    }
    for (int i = 0; i < m_size; i++)
    {
        if (m_string[i] != s.getCharAt(i))
        {
            return false;
        }
    }
    return true;
}

void String::append(String s) {
    int length = s.length();
    int total = m_size + length;
    char* new_string = new char[total + 1];
    for (int i = 0; i < m_size; i++)
    {
        new_string[i] = m_string[i];
    }
    for (int i = 0; i < length; i++)
    {
        new_string[m_size + i] = s.getCharAt(i);
    }
    delete[] m_string;
    new_string[total] = '\0';
    m_string = new_string;
    m_size = total;
}

char* String::charArray() {
    return m_string;
}

void String::clear() {
    delete[] m_string;
    m_size = 0;
}