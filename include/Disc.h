#ifndef __DISC__
#define __DISC__

#include "Point.h"

typedef struct{
    Point center; /* Coordonnées du centre */
    int radius; /* Rayon */
} Disc; /* Représente un disque */

int superposition_between_discs(Disc d1, Disc d2);
int point_inside_disc(Point a, Disc disc);

#endif