#include "../include/Guard.h"
#include <stdio.h>
#include <stdlib.h>


/* Initialise le gardien 'guard' */
void initialize_guard(Guard *guard){
    Point center = {0.0, 0.0};
    guard->disc.center = center;
    guard->disc.radius = SIZE_CASE / 2;
    enter_normal_mode(&(*guard));
    change_direction_speed_guard(&(*guard));
}

/* Fait entrer le gardien 'guard' en mode normal */
void enter_normal_mode(Guard *guard){
    guard->mode = NORMAL;
    guard->detection_dist = DETECT_DIST_NL;
}

/* Fait entrer le gardien 'guard' en mode panique */
void enter_panic_mode(Guard *guard){
    guard->mode = PANIC;
    guard->speed = SPEED_PC;
    guard->detection_dist = DETECT_DIST_PC;
}

/* Renvoie une vitesse aléatoire entre 'min' et 'max' */
double random_speed(double min, double max){
    return rand() / (double) RAND_MAX * (max - min) + min;
}

/* Renvoie aléatoirement l'une des 4 directions UP, DOWN, LEFT et RIGHT */
Direction random_direction(){
    return rand() % 4;
}

/* Change aléatoirement la direction et la vitesse du gardien 'guard' */
void change_direction_speed_guard(Guard *guard){
    guard->dir = random_direction();
    if (guard->mode == NORMAL) guard->speed = random_speed(MIN_SPEED_NL, MAX_SPEED_NL);
}

/* Change la position du gardien 'guard' */
void change_position_guard(Guard *guard){
    change_position(&guard->disc, guard->dir, guard->speed);
}
