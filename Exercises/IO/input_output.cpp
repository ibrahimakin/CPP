#include <stdio.h>
#include <iostream>
#include "AnyReader.hpp"
#include "AnyWriter.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"
#include "TerminalReader.hpp"
#include "TerminalWriter.hpp"
using namespace std;

bool copy(AnyReader* reader, AnyWriter* writer) {
    int ch = reader->getchar();
    while (ch != EOF)
    {
        writer->putchar(ch);
        ch = reader->getchar();
    }
    return true;
}

int main()
{
    AnyReader* fr = new FileReader("file.txt");
    AnyWriter* fw = new FileWriter("file_new.txt");
    copy(fr, fw);
    delete fr;
    delete fw;
    cout << endl << "From Terminal To File" << endl;
    AnyReader* tr = new TerminalReader();
    fw = new FileWriter("output.txt");
    copy(tr, fw);
    delete tr;
    delete fw;
    cout << endl << "From File To Terminal" << endl;
    fr = new FileReader("output.txt");
    AnyWriter* tw = new TerminalWriter();
    copy(fr, tw);
    delete fr;
    delete tw;
    cout << endl << "The End" << endl;
    return 0;
}