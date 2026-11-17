#include <stdio.h>
#include "gestor_archivos.h"
#include "analizador.h"
#include "tablas.h"
#include "traductor.h"

int main(int argc, char *argv[]) {
    const char *ruta = (argc >= 2) ? argv[1] : "tests/entradas/ejemplo.asm";

    struct ErrorEnsamblador error = {ERROR_NINGUNO, 0, ""};
    struct ListaLineasFuente fuente;
    ListaTokens tokens;

    if (cargar_fuente(ruta, &fuente, &error) != 0) {
        fprintf(stderr, "Error [linea %zu]: %s\n", error.numero_linea, error.mensaje);
        return 1;
    }

    printf("=== ARCHIVO FUENTE LEIDO (%zu lineas) ===\n", fuente.cantidad);
    for (size_t i = 0; i < fuente.cantidad; i++) {
        printf("%2zu | %s\n", fuente.lineas[i].numero_linea, fuente.lineas[i].texto);
    }

    if (analizar_fuente_lexico(&fuente, &tokens, &error) != 0) {
        fprintf(stderr, "Error lexico [linea %zu]: %s\n", error.numero_linea, error.mensaje);
        liberar_fuente(&fuente);
        return 1;
    }

    printf("\n=== TOKENS IDENTIFICADOS (%zu tokens) ===\n", tokens.cantidad);
    printf("%-6s | %-7s | %-23s | %s\n", "Linea", "Columna", "Tipo", "Lexema");
    printf("-------+---------+-------------------------+-------------------------\n");
    for (size_t i = 0; i < tokens.cantidad; i++) {
        printf("%-6zu | %-7zu | %-23s | %s\n",
               tokens.datos[i].numeroLinea,
               tokens.datos[i].columna,
               nombre_tipo_token(tokens.datos[i].tipo),
               tokens.datos[i].lexema);
    }

    liberar_lista_tokens(&tokens);
    liberar_fuente(&fuente);
    return 0;
}
