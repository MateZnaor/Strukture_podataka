#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

typedef struct Stack* Position;
typedef struct Stack {
	int broj;
	Position next;
} stack;


int ReadFile(const char* dat, char* buffer, int velicina)
{
	FILE* fp = fopen(dat, "r");
	if (fp == NULL) {
		printf("Greska: datoteka '%s' se ne moze otvoriti.\n", dat);
		return -1;
	}

	if (fgets(buffer, velicina, fp) == NULL) {
		printf("Greska: datoteka je prazna.\n");
		fclose(fp);
		return -1;
	}

	fclose(fp);
	return 0;
}


int Push(Position head, int broj)
{
	Position newP = (Position)malloc(sizeof(stack));
	if (newP == NULL) {
		printf("Greska: alokacija memorije nije uspjela.\n");
		return -1;
	}

	newP->broj = broj;
	newP->next = head->next;
	head->next = newP;

	return 0;
}

int Pop(Position head, int* broj)
{
	Position q = head->next;
	if (q == NULL) {
		return -1; 
	}

	*broj = q->broj;
	head->next = q->next;
	free(q);

	return 0;
}


int Operacija(Position head, char operand)
{
	int a, b, rez;


	if (Pop(head, &b) != 0 || Pop(head, &a) != 0) {
		printf("Greska: nedovoljno operanada za operator '%c'.\n", operand);
		return -1;
	}

	switch (operand) {
	case '+': rez = a + b; break;
	case '-': rez = a - b; break;
	case '*': rez = a * b; break;
	case '/':
		if (b == 0) {
			printf("Greska: dijeljenje s nulom.\n");
			return -1;
		}
		rez = a / b;
		break;
	default:
		printf("Greska: nepoznat operator '%c'.\n", operand);
		return -1;
	}

	return Push(head, rez);
}


int Izracunaj(Position head, const char* buffer)
{
	const char* p = buffer;
	int broj, n;
	char znak;

	while (*p != '\0') {
		if (sscanf(p, " %d%n", &broj, &n) == 1) {
			if (Push(head, broj) != 0) {
				return -1;
			}
			p += n;
		}
		else if (sscanf(p, " %c%n", &znak, &n) == 1) {
			if (znak == '+' || znak == '-' || znak == '*' || znak == '/') {
				if (Operacija(head, znak) != 0) {
					return -1;
				}
				p += n;
			}
			else {
				printf("Greska: krivi unos '%c'.\n", znak);
				return -1;
			}
		}
		else {
			break; 
		}
	}

	return 0;
}


void FreeList(Position head)
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
	stack head = { .broj = 0, .next = NULL };
	char buffer[BUFFER_SIZE];
	int rezultat;

	if (ReadFile("Postfix.txt", buffer, BUFFER_SIZE) != 0) {
		return -1;
	}

	if (Izracunaj(&head, buffer) != 0) {
		FreeList(&head);
		return -1;
	}


	if (Pop(&head, &rezultat) != 0 || head.next != NULL) {
		printf("Greska: izraz nije ispravan (krivi broj operanada).\n");
		FreeList(&head);
		return -1;
	}

	printf("Rezultat: %d\n", rezultat);

	FreeList(&head);
	return 0;
}