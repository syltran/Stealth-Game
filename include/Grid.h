#ifndef __GRID__
#define __GRID__

#define NB_LIN 45 /* Hauteur de la grille */
#define NB_COL 60 /* Longueur de la grille */
#define SIZE_CASE 15 /* Longueur des côtés d'une case */

typedef struct{
    int lin, col;
} Case; /* Représente une case de la grille */

typedef enum{
    EMPTY, /* Vide */
    ENTRANCE, /* Entrée */
    WALL, /* Mur */
    GAP, /* Ouverture */
    MANA, /* Traces de mana */
    RELIC, /* Relique */
    RELIC_TAKEN, /* Relique prise */
    RELIC_DETECT /* Relique détectée */
} InfoCase; /* Donne une information sur une case */

typedef struct{
    InfoCase tab[NB_LIN][NB_COL];
} Grid; /* Représente la grille du jeu */

void initialize_grid(Grid *grid);
void show_grid(const Grid *grid);
int is_empty(const Grid *grid, Case coord);
int is_entrance(const Grid *grid, Case coord);
int is_gap(const Grid *grid, Case coord);
int is_wall(const Grid *grid, Case coord);
int is_relic(const Grid *grid, Case coord);
int is_mana(const Grid *grid, Case coord);
int is_relic_taken(const Grid *grid, Case coord);
int is_relic_detect(const Grid *grid, Case coord);

#endif