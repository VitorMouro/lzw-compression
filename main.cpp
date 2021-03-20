#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include "LZW.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    string input;
    cout << "Digite a string a ser comprimida: ";
    std::getline(cin, input);
    cout << "Entrada:" << endl;
    cout << input.size() << " bytes" << endl << endl;
    cout << "Saída:" << endl;
    string output = LZW::compress(input);
    cout << output.size() << " bytes" << endl;
    for (auto &&c : output)
    {
        cout << bitset<8>(c).to_string() << " ";
    }
    cout << endl << endl;
    cout << "Fator de compressão: " << (float)input.size()/output.size() << endl << endl;
    
}