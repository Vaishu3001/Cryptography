/*
The given algorithm is a product cipher, combining the strengths of Playfair, Vigenère, and Columnar Transposition ciphers to enhance the security of the encryption process.
Playfair cipher: Employing the Playfair cipher involves the generation of a key table derived from a given keyword. The plaintext is then broken into digraphs, pairs of letters, and each digraph undergoes the Playfair encryption process. This step serves as the initial layer in our product cipher, introducing a spatial transformation to the original message.
Vigenère Cipher: The Vigenère cipher introduces an additional layer of complexity. Utilizing a chosen keyword, it is repeated to match the length of the plaintext. The combination of this extended keyword and the original plaintext occurs through the Vigenère square, resulting in a transformed ciphertext. This second layer adds a dynamic element to our cryptographic puzzle.
Columnar Transposition Cipher: The third and final layer involves the Columnar Transposition cipher. The Vigenère ciphertext is systematically rearranged into a grid, adhering to a specified columnar transposition key. Reading the columns of this grid in the order dictated by the key produces the ultimate ciphertext. This sequential application of ciphers in a product fashion contributes to the overall robustness of our cryptographic algorithm.
*/
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <set>
#include <numeric>
#include <map>
using namespace std;
vector<vector<char>> createPlayfairMatrix(const string& key) {
    vector<vector<char>> matrix(5, vector<char>(5, 0));
    set<char> usedChars;
    int row = 0, col = 0;
    for (char ch : key) {
        ch = toupper(ch);
        if (ch == 'J') 
        {
            ch = 'I';
        }
        if (usedChars.find(ch) == usedChars.end()) {
            matrix[row][col] = ch;
            usedChars.insert(ch);
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }
    for (char ch = 'A'; ch <= 'Z'; ++ch) {
        if (ch == 'J') continue;
        if (usedChars.find(ch) == usedChars.end()) {
            matrix[row][col] = ch;
            col++;
            if (col == 5) {
                col = 0;
                row++;
            }
        }
    }
    return matrix;
}
string playfair_decrypt(const string& cipherText, const string& keyword) {
    vector<vector<char>> matrix = createPlayfairMatrix(keyword);
    string decryptedText;
    int length = cipherText.length();
    for (int i = 0; i < length; i += 2) {
        char first = cipherText[i];
        char second = cipherText[i + 1];

        int row1, col1, row2, col2;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (matrix[row][col] == first) {
                    row1 = row;
                    col1 = col;
                }
                if (matrix[row][col] == second) {
                    row2 = row;
                    col2 = col;
                }
            }
        }
        if (row1 == row2) {
            decryptedText += matrix[row1][(col1 - 1 + 5) % 5];
            decryptedText += matrix[row2][(col2 - 1 + 5) % 5];
        } else if (col1 == col2) {
            decryptedText += matrix[(row1 - 1 + 5) % 5][col1];
            decryptedText += matrix[(row2 - 1 + 5) % 5][col2];
        } else {
            decryptedText += matrix[row1][col2];
            decryptedText += matrix[row2][col1];
        }
    }
    return decryptedText;
}

string vigenere_decrypt(const string& ciphertext, const string& keyword) {
    string decryptedText;
    int textLength = ciphertext.length();
    int keywordLength = keyword.length();

    for (int i = 0; i < textLength; ++i) {
        char ciphertextChar = ciphertext[i];
        char keywordChar = keyword[i % keywordLength];
        if (isupper(ciphertextChar)) {
            char decryptedChar = 'A' + (ciphertextChar - keywordChar + 26) % 26;
            decryptedText += decryptedChar;
        } else if (islower(ciphertextChar)) {
            char decryptedChar = 'a' + (ciphertextChar - keywordChar + 26) % 26;
            decryptedText += decryptedChar;
        } else {
            decryptedText += ciphertextChar;
        }
    }

    return decryptedText;
}
string transposition_decrypt(const string& ciphertext, const string& keyword) {
    int cipher_len = ciphertext.length();
    int key_len = keyword.length();
    int rows = (cipher_len + key_len - 1) / key_len;

    char matrix[rows][key_len];
    vector<int> key_indices(key_len);
    iota(key_indices.begin(), key_indices.end(), 0);
    sort(key_indices.begin(), key_indices.end(), [&](int a, int b) {
        return keyword[a] < keyword[b];
    });

    int index = 0;
    for (int j : key_indices) {
        for (int i = 0; i < rows; i++) {
            matrix[i][j] = (index < cipher_len) ? ciphertext[index++] : 'X';
        }
    }

    string result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key_len; j++) {
            if (matrix[i][j] != 'X') {
                result += matrix[i][j];
            }
        }
    }

    return result;
}

string decrypt_algo(const string& playfair_key, const string& vigenere_key,
                              const string& transposition_key, const string& ciphertext) {
    string transposition_result = transposition_decrypt(ciphertext, transposition_key);
    string vigenere_result = vigenere_decrypt(transposition_result, vigenere_key);
    string plaintext = playfair_decrypt(vigenere_result, playfair_key);

    return plaintext;
}


int main() {

    string playfair_key;
    string vigenere_key;
    string transposition_key;
    string cipher_text;
    cin>>playfair_key;
    cin.ignore(); 
    cin>>vigenere_key;
    cin.ignore(); 
    cin>>transposition_key;
    cin.ignore(); 
    cin>>cipher_text;
    string plaintext = decrypt_algo(playfair_key, vigenere_key, transposition_key, cipher_text);
    for (char& c : plaintext) {
        cout << char(toupper(c));
    }
    return 0;
}
