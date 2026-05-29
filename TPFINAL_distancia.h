#ifndef DISTANCIA_H
#define DISTANCIA_H

double distancia_xy(double x1, double y1, double x2, double y2);
// Precondición: Los parámetros `x1`, `y1`, `x2` y `y2` representan las coordenadas de dos puntos en el plano.
// Postcondición: Devuelve la distancia euclidiana entre los dos puntos.

double distancia_p_segmento(double xd, double yd, double x1, double y1, double x2, double y2);
// Precondición: Los parámetros `xd` y `yd` representan las coordenadas de un punto en el plano.
// Los parámetros `x1`, `y1`, `x2` y `y2` representan las coordenadas de dos puntos que definen un segmento en el plano.
// Postcondición: Calcula la distancia perpendicular entre el punto (`xd`, `yd`) y el segmento definido por los puntos (`x1`, `y1`) y (`x2`, `y2`).
// Devuelve la distancia perpendicular entre el punto y el segmento.

#endif