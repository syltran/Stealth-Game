#include "../include/Disc.h"
#include <stdio.h>


/* Renvoie 1 si les disques 'd1' et 'd2' se touchent ou se superposent, 0 sinon */
int superposition_between_discs(Disc d1, Disc d2){
    return distance(d1.center, d2.center) - d1.radius - d2.radius <= 0;
}

/* Renvoie 1 si le point 'a' est à l'intérieur du disque 'disc', 0 sinon */
int point_inside_disc(Point a, Disc disc){
    return distance(a, disc.center) < disc.radius;
}