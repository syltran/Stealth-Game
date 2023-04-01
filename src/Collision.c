#include "../include/Collision.h"
#include <stdio.h>
#include <math.h>


/* Détermine et récupère les 3 cases ('front', 'front_side1' et 'front_side2') en face de l'agent 'agent' 
 * suivant la direction du déplacement 'dir' et à une distance 'dist' de celui-ci.
 * Par exemple,
 * Si 'dist' = 0, les 3 cases récupérées sont celles immédiatements situées en face de l'agent 'agent'.
 * Si 'dist' = 1, ce sont les 3 cases situées 1 case plus loin qui sont récupérées
*/
void determine_front_cases(Disc agent, Direction dir, int dist, Case *front, Case *front_side1, Case *front_side2){
    Case coord; /* Case contenant le centre du disque 'agent' */
    int offset_lin[] = {-(1 + dist), 1 + dist, 0, 0};
    int offset_col[] = {0, 0, -(1 + dist), 1 + dist};
    int lin, col;
    coord = point_case(agent.center);

    /* Pour Haut et Bas, les 3 cases ont la même ligne. Seule la colonne change */
    if (dir == UP || dir == DOWN){
        lin = coord.lin + offset_lin[dir];
        front->lin = front_side1->lin = front_side2->lin = lin;
        front->col = coord.col; /* Case devant */
        front_side1->col = coord.col - 1; /* Case devant à gauche */
        front_side2->col = coord.col + 1; /* Case devant à droite */
    }

    /* Pour Gauche et Droite, les 3 cases ont la même colonne. Seule la ligne change */
    else if (dir == LEFT || dir == RIGHT){
        col = coord.col + offset_col[dir];
        front->lin = coord.lin; /* Case devant */
        front_side1->lin = coord.lin - 1; /* Case devant en haut */
        front_side2->lin = coord.lin + 1; /* Case devant en bas */
        front->col = front_side1->col = front_side2->col = col;
    }
}

/* Renvoie 1 si l'agent 'agent' risque d'entrer en collision avec un mur dans la grille 'grid' 
 * situé à distance 'dist' de celui-ci suivant sa direction 'dir', 0 sinon 
*/
int risk_collide_wall(Disc agent, Direction dir, int dist, const Grid *grid){
    Case f, f_s1, f_s2;
    determine_front_cases(agent, dir, dist, &f, &f_s1, &f_s2);
    return is_wall(&(*grid), f) || is_wall(&(*grid), f_s1) || is_wall(&(*grid), f_s2);
}


/* Redimensionne la coordonnée 'x' pour l'adapter aux dimensions "réduites" du terrain */
void resize_mini_coordinate(double *x){
    *x /= SIZE_CASE;
}

/* Redimensionne la coordonnée 'x' pour l'adapter aux dimensions "réelles" du terrain */
void resize_real_coordinate(double *x){
    *x *= SIZE_CASE;
}


/* Renvoie 1 si une limite de déplacement en y a été calculée pour l'agent 'agent', 0 sinon.
 * Si c'est le cas, cette limite est récupérée avec 'ymax'
*/
int travel_limit_in_y(Disc agent, Direction dir, int dist, const Grid *grid, double *ymax){
    Case f, f_s1, f_s2; /* Les 3 cases de devant à distance 'dist' du disque 'agent' suivant 'dir' */
    Case coord; /* Case contenant le centre du disque 'agent' */
    int x, y; /* Coordonnées du coin en haut à gauche de 'coord' */
    double x0; /* Coordonnée x du centre du disque 'agent' */
    double mid; /* Coordonnée du milieu du segment [⌊x0⌋, ⌊x0⌋ + 1] */
    double height; /* Hauteur (du triangle) à additionner ou à soustraire à 'ymax' */
    int limit; /* limit = 1 si une limite de déplacement a été calculée */
    
    determine_front_cases(agent, dir, dist, &f, &f_s1, &f_s2);
    coord = point_case(agent.center);
    x = coord.col;
    y = coord.lin;
    x0 = agent.center.x;
    mid = x + (1 / (double) 2);
    resize_real_coordinate(&mid);
    limit = 0;

    /* 1ER Cas: */
    if (is_wall(&(*grid), f_s1) && x0 < mid){
        resize_mini_coordinate(&x0);
        height = sqrt(1 / (double)4 - pow(x0 - x, 2));
        
        if (dir == DOWN) *ymax = y + 1 - height;
        else if (dir == UP) *ymax = y + height;
        limit = 1;
    }
    /* 2ND Cas: */
    if (is_wall(&(*grid), f_s2) && x0 > mid){
        resize_mini_coordinate(&x0);
        height = sqrt(1 / (double)4 - pow(x + 1 - x0, 2));

        if (dir == DOWN) *ymax = y + 1 - height;
        else if (dir == UP) *ymax = y + height;
        limit = 1;
    }
    /* 3ÈME Cas: */
    if (is_wall(&(*grid), f)){
        *ymax = y + (1 / (double) 2);
        limit = 1;
    }
    resize_real_coordinate(&(*ymax));
    if (limit) return 1;
    return 0;
}

/* Renvoie 1 si une limite de déplacement en x a été calculée pour l'agent 'agent', 0 sinon.
 * Si c'est le cas, cette limite est récupérée avec 'xmax'
*/
int travel_limit_in_x(Disc agent, Direction dir, int dist, const Grid *grid, double *xmax){
    Case f, f_s1, f_s2; /* Les 3 cases de devant à distance 'dist' du disque 'agent' suivant 'dir' */
    Case coord; /* Case contenant le centre du disque 'agent' */
    int x, y; /* Coordonnées du coin en haut à gauche de 'coord' */
    double y0; /* Coordonnée y du centre du disque 'agent' */
    double mid; /* Coordonnée du milieu du segment [⌊y0⌋, ⌊y0⌋ + 1] */
    double height; /* Hauteur (du triangle) à additionner ou à soustraire à 'xmax' */
    int limit; /* limit = 1 si une limite de déplacement a été calculée */

    determine_front_cases(agent, dir, dist, &f, &f_s1, &f_s2);
    coord = point_case(agent.center);
    x = coord.col;
    y = coord.lin;
    y0 = agent.center.y;
    mid = y + (1 / (double) 2);
    resize_real_coordinate(&mid);
    limit = 0;

    /* 1ER Cas: */
    if (is_wall(&(*grid), f_s1) && y0 < mid){
        resize_mini_coordinate(&y0);
        height = sqrt(1 / (double)4 - pow(y0 - y, 2));

        if (dir == RIGHT) *xmax = x + 1 - height;
        else if (dir == LEFT) *xmax = x + height;
        limit = 1;
    }
    /* 2ND Cas: */
    if (is_wall(&(*grid), f_s2) && y0 > mid){
        resize_mini_coordinate(&y0);
        height = sqrt(1 / (double)4 - pow(y + 1 - y0, 2));

        if (dir == RIGHT) *xmax = x + 1 - height;
        else if (dir == LEFT) *xmax = x + height;
        limit = 1;
    }
    /* 3ÈME Cas: */
    if (is_wall(&(*grid), f)){
       *xmax = x + (1 / (double) 2);
        limit = 1;
    }
    resize_real_coordinate(&(*xmax));
    if (limit) return 1;
    return 0;
}

/* Renvoie 1 si une limite de déplacement (en x ou en y suivant
 * la direction du déplacement 'dir') a été calculée pour l'agent 'agent', 0 sinon.
 * Si c'est le cas, cette limite est récupérée avec 'limit'
*/
int travel_limit(Disc agent, Direction dir, int dist, const Grid *grid, double *limit){
    if (dir == UP || dir == DOWN){
        if (travel_limit_in_y(agent, dir, dist, &(*grid), &(*limit))) return 1;
    }
    else if (dir == LEFT || dir == RIGHT){
        if (travel_limit_in_x(agent, dir, dist, &(*grid), &(*limit))) return 1;
    }
    return 0;
}


/* Renvoie 1 (0 sinon) si l'agent 'agent' est entré en collision avec un mur dans la direction 'dir' 
 * c'est-à-dire si l'une de ces coordonnées a dépassée la limite qui lui a été imposée par 'limit' 
*/
int collide_wall(Disc *agent, Direction dir, double limit){
    if ((dir == UP && agent->center.y < limit) || (dir == DOWN && agent->center.y > limit)){
        agent->center.y = limit;
        return 1;
    }
    else if ((dir == LEFT && agent->center.x < limit) || (dir == RIGHT && agent->center.x > limit)){
        agent->center.x = limit;
        return 1;
    }
    return 0;
}

/* Renvoie 1 si le gardien 'guard' est entré en collision avec un autre gardien dans 'tab' de taille 'size', 0 sinon.
 * 'index' est l'indice du gardien 'guard' dans le tableau 'tab'
*/
int collide_guard(Guard *guard, int index, const Guard *tab, int size){
    double x0, y0;
    int collide; /* = 1 si le gardien 'guard' entre en collision avec un autre gardien dans 'tab' */
    x0 = guard->disc.center.x;
    y0 = guard->disc.center.y;
    collide = 0;

    change_position_guard(&(*guard));
    if (superposition_between_guards(&(*guard), index, tab, size)) collide = 1;
    guard->disc.center.x = x0; /* 'guard' récupère son ancienne coordonnée x */
    guard->disc.center.y = y0; /* 'guard' récupère son ancienne coordonnée y */
    if (collide) return 1;
    return 0;
}
