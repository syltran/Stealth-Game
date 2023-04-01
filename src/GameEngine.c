#include "../include/GameEngine.h"
#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>


/* Initialise la salle 'room' */
void initialize_room(Room *room){
    int i;
    /* Grille */
    initialize_grid(&room->grid);
    generate_compartments(&room->grid, NB_COL - 2, 1, NB_COL - 2, NB_LIN - 2, 1, NB_LIN - 2);

    /* Joueur */
    initialize_player(&room->player);
    place_player_in_grid(&room->player);

    /* Reliques */
    place_relics_in_grid(NB_RELICS, &room->grid, &room->player);

    /* Gardiens */
    for (i = 0; i < NB_GUARDS; i++){
        initialize_guard(&room->guards[i]);
        place_guard_in_grid(&room->guards[i], i, room->guards, NB_GUARDS, &room->grid, &room->player);
    }

    /* Manas */
    place_manas_in_grid(&room->grid);
}


/* Déplace le gardien 'guard' dans la grille 'grid' en prenant en compte les autres gardiens dans 'tab' de taille 'size'.
 * 'index' est l'indice du gardien 'guard' dans le tableau 'tab'
*/
void move_guard(Guard *guard, int index, const Guard *tab, int size, const Grid *grid){
    int dist; /* La distance entre le gardien 'guard' et les 3 murs face à lui
               suivant sa direction, dont il cherche à éviter la collision */
    double limit; /* Limite de déplacement (en x ou y) */
    dist = 0;

    if (collide_guard(&(*guard), index, tab, size)){
        change_direction_speed_guard(&(*guard));
        return;
    }

    /* On veut qu'un gardien en mode panique change de direction si un mur est à une case de distance
     (donc pas directement devant lui mais 1 case plus loin) */
    if (guard->mode == PANIC && !risk_collide_wall(guard->disc, guard->dir, 0, &(*grid))){
        dist = 1;
    }

    if (travel_limit(guard->disc, guard->dir, dist, &(*grid), &limit)){
        /* Si une limite de déplacement a été calculée, 
         on fait en sorte que le gardien ne dépasse pas cette limite */
        change_position_guard(&(*guard));
        if (collide_wall(&guard->disc, guard->dir, limit)){
            change_direction_speed_guard(&(*guard));
            return;
        }
    }
    else change_position_guard(&(*guard));
    
    /* 1 chance sur 50 de changer de direction et de vitesse à chaque frame */
    if (rand() % 50 == 1) change_direction_speed_guard(&(*guard));
}

/* Déplace les gardiens dans la salle 'room' */
void move_guards_in_room(Room *room){
    int i;
    for (i = 0; i < NB_GUARDS; i++){
        move_guard(&room->guards[i], i, room->guards, NB_GUARDS, &room->grid);
    }
}


/* Déplace le joueur 'player' dans la grille 'grid' suivant la direction choisie 'pressed' */
void move_player(Player *player, const Grid *grid, Direction pressed){
    double limit; /* Limite de déplacement (en x ou y) */

    change_direction_speed_player(&(*player), pressed);
    if (travel_limit(player->disc, player->dir, 0, &(*grid), &limit)){
        /* Fait en sorte que le joueur ne rentre pas en collision avec un mur */
        change_position_player(&(*player));
        collide_wall(&player->disc, player->dir, limit);
    }
    else change_position_player(&(*player));
}

/* Renvoie 1 si la touche 'key' du clavier a été pressée, 0 sinon */
static int key_pressed(MLV_Keyboard_button key){
    return MLV_get_keyboard_state(key) == MLV_PRESSED;
}

/* Déplace le joueur dans la salle 'room' avec les touches z, q, s, d du clavier */
void move_player_in_room(Room *room){
    int zqsd;
    zqsd = 0;
    if (key_pressed(SDLK_z)){ /* Touche z */
        move_player(&room->player, &room->grid, UP);
        zqsd = 1;
    }
    if (key_pressed(SDLK_q)){ /* Touche q */
        move_player(&room->player, &room->grid, LEFT);
        zqsd = 1;
    }
    if (key_pressed(SDLK_s)){ /* Touche s */
        move_player(&room->player, &room->grid, DOWN);
        zqsd = 1;
    }
    if (key_pressed(SDLK_d)){ /* Touche d */
        move_player(&room->player, &room->grid, RIGHT);
        zqsd = 1;
    }
    if (!zqsd) room->player.speed = INITIAL_SPEED;
}


/* Permet au joueur 'player' d'absorber une trace de mana dans la grille 'grid' */
void absorb_mana(Player *player, Grid *grid){
    Case coord;
    coord = point_case(player->disc.center);
    if (player->disc.center.x != INITIAL_POS_X || player->disc.center.y != INITIAL_POS_Y){
        if (is_mana(&(*grid), coord) && player->mana < player->max_mana){
            grid->tab[coord.lin][coord.col] = EMPTY;
            player->mana += 1;
        }
    }
}

/* Permet d'absorber les traces de mana dans la salle 'room' */
void absorb_manas_in_room(Room *room){
    absorb_mana(&room->player, &room->grid);
}


/* Permet d'utiliser les compétences du joueur dans la salle 'room'.
 * La quantité de mana dépensée pour l'usage de ces compétences est ajoutée à 'mana_used'
*/
void use_player_competences_in_room(Room *room, int *mana_used){
    int acc, inv;
    int i, prev_mana, consume;
    acc = 0;
    inv = 0;
    prev_mana = room->player.mana; /* Ancienne quantité de mana */

    if (key_pressed(SDLK_LSHIFT) || key_pressed(SDLK_RSHIFT)){ /* Touche shift gauche ou droit */
        activate_acceleration(&room->player);
        use_acceleration(&room->player);
        acc = 1; 
    }
    if (key_pressed(SDLK_SPACE)){ /* Touche espace */
        activate_invisibility(&room->player);
        use_invisibility(&room->player);  
        inv = 1;
    }
    if (!acc) desactivate_acceleration(&room->player);
    if (!inv) desactivate_invisibility(&room->player);

    /* On calcule la quantité de mana consommée */
    consume = prev_mana - room->player.mana;
    *mana_used += consume;
    /* On fait retourner cette quantité de mana sur des tuiles aléatoires
     et dont les traces de mana avaient été absorbée */
    for (i = 0; i < consume; i++){
        place_random_mana_in_grid(&room->grid);
    }
}


/* Permet au joueur 'player' de récupérer une relique dans la grille 'grid' */
void take_relic(const Player *player, Grid *grid){
    Case coord;
    coord = point_case(player->disc.center);
    if (is_relic(&(*grid), coord)){
        grid->tab[coord.lin][coord.col] = RELIC_TAKEN;
    }
}

/* Permet de récupérer les reliques dans la salle 'room' */
void take_relics_in_room(Room *room){
    take_relic(&room->player, &room->grid);
}


/* Fait entrer les gardiens de la salle 'room' en mode panique */
void guards_enter_panic_mode_in_room(Room *room){
    int i;
    for (i = 0; i < NB_GUARDS; i++){
        enter_panic_mode(&room->guards[i]);
    }
}

/* Fait entrer les gardiens de la salle 'room' en mode normal */
void guards_enter_normal_mode_in_room(Room *room){
    int i;
    for (i = 0; i < NB_GUARDS; i++){
        enter_normal_mode(&room->guards[i]);
    }
}

/* Gère les modes des gardiens dans la salle 'room' */
void manage_guards_modes_in_room(Room *room){
    int i;
    static int cmpt, panic = 0;

    for (i = 0; i < NB_GUARDS; i++){
        /* Si un gardien voit qu'une relique a disparu, on fait entrer tous les gardiens en mode panique */
        if (detect_missing_relics(&room->guards[i], &room->grid)){
            guards_enter_panic_mode_in_room(&(*room));
            panic = 1; /* Mode panique activé */
            cmpt = 0; /* On initialise une sorte de chrono */
        }
    }
    /* Si le mode panique est activé, on laisse tourner le chrono */
    if (panic){
        cmpt++;
        /* Après une certaine durée, les gardiens reviennent en mode normal */
        /* (si NB_FRAMES_PER_S = 60, 30 secondes équivaut à 1800 tours de boucle du while dans le main) */
        if (cmpt >= NB_FRAMES_PER_S * TIME_PC){
            guards_enter_normal_mode_in_room(&(*room));
            panic = 0;
        }
    }
}


/* Renvoie 1 si le joueur est détecté par un gardien dans la salle 'room', 0 sinon */
int player_detected_in_room(const Room *room){
    int n;
    for (n = 0; n < NB_GUARDS ; n++){
        /* Si le joueur est directement visible par un gardien et que sa compétence Invisibilité est désactivée
           alors le joueur perd et la partie se termine */
        if(!wall_between_point_guard(room->player.disc.center, &room->guards[n], &room->grid)
            && player_in_zone(&room->player, &room->guards[n]) && room->player.comp.invisibility == UNUSED){
                return 1;
        }
    }
    return 0;
}


/* Renvoie le nombre de reliques récupérées dans la salle 'room' */
int nb_relics_taken_in_room(const Room *room){
    int i, j, nb;
    nb = 0;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            Case coord = {i, j};
            /* Si une relique a été détectée c'est qu'elle a bien été récupérée avant */
            if (is_relic_taken(&room->grid, coord) || is_relic_detect(&room->grid, coord)) nb++;
        }
    }
    return nb;
}

/* Renvoie 1 si toutes les reliques de la salle 'room' ont été récupérées, 0 sinon */
int all_relics_taken_in_room(const Room *room){
    return nb_relics_taken_in_room(&(*room)) == NB_RELICS;
}

/* Renvoie 1 si le joueur a accompli sa mission :
* celui d'infiltrer la salle 'room', de revenir à l'entrée avec toutes les reliques sans être détecté par les gardiens 
*/
int player_mission_accomplished(const Room *room){
    Case coord;
    coord = point_case(room->player.disc.center);
    if (is_entrance(&room->grid, coord) && (coord.col == 1 && coord.lin == 1)){
        if (all_relics_taken_in_room(&(*room))) return 1;
    }
    return 0;
}
