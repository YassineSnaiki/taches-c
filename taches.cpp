#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct Tache {
    int id;
    char titre[100];
    char description[255];
    struct tm deadline;
    int statut;  // 0 = to do, 1 = doing, 2 = done
};

int rechercherTacheParId(struct Tache* taches, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (taches[i].id == id) {
            return i;
        }
    }
    return -1; // Not found
}

void afficherStatut(int statut) {
    switch (statut) {
        case 0:
            printf("To Do");
            break;
        case 1:
            printf("Doing");
            break;
        case 2:
            printf("Done");
            break;
        default:
            printf("Unknown");
            break;
    }
}

void ajouterTache(struct Tache* taches, int* n) {
    struct Tache nouvelleTache;
    nouvelleTache.id = (*n) + 1;

    printf("Titre de la tâche: ");
    fgets(nouvelleTache.titre, sizeof(nouvelleTache.titre), stdin);

    printf("Description de la tâche: ");
    fgets(nouvelleTache.description, sizeof(nouvelleTache.description), stdin);

    printf("Deadline (jj mm aaaa): ");
    scanf("%d %d %d", &nouvelleTache.deadline.tm_mday, &nouvelleTache.deadline.tm_mon, &nouvelleTache.deadline.tm_year);
    nouvelleTache.deadline.tm_year -= 1900;
    nouvelleTache.deadline.tm_mon -= 1;

    printf("Statut (0 = à réaliser, 1 = en cours, 2 = finalisée): ");
    scanf("%d", &nouvelleTache.statut);
    getchar();

    taches[*n] = nouvelleTache;
    (*n)++;
}

void afficherTaches(struct Tache* taches, int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d | Titre: %s | Description: %s | Deadline: %02d-%02d-%d | Statut: ",
            taches[i].id, taches[i].titre, taches[i].description,
            taches[i].deadline.tm_mday, taches[i].deadline.tm_mon + 1, taches[i].deadline.tm_year + 1900);
        afficherStatut(taches[i].statut);
        printf("\n");
    }
}

void modifierTache(struct Tache* taches, int n) {
    int id;
    printf("ID de la tâche à modifier: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(taches, n, id);
    if (index != -1) {
        printf("Nouvelle description: ");
        fgets(taches[index].description, sizeof(taches[index].description), stdin);

        printf("Nouveau statut (0 = à réaliser, 1 = en cours, 2 = finalisée): ");
        scanf("%d", &taches[index].statut);
        getchar();

        printf("Nouveau deadline (jj mm aaaa): ");
        scanf("%d %d %d", &taches[index].deadline.tm_mday, &taches[index].deadline.tm_mon, &taches[index].deadline.tm_year);
        taches[index].deadline.tm_year -= 1900;
        taches[index].deadline.tm_mon -= 1;

        printf("Tâche modifiée avec succès.\n");
    } else {
        printf("Tâche non trouvée.\n");
    }
}

void supprimerTache(struct Tache* taches, int* n) {
    int id;
    printf("ID de la tâche à supprimer: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(taches, *n, id);
    if (index != -1) {
        for (int j = index; j < (*n) - 1; j++) {
            taches[j] = taches[j + 1];
        }
        (*n)--;
        printf("Tâche supprimée avec succès.\n");
    } else {
        printf("Tâche non trouvée.\n");
    }
}

int main() {
    struct Tache taches[100];
    int nombreTaches = 0;
    int choix;

    do {
        printf("\nMenu\n1. Ajouter une tâche\n2. Afficher toutes les tâches\n3. Modifier une tâche\n4. Supprimer une tâche\n5. Quitter\nChoix: ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                ajouterTache(taches, &nombreTaches);
                break;
            case 2:
                afficherTaches(taches, nombreTaches);
                break;
            case 3:
                modifierTache(taches, nombreTaches);
                break;
            case 4:
                supprimerTache(taches, &nombreTaches);
                break;
            case 5:
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix non valide.\n");
                break;
        }
    } while (choix != 5);

    return 0;
}
