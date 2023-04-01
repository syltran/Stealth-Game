#include "../include/Grid.h"
#include <stdio.h>


/* Initialise la grille 'grid' */
void initialize_grid(Grid *grid){
    int i, j;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            /* Création des 4 murs extérieurs */
            if (i == 0 || i == NB_LIN - 1 || j == 0 || j == NB_COL - 1) grid->tab[i][j] = WALL;
            /* Création de l'entrée */
            else if (i >= 1 && i <= 2 && j >= 1 && j <= 2) grid->tab[i][j] = ENTRANCE;
            else grid->tab[i][j] = EMPTY;
        }
    }
}

/* Affiche la grille 'grid' */
void show_grid(const Grid *grid){
    int i, j;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            switch(grid->tab[i][j]){
                case WALL: printf("x"); break;
                case RELIC: printf("r"); break;
                case MANA: printf("-"); break;
                default: printf(" ");
            }
        }
        printf("\n");
    }
}

/* Renvoie 1 si la case 'coord' de la grille est vide, 0 sinon */
int is_empty(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == EMPTY;
}

/* Renvoie 1 si la case 'coord' de la grille est une case d'entrée, 0 sinon */
int is_entrance(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == ENTRANCE;
}

/* Renvoie 1 si la case 'coord' de la grille est une case d'ouverture, 0 sinon */
int is_gap(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == GAP;
}

/* Renvoie 1 si la case 'coord' de la grille est une case de mur, 0 sinon */
int is_wall(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == WALL;
}

/* Renvoie 1 si la case 'coord' de la grille est une case de relique, 0 sinon */
int is_relic(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == RELIC;
}

/* Renvoie 1 si la case 'coord' de la grille est une case de mana, 0 sinon */
int is_mana(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == MANA;
}

/* Renvoie 1 si la case 'coord' de la grille est une case indiquant une relique prise, 0 sinon */
int is_relic_taken(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == RELIC_TAKEN;
}

/* Renvoie 1 si la case 'coord' de la grille est une case indiquant une relique détectée, 0 sinon */
int is_relic_detect(const Grid *grid, Case coord){
    return grid->tab[coord.lin][coord.col] == RELIC_DETECT;
}