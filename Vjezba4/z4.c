#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

struct _Polynom;
typedef struct _Polynom* Position;
typedef struct _Polynom {
    int coeff;
    int pow;
    Position next;
} Polynom;


int insertSorted(Position head, int coeff, int pow)
{
    Position q = head;
    Position toDelete = NULL;

    if (coeff == 0)
        return 0;

    while (q->next != NULL && q->next->pow > pow)
        q = q->next;

    if (q->next != NULL && q->next->pow == pow) {
        q->next->coeff += coeff;
        if (q->next->coeff == 0) {
            toDelete = q->next;
            q->next = toDelete->next;
            free(toDelete);
        }
        return 0;
    }

    Position newEl = (Position)malloc(sizeof(Polynom));
    if (newEl == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return -1;
    }
    newEl->coeff = coeff;
    newEl->pow = pow;
    newEl->next = q->next;
    q->next = newEl;
    return 0;
}

int readFromFile(Position head, const char* filename)
{
    FILE* fp = fopen(filename, "r");
    int coeff, pow;

    if (fp == NULL) {
        printf("Datoteku \"%s\" nije moguce otvoriti.\n", filename);
        return -1;
    }

    while (fscanf(fp, "%d %d", &coeff, &pow) == 2) {
        if (insertSorted(head, coeff, pow) != 0) {
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

int addPoly(Position first, Position second, Position result)
{
    Position p;

    for (p = first->next; p != NULL; p = p->next)
        if (insertSorted(result, p->coeff, p->pow) != 0)
            return -1;

    for (p = second->next; p != NULL; p = p->next)
        if (insertSorted(result, p->coeff, p->pow) != 0)
            return -1;

    return 0;
}


int mulPoly(Position first, Position second, Position result)
{
    Position p, q;

    for (p = first->next; p != NULL; p = p->next)
        for (q = second->next; q != NULL; q = q->next)
            if (insertSorted(result, p->coeff * q->coeff, p->pow + q->pow) != 0)
                return -1;

    return 0;
}

void printPoly(Position first)
{
    int isFirst = 1;

    if (first == NULL) {
        printf("0\n");
        return;
    }

    for (first != NULL; first = first->next) {
        if (isFirst) {
            printf("%dx^%d", first->coeff, first->pow);
            isFirst = 0;
        }
        else if (first->coeff > 0) {
            printf(" + %dx^%d", first->coeff, first->pow);
        }
        else {
            printf(" - %dx^%d", -first->coeff, first->pow);
        }
    }
    printf("\n");
}

void deleteAll(Position head)
{
    Position temp;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}
int main(void)
{
    Polynom headFirst = { 0, 0, NULL };
    Polynom headSecond = { 0, 0, NULL };
    Polynom headSum = { 0, 0, NULL };
    Polynom headProd = { 0, 0, NULL };

    if (readFromFile(&headFirst, "p1.txt") != 0) {
        printf("Greska pri citanju prve datoteke.\n");
        deleteAll(&headFirst);
        return EXIT_FAILURE;
    }

    if (readFromFile(&headSecond, "p2.txt") != 0) {
        printf("Greska pri citanju druge datoteke.\n");
        deleteAll(&headFirst);
        deleteAll(&headSecond);
        return EXIT_FAILURE;
    }

    printf("\nP(x) = ");
    printPoly(headFirst.next);
    printf("Q(x) = ");
    printPoly(headSecond.next);

    if (addPoly(&headFirst, &headSecond, &headSum) == 0) {
        printf("\nP(x) + Q(x) = ");
        printPoly(headSum.next);
    }

    if (mulPoly(&headFirst, &headSecond, &headProd) == 0) {
        printf("P(x) * Q(x) = ");
        printPoly(headProd.next);
    }

    deleteAll(&headFirst);
    deleteAll(&headSecond);
    deleteAll(&headSum);
    deleteAll(&headProd);
    return EXIT_SUCCESS;
}
