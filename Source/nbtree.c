#include <stdio.h>
#include <stdlib.h>
#include "nbtree.h"
#include "boolean.h"

void Create_tree(Isi_Tree X, int Jml_Node)
{
    int i;

    // Inisialisasi semua node
    for (i = 0; i < jml_maks; i++) {
        X[i].info = ' ';
        X[i].ps_fs = nil;
        X[i].ps_nb = nil;
        X[i].ps_pr = nil;
    }

    // Isi info A - J
    for (i = 0; i < Jml_Node; i++) {
        X[i].info = 'A' + i;
    }

    // Struktur pohon sesuai dengan target preorder: A B D E I J C F G H
    X[0].ps_fs = 1;               // A -> B
    X[1].ps_fs = 3; X[1].ps_nb = 2; X[1].ps_pr = 0;   // B -> D, sibling = C
    X[2].ps_fs = 5; X[2].ps_nb = nil; X[2].ps_pr = 0; // C -> F
    X[3].ps_fs = nil; X[3].ps_nb = 4; X[3].ps_pr = 1; // D, sibling = E
    X[4].ps_fs = 8; X[4].ps_nb = nil; X[4].ps_pr = 1; // E -> I
    X[5].ps_fs = nil; X[5].ps_nb = 6; X[5].ps_pr = 2; // F -> G
    X[6].ps_fs = nil; X[6].ps_nb = 7; X[6].ps_pr = 2; // G -> H
    X[7].ps_fs = nil; X[7].ps_nb = nil; X[7].ps_pr = 2; // H
    X[8].ps_fs = nil; X[8].ps_nb = 9; X[8].ps_pr = 4; // I -> J
    X[9].ps_fs = nil; X[9].ps_nb = nil; X[9].ps_pr = 4; // J
}

boolean IsEmpty (Isi_Tree P)
{
    if(P[0].ps_fs == nil)
    {
    	return true;
	}
}

void PreOrderNode(Isi_Tree P, address root) {
    if (root == nil) return;
    
    // Cetak karakter info sesuai node yang sedang diproses
    printf("%c ", P[root].info);  // P[root].info, bukan P[0].info
    
    // Traversal ke anak pertama
    PreOrderNode(P, P[root].ps_fs);
    
    // Traversal ke saudara berikutnya
    PreOrderNode(P, P[root].ps_nb);
}

void PreOrder(Isi_Tree P) {
    PreOrderNode(P, P[0].ps_fs);  // Tidak perlu printf di sini
}

void PostOrderNode(Isi_Tree P, address root) {
    if (root == nil) return;  // Jika node kosong (nil atau -1)

    // Traversal ke anak pertama (subtree kiri)
    PostOrderNode(P, P[root].ps_fs);

    // Proses node (print info node)
    printf("%c ", P[root].info);

    // Traversal ke saudara berikutnya (subtree kanan)
    PostOrderNode(P, P[root].ps_nb);
}

void PostOrder(Isi_Tree P) {
    // Mulai traversal dari root (P[0])
    PostOrderNode(P, 0);  // Asumsikan P[0] adalah root
}

void InOrder(Isi_Tree P)
{
    int i = 0;               // Mulai dari root (misal index 0)
    boolean resmi = true;

    while (i != nil) {
        if (P[i].ps_fs != nil && resmi) {
            i = P[i].ps_fs;
        } else {
            if (resmi) {
                printf(" %c", P[i].info);
            }
            if (i == P[P[i].ps_pr].ps_fs) {
                printf(" %c", P[P[i].ps_pr].info);
            }
            if (P[i].ps_nb != nil) {
                i = P[i].ps_nb;
                resmi = true;
            } else {
                i = P[i].ps_pr;
                resmi = false; // numpang lewat
            }
        }
    }
}


void LevelOrder(Isi_Tree X) {
    int i;
    for (i = 1; i <= 10; i++) {
        if (X[i].info != ' ')
            printf("%c ", X[i].info);
    }
    printf("\n");
}

boolean SearchNode(Isi_Tree P, address root, infotype X) {
    if (root == nil) return false;

    if (P[root].info == X) return true;

    if (SearchNode(P, P[root].ps_fs, X)) return true;
    return SearchNode(P, P[root].ps_nb, X);
}

boolean Search(Isi_Tree P, infotype X) {
    return SearchNode(P, 1, X); 
}

int nbElmtRekursif(Isi_Tree P, int root) {
    if (root == nil) return 0;

    int count = 1; 
    count += nbElmtRekursif(P, P[root].ps_fs); // anak
    count += nbElmtRekursif(P, P[root].ps_nb); // saudara
    return count;
}

int nbElmt(Isi_Tree P)
{
	return nbElmtRekursif(P, 0);
}

int nbDaunRekursif(Isi_Tree P, int root) {
    if (root == nil) return 0;

    if (P[root].ps_fs == nil)
        return 1 + nbDaunRekursif(P, P[root].ps_nb); 
    else
        return nbDaunRekursif(P, P[root].ps_fs) + nbDaunRekursif(P, P[root].ps_nb); 
}

int nbDaun(Isi_Tree P) {
    return nbDaunRekursif(P, 0); 
}

int Level(Isi_Tree P, infotype X) {
    int i, level = 0;

    // Cari index node X
    for (i = 0; i < jml_maks; i++) {
        if (P[i].info == X) {
            break;
        }
    }

    if (i == jml_maks) {
        return -1; 
    }

    while (P[i].ps_pr != nil) {
        level++;
        i = P[i].ps_pr;
    }

    return level;
}


int Depth(Isi_Tree P) {
    return DepthRekursif(P, 0); 
}

int DepthRekursif(Isi_Tree P, int node) {
    if (node == nil) return 0;

    int maxChildDepth = 0;
    int child = P[node].ps_fs;

    while (child != nil) {
        int childDepth = DepthRekursif(P, child);
        if (childDepth > maxChildDepth) {
            maxChildDepth = childDepth;
        }
        child = P[child].ps_nb;
    }

    return 1 + maxChildDepth;
}







