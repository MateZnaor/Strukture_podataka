#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_BODOVA 50

typedef struct Student
{
    char ime[50];
    char prezime[50];
    int bodovi;

} stud;

int main()
{
    int i = 0;
    char buffer[50] = { 0 };
    int brojac = 0;
    double relativni_bodovi = 0;
    FILE* fp;
    fp = fopen("popis.txt", "r");
    if (!fp)
    {
        printf("Greska.");
        return -1;
    }
    while (!feof(fp))
    {
        fgets(buffer, 50, fp);
        brojac++;
    }
    rewind(fp);
    if (brojac == 0)
    {
        printf("Datoteka je prazna\n");
        fclose(fp);
        return 0;
    }

    stud* studenti = NULL;
    studenti = (stud*)malloc(brojac * sizeof(stud));

    while (!feof(fp))
    {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
        i++;
    }
    for (int i = 0; i < brojac; i++)
    {
        relativni_bodovi = (double)studenti[i].bodovi / MAX_BODOVA * 100;
        printf("%s %s %d(apsolutni bodovi) %.2f(relativni bodovi)\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni_bodovi);
    }

    free(studenti);
    fclose(fp);
    return 0;
}
