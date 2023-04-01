#include "../include/Detection.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* calcule la position relative d'un point Pa par raport à l'abscisse (x = a) */
double position_Pa(int a, Point point_0, Point point_1 ){
    return (a - point_1.x ) / (point_0.x - point_1.x);
}

/* calcule la position relative d'un point Pb par raport à l'ordonnée (y = b) */
double position_Pb(int b, Point point_0, Point point_1 ){
    return (b - point_1.y ) / (point_0.y - point_1.y);
}

/* vérification : 1 si la position relative appartient au segment [P0,P1] sinon 0 */
int p_in_segment(double p){
    return (p >= 0 && p <= 1);
}

/* calcule de l'ordonnée(y) du point Pa */
double ordinate_Pa(int a, Point point_0, Point point_1 ){ 
    return (point_1.y + ((point_0.y - point_1.y) / (point_0.x - point_1.x))* (a - point_1.x));
}

/* calcule de l'abscisse(x) du point Pb */
double abscissa_Pb(int b, Point point_0, Point point_1 ){ 
    return (point_1.x + ((point_0.x - point_1.x) / (point_0.y - point_1.y))* (b - point_1.y));
}


/* trouve le point qu'a le x plus petit (le plus haut) */
Point x_higher(const Point point_1, const Point point_2){
    return (point_1.x < point_2.x)? point_1 : point_2;
}

/* trouve le point qu'a le x plus grand (le plus bas) */
Point x_lower(const Point point_1, const Point point_2){
    return (point_1.x < point_2.x)? point_2 : point_1;
}

/* trouve le point qu'a le y plus petit (le plus haut) */
Point y_higher(const Point point_1, const Point point_2){
    return (point_1.y < point_2.y)? point_1 : point_2;
}

/* trouve le point qu'a le y plus grand (le plus bas) */
Point y_lower(const Point point_1, const Point point_2){
    return (point_1.y < point_2.y)? point_2 : point_1;
}

/* copie un point dans un nouveau point */
Point copy_point(const Point point){
    Point p;
    p.x = point.x;
    p.y = point.y;
    return p;
}

/* case traversée à partir d'un point de croisement des droites avec un coté d'un carré par le segment [po, p1] */
Case point_to_case(double x, double y){
    Case c;
    c.col = floor(x / SIZE_CASE);
    c.lin = floor(y / SIZE_CASE);    
    return c;
}

/*p0 = 'center' et p1 = centre du gardien 'guard'
  1- trouve les points de croisement entre le segment [p0, p1] avec les axes
  2- les convertir en case + case adjacente
  3- renvoie 1 si l'une de ces cases est un mur, sinon 0 */
int wall_between_point_guard(Point center, const Guard *guard, const Grid *grid){
    int i, j;
    double pa, ya, pb, yb;
    Point point_highest, point_lower;

    /* les points du croisement entre abscisse et segment x = a (= i) */
    point_highest = copy_point( x_higher(center, guard->disc.center));
    point_lower = copy_point(x_lower(center, guard->disc.center));

    for ( i = floor(point_highest.x); i < floor(point_lower.x) + 1; i++ ){
        pa = position_Pa(i , point_highest, point_lower);
        if( p_in_segment(pa) ){
            ya = ordinate_Pa( i, point_highest, point_lower );
            /* si une case adjacente du point est mur */
            if(is_wall(grid, point_to_case(i, ya)) 
                || is_wall(grid, point_to_case(i - 1, ya))){
                return 1;
            }
        }
    }
    /* les points du croisement entre ordonnée et segment y = b (= j) */
    point_highest = copy_point( y_higher(center, guard->disc.center));
    point_lower = copy_point(y_lower(center, guard->disc.center));
    for ( j = floor(point_highest.y); j < floor(point_lower.y) + 1 ; j++ ){
        pb = position_Pb(j , point_highest, point_lower);
        if( p_in_segment(pb)){
            yb = abscissa_Pb(j , point_highest, point_lower);
            /* si une case adjacente du point est mur */
            if(is_wall(grid, point_to_case(yb, j)) 
                || is_wall(grid, point_to_case(yb, j - 1))){
                    return 1;
            }
        }
    }
    return 0;
}

/* vérifie si le centre de player est dans la zone de détection du guard */
int player_in_zone(const Player *player, const Guard *guard){
    double dist; /* distance entre le centre du player et le centre du guard */
    dist = distance(player->disc.center, guard->disc.center) ;
    if (dist <= ((guard->detection_dist) * SIZE_CASE)){
        return 1;
    }
    return 0;
}

/* renvoie 1 si le gardien 'guard' détecte qu'une relique a disparu dans la grille, sinon 0 */
int detect_missing_relics(const Guard *guard, Grid *grid){
    int i, j;
    Disc disc; /* disque avec pour rayon la distance de détection du gardien */
    Point middle;

    disc.center = guard->disc.center;
    disc.radius = guard->detection_dist * SIZE_CASE;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            /* pour chaque case, on calcule les coordonnées du milieu dans un point */
            Case coord = {i, j};
            middle.x = coord.col + (1 / (double) 2);
            middle.y = coord.lin + (1 / (double) 2);
            resize_point(&middle);
            if (point_inside_disc(middle, disc)){
                if (!wall_between_point_guard(middle, &(*guard), &(*grid))){
                    if (is_relic_taken(&(*grid), coord)){
                        /* on met cette relique comme étant déjà détectée */
                        grid->tab[coord.lin][coord.col] = RELIC_DETECT;
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}