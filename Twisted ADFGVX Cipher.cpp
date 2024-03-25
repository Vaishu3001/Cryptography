/*
Given a key square (polybius square), keyword and plaintext message, encrypt the message using the modified ADFGVX cipher and return the cipher text.

About the algorithm:

This algorithm is a product cipher of two ciphers: polybius square cipher and columnar transposition. The encrypting takes place in two phases. First phase is performing substitution while the second phase is fractionating.

During the substitution phase, we substitute each letter with two letters retrieved from the polybius square
After this, fill the enciphered text below the keyword in a matrix format left to right in a row and top to bottom fashion. Now perform columnar transposition by sorting the keyword in alphabetical order.
Now retrieve the text from the matrix column wise top to down to get the final ciphered text.

*/

#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <numeric>
using namespace std;


string encrypt_algo(string polybius_sq_data, string transposition_key, string plain_text) {
    string cipher_text = "";
    char column[6] = {'G', 'R', 'A', 'P', 'H', 'Y'};
    char row[6] = {'C', 'R', 'Y', 'P', 'T', 'O'};
    char square[6][6];
    int r = 0, c = 0;
    for (char ch : polybius_sq_data) {
        square[r][c] = ch;
        c++;
        if (c == 6) {
            c = 0;
            r++;
        }
    }
    string cipher1;
    for (int i = 0; i < plain_text.length(); i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (plain_text[i] == square[j][k]) {
                    cipher1 += row[j];
                    cipher1 += column[k];
                }
            }
        }
    }

    int cipherLen = cipher1.length();
    int keyLen = transposition_key.length();
    int rows = (cipherLen + keyLen - 1) / keyLen;
    char matrix[rows][keyLen];
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < keyLen; j++) {
            int index = i * keyLen + j;
            matrix[i][j] = (index < cipherLen) ? cipher1[index] : 'X';
        }
    }
    vector<int> keyIndices(keyLen);
    iota(keyIndices.begin(), keyIndices.end(), 0);
    sort(keyIndices.begin(), keyIndices.end(), [&](int a, int b) {
        return transposition_key[a] < transposition_key[b];
    });
    string result = "";
    for (int j : keyIndices) {
        for (int i = 0; i < rows; i++) {
            result += matrix[i][j];
        }
    }

    return result;
}
int main() {
    string transposition_key;
    string plain_text;
    string polybius_sq_data;
    cin>> transposition_key;
    cin.ignore(); 
    cin>> polybius_sq_data;
    cin.ignore();
    cin>> plain_text;
    string cipher_text = encrypt_algo(polybius_sq_data, transposition_key, plain_text);
    for (char& c : cipher_text) {
        cout << char(c);
    }
    return 0;
}