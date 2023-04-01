#include "../include/Terrain.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/* Génère un mur vertical dans la grille 'grid' de la ligne 'start_y' à la ligne 'end_y' à la colonne n°'column' */
void generate_vertical_wall(Grid *grid, int column, int start_y, int end_y){
    int i, alea;

    alea = rand() % 2; /* Pour savoir à quelle extrémité du mur les cases d'ouvertures doivent être placées */
    for (i = start_y; i <= end_y; i++){
        grid->tab[i][column] = WALL;
        if (alea == 0 && i < start_y + LENGTH_GAP) grid->tab[i][column] = GAP;
        if (alea == 1 && i > end_y - LENGTH_GAP) grid->tab[i][column] = GAP;
    }
}

/* Génère un mur horizontal dans la grille 'grid' de la colonne 'start_x' à la colonne 'end_x' à la ligne n°'line' */
void generate_horizontal_wall(Grid *grid, int line, int start_x, int end_x){
    int i, alea;

    alea = rand() % 2; /* Pour savoir à quelle extrémité du mur les cases d'ouvertures doivent être placées */
    for (i = start_x; i <= end_x; i++){
        grid->tab[line][i] = WALL;
        if (alea == 0 && i < start_x + LENGTH_GAP) grid->tab[line][i] = GAP;
        if (alea == 1 && i > end_x - LENGTH_GAP) grid->tab[line][i] = GAP;
    }
}

/* Cherche aléatoirement un numéro de ligne ou colonne entre 'start' et 'end' pour savoir où placer un mur de façon 
 * à ce que la taille (en longueur ou largeur) des deux compartiments crées à gauche et à droite soit plus grande que MINSIDE.
 * On récupère via 'pos' la position de la ligne ou colonne où le mur sera généré.
 * On récupère également au passage avec 'left' et 'right' la taille des compartiments gauche et droite crées
*/
void random_position_for_wall(int *pos, int start, int end, int *left, int *right){
    while (*left < MINSIDE || *right < MINSIDE){
        *pos = random_int(start, end);
        *left = *pos - start;
        *right = end - *pos;
    }
}

/* Subdivise la grille 'grid' en plusieurs compartiments dans l'espace libre de taille 'x' x 'y'.
 * La longueur 'x' débute à la colonne 'start_x' et finit à la colonne 'end_x'.
 * De même, la largeur 'y' débute à la ligne 'start_y' et finit à la ligne 'end_y'
*/
void generate_compartments(Grid *grid, int x, int start_x, int end_x, int y, int start_y, int end_y){
    int pos;
    int length_left = -1, length_right = -1;

    if (x > y){
        if (x < 2 * MINSIDE + 1) return;
        else if (x < 4 * MINSIDE){
            if (rand() % 2 == 0) return;   
        }
        random_position_for_wall(&pos, start_x, end_x, &length_left, &length_right);
        generate_vertical_wall(&(*grid), pos, start_y, end_y);
        /* On relance l'algo récursivement sur les deux compartiments générés */
        generate_compartments(&(*grid), length_left, start_x, pos - 1, y, start_y, end_y);
        generate_compartments(&(*grid), length_right, pos + 1, end_x, y, start_y, end_y);
    }
    else if (y > x){
        if (y < 2 * MINSIDE + 1) return;
        else if (y < 4 * MINSIDE){
            if (rand() % 2 == 0) return;   
        }
        random_position_for_wall(&pos, start_y, end_y, &length_left, &length_right);
        generate_horizontal_wall(&(*grid), pos, start_x, end_x);
        /* On relance l'algo récursivement sur les deux compartiments générés */
        generate_compartments(&(*grid), x, start_x, end_x, length_left, start_y, pos - 1);
        generate_compartments(&(*grid), x, start_x, end_x, length_right, pos + 1, end_y);
    }
}


/* Place le joueur 'player' dans la grille */
void place_player_in_grid(Player *player){
    player->disc.center.x = INITIAL_POS_X;
    player->disc.center.y = INITIAL_POS_Y;
}


/* Renvoie 1 si le point 'point' est à une distance d'au moins MINDIST de la position du joueur 'player', 0 sinon */
int far_enough_from_the_player(Point point, const Player *player){
    return distance(point, player->disc.center) >= MINDIST;
}

/* Renvoie la case contenant le point 'point' */
Case point_case(Point point){
    Case c;
    c.lin = floor(point.y / SIZE_CASE);
    c.col = floor(point.x / SIZE_CASE);
    return c;
}

/* Redimensionne les coordonnées du point 'point' pour l'adapter aux dimensions "réelles" du terrain */
void resize_point(Point *point){
    point->x *= SIZE_CASE;
    point->y *= SIZE_CASE;
}


/* Place 'nb_relics' reliques dans la grille 'grid' assez loin du joueur 'player' */
void place_relics_in_grid(int nb_relics, Grid *grid, const Player *player){
    int i, valid;
    Case coord;
    Point point;

    for (i = 0; i < nb_relics; i++){
        valid = 0;
        while (!valid){
            point = random_point(1, NB_COL - 2, 1, NB_LIN - 2);
            resize_point(&point);
            if (far_enough_from_the_player(point, &(*player))){
                coord = point_case(point);
                if (is_empty(&(*grid), coord)){
                    grid->tab[coord.lin][coord.col] = RELIC;
                    valid = 1;
                }
            }
        }
    }
}

/* Place les cases de mana dans la grille 'grid' */
void place_manas_in_grid(Grid *grid){
    int i, j;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            Case coord = {i, j};
            if (is_empty(&(*grid), coord) || is_gap(&(*grid), coord)) grid->tab[i][j] = MANA;
        }
    }
}

/* Place une tuile de mana aléatoirement dans la grille 'grid' */
void place_random_mana_in_grid(Grid *grid){
    int valid;
    Point point;
    Case coord;

    valid = 0;
    while (!valid){
        point = random_point(1, NB_COL - 2, 1, NB_LIN - 2);
        resize_point(&point);
        coord = point_case(point);
        if (is_empty(&(*grid), coord)){
            grid->tab[coord.lin][coord.col] = MANA;
            valid = 1;
        }
    }
}


/* Renvoie 1 si le gardien 'guard' touche ou se superpose à un autre gardien de 'tab' de taille 'size'.
 * 'index' est l'indice du gardien 'guard' dans le tableau 'tab'.
*/
int superposition_between_guards(const Guard *guard, int index, const Guard *tab, int size){
    int i;
    for (i = 0; i < size; i++){
        /* On ne compare pas le gardien 'guard' à lui-même */
        if (i != index){
            if (superposition_between_discs(guard->disc, tab[i].disc)) return 1;
        }
    }
    return 0;
}

/* Renvoie 1 si le gardien 'guard' rentre dans un mur 
 * possiblement situé sur l'une des 4 cases diagonales adjacentes à la sienne, 0 sinon 
*/
int in_diagonal_wall(const Guard *guard, const Grid *grid){
    int i, j;
    Case c_guard; /* Case contenant le centre du disque du gardien */
    c_guard = point_case(guard->disc.center);

    for (i = -1; i < 2; i++){
        for (j = -1; j < 2; j++){
            if (i != 0 && j != 0){
                /* On ne veut que les cases situées sur les diagonales */
                Case diagonal = {c_guard.lin + i, c_guard.col + j};

                if (is_wall(&(*grid), diagonal)){
                    /* On veut le coin de case qui est commun à la case 'c_guard' et 'diagonal' */
                    if (i == -1) diagonal.lin += 1;
                    if (j == -1) diagonal.col += 1;
                    Point corner = {diagonal.col, diagonal.lin};
                    resize_point(&corner);
                    /* Si ce coin est à l'intérieur du disque du gardien, alors 
                    cela veut dire que le gardien rentre dans la case 'diagonal' */
                    if (point_inside_disc(corner, guard->disc)) return 1;
                }
            }
        }
    }
    return 0;
}

/* Renvoie 1 si le gardien 'guard' rentre dans un mur ou une ouverture
 * possiblement situé sur l'une des 4 cases latérales adjacentes à la sienne, 0 sinon 
*/
int in_lateral_wall_or_gap(const Guard *guard, const Grid *grid){
    int i, j;
    Case c_guard; /* Case contenant le centre du disque du gardien */
    Point middle; /* Point du milieu commun à la case 'c_guard' 
                   et la case ('up_down' ou 'left_right') dont on cherche à savoir si c'est un mur ou une ouverture */
    c_guard = point_case(guard->disc.center);
    
    /* Case en haut et en bas */
    for (i = -1; i < 2; i++){
        Case up_down = {c_guard.lin + i, c_guard.col};
        if (i != 0 && (is_wall(&(*grid), up_down) || is_gap(&(*grid), up_down))){
            middle.x = up_down.col + (1 / (double) 2);

            if (i == -1) middle.y = up_down.lin + 1;
            else if (i == 1) middle.y = up_down.lin;

            resize_point(&middle);
            if (point_inside_disc(middle, guard->disc)) return 1;
        }
    }
    /* Case à gauche et à droite */
    for (j = -1; j < 2; j++){
        Case left_right = {c_guard.lin, c_guard.col + j};
        if (j != 0 && (is_wall(&(*grid), left_right) || is_gap(&(*grid), left_right))){
            middle.y = left_right.lin + (1 / (double) 2);

            if (j == -1) middle.x = left_right.col + 1;
            else if (j == 1) middle.x = left_right.col;

            resize_point(&middle);
            if (point_inside_disc(middle, guard->disc)) return 1;
        }
    }
    return 0;
}

/* Place le gardien 'guard' dans la grille 'grid' assez loin du joueur 'player'
 * et en prenant en compte les autres gardiens dans 'tab' de taille 'size'.
 * 'index' est l'indice du gardien 'guard' dans le tableau 'tab'.
*/
void place_guard_in_grid(Guard *guard, int index, const Guard *tab, int size, const Grid *grid, const Player *player){
    int valid;
    Case coord;
    Point point;
    
    valid = 0;
    while (!valid){
        point = random_point(1, NB_COL - 2, 1, NB_LIN - 2);
        resize_point(&point);
        if (far_enough_from_the_player(point, &(*player))){

            coord = point_case(point);
            if (!is_wall(&(*grid), coord) && !is_gap(&(*grid), coord)){

                guard->disc.center.x = point.x;
                guard->disc.center.y = point.y;
                if (!superposition_between_guards(&(*guard), index, tab, size)){
                    if (!in_diagonal_wall(&(*guard), &(*grid))){
                        if (!in_lateral_wall_or_gap(&(*guard), &(*grid))) valid = 1;
                    }
                }
            }
        }
    }
}