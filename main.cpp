#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <exception>
#include "LZW.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    try
    {
        if (argc != 2)
            throw invalid_argument("Parâmetro do programa inválido");
        string filename = argv[1];
        ifstream input_file(argv[1]);
        if (!input_file)
            throw invalid_argument("Arquivo não encontrado");
        string input((istreambuf_iterator<char>(input_file)),
                     istreambuf_iterator<char>());
        cout << "Entrada:" << endl;
        cout << input.size() << " bytes" << endl
             << endl;
        string output = LZW::compress(input);
        cout << "Imprimir saída na tela ou em arquivo?" << endl;
        cout << "1 - Imprimir na tela" << endl;
        cout << "2 - Imprimir em arquivo" << endl;
        int i;
        cin >> i;
        switch (i)
        {
        case 1:
            cout << "Saída:" << endl;
            cout << output.size() << " bytes" << endl;
            cout << output << endl;
            for (auto &&c : output)
            {
                cout << bitset<8>(c).to_string() << " ";
            }
            cout << endl;
            break;
        case 2:
        {
            ofstream output_file(filename+".lzw");
            cout << "Arquivo de saída " << filename+".lzw" << " criado" << endl;
            output_file << output;
            break;
        }
        default:
            throw invalid_argument("Opção inválida");
            break;
        }
        cout << "Fator de compressão: " << (float)input.size() / output.size() << endl
         << endl;
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
        return 0;
    }
}