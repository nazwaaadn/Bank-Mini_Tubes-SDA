/* File        : stack.cpp */
/* Deskripsi   : Body ADT stack yang diimplementasikan dgn linked list */
/* Dibuat oleh : Ade Chandra Nugraha*/

#include <limits.h>
#include <malloc.h>
#include "stack.h"
#include "terminal.h"

addressStack AlokasiStack(nbtree X) {
    addressStack S = (addressStack)malloc(sizeof(ElmtStack));
    if (S != NULL) {
        Info(S) = X;
        Next(S) = Nil;
    } else {
        printf("Alokasi Stack gagal\n");
    }
    return S;
}

void DealokasiStack(addressStack P)
{
	free(P);
}

/*    PROTO TYPE    */
/**** Konstruktor/Kreator ****/
void CreateEmptyStack (Stack *S)
/* IS : S sembarang */
/* FS : Membuat sebuah stack S yang kosong */
/* Ciri stack kosong : Top bernilai NULL */
{
	
	Top(S) = Nil;
}

/**** Predikat untuk test keadaan KOLEKSI ****/
boolean IsEmptyStack (Stack S)
/* Mengirim true jika Stack Kosong */
{
	if (S.Top == NULL)
		return true;
	else
		return false;
}

/**** Menambahkan sebuah elemen ke Stack ****/
void Push (Stack *S, nbtree X)
/* Menambahkan X sebagai elemen stack S */
/* IS : S mungkin kosong */
/* FS : X menjadi Top yang baru */
{
	addressStack P;
	P = Alokasi(X);
	if (!IsEmptyStack(*S))
	{
		InsertTop(S, P);
	} else {
		Next(P) = Nil;
		Top(S) = P;
	}
}

/**** Menghapus sebuah elemen Stack ****/
void Pop (Stack *S, nbtree *X)
/* Menghapus X dari Stack S */
/* IS : S tidak mungkin kosong */
/* FS : X adalah nilai elemen Top yang lama */
{
	addressStack P;
	P = Top(S);
	if (!IsEmptyStack(*S))
	{
		DelTop(S, &P);
		DeAlokasi(P);
	}
	else
	{
		X = Nil;
	}
}

void InsertTop(Stack *S, addressStack P)
{
	if (IsEmptyStack(*S)) {
		Top(S) = P;
	} else {
		Next(P) = Top(S);
		Top(S) = P;
	}
}

void DelTop(Stack *S, addressStack *P)
{
	if (!isEmptyStack(*S))
	{
		Top(S) = Next(Top(S));
		*P = Top(S);
	} else {
		*P = Nil;
	}
	
}

