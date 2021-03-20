#pragma once
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>
#include <algorithm>
#include <bitset>

class LZW
{
private:
    LZW();
    ~LZW();

public:

    static std::string compress(const std::string &input, unsigned n_bits = 12)
    {
        // Dicionário
        std::unordered_map<std::string, unsigned> dictionary;
        // Próximo valor disponível no dicionário
        unsigned current_code;
        // Tamanho do simbolo atual
        unsigned bits_size = 8;
        // Inicialização do dicionário com os caracteres da tabela ASCII
        for (current_code = 0; current_code < 256; current_code++)
        {
            dictionary[std::string(1, current_code)] = current_code;
        }
        // Inicializa string atual e string comprimida
        std::string current_string;
        // Inicializa vetor de bits que representa a string comprimida
        std::vector<std::bitset<1>> output;
        for (auto &&c : input)
        {
            // Adiciona o proximo char à string selecionada
            current_string += c;

            // Se a string atual não estiver mapeada no dicionário
            if (dictionary.find(current_string) == dictionary.end())
            {
                // Se o dicionário não estiver cheio
                if (current_code < std::pow(2, n_bits))
                {
                    // Mapeia a string atual ao próximo código disponível no dicionário
                    dictionary[current_string] = current_code++;
                    // Altera a larguda do símbolo em 1 bit caso necessário.
                    // Nessa posição evita early change.
                    if (current_code >= std::pow(2, bits_size))
                        bits_size++;
                }
                // Remove o último char da string atual
                current_string.erase(current_string.size() - 1);
                // Pega o código da string atual
                unsigned current_string_code = dictionary[current_string];

                // Inicializa um vetor de bits para armazenar o símbolo
                std::vector<std::bitset<1>> symbol;
                // Converte de decimal para binário e insere no vetor
                while (current_string_code != 0)
                {
                    symbol.push_back(std::bitset<1>(current_string_code % 2));
                    current_string_code /= 2;
                }
                // Completa com zeros à esquerda caso necessário
                // Menos para os dois primeiros caracteres
                if (output.size() >= 16)
                {
                    unsigned r = symbol.size() % bits_size;
                    if (r != 0)
                        for (unsigned i = 0; i < bits_size - r; i++)
                            symbol.push_back(std::bitset<1>(0));
                }
                else
                {
                    unsigned r = symbol.size() % 8;
                    if (r != 0)
                        for (unsigned i = 0; i < 8 - r; i++)
                            symbol.push_back(std::bitset<1>(0));
                }
                // Inverte o símbolo e insere na saída
                std::reverse(symbol.begin(), symbol.end());
                output.insert(output.end(), symbol.begin(), symbol.end());
                // Torna a string atual somente o char
                current_string = c;
            }
        }
        // Se a string atual não for vazia
        if (!current_string.empty())
        {
            // Pega o código da string atual
            unsigned current_string_code = dictionary[current_string];
            // Inicializa um vetor de bits para armazenar o símbolo
            std::vector<std::bitset<1>> symbol;
            // Converte de decimal para binário e insere no vetor
            while (current_string_code != 0)
            {
                symbol.push_back(std::bitset<1>(current_string_code % 2));
                current_string_code /= 2;
            }
            // Completa com zeros à esquerda caso necessário
            if (output.size() >= 16)
            {
                unsigned r = symbol.size() % bits_size;
                if (r != 0)
                    for (unsigned i = 0; i < bits_size - r; i++)
                        symbol.push_back(std::bitset<1>(0));
            }
            else
            {
                unsigned r = symbol.size() % 8;
                if (r != 0)
                    for (unsigned i = 0; i < 8 - r; i++)
                        symbol.push_back(std::bitset<1>(0));
            }
            // Inverte o símbolo e insere na saída
            std::reverse(symbol.begin(), symbol.end());
            output.insert(output.end(), symbol.begin(), symbol.end());
        }
        // Completa a vetor de bits com bits nulos para ser byte-aligned
        unsigned r = output.size() % 8;
        if (r != 0)
            for (unsigned i = 0; i < 8 - r; i++)
                output.push_back(std::bitset<1>(0));
        // Converte o vetor de bits para string
        std::string compressed_string;
        for (unsigned i = 0; i < output.size() / 8; i++)
        {
            std::string byte;
            for (unsigned j = 0; j < 8; j++)
            {
                byte.append(output[(8 * i) + j].to_string());
            }
            unsigned char c = std::stoi(byte, 0, 2);
            compressed_string.push_back(c);
        }
        return compressed_string;
    }
};