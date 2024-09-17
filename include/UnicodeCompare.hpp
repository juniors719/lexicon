/**
 * @file UnicodeCompare.hpp
 * @author Júnior Silva (junior.silva@alu.ufc.br)
 * @brief Classe que compara duas strings Unicode usando a biblioteca ICU
 * @version 0.1
 * @date 10-09-2024
 *
 * Esta classe fornece uma maneira de comparar duas strings Unicode de acordo com a
 * configuração de collation (ordenação) da biblioteca ICU (International Components for Unicode).
 * A comparação é feita com base na ordenação local apropriada para Unicode.
 */

#pragma once

#include <unicode/coll.h>
#include <unicode/unistr.h>

/**
 * @brief Estrutura que fornece uma função de comparação para strings Unicode.
 *
 * Esta estrutura usa a classe `icu::Collator` da biblioteca ICU para comparar strings Unicode de
 * acordo com as regras de collation definidas. A comparação é feita lexicograficamente.
 */
struct UnicodeCompare {
    icu::Collator* collator;  // Ponteiro para o objeto `icu::Collator` usado para comparação.

    /**
     * @brief Constrói um novo objeto `UnicodeCompare`.
     *
     * O construtor cria uma instância do collator usando as configurações padrão. Se a criação falhar,
     * a variável `collator` será um ponteiro nulo.
     */
    UnicodeCompare() {
        UErrorCode status = U_ZERO_ERROR;
        collator = icu::Collator::createInstance(status);
        if (U_FAILURE(status)) {
            // Manejo de erro: pode lançar uma exceção ou definir um comportamento padrão
            collator = nullptr;
        }
    }

    /**
     * @brief Destrói o objeto `UnicodeCompare`.
     *
     * O destruidor libera a memória alocada para o collator.
     */
    ~UnicodeCompare() {
        delete collator;
    }

    /**
     * @brief Compara duas strings Unicode.
     *
     * Este operador sobrecarregado compara duas instâncias de `icu::UnicodeString` usando o collator.
     * O resultado é verdadeiro se a string `lhs` for lexicograficamente menor que a string `rhs`.
     *
     * @param lhs String Unicode à esquerda da comparação.
     * @param rhs String Unicode à direita da comparação.
     * @return true se `lhs` for menor que `rhs`; caso contrário, retorna false.
     */
    bool operator()(const icu::UnicodeString& lhs, const icu::UnicodeString& rhs) const {
        if (collator == nullptr) {
            // Manejo de erro: collator não foi inicializado corretamente
            throw std::runtime_error("Collator not initialized.");
        }
        UErrorCode status = U_ZERO_ERROR;
        return collator->compare(lhs, rhs, status) < 0;
    }
};
