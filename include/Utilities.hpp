/**
 * @file UnicodeHash.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Funções auxiliares para manipulação e hashing de strings Unicode usando a biblioteca ICU
 * @version 0.1
 * @date 10-09-2024
 *
 * Este arquivo contém definições para a estrutura `hash_unicode` que fornece uma função de hash para
 * strings Unicode, bem como uma sobrecarga do operador de saída para `icu::UnicodeString`.
 */

#pragma once

#include <unicode/coll.h>
#include <unicode/uchar.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <iostream>

/**
 * @namespace icu_74
 * @brief Namespace para funções auxiliares de manipulação de strings Unicode.
 *
 * Este namespace contém funções e operadores auxiliares para trabalhar com strings Unicode
 * utilizando a biblioteca ICU. Atualmente, inclui uma sobrecarga do operador de inserção
 * para a classe `icu::UnicodeString`.
 */

namespace icu_75 {
std::ostream& operator<<(std::ostream& os, const UnicodeString& u_string) {
    std::string str;
    u_string.toUTF8String(str);
    os << str;
    return os;
}
}  // namespace icu_75

/**
 * @brief Estrutura que fornece uma função de hash para strings Unicode.
 *
 * Esta estrutura implementa uma função de hash para objetos da classe `icu::UnicodeString` usando
 * a conversão para `std::string` codificada em UTF-8, e em seguida aplica a função de hash padrão
 * para `std::string`.
 */
struct hash_unicode {
    /**
     * @brief Calcula o hash para uma string Unicode.
     *
     * Esta função converte a string Unicode (`icu::UnicodeString`) para uma `std::string` UTF-8 e
     * usa a função de hash padrão para `std::string` para calcular o valor de hash.
     *
     * @param ustr string Unicode a ser hasheada
     * @return size_t - valor do hash
     */
    size_t operator()(const icu::UnicodeString& ustr) const {
        std::string str;
        ustr.toUTF8String(str);
        std::hash<std::string> hash_fn;
        return hash_fn(str);
    }
};
