#include "gestor_archivos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void registrar_error(struct ErrorEnsamblador *error,
                            enum CodigoError codigo,
                            size_t linea,
                            const char *mensaje) {
    if (!error) return;
    error->codigo = codigo;
    error->numero_linea = linea;
    snprintf(error->mensaje, MAX_MENSAJE_ERROR, "%s", mensaje);
}

void inicializar_fuente(struct ListaLineasFuente *fuente) {
    if (!fuente) return;
    fuente->lineas = NULL;
    fuente->cantidad = 0;
    fuente->capacidad = 0;
}

static int agregar_linea(struct ListaLineasFuente *fuente,
                         const char *texto,
                         size_t numero_linea) {
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

int cargar_fuente(const char *ruta,
                  struct ListaLineasFuente *fuente,
                  struct ErrorEnsamblador *error) {
    if (!ruta || !fuente) {
        registrar_error(error, ERROR_ARGUMENTOS, 0, "Ruta o estructura fuente nula.");
        return -1;
    }

    inicializar_fuente(fuente);

    FILE *archivo = fopen(ruta, "r");
    if (!archivo) {
        registrar_error(error, ERROR_ARCHIVO, 0, "No se pudo abrir el archivo fuente.");
        return -1;
    }

    size_t cap_buf = 256;
    char *buffer = malloc(cap_buf);
    if (!buffer) {
        fclose(archivo);
        registrar_error(error, ERROR_MEMORIA, 0, "Sin memoria para buffer de lectura.");
        return -1;
    }

    size_t num_linea = 1;
    size_t pos = 0;
    int ch;

    while ((ch = fgetc(archivo)) != EOF) {
        if (ch == '\r') {
            continue;
        }
        if (ch == '\n') {
            buffer[pos] = '\0';
            if (agregar_linea(fuente, buffer, num_linea) != 0) {
                free(buffer);
                fclose(archivo);
                liberar_fuente(fuente);
                registrar_error(error, ERROR_MEMORIA, num_linea, "Error al reservar memoria para linea.");
                return -1;
            }
            pos = 0;
            num_linea++;
        } else {
            if (pos + 1 >= cap_buf) {
                size_t nueva_cap = cap_buf * 2;
                char *tmp = realloc(buffer, nueva_cap);
                if (!tmp) {
                    free(buffer);
                    fclose(archivo);
                    liberar_fuente(fuente);
                    registrar_error(error, ERROR_MEMORIA, num_linea, "Linea demasiado larga.");
                    return -1;
                }
                buffer = tmp;
                cap_buf = nueva_cap;
            }
            buffer[pos++] = (char)ch;
        }
    }

    if (pos > 0 || (num_linea == 1 && pos == 0)) {
        buffer[pos] = '\0';
        if (agregar_linea(fuente, buffer, num_linea) != 0) {
            free(buffer);
            fclose(archivo);
            liberar_fuente(fuente);
            registrar_error(error, ERROR_MEMORIA, num_linea, "Error al reservar memoria para ultima linea.");
            return -1;
        }
    }

    free(buffer);
    fclose(archivo);
    return 0;
}

/* Pendiente para la etapa de generacion de codigo binario */
int escribir_imagen_binaria(const char *ruta,
                            const struct ImagenBinaria *imagen,
                            struct ErrorEnsamblador *error) {
    (void)ruta;
    (void)imagen;
    registrar_error(error, ERROR_NINGUNO, 0, "Escritura binaria pendiente para etapa posterior.");
    return 0;
}

void liberar_fuente(struct ListaLineasFuente *fuente) {
    if (!fuente) return;
    for (size_t i = 0; i < fuente->cantidad; i++) {
        free(fuente->lineas[i].texto);
    }
    free(fuente->lineas);
    fuente->lineas = NULL;
    fuente->cantidad = 0;
    fuente->capacidad = 0;
}
