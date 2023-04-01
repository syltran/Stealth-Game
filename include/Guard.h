#ifndef __GUARD__
#define __GUARD__

#include "Point.h"
#include "Disc.h"
#include "Direction.h"
#include "Grid.h"
#include "Player.h" /* Les calculs de vitesse d'un gardien sont calqués sur le joueur et ont donc besoin de "v" */

#define MIN_SPEED_NL 0.3 * V /* Vitesse minimale d'un gardien en mode normal */
#define MAX_SPEED_NL 0.8 * V /* Vitesse maximale d'un gardien en mode normal */
#define SPEED_PC V /* Vitesse d'un gardien en mode panique */
#define DETECT_DIST_NL 4 /* Distance de détection d'un gardien en mode normal */
#define DETECT_DIST_PC 6 /* Distance de détection d'un gardien en mode panique */
#define TIME_PC 30 /* Durée du mode panique en seconde */

typedef enum{
    NORMAL, /* Mode normal */
    PANIC /* Mode panique */
} Mode; /* Mode du gardien */

typedef struct{
    Disc disc; /* Représentation du gardien par un disque */
    Mode mode; /* Le mode activé */
    Direction dir; /* Sa direction */
    double speed; /* Sa vitesse */
    int detection_dist; /* Distance de détection en nombre de cases */
} Guard; /* Représente un gardien */

void initialize_guard(Guard *guard);
void enter_normal_mode(Guard *guard);
void enter_panic_mode(Guard *guard);

double random_speed(double min, double max);
Direction random_direction();
void change_direction_speed_guard(Guard *guard);
void change_position_guard(Guard *guard);

#endif