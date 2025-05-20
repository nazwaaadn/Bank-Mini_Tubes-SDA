/* File : list1.c */
/* Deskripsi : Body ADT Linked list yang hanya dikenali First(L) */
/* Dibuat oleh : Ade Chandra Nugraha */
/* Tanggal : 24-10-2001 */
#include <limits.h>
#include <malloc.h>
#include "list1.h"
/********** BODY SUB PROGRAM ***********/
/**** Predikat untuk test keadaan LIST ****/
boolean ListEmpty (List L)
/* Mengirim true jika List Kosong */
{
	return (First(L) == Nil);
}
/**** Konstruktor/Kreator List Kosong ****/
void CreateList (List * L)
/* IS : L sembarang */
/* FS : Terbentuk List Kosong */
{
	First(*L) = Nil;
}
/**** Manajemen Memory ****/
address Alokasi (char X)
{
	/* Kamus Lokal */
	address P;
	/* Algoritma */
	P = (address) malloc (sizeof (ElmtList));
	if (P != Nil) /* Alokasi berhasil */
	{
		Info(P) = X;
		Next(P) = Nil;
	}
	return (P);
}
void DeAlokasi (address P)
{
	if (P != Nil)
	{
		free (P);
	}
}

/**** Pencarian sebuah elemen List ****/
address Search (List L, char X)
/* Mencari apakah ada elemen list dengan Info(P) = X */
/* Jika ada, mengirimkan address elemen tsb. */
/* Jika tidak ada, mengirimkan Nil */
/* Menggunakan variabel bertype boolean */
{
	 /* Kamus Lokal */
	 address P;
	 boolean found =  false;
	 /* algoritma */
	 P = First(L);
	 while ((P != Nil) && (!found))
	 {
		if (Info(P) == X)
		{	found = true; 	}
		else
		{	P = Next(P);	}
	 }	/* P = Nil atau Ketemu */
	
	 return (P);
}

boolean FSearch (List L, address P)
/* Mencari apakah ada elemen list yang beralamat P */
/* Mengirimkan true jika ada, false jika tidak ada */
{
	 /* Kamus Lokal */
	 boolean found=false;
	 address PSearch;
	 /* Algortima */
	 PSearch = First(L);
	 while ((PSearch != Nil) && (!found))
	 {
		if (PSearch == P)
		{	found = true; 	}
		else
		{	PSearch = Next(PSearch);	}
	 }	/* PSearch = Nil atau Ketemu */
	 
	 return (found);
}

address SearchPrec (List L, char X)
/* Mengirimkan address elemen sebelum elemen yang nilainya = X */
/* Mencari apakah ada elemen list dengan Info(P) = X */
/* Jika ada, mengirimkan address Prec, dengan Next(Prec) = P dan Info(P) = X */
/* Jika tidak ada, mengirimkan Nil */
/* Jika P adalah elemen pertama, maka Prec = Nil */
/* Search dengan spesifikasi seperti ini menghindari */
/* traversal ulang jika setelah Search akan dilakukan operasi lain */
{
	 /* Kamus Lokal */
	address Prec, P;
	boolean found=false;
	 /* Algoritma */
	Prec = Nil;
	P = First(L);
	while ((P != Nil) && (!found))
	{
		 if (Info(P) == X)
		 {	found = true;	}
		 else
		 {
			Prec = P;
			P = Next(P);
		 }
	}    /* P = Nil atau Ketemu */
	if (found)
	{	return (Prec);		}
	else
	{	return (Nil);		}
}


/**** PRIMITIF BERDASARKAN NILAI ****/
/**** Penambahan Elemen ****/
void InsVFirst (List * L, char X)
{
	/* Kamus Lokal */
	address P;
	/* Aloritma */
	P = Alokasi (X);
	if (P != Nil)
	{ 
		InsertFirst (&(*L), P); 
	}
}

void InsVLast (List * L, char X){
    address P;
    /* Aloritma */
	P = Alokasi (X);
	if (P != Nil)
	{ 
		InsertLast (&(*L), P); 
	}
}

void DelVFirst (List * L, char * X){
    address P;
    /* Algoritma */
    P = First(*L);
    if (P != Nil)
    { 
        *X = Info(P);
        DelFirst (&(*L), &P);
        DeAlokasi (P);
    }
}

void DelVLast(List *L, char *X) {
    /* Kamus Lokal */
    address P, Q;
    
    /* Algoritma */\
        P = First(*L);
        
        if (Next(P) == Nil) {  // Jika hanya ada satu elemen
            *X = Info(P);
            First(*L) = Nil;   // List menjadi kosong
        } else {
            while (Next(Next(P)) != Nil) {  // Cari elemen sebelum terakhir
                P = Next(P);
            }
            Q = Next(P);  // Elemen terakhir
            *X = Info(Q);
            Next(P) = Nil;  // Putuskan hubungan ke elemen terakhir
        }

        DeAlokasi(Q);  // Dealokasi elemen terakhir
    
}


void InsertFirst (List * L, address P){
    Next(P) = First(*L);
    First(*L) = P;
}

void InsertAfter (List * L, address P, address Prec){
    Next(P) = Next(Prec);
    Next(Prec) = P;
}

void InsertLast (List * L, address P){
    address Last = First(*L);
    if (Last == Nil) {
        First(*L) = P;
    } else {
        while (Next(Last) != Nil) {
            Last = Next(Last);
        }
        Next(Last) = P;
    }
}

void DelFirst (List * L, address * P){
    
    /* Algoritma */
    *P = First(*L);
    if (*P != Nil)
    {
        First(*L) = Next(*P);
        Next(*P) = Nil;
    }
}

void DelP (List * L, char X){
    address P, Prec;
    /* Algoritma */
    P = First(*L);
    Prec = Nil;
    while (P != Nil)
    {
        if (Info(P) == X)
        {
            if (Prec == Nil)
            {
                DelFirst(&(*L), &P);
            }else{
                DelAfter(&(*L), &P, Prec);
            }
            DeAlokasi(P);
            break;
        }else{
            Prec = P;
            P = Next(P);
        }
    }
}

void DelLast (List * L, address * P){
    address Prec;
    /* Algoritma */
    *P = First(*L);
    if (Next(*P) == Nil) { // Hanya satu elemen dalam list
        First(*L) = Nil;
    } else {
        Prec = Nil;
        while (Next(*P) != Nil) { 
            Prec = *P;
            *P = Next(*P);
        }
        Next(Prec) = Nil;
    }
}

void DelAfter (List * L, address * Pdel, address Prec){
    address P;
    /* Algoritma */
    P = Next(Prec);
    if (P != Nil)
    {
        Next(Prec) = Next(P);
        Next(P) = Nil;
    }
}

void PrintInfo (List L)

{
	/* Kamus Lokal */
	address P;
	/* Algoritma */
	if (First(L) == Nil)
	{
		printf ("List Kosong ..... \a\n");
	}else /* List memiliki elemen */
	{
		P = First(L);
		for (;;)
        {
            
            if (P == Nil)
            {
                printf("\n");break;
            }
            else /* Belum berada di akhir List */
            {
                printf ("%d ", Info(P));
                P = Next(P);
            }
        }
	}
}


void DelAll(List *L) {
    address PDel = First(*L);
    
    while (PDel != Nil) {
        DelFirst(L, &PDel);
        DeAlokasi(PDel);
        PDel = First(*L);
    }
}





