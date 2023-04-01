#include "../include/Ranking.h"


/* Initialise le score 'score' */
void initialize_score(Score *score){
    score->time = 0.0;
    score->mana_used = 0;
}

/* Initialise les classements dans 'r' */
void initialize_rankings(Rankings *r){
    int i;
    Score score;
    initialize_score(&score);
    for (i = 0; i < MAX_S; i++){
        r->best_times[i] = score;
        r->best_mana_used[i] = score;
    }
    r->nb_t = 0;
    r->nb_m = 0;
}


/* Renvoie le temps passé en seconde entre 'start' et 'end' */
double time_achieved(struct timespec *start, struct timespec *end){
    return (end->tv_sec - start->tv_sec) + ((end->tv_nsec - start->tv_nsec) / 1.0E9);
}


/* Décale tous les éléments Score de 'tab' de taille 'size' à droite à partir de l'indice 'index' */
void shift_right(Score *tab, int size, int index){
    int i;
    for (i = size - 1; i > index; i--){
       tab[i] = tab[i - 1];
    }
}

/* Met à jour dans 'r' le classement par meilleur temps en y ajoutant possiblement le nouveau score 'score'.
 "possiblement" car si 'score' n'a pas un meilleur temps que les MAX_S scores déjà inscrits, il ne sera pas ajouté
*/
void update_best_times(Rankings *r, const Score *score){
    int i;
    for (i = 0; i < MAX_S; i++){
        if (r->nb_t == i){
            r->best_times[i] = *score;
            r->nb_t++;
            return;
        }
        else if (score->time < r->best_times[i].time){
            shift_right(r->best_times, MAX_S, i);
            r->best_times[i] = *score;
            return;
        }
    }
}

/* Met à jour dans 'r' le classement par meilleure consommation de mana en y ajoutant possiblement le nouveau score 'score'.
 "possiblement" car si 'score' n'a pas une meilleure consommation de mana que les MAX_S scores déjà inscrits, il ne sera pas ajouté
*/
void update_best_mana_used(Rankings *r, const Score *score){
    int i;
    for (i = 0; i < MAX_S; i++){
        if (r->nb_m == i){
            r->best_mana_used[i] = *score;
            r->nb_m++;
            return;
        }
        else if (score->mana_used < r->best_mana_used[i].mana_used){
            shift_right(r->best_mana_used, MAX_S, i);
            r->best_mana_used[i] = *score;
            return;
        }
    }
}

/* Met à jour les classements dans 'r' en y ajoutant possiblement le nouveau score 'score' */
void update_rankings(Rankings *r, const Score *score){
    update_best_times(&(*r), score);
    update_best_mana_used(&(*r), score);
}


/* Permet de sauvegarder dans le fichier nommé 'name' le score 'score' */
int save_score(char *name, const Score *score){
    FILE *file;
    file = fopen(name, "ab");
    if (file == NULL) return -1;
    if (fwrite(&(*score), sizeof(Score), 1, file) != 1) return -1; /* Erreur lors de l'écriture */
    fclose(file);
    return 0;
}

/* Permet de récupérer les éléments Score dans le fichier nommé 'name' pour remplir les classements dans 'r' */
int get_scores(char *name, Rankings *r){
    int n;
    Score score;
    FILE *file;
    file = fopen(name, "rb");
    if (file == NULL) return -1;

    /* tant qu'on a pas lu tous les Score de 'file' */
    while ((n = fread(&score, sizeof(Score), 1, file)) == 1){
        update_rankings(&(*r), &score);
    }
    fclose(file);
    return 0;
}
