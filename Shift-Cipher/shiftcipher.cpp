#include <iostream>
#include <string>

using namespace std;

void encrypt(string& str, int shift){
        for(int i = 0; i < str.length(); i++){
        if(isalpha(str[i])){
            char base = islower(str[i]) ? 'a' : 'A';
            str[i] = (str[i] - base + shift) % 26 + base;
        }
    }
}

void decrypt(string& str, int shift){
        for(int i = 0; i < str.length(); i++){
        if(isalpha(str[i])){
            char base = islower(str[i]) ? 'a' : 'A';
            str[i] = (str[i] - base - shift + 26) % 26 + base;
        }
    }
}

int main(){
    int shift = 0;
    string str = "";

    cout << "Text to Encrypt : ";
    getline(cin, str);
    cout << "Shift by : ";
    cin >> shift;

    encrypt(str, shift);
    cout << "Encrypted Text : " << str << "\n";
    decrypt(str, shift);
    cout << "Decrypted Text : " << str;

}