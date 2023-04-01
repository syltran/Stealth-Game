#include "../include/Player.h"
#include <stdio.h>


/* Initialise le joueur 'player' */
void initialize_player(Player *player){
    Point center = {0.0, 0.0};
    player->disc.center = center;
    player->disc.radius = SIZE_CASE / 2;
    player->dir = RIGHT;
    player->speed = INITIAL_SPEED;
    player->max_speed = MAX_SPEED;
    player->mana = 0;
    player->max_mana = MAX_MANA;
    player->comp.acceleration = UNUSED;
    player->comp.invisibility = UNUSED;
}

/* Change la position de l'agent 'agent' suivant sa direction 'dir' et sa vitesse 'speed' */
void change_position(Disc *agent, Direction dir, double speed){
    double offset_x[] = {0, 0, -speed, speed};
    double offset_y[] = {-speed, speed, 0, 0};
    agent->center.x += offset_x[dir];
    agent->center.y += offset_y[dir];    
}

/* Change la position du joueur 'player' */
void change_position_player(Player *player){
    change_position(&player->disc, player->dir, player->speed);
}

/* Renvoie 1 si les directions 'd1' et 'd2' sont les mêmes, 0 sinon.
 * Les directions latérales sont uniquement comparées entre elles et pour les directions horizontales également
 * (donc Haut est seulement comparée à Bas et vice versa et Gauche seulement à Droite et vice versa)
*/
int same_direction(Direction d1, Direction d2){
    if ((d1 == UP && d2 == DOWN) || (d1 == DOWN && d2 == UP)) return 0;
    if ((d1 == LEFT && d2 == RIGHT) || (d1 == RIGHT && d2 == LEFT)) return 0;
    return 1;
}

/* Change la direction et la vitesse du joueur 'player' suivant la direction choisie 'pressed' */
void change_direction_speed_player(Player *player, Direction pressed){
    if (same_direction(pressed, player->dir)){
        player->speed += ACC;
        if (player->speed > player->max_speed) player->speed = player->max_speed;
    }
    /* Quand on change de direction, la vitesse est réinitialisée */
    else player->speed = INITIAL_SPEED;
    player->dir = pressed;
}

/* Active la compétence Accélération du joueur 'player' */
void activate_acceleration(Player *player){
    if (player->mana >= ACC_MANA && player->comp.acceleration == UNUSED){
        player->comp.acceleration = USED;
        player->max_speed = MAX_SPEED_ACC;
    }    
}

/* Désactive la compétence Accélération du joueur 'player' */
void desactivate_acceleration(Player *player){
    player->comp.acceleration = UNUSED;
    player->max_speed = MAX_SPEED;
}

/* Fait utiliser la compétence Accélération du joueur 'player' donc du mana est consommé */
void use_acceleration(Player *player){
    if (player->comp.acceleration == USED && player->mana >= ACC_MANA) player->mana -= ACC_MANA;
    if (player->mana < ACC_MANA) desactivate_acceleration(&(*player));
}

/* Active la compétence Invisibilité du joueur 'player' */
void activate_invisibility(Player *player){
    if (player->mana >= INV_MANA && player->comp.invisibility == UNUSED){
        player->comp.invisibility = USED;
    }
}

/* Désactive la compétence Invisibilité du joueur 'player' */
void desactivate_invisibility(Player *player){
    player->comp.invisibility = UNUSED;
}

/* Fait utiliser la compétence Invisibilité du joueur 'player' donc du mana est consommé */
void use_invisibility(Player *player){
    if (player->comp.invisibility == USED && player->mana >= INV_MANA){
        player->mana -= INV_MANA;
    }
    if (player->mana < INV_MANA) desactivate_invisibility(&(*player));
}
