#define _POSIX_C_SOURCE 199309L
#include "../include/GameEngine.h"
#include "../include/Graphic.h"
#include "../include/Ranking.h"
#include <MLV/MLV_all.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){
    srand(time(NULL));
    MLV_Music* music;
    int quit, win;
    struct timespec start, end;
    Room room;
    Rankings rankings;
    Score score;

    /* Initialisation de variables */
    initialize_room(&room);
    initialize_rankings(&rankings);
    initialize_score(&score);
    quit = 0;

    /* Fenêtre du jeu */
    MLV_create_window("Game", "Game", WINDOW_WIDTH, WINDOW_HEIGHT + SPACE);

    /* Initialisation d'une musique de fond */
    MLV_init_audio();
    music = MLV_load_music("sound/sound.ogg");
    if (music == NULL) {
        fprintf(stderr, "Error while loading sound\n");
        exit(EXIT_FAILURE);
    }
    MLV_play_music(music, 1.0, -1);

    /* Menu */
    if (draw_menu("img/menu.jpg") == -1){
        fprintf(stderr, "Error while drawing menu\n");
        exit(EXIT_FAILURE);
    }
    if (!play()) exit(EXIT_SUCCESS);


    MLV_clear_window(MLV_COLOR_BLACK);
    clock_gettime(CLOCK_REALTIME, &start);
    /* Boucle maîtresse gérant le jeu frame par frame 
     (le programme doit faire NB_FRAMES_PER_S frames par seconde) */
    while(!quit){
        struct timespec end_time, new_time;
        double frametime, extratime;
        /* On récupère le temps en nanoseconde au début de la frame */
        clock_gettime(CLOCK_REALTIME, &end_time);

        /* Affichage de la frame courante */
        draw_room(&room);

        /* Gère les mécanismes du jeu */
        move_guards_in_room(&room);
        move_player_in_room(&room);
        absorb_manas_in_room(&room);
        use_player_competences_in_room(&room, &score.mana_used);
        take_relics_in_room(&room);
        manage_guards_modes_in_room(&room);

        /* Conditions de fin de partie */
        if (player_detected_in_room(&room)){
            win = 0;
            quit = 1;
        }
        if (player_mission_accomplished(&room)){
            win = 1;
            quit = 1;
        }

        /* On récupère le temps en nanoseconde à la fin de la frame */
        clock_gettime(CLOCK_REALTIME, &new_time);

        /* Calcule du temps passé entre le début et la fin de la frame */
        frametime = new_time.tv_sec - end_time.tv_sec;
        frametime += (new_time.tv_sec - end_time.tv_sec) / 1.0E9;

        /* On force le programme à passer au moins 1/NB_FRAMES_PER_S seconde au total */
        extratime = 1.0 / NB_FRAMES_PER_S - frametime;
        if (extratime > 0){
            MLV_wait_milliseconds((int) (extratime * 1000));
        }
    }
    clock_gettime(CLOCK_REALTIME, &end);
    MLV_wait_seconds(1);
    MLV_clear_window(MLV_COLOR_BLACK);

    /* Calcule du temps réalisé par le joueur */
    score.time = time_achieved(&start, &end);

    /* Affichage du résultat de la partie, du temps réalisé et de la quantité de mana dépensée */
    draw_end_game(win, &score);

    /* Si le joueur gagne, il peut entrer son nom dans le tableau des meilleurs scores */
    if (win){
        draw_enter_name(score.name, SIZE_NAME);
        if (save_score("rankings.bin", &score) == -1){
            fprintf(stderr, "Error while saving score\n");
            exit(EXIT_FAILURE);
        }
    }

    /* Affichage du tableau de classement */
    if (get_scores("rankings.bin", &rankings) == -1){
        fprintf(stderr, "Error while getting scores\n");
        exit(EXIT_FAILURE);
    }
    draw_rankings(&rankings);

    MLV_wait_seconds(30);
    MLV_stop_music();
    MLV_free_music(music);
    MLV_free_audio();
    MLV_free_window();
    return 0;
}