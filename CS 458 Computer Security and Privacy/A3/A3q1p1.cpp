#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    ifstream in;
    int text[300];
    char ch;
    in.open("ciphertext1");
    for (int i = 0; i < 300; i++)
    {
        in >> ch;
        text[i] = (int)ch;
    }
    in.close();
    in.open("ciphertext2");
    for (int i = 0; i < 300; i++)
    {
        in >> ch;
        text[i] = (text[i] - ch) % 256;
    }
    in.close();

    ofstream out;
    out.open("p2p1");
    for (int i = 0; i < 300; i++)
    {
        out << (char)text[i];
    }
    out.close();
    return 0;
}