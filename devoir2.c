#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Pour getopt
#include <time.h>

// Fonction pour générer une matrice carrée aléatoire
void generate_matrix(int dim, int matrix[dim][dim]) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matrix[i][j] = rand() % 100 + 1; // Valeurs entre 1 et 100
        }
    }
}

// Fonction pour sauvegarder la matrice en mode binaire
void save_matrix_binary(const char *filename, int dim, int matrix[dim][dim]) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }
    fwrite(&dim, sizeof(int), 1, file);          // Sauvegarder la dimension
    fwrite(matrix, sizeof(int), dim * dim, file); // Sauvegarder les données
    fclose(file);
    printf("Matrice sauvegardée en mode binaire dans '%s'.\n", filename);
}

// Fonction pour sauvegarder la matrice en mode texte
void save_matrix_text(const char *filename, int dim, int matrix[dim][dim]) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%d\n", dim); // Sauvegarder la dimension
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Matrice sauvegardée en mode texte dans '%s'.\n", filename);
}

// Fonction pour lire et afficher une matrice depuis un fichier binaire
void read_matrix_binary(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier binaire");
        exit(EXIT_FAILURE);
    }

    int dim;
    fread(&dim, sizeof(int), 1, file); // Lire la dimension

    int matrix[dim][dim];
    fread(matrix, sizeof(int), dim * dim, file); // Lire les données
    fclose(file);

    printf("Matrice lue depuis '%s' (binaire) :\n", filename);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour lire et afficher une matrice depuis un fichier texte
void read_matrix_text(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier texte");
        exit(EXIT_FAILURE);
    }

    int dim;
    fscanf(file, "%d", &dim); // Lire la dimension

    int matrix[dim][dim];
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    fclose(file);

    printf("Matrice lue depuis '%s' (texte) :\n", filename);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    int dim = 0;
    char *filename = NULL;
    int create = 0, display = 0;
    int binary_format = 1; // Par défaut, le fichier est en binaire
    int opt;

    // Lecture des options
    while ((opt = getopt(argc, argv, "cad:f:bt")) != -1) {
        switch (opt) {
            case 'c':
                create = 1;
                break; // Demander la création d'une matrice
            case 'a':
                display = 1;
                break; // Afficher le contenu d'un fichier
            case 'd':
                dim = atoi(optarg);
                break; // Spécifier la dimension de la matrice
            case 'f':
                filename = optarg;
                break; // Nom du fichier
            case 'b':
                binary_format = 1;
                break; // Spécifier le format binaire (par défaut)
            case 't':
                binary_format = 0;
                break; // Spécifier le format texte
            default:
                fprintf(stderr, "Usage: %s -c|-a -d <dimension> -f <filename> [-b|-t]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Vérifications des arguments obligatoires
    if (!filename) {
        fprintf(stderr, "Erreur : nom de fichier non spécifié.\n");
        exit(EXIT_FAILURE);
    }
    if (create && dim <= 0) {
        fprintf(stderr, "Erreur : dimension invalide ou manquante pour la création.\n");
        exit(EXIT_FAILURE);
    }

    if (create) {
        // Création d'une matrice
        int matrix[dim][dim];
        srand(time(NULL)); // Initialiser les nombres aléatoires
        generate_matrix(dim, matrix);

        // Sauvegarder la matrice dans les deux formats
        save_matrix_binary(filename, dim, matrix);
        save_matrix_text(filename, dim, matrix);

    } else if (display) {
        // Lecture et affichage de la matrice
        if (binary_format) {
            read_matrix_binary(filename);
        } else {
            read_matrix_text(filename);
        }
    } else {
        fprintf(stderr, "Erreur : spécifiez -c (création) ou -a (affichage).\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}