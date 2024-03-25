/*
This algorithm is an example of a substitution cipher. More specifically, it’s a type of steganography, where a message is encoded within another message. The algorithm is roughly explained as follows:
Convert each letter to its binary representation.
Each binary bit is then substituted with either SecretCharacter1 (SC1) or SecretCharacter2 (SC2), based on its value.
*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <bitset>
using namespace std;

string decrypt_Ciphertext(string cipher_text, char sc1, char sc2)
{
    string plain_text="";
    int bits_per_char = 5;
    
    for(int i =0;i<cipher_text.length();i+=bits_per_char)
    {
        string bits = cipher_text.substr(i, bits_per_char);
        
        bitset<5> bin;
        for(int i =0; i<bits.length() ;i++)
        {
            if(bits[i] == sc1)
            {
                bin[bits.length()-i-1]=0;
            }
            else
            {
               bin[bits.length()-i-1]=1;
            }
        }
        int decimal = bin.to_ulong();
        char c = 'A'+decimal;
        plain_text += c;
    }
    return plain_text;
    
}


int main() {
    
    char sc1, sc2;
    string cipher_text;

    cin >> sc1 >> sc2;
    cin.ignore(); 
    getline(cin, cipher_text);

    string plaintext = decrypt_Ciphertext(cipher_text, sc1, sc2);

    for (char& c : plaintext) {
        cout << char(toupper(c));
    }
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    return 0;
}