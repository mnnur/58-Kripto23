#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void encrypt(string& str, string key){
    for(int i = 0, j = 0; i < str.length(); i++, j++){
        if(isalpha(str[i])){
            if(j >= key.length()){
                j = 0;
            }
            int shift = tolower(key[j]) - 'a';
            if (isupper(str[i])) {
                str[i] = ((str[i] - 'A' + shift) % 26 + 'A');
            } else {
                str[i] = ((str[i] - 'a' + shift) % 26 + 'a');
            }
        }
    }
}

void decrypt(string& str, string key){
    for(int i = 0, j = 0; i < str.length(); i++, j++){
        if(isalpha(str[i])){
            if(j >= key.length()){
                j = 0;
            }
            int shift = tolower(key[j]) - 'a';
            if (isupper(str[i])) {
                str[i] = ((str[i] - 'A' - shift + 26) % 26 + 'A');
            } else {
                str[i] = ((str[i] - 'a' - shift + 26) % 26 + 'a');
            }
        }
    }
}

int main(){
    string key = "";
    string str = "";

    cout << "Text to Encrypt : ";
    getline(cin, str);
    cout << "Key : ";
    cin >> key;

    encrypt(str, key);
    cout << "Encrypted Text : " << str << "\n";
    decrypt(str, key);
    cout << "Decrypted Text : " << str;

}