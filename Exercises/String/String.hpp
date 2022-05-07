#if !defined(STRING_HPP_INCLUDED)
#define STRING_HPP_INCLUDED

class String {
    private:
        char* m_string;
        int m_size = 0;
    public:
        String() {};
        String(const char*);
        String(const String&);
        bool empty();
        int length();
        static int length(const char*);
        void setCharAt(int, char);
        char getCharAt(int);
        bool equals(String);
        void append(String);
        char* charArray();
        void clear();
};

#endif // STRING_HPP_INCLUDED