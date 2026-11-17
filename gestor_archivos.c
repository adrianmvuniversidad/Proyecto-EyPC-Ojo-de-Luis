#include "gestor_archivos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void registrar_error(struct ErrorEnsamblador *error, enum CodigoError codigo, size_t linea,const char *mensaje){
    if (!error) return;
    error->codigo = codigo;
    error->numero_linea = linea;
    snprintf(error->mensaje, MAX_MENSAJE_ERROR, "%s", mensaje);
}

void inicializar_fuente(struct ListaLineasFuente *fuente){
    if (!fuente) return;
    fuente->lineas = NULL;
    fuente->cantidad = 0;
    fuente->capacidad = 0;
}

static int agregar_linea(struct ListaLineasFuente *fuente, const char *texto, size_t numero_linea) {
    if (fuente->cantidad == fuente->capacidad) {
        size_t nueva_cap = (fuente->capacidad == 0) ? 16 : fuente->capacidad * 2;
        struct LineaFuente *nuevo_arr = realloc(
            fuente->lineas, nueva_cap * sizeof(struct LineaFuente));
        if (!nuevo_arr) {
            return -1;
        }
        fuente->lineas = nuevo_arr;
        fuente->capacidad = nueva_cap;
    }

    size_t len = strlen(texto);
    char *copia = malloc(len + 1);
    if (!copia) {
        return -1;
    }
    memcpy(copia, texto, len + 1);

    fuente->lineas[fuente->cantidad].texto = copia;
    fuente->lineas[fuente->cantidad].numero_linea = numero_linea;
    fuente->cantidad++;
    return 0;
}
