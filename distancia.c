#include <math.h>

#include "distancia.h"

double distancia_xy(double x1, double y1, double x2, double y2){
    return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

double distancia_p_segmento(double xd, double yd, double x1, double y1, double x2, double y2){
    double a = ((xd - x1) * (x2 - x1) + (yd - y1) * (y2 - y1))/((double)(x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
    double xp, yp;
        if(a < 1 && a > 0){
            xp = x1 + a * (x2 - x1);
            yp = y1 + a * (y2 - y1);
        } else if(a >= 1){
            xp = x2;
            yp = y2;
        } else{
            xp = x1;
            yp = y1;
        }
    return distancia_xy(xd, yd, xp, yp);
}