#ifndef __GAMEENGINE__
#define __GAMEENGINE__

#include "Grid.h"
#include "Player.h"
#include "Guard.h"
#include "Terrain.h"
#include "Collision.h"
#include "Detection.h"
#define NB_GUARDS 5 /* Nombre de gardiens */
#define NB_RELICS 3 /* Nombre de reliques */
#define NB_FRAMES_PER_S 60 /* Nombre de frames par seconde */

typedef struct{
    Grid grid;
    Player player;
    Guard guards[NB_GUARDS];
} Room; /* Représente la salle du jeu rassemblant tous les objets du jeu */

void initialize_room(Room *room);

void move_guard(Guard *guard, int index, const Guard *tab, int size, const Grid *grid);
void move_guards_in_room(Room *room);

void move_player(Player *player, const Grid *grid, Direction pressed);
void move_player_in_room(Room *room);
void absorb_mana(Player *player, Grid *grid);
void absorb_manas_in_room(Room *room);
void use_player_competences_in_room(Room *room, int *mana_used);
void take_relic(const Player *player, Grid *grid);
void take_relics_in_room(Room *room);

void guards_enter_panic_mode_in_room(Room *room);
void guards_enter_normal_mode_in_room(Room *room);
void manage_guards_modes_in_room(Room *room);

int player_detected_in_room(const Room *room);
int nb_relics_taken_in_room(const Room *room);
int all_relics_taken_in_room(const Room *room);
int player_mission_accomplished(const Room *room);

#endif