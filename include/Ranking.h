#ifndef __RANKING__
#define __RANKING__

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <time.h>
#define SIZE_NAME 10 /* Taille d'un nom rentré */
#define MAX_S 10 /* Nombre maximal de score affiché par classement */

typedef struct{
    double time; /* Temps réalisé en seconde */
    int mana_used; /* Quantité de mana dépensée */
    char name[SIZE_NAME]; /* Nom rentré */
} Score; /* Représente le score du joueur en fin de partie */

typedef struct{
    Score best_times[MAX_S]; /* Par meilleur temps */
    Score best_mana_used[MAX_S]; /* Par meilleure consommation de mana */
    int nb_t; /* Nombre de score déjà inscrit dans best_times */
    int nb_m; /* Nombre de score déjà inscrit dans best_mana_used */
} Rankings; /* Représente les classements */

void initialize_score(Score *score);
void initialize_rankings(Rankings *r);
double time_achieved(struct timespec *start, struct timespec *end);
void shift_right(Score *tab, int size, int index);
void update_best_times(Rankings *r, const Score *score);
void update_best_mana_used(Rankings *r, const Score *score);
void update_rankings(Rankings *r, const Score *score);
int save_score(char *name, const Score *score);
int get_scores(char *name, Rankings *r);

#endif