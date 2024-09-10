#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_SIZE 10

typedef struct {
    char nom[50];
    char prenom[50];
    char email[100];
} Collaborateur;

typedef struct {
    int id;
    char titre[100];
    char description[255];
    struct tm deadline;
    struct tm dateCreation;
    int statut;  // 0 = to do, 1 = doing, 2 = done
    Collaborateur *collaborateurs;
    int nbCollaborateurs;
} Tache;

void ajouterTache(Tache **taches, int *n);
void afficherTaches(Tache *taches, int n);
void modifierTache(Tache *taches, int n);
void supprimerTache(Tache **taches, int *n);
int rechercherTacheParId(Tache *taches, int n, int id);
void afficherStatut(int statut);
void enregistrerTachesDansFichier(Tache *taches, int n);
void chargerTachesDepuisFichier(Tache **taches, int *n);
void affecterCollaborateur(Tache *taches, int n);
void finaliserTache(Tache *taches, int n);

int main() {
    Tache *taches = NULL;
    int nombreTaches = 0;
    int choix;

    chargerTachesDepuisFichier(&taches, &nombreTaches);

    do {
        printf("\nMenu\n1. Ajouter une tâche\n2. Afficher toutes les tâches\n3. Modifier une tâche\n4. Supprimer une tâche\n5. Finaliser une tâche\n6. Quitter\nChoix: ");
        scanf("%d", &choix);
        getchar();

        switch (choix) {
            case 1:
                ajouterTache(&taches, &nombreTaches);
                break;
            case 2:
                afficherTaches(taches, nombreTaches);
                break;
            case 3:
                modifierTache(taches, nombreTaches);
                break;
            case 4:
                supprimerTache(&taches, &nombreTaches);
                break;
            case 5:
                finaliserTache(taches, nombreTaches);
                break;
            case 6:
                enregistrerTachesDansFichier(taches, nombreTaches);
                printf("Au revoir!\n");
                break;
            default:
                printf("Choix non valide.\n");
                break;
        }
    } while (choix != 6);

    // Libération de la mémoire
    for (int i = 0; i < nombreTaches; i++) {
        free(taches[i].collaborateurs);
    }
    free(taches);

    return 0;
}

void ajouterTache(Tache **taches, int *n) {
    *taches = realloc(*taches, (*n + 1) * sizeof(Tache));
    Tache *nouvelleTache = &(*taches)[*n];

    nouvelleTache->id = (*n) + 1;
    printf("Titre de la tâche: ");
    fgets(nouvelleTache->titre, sizeof(nouvelleTache->titre), stdin);
    strtok(nouvelleTache->titre, "\n");

    printf("Description de la tâche: ");
    fgets(nouvelleTache->description, sizeof(nouvelleTache->description), stdin);
    strtok(nouvelleTache->description, "\n");

    time_t now = time(NULL);
    nouvelleTache->dateCreation = *localtime(&now);

    printf("Deadline (jj mm aaaa): ");
    scanf("%d %d %d", &nouvelleTache->deadline.tm_mday, &nouvelleTache->deadline.tm_mon, &nouvelleTache->deadline.tm_year);
    nouvelleTache->deadline.tm_year -= 1900;
    nouvelleTache->deadline.tm_mon -= 1;

    printf("Statut (0 = à réaliser, 1 = en cours, 2 = finalisée): ");
    scanf("%d", &nouvelleTache->statut);
    getchar();

    nouvelleTache->nbCollaborateurs = 0;
    nouvelleTache->collaborateurs = NULL;

    (*n)++;
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

void afficherTaches(Tache *taches, int n) {
    for (int i = 0; i < n; i++) {
        printf("ID: %d | Titre: %s | Description: %s | Deadline: %02d-%02d-%d | Date de création: %02d-%02d-%d | Statut: ",
            taches[i].id, taches[i].titre, taches[i].description,
            taches[i].deadline.tm_mday, taches[i].deadline.tm_mon + 1, taches[i].deadline.tm_year + 1900,
            taches[i].dateCreation.tm_mday, taches[i].dateCreation.tm_mon + 1, taches[i].dateCreation.tm_year + 1900);
        afficherStatut(taches[i].statut);
        printf("\nCollaborateurs:\n");
        for (int j = 0; j < taches[i].nbCollaborateurs; j++) {
            printf(" - %s %s (%s)\n", taches[i].collaborateurs[j].nom, taches[i].collaborateurs[j].prenom, taches[i].collaborateurs[j].email);
        }
        printf("\n");
    }
}

void modifierTache(Tache *taches, int n) {
    int id;
    printf("ID de la tâche à modifier: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(taches, n, id);
    if (index != -1) {
        printf("Nouvelle description: ");
        fgets(taches[index].description, sizeof(taches[index].description), stdin);
        strtok(taches[index].description, "\n");

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

void supprimerTache(Tache **taches, int *n) {
    int id;
    printf("ID de la tâche à supprimer: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(*taches, *n, id);
    if (index != -1) {
        free((*taches)[index].collaborateurs);

        for (int j = index; j < (*n) - 1; j++) {
            (*taches)[j] = (*taches)[j + 1];
        }
        *taches = realloc(*taches, (*n - 1) * sizeof(Tache));
        (*n)--;
        printf("Tâche supprimée avec succès.\n");
    } else {
        printf("Tâche non trouvée.\n");
    }
}

int rechercherTacheParId(Tache *taches, int n, int id) {
    for (int i = 0; i < n; i++) {
        if (taches[i].id == id) {
            return i;
        }
    }
    return -1; // Not found
}

void enregistrerTachesDansFichier(Tache *taches, int n) {
    FILE *file = fopen("taches.dat", "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        return;
    }

    fwrite(&n, sizeof(int), 1, file);
    for (int i = 0; i < n; i++) {
        fwrite(&taches[i], sizeof(Tache), 1, file);
    }

    fclose(file);
}

void chargerTachesDepuisFichier(Tache **taches, int *n) {
    FILE *file = fopen("taches.dat", "rb");
    if (file == NULL) {
        *taches = NULL;
        *n = 0;
        return;
    }

    fread(n, sizeof(int), 1, file);
    *taches = malloc(*n * sizeof(Tache));

    for (int i = 0; i < *n; i++) {
        fread(&(*taches)[i], sizeof(Tache), 1, file);
        (*taches)[i].collaborateurs = NULL;  // Initialiser les collaborateurs pour éviter les fuites de mémoire
    }

    fclose(file);
}

void affecterCollaborateur(Tache *taches, int n) {
    int id;
    printf("ID de la tâche à laquelle ajouter un collaborateur: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(taches, n, id);
    if (index != -1) {
        Tache *tache = &taches[index];
        tache->collaborateurs = realloc(tache->collaborateurs, (tache->nbCollaborateurs + 1) * sizeof(Collaborateur));

        printf("Nom du collaborateur: ");
        fgets(tache->collaborateurs[tache->nbCollaborateurs].nom, sizeof(tache->collaborateurs[tache->nbCollaborateurs].nom), stdin);
        strtok(tache->collaborateurs[tache->nbCollaborateurs].nom, "\n");

        printf("Prénom du collaborateur: ");
        fgets(tache->collaborateurs[tache->nbCollaborateurs].prenom, sizeof(tache->collaborateurs[tache->nbCollaborateurs].prenom), stdin);
        strtok(tache->collaborateurs[tache->nbCollaborateurs].prenom, "\n");

        printf("Email du collaborateur: ");
        fgets(tache->collaborateurs[tache->nbCollaborateurs].email, sizeof(tache->collaborateurs[tache->nbCollaborateurs].email), stdin);
        strtok(tache->collaborateurs[tache->nbCollaborateurs].email, "\n");

        tache->nbCollaborateurs++;
        printf("Collaborateur ajouté avec succès.\n");
    } else {
        printf("Tâche non trouvée.\n");
    }
}

void finaliserTache(Tache *taches, int n) {
    int id;
    printf("ID de la tâche à finaliser: ");
    scanf("%d", &id);
    getchar();

    int index = rechercherTacheParId(taches, n, id);
    if (index != -1) {
        taches[index].statut = 2; // Finaliser la tâche
        printf("Tâche finalisée avec succès.\n");
    } else {
        printf("Tâche non trouvée.\n");
    }
}
