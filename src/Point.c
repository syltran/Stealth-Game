#include "../include/Point.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* Calcule la distance euclidienne entre les points 'a' et 'b' */
double distance(Point a, Point b){
    double dx, dy;
    dx = b.x - a.x;
    dy = b.y - a.y;
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

/* Renvoie un entier aléatoire entre les entiers 'a' et 'b' (tous deux compris) */
int random_int(int a, int b){
    return rand() % (b - a + 1) + a;
}

/* Renvoie un double aléatoire entre les entiers 'a' et 'b' (tous deux compris) */
double random_double(int a, int b){
    int n, nb_decimal;
    nb_decimal = 1;
    n = pow(10, nb_decimal);
    a *= n;
    b *= n;
    return (random_int(a, b)) / (double) n;
}

/* Renvoie un point aléatoire.
 * La coordonnée x (resp. y) du point est entre 'x1' et 'x2' (resp. 'y1' et 'y2').
*/
Point random_point(int x1, int x2, int y1, int y2){
    Point point;
    point.x = random_double(x1, x2);
    point.y = random_double(y1, y2);
    return point;
}