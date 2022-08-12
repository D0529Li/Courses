#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main()
{
    ifstream in1, in2;
    in1.open("ciphertext1", ios::binary);
    in2.open("ciphertext2", ios::binary);
    char cipherText1[300], cipherText2[300];
    for (int i = 0; i < 300; i++)
    {
        in1 >> noskipws >> cipherText1[i];
        in2 >> noskipws >> cipherText2[i];
    }
    in1.close();
    in2.close();

    string s;
    while (getline(cin, s))
    {
        string s1, s2;
        for (int i = 0; i <= 300 - s.length(); i++)
        {
            s1.clear();
            s2.clear();
            bool skip1 = false, skip2 = false;
            for (int j = 0; j < s.length(); j++)
            {
                int val = (int)(cipherText2[i + j] - cipherText1[i + j] + (char)s[j]);
                val %= 256;
                if (val < 0)
                    val += 256;
                if (val >= 0x20 && val <= 0x7e)
                    s1 += (char)val;
                else
                    skip1 = true;

                val = (int)(cipherText1[i + j] - cipherText2[i + j] + (char)s[j]);
                val %= 256;
                if (val < 0)
                    val += 256;
                if (val >= 0x20 && val <= 0x7e)
                    s2 += (char)val;
                else
                    skip2 = true;
            }
            if (!skip1)
                cout << i << " " << s1 << "   by 1" << endl;
            if (!skip2)
                cout << i << " " << s2 << "   by 2" << endl;
        }
        cout << endl;
    }
    return 0;
}
