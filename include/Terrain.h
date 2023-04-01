#ifndef __TERRAIN__
#define __TERRAIN__

#include "Grid.h"
#include "Point.h"
#include "Disc.h"
#include "Player.h"
#include "Guard.h"

#define MINSIDE 9 /* Longueur minimale des côtés des compartiments crées */
#define LENGTH_GAP 3 /* Taille d'une ouverture : 3 cases */
#define INITIAL_POS_X 2.0 * SIZE_CASE /* Coordonnée x de la position initiale du joueur */
#define INITIAL_POS_Y 2.0 * SIZE_CASE /* Coordonnée y de la position initiale du joueur */
#define MINDIST 20 * SIZE_CASE /* Distance euclidienne minimale qu'il doit y avoir entre
                                la position initiale du joueur et la position des gardiens et des reliques */


void generate_vertical_wall(Grid *grid, int column, int start_y, int end_y);
void generate_horizontal_wall(Grid *grid, int line, int start_x, int end_x);
void random_position_for_wall(int *pos, int start, int end, int *left, int *right);
void generate_compartments(Grid *grid, int x, int start_x, int end_x, int y, int start_y, int end_y);

void place_player_in_grid(Player *player);

int far_enough_from_the_player(Point point, const Player *player);
Case point_case(Point point);
void resize_point(Point *point);

void place_relics_in_grid(int nb_relics, Grid *grid, const Player *player);
void place_manas_in_grid(Grid *grid);
void place_random_mana_in_grid(Grid *grid);

int superposition_between_guards(const Guard *guard, int index, const Guard *tab, int size);
int in_diagonal_wall(const Guard *guard, const Grid *grid);
int in_lateral_wall_or_gap(const Guard *guard, const Grid *grid);
void place_guard_in_grid(Guard *guard, int index, const Guard *tab, int size, const Grid *grid, const Player *player);

#endif