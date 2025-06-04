/* File        : stack.cpp */
/* Deskripsi   : Body ADT stack yang diimplementasikan dgn linked list */
/* Dibuat oleh : Ade Chandra Nugraha*/

#include <limits.h>
#include <malloc.h>
#include "STACK.h"
#include "linked.h"
#include "queue.h"

/**** Perubahan nilai komponen struktur ****/
void SetFirst (Stack *S, Stack NewFirst )
/* Memberi Nilai First yang baru dengan NewFirst */
{
	*S = NewFirst;
}

/*    PROTO TYPE    */
/**** Konstruktor/Kreator ****/
void CreateEmpty (Stack *S)
/* IS : S sembarang */
/* FS : Membuat sebuah stack S yang kosong */
/* Ciri stack kosong : First bernilai NULL */
{
	
	(*S).First = Nil;
}

/**** Predikat untuk test keadaan KOLEKSI ****/
boolean IsEmpty (Stack S)
/* Mengirim true jika Stack Kosong */
{
	if (S.First == NULL)
		return true;
	else
		return false;
}

/**** Menambahkan sebuah elemen ke Stack ****/
void Push (Stack *S, infotype X)
/* Menambahkan X sebagai elemen stack S */
/* IS : S mungkin kosong */
/* FS : X menjadi First yang baru */
{
	address P;
	P = Alokasi(X);
	if (!IsEmpty(*S))
	{
		InsertFirst(S, P);
	} else {
		Next(P) = Nil;
		(*S).First = P;
	}
}

/**** Menghapus sebuah elemen Stack ****/
void Pop (Stack *S, infotype *X)
/* Menghapus X dari Stack S */
/* IS : S tidak mungkin kosong */
/* FS : X adalah nilai elemen First yang lama */
{
	address P;
	P = (*S).First;
	if (!IsEmpty(*S))
	{
		DelFirst(S, &P);
		DeAlokasi(P);
	}
	else
	{
		*X = 0;
	}
}

