#ifndef __PLAYER__
#define __PLAYER__

#include "Point.h"
#include "Disc.h"
#include "Direction.h"
#include "Grid.h"

#define V 4.4 /* Vitesse "v" sur laquelle les calculs de vitesse du joueur sont basés */
#define INITIAL_SPEED 0.1 * V /* Vitesse initiale quand le joueur démarre ou change de direction */
#define ACC 0.03 * V /* On accélère INITIAL_SPEED par ACC à chaque frame où la touche de direction reste enfoncée */
#define MAX_SPEED 0.9 * V /* Vitesse maximale atteignable par le joueur */
#define MAX_SPEED_ACC 1.2 * V /* Vitesse maximale atteignable avec la compétence Accélération */
#define MAX_MANA 100 /* Quantité maximale de mana absorbable */
#define INV_MANA 1 /* Quantité de mana consommée par la compétence Invisibilité à chaque frame */
#define ACC_MANA 2 /* Quantité de mana consommée par la compétence Accélération à chaque frame */
#define USED 1 /* Compétence utilisée */
#define UNUSED 0 /* Compétence non utilisée */

typedef struct{
    int acceleration; /* Accélération */
    int invisibility; /* Invisibilité */
} Competence; /* Compétences magiques du joueur */

typedef struct{
    Disc disc; /* Représentation du joueur par un disque */
    Direction dir; /* Sa direction */
    double speed, max_speed; /* Sa vitesse et la vitesse maximale atteignable */
    int mana, max_mana; /* Sa réverse de mana et la quantité maximale de mana absorbable */
    Competence comp; /* Ses compétences */
} Player; /* Représente le joueur */

void initialize_player(Player *player);
void change_position(Disc *agent, Direction dir, double speed);
void change_position_player(Player *player);
int same_direction(Direction d1, Direction d2);
void change_direction_speed_player(Player *player, Direction pressed);

void activate_acceleration(Player *player);
void desactivate_acceleration(Player *player);
void use_acceleration(Player *player);
void activate_invisibility(Player *player);
void desactivate_invisibility(Player *player);
void use_invisibility(Player *player);

#endif