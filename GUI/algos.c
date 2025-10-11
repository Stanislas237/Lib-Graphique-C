#include <stdio.h>
#include <stdlib.h>

static int find_min_index(int *arr, int start, int n) {
    int min = start;
    for (int i = start + 1; i < n; i++)
    if (arr[i] < arr[min]) min = i;
    return min;
}

/* fusionne arr[l..m] et arr[m+1..r] avec tmp comme tampon */
static void merge_with_tmp(int *arr, int l, int m, int r, int *tmp) {
    int i = l;      // index courant dans la moitié gauche
    int j = m + 1;  // index courant dans la moitié droite
    int k = l;      // index courant dans tmp (écriture)

    // 1) Parcourir les deux sous-tableaux et copier l'élément le plus petit
    while (i <= m && j <= r) {
        // Choisir arr[i] quand égalité => stable
        if (arr[i] <= arr[j]) {
            tmp[k++] = arr[i++];   // prendre élément gauche
        } else {
            tmp[k++] = arr[j++];   // prendre élément droit
        }
    }

    // 2) Copier les restes (s'il reste des éléments à gauche)
    while (i <= m) tmp[k++] = arr[i++];

    // 3) Copier les restes (s'il reste des éléments à droite)
    while (j <= r) tmp[k++] = arr[j++];

    // 4) Recopier tmp[l..r] dans arr[l..r]
    for (k = l; k <= r; ++k) arr[k] = tmp[k];
}

/* === Tri par sélection (récursif) === */
void selection_sort_rec(int *arr, int n) {
    if (n <= 1) return;
    int min = find_min_index(arr, 0, n);
    if (min != 0) {
        int tmp = arr[0];
        arr[0] = arr[min];
        arr[min] = tmp;
    }
    selection_sort_rec(arr + 1, n - 1);
}

/* === Tri par sélection (itératif) === */
void selection_sort_iter(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = find_min_index(arr, i, n);
        if (min != i) {
            int tmp = arr[i];
            arr[i] = arr[min];
            arr[min] = tmp;
        }
    }
}


/* === Tri par fusion (itératif) === */
void merge_sort_iter(int *arr, int n) {
    if (n <= 1) return;
    int *tmp = malloc(n * sizeof(int));
    if (!tmp) return;

    // width = taille des sous-tableaux déjà triés (1, 2, 4, ...)
    for (int width = 1; width < n; width *= 2) {
        // fusionner paires de segments [left .. left+width-1] et [left+width .. left+2*width-1]
        for (int left = 0; left < n; left += 2 * width) {
            int mid = left + width - 1;
            if (mid >= n - 1) { // pas de moitié droite => rien à fusionner
                continue;
            }
            int right = left + 2 * width - 1;
            if (right >= n) right = n - 1;
            merge_with_tmp(arr, left, mid, right, tmp);
        }
    }

    free(tmp);
}

/* === Tri par fusion (récursif) === */
void merge_sort_rec(int *arr, int left, int right, int *tmp) {
    if (left >= right) return; // Cas de base : sous-tableau de 1 élément ou vide

    int mid = left + (right - left) / 2; // Calculer le milieu (évite overflow)

    // Trier récursivement la première moitié
    merge_sort_rec(arr, left, mid, tmp);

    // Trier récursivement la seconde moitié
    merge_sort_rec(arr, mid + 1, right, tmp);

    // Fusionner les deux moitiés triées
    merge_with_tmp(arr, left, mid, right, tmp);
}
