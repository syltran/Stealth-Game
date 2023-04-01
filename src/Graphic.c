#include "../include/Graphic.h"
#include <stdio.h>


/* Dessine la grille 'grid' */
void draw_grid(const Grid *grid){
    int i, j;
    int x, y;
    for (i = 0; i < NB_LIN; i++){
        for (j = 0; j < NB_COL; j++){
            x = j * SIZE_CASE;
            y = i * SIZE_CASE;

            MLV_draw_filled_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_GRAY);
            MLV_draw_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_GRAY50);            
            switch(grid->tab[i][j]){
                case ENTRANCE:
                    MLV_draw_filled_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_GRAY40);
                    break;
                case WALL:
                    MLV_draw_filled_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_BLACK);
                    break;
                case RELIC: 
                    MLV_draw_filled_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_YELLOW1);
                    break;
                case MANA:
                    MLV_draw_filled_circle(x + SIZE_CASE / 2, y + SIZE_CASE / 2, SIZE_CASE / 4, MLV_COLOR_SKY_BLUE);
                    MLV_draw_circle(x + SIZE_CASE / 2, y + SIZE_CASE / 2, SIZE_CASE / 4, MLV_COLOR_BLACK);
                    break;
                default: ;
            }
            /* Reliques prises ou détectées */
            if (grid->tab[i][j] == RELIC_TAKEN || grid->tab[i][j] == RELIC_DETECT){
                MLV_draw_filled_rectangle(x, y, SIZE_CASE, SIZE_CASE, MLV_COLOR_YELLOW4);
                MLV_draw_line(x, y, x + SIZE_CASE, y + SIZE_CASE, MLV_COLOR_RED1);
                MLV_draw_line(x + SIZE_CASE, y, x, y + SIZE_CASE, MLV_COLOR_RED1);
            }
        }
    }
}


/* Dessine le disque 'disc' avec la couleur 'color' */
void draw_disc(Disc disc, MLV_Color color){
    MLV_draw_filled_circle(disc.center.x, disc.center.y, disc.radius, color);
    MLV_draw_circle(disc.center.x, disc.center.y, disc.radius, MLV_COLOR_BLACK);
}

/* Dessine le joueur 'player' */
void draw_player(const Player *player){
    int acc, inv;
    acc = 0;
    inv = 0;
    if (player->comp.acceleration == USED){
        draw_disc(player->disc, MLV_COLOR_LIGHT_GREEN);
        acc = 1;
    }
    if (player->comp.invisibility == USED){
        draw_disc(player->disc, MLV_rgba(255, 0, 0, 80)); /* Effet camouflage en réduisant la transparence */
        inv = 1;
    }
    if (!acc && !inv) draw_disc(player->disc, MLV_COLOR_RED);
}


/* Dessine la zone de détection de rayon 'dist' de l'agent 'agent' avec la couleur 'color' */
void draw_detect_zone(Disc agent, int dist, MLV_Color color){
    MLV_draw_filled_circle(agent.center.x, agent.center.y, dist, color);
    MLV_draw_circle(agent.center.x, agent.center.y, dist, MLV_COLOR_BLACK);
}

/* Dessine le gardien 'guard' */
void draw_guard(const Guard *guard){
    MLV_Color color, zone;
    if (guard->mode == NORMAL){
        color = MLV_COLOR_BLUE2;
        zone = MLV_rgba(191, 191, 191, 80);
    }
    else{ /* Mode PANIC */
        color = MLV_COLOR_RED2;
        zone = MLV_rgba(255, 0, 0, 80);
    }
    draw_detect_zone(guard->disc, guard->detection_dist * SIZE_CASE, zone);
    draw_disc(guard->disc, color);
}


/* Dessine la barre de mana du joueur 'player' */
void draw_mana_bar(const Player *player){
    int x, y, total_width, curr_witdh; 
    total_width = SIZE_CASE * 13; /* Largeur totale de la barre */
    curr_witdh = player->mana * total_width / MAX_MANA; /* Progression de la barre 
                                                           en fonction de la quantité de mana de 'player' */
    x = SIZE_CASE;
    y = WINDOW_HEIGHT;
    MLV_draw_text(x, y, "Mana : %d / %d", MLV_COLOR_BLUE, player->mana, MAX_MANA);
    MLV_draw_filled_rectangle(x, y + 15, curr_witdh, SIZE_CASE, MLV_COLOR_BLUE1);
    MLV_draw_rectangle(x, y + 15, total_width, SIZE_CASE, MLV_COLOR_BLACK);
}

/* Dessine le nombre de reliques récupérées dans la salle 'room' */
void draw_nb_relics_taken_in_room(const Room *room){
    int x, y, nb;
    nb = nb_relics_taken_in_room(&(*room));
    x = SIZE_CASE * 15;
    y = WINDOW_HEIGHT + 15;
    MLV_draw_text(x, y, "Relics : %d / %d", MLV_COLOR_RED, nb, NB_RELICS);
}


/* Dessine la salle du jeu 'room' */
void draw_room(const Room *room){
    int i;
    /* Grille */
    draw_grid(&room->grid);
    /* Joueur + Gardiens */
    draw_player(&room->player);
    for (i = 0; i < NB_GUARDS; i++){
        draw_guard(&room->guards[i]);
    }
    /* Informations utiles en bas de la fenêtre */
    MLV_draw_filled_rectangle(0, WINDOW_HEIGHT, WINDOW_WIDTH, SPACE, MLV_COLOR_BEIGE);
    draw_mana_bar(&room->player);
    draw_nb_relics_taken_in_room(&(*room));
    MLV_actualise_window();
}


/* Affiche un texte indiquant si le joueur a gagné ou non (selon la valeur de 'win')
 * + le temps réalisé par celui-ci et la quantité de mana dépensée (contenus dans 'score')
*/
void draw_end_game(int win, const Score *score){
    int x, y;
    x = WINDOW_WIDTH / 2 - SIZE_CASE * 6;
    y = SIZE_CASE + SIZE_CASE * 4;
    if (win) MLV_draw_text(x, y, "YOU WON !", MLV_COLOR_RED);
    else MLV_draw_text(x, y, "YOU LOST !", MLV_COLOR_RED);
    MLV_draw_text(x, y + 30, "Time spent : %0.2f seconds", MLV_COLOR_BISQUE, score->time);
    MLV_draw_text(x, y + 50, "Mana used : %d", MLV_COLOR_BISQUE, score->mana_used);
    MLV_update_window();
}


/* Dessine les 'nb' éléments Score dans 'tab' à partir du coin Nord Ouest 'x', 'y' */
void draw_scores(Score *tab, int nb, int x, int y){
    int i;
    MLV_draw_text(x, y, "Name :", MLV_COLOR_BLUE_VIOLET);
    MLV_draw_text(x + SIZE_CASE * 9, y, "Time :", MLV_COLOR_BLUE_VIOLET);
    MLV_draw_text(x + SIZE_CASE * 16, y, "Mana used :", MLV_COLOR_BLUE_VIOLET);
    y += 20;
    for (i = 0; i < nb; i++){
        MLV_draw_text(x, y + (20 * i), "%s", MLV_COLOR_WHITE, tab[i].name);
        MLV_draw_text(x + SIZE_CASE * 9, y + (20 * i), "%0.2fs", MLV_COLOR_WHITE, tab[i].time);
        MLV_draw_text(x + SIZE_CASE * 16, y + (20 * i), "%d", MLV_COLOR_WHITE, tab[i].mana_used);
    }
}

/* Dessine les classements de 'r' */
void draw_rankings(Rankings *r){
    int x, y;
    x = SIZE_CASE;
    y = WINDOW_HEIGHT / 2 + 50;
    MLV_draw_text(x + SIZE_CASE * 4, y, "Rankings by best times :", MLV_COLOR_RED);
    draw_scores(r->best_times, r->nb_t, x, y + 30);

    x = WINDOW_WIDTH / 2 + SIZE_CASE * 4;
    MLV_draw_text(x + SIZE_CASE * 4 , y, "Rankings by best mana used :", MLV_COLOR_RED);
    draw_scores(r->best_mana_used, r->nb_m, x, y + 30);
    MLV_update_window();
}


/* Permet au joueur d'entrer son nom 'name' de taille maximal 'size' 
* qui sera plus tard ajouté dans le tableau de classement 
*/
void draw_enter_name(char *name, int size){
    MLV_Keyboard_button key; /* Code de la touche pressée */
    int unicode;
    char *text;
    int i, space;
    int x, y;
    i = 0; /* Sert d'indice à 'name' */
    space = 0; /* Espace entre les différents caractères entrés au clavier par le joueur */
    x = WINDOW_HEIGHT / 2 - SIZE_CASE * 2;
    y = SIZE_CASE * 15;

    MLV_draw_text(x, y, "Enter Name : ", MLV_COLOR_GREEN);
    while(1){
        MLV_actualise_window();
        MLV_wait_keyboard(&key, NULL, &unicode);
        if (key == MLV_KEYBOARD_RETURN) return; /* Touche entrée */
        else if (key == MLV_KEYBOARD_BACKSPACE){ /* Touche de suppression pour revenir en arrière */
            if (i > 0){ /* Pour éviter name[-1] */
                i--;
                name[i] = '\0';
                space -= 8;
                /* On dessine un rectangle noir sur les caractères pour les effacer */
                MLV_draw_filled_rectangle(x + SIZE_CASE * 7 + space, y, 20, 20, MLV_COLOR_BLACK);
            }
        }
        else if (i < size) { /* Pour éviter un index out of range */
            text = MLV_convert_unicode_to_string(unicode); /* on récupère le caractère avec text.. */
            MLV_draw_text(x + SIZE_CASE * 7 + space, y, text, MLV_COLOR_WHITE);
            name[i] = text[0]; /* ..qu'on ajoute à 'name' */
            free(text);
            space += 8;
            i++;
        }
    }
}


/* Initialisation de l'image 'image' et changement de sa taille pour l'adapter au jeu */
int init_image(const char *image){
    MLV_Image *img;
    img = MLV_load_image(image);
    if (img == NULL) return -1;
    MLV_resize_image(img, WINDOW_WIDTH + SPACE, WINDOW_HEIGHT + SPACE);
    MLV_draw_image(img, 0, 0);
    return 0;
}

/* Permet d'afficher du texte avec fonte */
int draw_text(int x, int y, char* text, int size, MLV_Color color){
    MLV_Font *font;
    font = MLV_load_font("img/MustardSmile.ttf", size);
    if (font == NULL) return -1;
    MLV_draw_text_with_font(x, y, text, font, color);
    return 0;
}

/* Permet l'affichage du logo du jeu */
int draw_logo(){
    if (draw_text(WINDOW_WIDTH / 10 - 5, WINDOW_HEIGHT / 7,  "Stealth ", 68, MLV_COLOR_YELLOW2) == -1) return -1;
    if (draw_text(WINDOW_WIDTH / 10 + 47, WINDOW_HEIGHT / 7 + 70 + 30 , " Game", 68, MLV_COLOR_YELLOW2) == -1) return -1;
    if (draw_text(WINDOW_WIDTH / 10 - 5, WINDOW_HEIGHT / 7 ,"Stealth ", 65, MLV_COLOR_RED2) == -1) return -1;
    if (draw_text(WINDOW_WIDTH / 10 + 47, WINDOW_HEIGHT / 7 + 70 + 30, " Game", 65, MLV_COLOR_RED2) == -1) return -1;
    return 0;
}

/* Écrit les choix du menu dans un rectangle adapté */
int draw_box(){
    MLV_Font *font;
    font = MLV_load_font("img/MustardSmile.ttf", 50);
    if (font == NULL) return -1;
    /* Choix pour Jouer */
    MLV_draw_adapted_text_box_with_font(
                X_BOX_PLAY , Y_BOX_PLAY,
                " PLAY ",
                font,
                10,
                MLV_COLOR_RED, MLV_COLOR_RED, MLV_COLOR_BLUE4,
                MLV_TEXT_CENTER
    );
    /* Choix pour Quitter */
    MLV_draw_adapted_text_box_with_font(
                X_BOX_QUIT, Y_BOX_QUIT,
                " QUIT ",
                font,
                10,
                MLV_COLOR_YELLOW, MLV_COLOR_YELLOW, MLV_COLOR_BLUE4,
                MLV_TEXT_CENTER
    ); 
    return 0;
}

/* Affiche l'intégralité du menu comprenant à l'arrière plan l'image de nom 'img_name' */
int draw_menu(char *img_name){
    if (init_image(img_name) == -1) return -1;
    if (draw_logo() == -1) return -1;
    if (draw_box() == -1) return -1;
    MLV_actualise_window();
    return 0;
}


/* Gestion de clic dans le menu. Renvoie 1 si le clic est sur Play, sinon 0 car sur Quit */
int play(){
    int x, y;
    while(1){
        MLV_get_mouse_position(&x, &y);
        /* Clic pour Play */
        if (MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED ){
            if (x > X_BOX_PLAY && x < X_BOX_PLAY + 160 && y > Y_BOX_PLAY && y < Y_BOX_PLAY + 75){
                return 1;
            }
        }
        /* Clic pour Quit */
        if (MLV_get_mouse_button_state( MLV_BUTTON_LEFT ) == MLV_PRESSED ){
            if (x > X_BOX_QUIT && x < X_BOX_QUIT + 160 && y > Y_BOX_QUIT && y < Y_BOX_QUIT + 75){
                return 0;
            }   
        }
        MLV_actualise_window();
    }
}