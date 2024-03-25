#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <vector>
using namespace std;

struct RailFenceData {
    vector<vector<char>> fence;
    vector<int> railIndices;
    int index;
};

string straddle_checker_decrypt(const string &cipher, const map<char, string> &table, const char *first, const char *second, const char *third, int rowU, int rowV)
{
    string decrypted_text;
    int state = 0;
    for (int i = 0; i < cipher.size(); ++i)
    {
        int n = cipher[i] - '0';
        char next = 0;

        if (state == 1)
            next = second[n];
        else if (state == 2)
            next = third[n];
        else if (state == 3)
            next = cipher[i];
        else if (n == rowU)
            state = 1;
        else if (n == rowV)
            state = 2;
        else
            next = first[n];

        if (next == '/')
            state = 3;
        else if (next != 0)
        {
            state = 0;
            decrypted_text += next;
        }
    }
    return decrypted_text;
}

RailFenceData initializeRailFence(int key, const string &ciphertext) {
    RailFenceData data;
    data.fence.resize(key, vector<char>(ciphertext.length(), '\0'));
    for (int i = 0; i < key - 1; ++i)
        data.railIndices.push_back(i);
    for (int i = key - 1; i > 0; --i)
        data.railIndices.push_back(i);
    data.index = 0;
    return data;
}
void fillRailFenceCharacters(RailFenceData &data, const string &ciphertext,int key) {
    for (int i = 0; i < key; ++i) {
        for (int j = 0; j < ciphertext.length(); ++j) {
            if (data.fence[i][j] == '*' && data.index < ciphertext.length()) {
                data.fence[i][j] = ciphertext[data.index++];
            }
        }
    }
}

string rail_fence_decrypt(const string& ciphertext, int key) {
    RailFenceData data = initializeRailFence(key, ciphertext);
    for (int i = 0; i < ciphertext.length(); ++i)
        data.fence[data.railIndices[i % data.railIndices.size()]][i] = '*';
    fillRailFenceCharacters(data, ciphertext,key);
    string decryptedText;
    for (int i = 0; i < ciphertext.length(); ++i) {
        decryptedText += data.fence[data.railIndices[i % data.railIndices.size()]][i];
    }
    return decryptedText;
}

int main() {
    string key_straddle, ciphertext;
    int key_transposition;
    int u,v;
    getline(cin, key_straddle);
    cin>>u>>v;
    cin.ignore();
    getline(cin, ciphertext);
    cin >> key_transposition;
    
    map<char, string> table;
    char first[10], second[10], third[10];
    int rowU = min(u, v);
    int rowV = max(u, v);

    for (int i = 0, j = 0; i < 10; ++i)
    {
        if (i != u && i != v)
        {
            first[i] = key_straddle[j];
            table[key_straddle[j]] = '0' + i;
            ++j;
        }

        second[i] = key_straddle[i + 8];
        table[key_straddle[i + 8]] = '0' + rowU;
        table[key_straddle[i + 8]] += '0' + i;

        third[i] = key_straddle[i + 18];
        table[key_straddle[i + 18]] = '0' + rowV;
        table[key_straddle[i + 18]] += '0' + i;
    }
    
    string cipher2=rail_fence_decrypt(ciphertext, key_transposition);
    string decrypted_text = straddle_checker_decrypt(cipher2,table,first,second,third,u,v);

    cout << decrypted_text << endl;

    return 0;
}
