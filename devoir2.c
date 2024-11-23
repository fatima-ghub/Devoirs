#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

void creer_matrice(int dim, const char *nom_fic, int b) 
{
    FILE *fic;
    if (b) 
    {
        fic = fopen(nom_fic, "wb");
    } 
    else 
    {
        fic = fopen(nom_fic, "w");
    }
    if (!fic) 
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            int v = rand() % 100;
            if (b) 
            {
                fwrite(&v, sizeof(int), 1, fic);
            } else 
            {
                fprintf(fic, "%d ", v);
            }
        }
        if (!b) 
        {
            fprintf(fic, "\n");
        }
    }

    fclose(fic);
}

void afficher_matrice(int dim, const char *nom_fic, int b) 
{
    FILE *fic;
    if (b) 
    {
        fic = fopen(nom_fic, "rb");
    } 
    else 
    {
        fic = fopen(nom_fic, "r");
    }
    if (!fic) 
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < dim; i++) 
    {
        for (int j = 0; j < dim; j++) 
        {
            int v;
            if (b) 
            {
                fread(&v, sizeof(int), 1, fic);
                printf("%d ", v);
            } 
            else 
            {
                fscanf(fic, "%d", &v);
                printf("%d ", v);
            }
        }
        printf("\n");
    }

    fclose(fic);
}

int main(int argc, char *argv[]) 
{
    int c = 0, d = 0, dim = 0, b = 1;
    char nom_fic[256] = "";

    int opt;
    while ((opt = getopt(argc, argv, "cad:btf:")) != -1) 
    {
        switch (opt) 
        {
            case 'c':
                c = 1;
                break;
            case 'a':
                d = 1;
                break;
            case 'd':
                dim = atoi(optarg);
                break;
            case 'b':
                b = 1;
                break;
            case 't':
                b = 0;
                break;
            case 'f':
                strncpy(nom_fic, optarg, sizeof(nom_fic) - 1);
                nom_fic[sizeof(nom_fic) - 1] = '\0';
                break;
            default:
                fprintf(stderr, "Commande invalide");
                return 1;
        }
    }

    if (dim <= 0 || strlen(nom_fic) == 0) 
    {
        fprintf(stderr, "Commande invalide");
        return 1;
    }

    if (c) 
    {
        creer_matrice(dim, nom_fic, b);
    }

    if (d) 
    {
        afficher_matrice(dim, nom_fic, b);
    }

    return 0;
}
