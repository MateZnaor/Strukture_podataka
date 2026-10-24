#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Person* Position;

typedef struct _Person {
    char ime[50];
    char prezime[50];
    int godina;

    Position next;
}Person;

Position createPerson(const char* ime, const char* prezime, int godina) {
    Position newP = (Position)malloc(sizeof(Person));
    if (!newP) return NULL;
    strcpy(newP->ime, ime);
    strcpy(newP->prezime, prezime);
    newP->godina = godina;
    newP->next = NULL;
    return newP;
}

int insertAfter(Position p, Position q) {

    q->next = p->next;
    p->next = q;
    return 0;
}

int insertBegin(Position head, const char* ime, const char* prezime, int godina) {
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;
    return insertAfter(head, newP);
}

int insertEnd(Position head, const char* ime, const char* prezime, int godina) {
    if (!head) return -1;
    Position p = head;
    while (p->next != NULL) p = p->next;
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;
    return insertAfter(p, newP);
}

void printList(Position head) {
    if (!head || !head->next) {
        printf("Lista prazna.\n");
        return;
    }
    Position q = head->next;
    while (q) {
        printf("%s %s %d\n", q->ime, q->prezime, q->godina);
        q = q->next;
    }
}

Position findBySurname(Position head, const char* prezime) {
    if (!head) return NULL;
    Position q = head->next;
    while (q) {
        if (strcmp(q->prezime, prezime) == 0) return q;
        q = q->next;
    }
    return NULL;
}

Position findPrevious(Position head, const char* prezime) {
    if (!head) return NULL;
    Position prev = head;
    if (prev->next == NULL) return NULL;

    for (prev->next; prev->next && strcmp(prev->next->prezime, prezime) != 0; prev = prev->next);

    if (strcmp(prev->next->prezime, prezime) != 0) {
        return NULL;
    }

    return prev;
}

int deleteBySurname(Position p, const char* prezime) {
    if (!p) return -1;
    Position prev = p;
    Position curr = p->next;
    while (curr && strcmp(curr->prezime, prezime) != 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!curr) {
        return 0;
    }


    prev->next = curr->next;
    free(curr);
    return 1;
}

void freeList(Position head) {
    if (!head) return;
    Position curr = head->next;
    while (curr) {
        Position temp = curr;
        curr = curr->next;
        free(temp);
    }
    head->next = NULL;
}

int addBefore(Position head, const char* ime, const char* prezime, int godina, const char* targetPrezime) {
    Position prev = findPrevious(head, targetPrezime);
    if (!prev) {
        printf("Osoba s prezimenom %s nije pronadena.\n", targetPrezime);
        return -1;
    }
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;

    if (prev) {
        return insertAfter(prev, newP);
    }
}

int addAfter(Position head, const char* ime, const char* prezime, int godina, const char* targetPrezime) {
    Position after = findBySurname(head, targetPrezime);
    if (!after) {
        printf("Osoba s prezimenom %s nije pronadena.\n", targetPrezime);
        return -1;
    }
    Position newP = createPerson(ime, prezime, godina);
    if (!newP) return -1;
    if (after) {
        return insertAfter(after, newP);
    }

}

int addPerson(Position head) {
    char ime[50], prezime[50];
    int god;
    char choice;
    printf("Unesite ime prezime godina:\n");
    if (scanf("%s %s %d", ime, prezime, &god) != 3) {
        printf("Neispravan unos.\n");
        return -1;
    }
    printf("Unesite 'p' za unos na pocetak ili 'k' za kraj liste, a ako zelite dodati ispred ili iza odredene osobe upisite i: ");
    scanf(" %c", &choice);
    if (choice == 'p' || choice == 'P')
        return insertBegin(head, ime, prezime, god);
    else if (choice == 'k' || choice == 'K')
        return insertEnd(head, ime, prezime, god);
    else if (choice == 'i' || choice == 'I') {
        char target[50];
        printf("Unesite prezime osobe ispred ili iza koje zelite dodati: ");
        scanf("%s", target);
        Position p = findBySurname(head, target);
        if (!p) {
            printf("Osoba s prezimenom %s nije pronadena.\n", target);
            return -1;
        }
        printf("Zelite li dodati ispred ili iza osobe s prezimenom %s? (i/a): ", target);
        char ia;
        do
        {
            scanf(" %c", &ia);
            if (ia == 'i' || ia == 'I') {
                return addBefore(head, ime, prezime, god, target);
            }
            else if (ia == 'a' || ia == 'A') {
                return addAfter(head, ime, prezime, god, target);
            }
            else {
                printf("Neispravan unos.\n");
                return -1;
            }
        } while (ia == 'i' || ia == 'I' || ia == 'a' || ia == 'A');

    }
    else
    {
        printf("Neispravan unos.\n");
        return -1;
    }
}

void sortBySurname(Position head) {
    if (!head || !head->next) return;
    Position sorted = NULL;
    Position current = head->next;
    while (current) {
        Position next = current->next;
        if (!sorted || strcmp(current->prezime, sorted->prezime) < 0) {
            current->next = sorted;
            sorted = current;
        }
        else {
            Position temp = sorted;
            while (temp->next && strcmp(current->prezime, temp->next->prezime) >= 0) {
                temp = temp->next;
            }
            current->next = temp->next;
            temp->next = current;
        }
        current = next;
    }
    head->next = sorted;
}

int main(void) {
    Person Head = { .ime = { 0 }, .prezime = { 0 }, .godina = 0, .next = NULL };
    Position head = &Head;

    printf("Zelite li upis u datoteku ili ispis iz datoteke? (u/i): ");
    char choice2;
    scanf(" %c", &choice2);
    if (choice2 == 'u' || choice2 == 'U') {
        FILE* fp = fopen("lista.txt", "w");
        if (!fp) {
            printf("Greska pri otvaranju datoteke za upis.\n");
            return -1;
        }

        int n = 0;
        printf("Koliko osoba zelite unijeti? ");
        if (scanf("%d", &n) != 1 || n <= 0) {
            printf("Neispravan unos.\n");
            fclose(fp);
            return -1;
        }

        for (int i = 0; i < n; i++) {
            if (addPerson(head) != 0) {
                printf("Greska pri dodavanju osobe.\n");
            }
        }

        printf("\nTrenutna lista:\n");
        printList(head);

        printf("\n");
        printf("Lista nakon sortiranja:\n");
        sortBySurname(head);
        printList(head);

        char target[50];
        printf("\nUnesi prezime za trazenje: ");
        scanf("%s", target);
        Position found = findBySurname(head, target);
        if (found) printf("Pronaden: %s %s %d\n", found->ime, found->prezime, found->godina);
        else printf("Osoba s prezimenom %s nije pronadena.\n", target);

        int deleted = 0;
        do
        {
            printf("\nUnesi prezime osobe za brisanje ili e/E za izlaz: ");
            scanf("%s", target);
            if (strcmp(target, "e") == 0 || strcmp(target, "E") == 0) {
                printf("Brisanje preskoceno.");
                break;
            }

            deleted = deleteBySurname(head, target);
            if (deleted == 1) {
                printf("Osoba izbrisana.\n");
            }
            else if (deleted == 0) {
                printf("Osoba nije pronadena.\n");
            }
            else {
                printf("Greska pri brisanju.\n");
                break;
            }

        } while (deleted != 1);

        printf("\nLista nakon brisanja:\n");
        printList(head);

        printf("Sortirana lista je u datoteci 'lista.txt'.\n");

        Position current = head->next;
        while (current) {
            fprintf(fp, "%s %s %d\n", current->ime, current->prezime, current->godina);
            current = current->next;
        }

        fclose(fp);
    }
    else if (choice2 == 'i' || choice2 == 'I') {
        FILE* fp = fopen("lista.txt", "r");
        if (!fp) {
            printf("Greska pri otvaranju datoteke za citanje.\n");
            return -1;
        }

        char ime[50], prezime[50];
        int godina;

        while (fscanf(fp, "%s %s %d", ime, prezime, &godina) == 3) {
            if (insertEnd(head, ime, prezime, godina) != 0) {
                printf("Greska pri dodavanju osobe iz datoteke.\n");
                fclose(fp);
                freeList(head);
                return -1;
            }
        }

        fclose(fp);
        printList(head);
    }
    else {
        printf("Neispravan unos.\n");
    }

    freeList(head);
    return 0;
}