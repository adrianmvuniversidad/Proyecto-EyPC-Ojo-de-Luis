#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include "tipos.h"

void inicializar_fuente(struct ListaLineasFuente *fuente);

int cargar_fuente(const char *ruta, struct ListaLineasFuente *fuente, struct ErrorEnsamblador *error);

int escribir_imagen_binaria(const char *ruta, const struct ImagenBinaria *imagen, struct ErrorEnsamblador *error);

void liberar_fuente(struct ListaLineasFuente *fuente);

#endif /* GESTOR_ARCHIVOS_H */
