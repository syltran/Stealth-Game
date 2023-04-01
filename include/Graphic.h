#ifndef __GRAPHIC__
#define __GRAPHIC__

#include "GameEngine.h"
#include "Ranking.h"
#include <MLV/MLV_all.h>
#define WINDOW_WIDTH NB_COL * SIZE_CASE /* Largeur de la fenêtre */
#define WINDOW_HEIGHT NB_LIN * SIZE_CASE /* Hauteur de la fenêtre */
#define SPACE 35 /* Espace supplémentaire pour ajouter des informations utiles dans la fenêtre */
#define X_BOX_PLAY WINDOW_WIDTH / 12 /* Coordonnée X du coin Nord-Ouest de la boite pour Play */
#define Y_BOX_PLAY WINDOW_HEIGHT - (WINDOW_HEIGHT / 6) /* Coordonnée Y du coin Nord-Ouest de la boite pour Play */
#define X_BOX_QUIT WINDOW_WIDTH - (WINDOW_WIDTH / 4) /* Coordonnée X du coin Nord-Ouest de la boite pour Quit */
#define Y_BOX_QUIT WINDOW_HEIGHT - (WINDOW_HEIGHT / 6) /* Coordonnée Y du coin Nord-Ouest de la boite pour Quit */

void draw_grid(const Grid *grid);
void draw_disc(Disc disc, MLV_Color color);
void draw_player(const Player *player);
void draw_detect_zone(Disc agent, int dist, MLV_Color color);
void draw_guard(const Guard *guard);
void draw_mana_bar(const Player *player);
void draw_nb_relics_taken_in_room(const Room *room);
void draw_room(const Room *room);

void draw_end_game(int win, const Score *score);
void draw_scores(Score *tab, int nb, int x, int y);
void draw_rankings(Rankings *r);
void draw_enter_name(char *name, int size);

int init_image(const char *image);
int draw_text(int x, int y, char* text, int size, MLV_Color color);
int draw_logo();
int draw_box();
int draw_menu(char *img_name);
int play();

#endif