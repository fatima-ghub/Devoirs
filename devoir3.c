#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void split(const char *fic, size_t taille) 
{
    FILE *fichier = fopen(fic, "rb");
    if (!fichier) 
    {
        perror("Erreur d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char stock[taille];
    size_t lecture;
    int numpart = 1;
    char nom_part[256];

    while ((lecture = fread(stock, 1, taille, fichier)) > 0) 
    {
        snprintf(nom_part, sizeof(nom_part), "%s.part%d", fic, numpart);
        FILE *part = fopen(nom_part, "wb");
        if (!part) 
        {
            perror("Erreur d'ouverture du fichier de partie");
            exit(EXIT_FAILURE);
        }
        fwrite(stock, 1, lecture, part);
        fclose(part);
        numpart++;
    }

    fclose(fichier);
}

void join(const char *nom_sortie, int id_parts, char *parties[]) 
{
    FILE *sortie = fopen(nom_sortie, "wb");
    if (!sortie) 
    {
        perror("Erreur d'ouverture du fichier de sortie");
        exit(EXIT_FAILURE);
    }

    char stock[1024];
    size_t lecture;

    for (int i = 0; i < id_parts; i++) 
    {
        FILE *part = fopen(parties[i], "rb");
        if (!part) 
        {
            perror("Erreur d'ouverture du fichier de partie");
            exit(EXIT_FAILURE);
        }

        while ((lecture = fread(stock, 1, sizeof(stock), part)) > 0) 
        {
            fwrite(stock, 1, lecture, sortie);
        }

        fclose(part);
    }

    fclose(sortie);
}

int main(int argc, char *argv[]) 
{
    if (argc < 3) {
        fprintf(stderr, "Split: %s\n", argv[0]);
        fprintf(stderr, "Join: %s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "split") == 0) 
    {
        if (argc != 4) 
        {
            fprintf(stderr, "Faites %s split + nom du fichier + taille de la partie\n", argv[0]);
            exit(EXIT_FAILURE);
        }
        const char *nom_fic = argv[2];
        size_t taille = atoi(argv[3]);
        split(nom_fic, taille);
    } 
    else 
    if (strcmp(argv[1], "join") == 0) 
    {
        if (argc < 4) 
        {
            fprintf(stderr, "Faites : %s join + nom fichier de sortie + part1 + part2 + ... + partN\n", argv[0]);
            exit(EXIT_FAILURE);
        }   
        const char *sortie = argv[2];
        join(sortie, argc - 3, &argv[3]);
    } 
    else 
    {
        fprintf(stderr, "Commande non reconnue: %s\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    return 0;
}

