/* Contador de palavras
 *
 * Este programa recebera uma serie de caracteres representando palavras em sua
 * entrada. Ao receber um caractere fim de linha ('\n'), deve imprimir na tela o
 * numero de palavras separadas que recebeu e, apos, encerrar.
 */

/*
 * Rafael Santos - 186154
 *
 * Suponho notação americana (ponto como separador decimal).
 * Também suponho que não haverá separador de milhar (que seria a vírgula na
 * notação americana).
 */

#include <stdio.h>
#include <stdlib.h>


/* Definimos as constantes dos estados */
#define ESPACO              0
#define INICIO_DIG_FLOAT    1       // "DIG" pois começa com digito
#define FIM_FLOAT           2
#define INICIO_SIG_FLOAT    3       // "SIG" pois começa com sinal
#define DECIMAL_SEP_FLOAT   4       // "SEP" pois é o separador decimal
#define DECIMAL_FLOAT       5       // parte decimal do float
#define CANCELAR            6       // recebemos algo esquisito


/*
 * Funções auxiliares de identificação de caractere
 */

int is_digito(char c) {
    return c >= '0' && c <= '9';
}

int is_terminador(char c) {
    return c == '\0' || c == '\n' || c == '\r';
}

int is_espaco(char c) {
    return c == ' ' || c == '\t';
}

int is_sinal(char c) {
    return c == '-' || c == '+';
}

int is_separador_decimal(char c) {
    return c == '.';
}


/*
 * Máquina de estado
 */

int get_novo_estado(int estado_atual, char input) {
    switch (estado_atual) {
        case ESPACO:
        case FIM_FLOAT:
            /* Se estamos num espaço ou no início de tudo, estamos em ponto de
             * aceitar novo float */
            if (is_digito(input)) {
                return INICIO_DIG_FLOAT;
            } else if (is_sinal(input)) {
                return INICIO_SIG_FLOAT;
            } else if (is_espaco(input) || is_terminador(input)) {
                return ESPACO;
            }

            return CANCELAR;

        case INICIO_DIG_FLOAT:
            /* Se já recebemos dígitos, estamos fazendo a parte inteira do
             * float, podemos terminar agora, encontrar um ponto ou continuar */
            if (is_digito(input)) {
                return INICIO_DIG_FLOAT;
            } else if (is_separador_decimal(input)) {
                return DECIMAL_SEP_FLOAT;
            } else if (is_espaco(input) || is_terminador(input)) {
                return FIM_FLOAT;
            }

            return CANCELAR;

        case INICIO_SIG_FLOAT:
            /* Se recebemos apenas um sinal, só há um caminho para um float:
             * receber dígitos. Se encontrarmos um espaço, cancelamos sem
             * gerar float */
            if (is_digito(input)) {
                return INICIO_DIG_FLOAT;
            }
        case DECIMAL_SEP_FLOAT:
            /* Se encontramos dígitos e um ponto, o único caminho para um float
             * é receber mais dígitos. Se encontrarmos um espaço, cancelamos
             * sem gerar float */
            if (is_digito(input)) {
                return DECIMAL_FLOAT;
            }

            /* === Comum aos 2 casos === */
            if (is_espaco(input) || is_terminador(input)) {
                return ESPACO;
            }

            return CANCELAR;

        case DECIMAL_FLOAT:
            /* Se estamos recebendo a parte decimal, podemos apenas receber
             * dígito (continuar) ou espaço (terminar float) */
            if (is_digito(input)) {
                return DECIMAL_FLOAT;
            } else if (is_espaco(input) || is_terminador(input)) {
                return FIM_FLOAT;
            }

            return CANCELAR;

        case CANCELAR:
            /* Se recebemos algum caractere esquisito, continuamos até encontrar
             * um espaço */
            if (is_espaco(input) || is_terminador(input)) {
                return ESPACO;
            }

            return CANCELAR;

        default:
            /* Se estamos num estado desconhecido, algo de errado não está
             * certo */
            fprintf(
                stderr,
                "Falha ao identificar estado atual: %d\n", estado_atual
            );
            exit(1);
    }
}


/*
 * Função principal
 */

int main() {

    /* Lemos a entrada */
    char input[1024];
    char c;
    int length = 0;
    do {
        scanf("%c", &c);
        input[length++] = c;
    } while (c != '\n' && c != '\0' && length < 1023);
    input[length] = '\0';

    /* Inicializamos nossa máquina de estado */
    int estado_atual = 0;

    /* Variável para guardar o início do float */
    int posicao_inicio_float = -1;
    /* Variável para guardar a soma de floats */
    float soma = 0;

    /* Passamos para a máquina de estado a entrada */
    for (int i = 0; i < length; i++) {
        estado_atual = get_novo_estado(estado_atual, input[i]);
        float f;

        switch (estado_atual) {
            /* Verificamos se terminamos um float */
            case FIM_FLOAT:
                sscanf(&(input[posicao_inicio_float]), "%f", &f);
                soma += f;
                posicao_inicio_float = -1;
                break;

            /* Verificamos se iniciamos um float (por digito ou sinal) */
            case INICIO_DIG_FLOAT:
            case INICIO_SIG_FLOAT:
                if (posicao_inicio_float < 0) {
                    posicao_inicio_float = i;
                }
                break;

            /* Verificamos se não é um float */
            case CANCELAR:
            case ESPACO:
                /* Cancelamos a posição inicial */
                posicao_inicio_float = -1;
                break;
        }

    }

    /* Imprimimos a soma dos floats encontrados */
    printf("%g\n", soma);

    return 0;
}


/*
 * Expressão regular para floats em texto
 * regexr.com/49d5l
 *
 * Problema: a sintaxe de expressão regular parece não ser igual a do site,
 * sinto que gastarei muito tempo aprendendo a sintaxe da regex.h, então irei
 * fazer com máquina de estados mesmo.
 */

// #include <regex.h>

// int main() {

//     /* Lemos a entrada */
//     char input[1024];
//     char c;
//     int i = 0;
//     do {
//         scanf("%c", &c);
//         input[i] = c == '\n' ? '\0' : c;
//         i++;
//     } while (c != '\n' && i < 1024);

//     /* Preparamos a expressão regular */
//     regex_t exp_regular;
//     if (regcomp(&exp_regular, "(?<!\\S)([-]{0,1}[0-9]+\\.[0-9]+|[-]{0,1}[0-9]+)(?!\\S)", 0)) {
//         fprintf(stderr, "Falha ao compilar expressão regular!\n");
//         return 1;
//     }

//      Testamos a expressão regular
//     int retorno = regexec(&exp_regular, input, 0, NULL, 0);
//     if(!retorno) {
//         printf("Match\n");
//     } else if(retorno == REG_NOMATCH) {
//         printf("Sem match (input=\"%s\").\n", input);
//     } else {
//         char erro[256];
//         regerror(retorno, &exp_regular, erro, sizeof(erro));
//         fprintf(stderr, "Expressão regular falhou: %s\n", erro);
//         return 2;
//     }

//     /* Liberamos a expressão regular */
//     regfree(&exp_regular);

//     return 0;
// }
