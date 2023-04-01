#ifndef __POINT__
#define __POINT__

typedef struct{
    double x, y;
} Point; /* Représente un point de coordonnées (x, y) */

double distance(Point a, Point b);
int random_int(int a, int b);
double random_double(int a, int b);
Point random_point(int x1, int x2, int y1, int y2);

#endif