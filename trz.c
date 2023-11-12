#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

typedef enum{
    get_min,
    get_max
} min_or_max_t;

const int Inf = INT_MAX;

int min(int a, int b) {
    return (a < b ? a : b);
}

int max(int a, int b) {
    return (a < b ? b : a);
}

int get_min_or_max(int a, int b, min_or_max_t type) {
    if(!type) {
        return min(a,b);
    } else {
        return max(a,b);
    }
}

bool is_invalid(int x) {
    return x == -1 || x == Inf;
}

// zwraca element neutralny odpowiednio dla min/max
int invalid_type(int i) {
    return i % 2 ? -1 : Inf;
}

// zwraca input+expr chyba ze is_invalid(input) wtedy zwraca input
int sum_or_invalid(int input, int expr) {
    return is_invalid(input) ? input : input + expr;
}

// sprawdza czy dany indeks ma inny typ niz typy elementow w danej tablicy
bool is_different(int index, int* tab, int tab_sz, int* types) {
    for(int j = 0; j < tab_sz; ++j) {
        if(types[tab[j]] == types[index]) {
            return false;
        }
    }

    return true;
}

bool pairwise_different(int a, int b, int c) {
    return a != b && b != c && c != a;
}

int main() {
    int n;
    assert(scanf("%d", &n) == 1);
    int* types = (int*) malloc(((size_t) n) * sizeof(int));
    int* distances = (int*) malloc(((size_t) n) * sizeof(int));

    // tablice trzymajace indeksy najblizszych hoteli innego typu na lewo/prawo
    int* l = (int*) malloc(((size_t) n) * sizeof(int));
    int* r = (int*) malloc(((size_t) n) * sizeof(int));
    l[0] = r[n - 1] = Inf;

    // bedziemy tez trzymac indeksy pierwszych trzech roznych typow hoteli od l/r, to umozliwia dla danego indeksu sprawdzenie w czasie stalym jaka maksymalna trojke moze tworzyc
    int three_diff_l[3];
    int three_diff_r[3];
    int three_diff_l_size = 1, three_diff_r_size = 1;

    for(int i = 0; i < n; ++i) {
        assert(scanf("%d %d", &types[i], &distances[i]) == 2);
        types[i]--;
    }

    assert(n > 0);

    three_diff_l[0] = 0;
    three_diff_r[0] = n - 1;
    // wyznaczamy nasze three diff l i r
    for(int i = 1; i < n; ++i) {
        int ir = n - 1 - i;
        // max trzy porownania
        if(three_diff_l_size < 3 && is_different(i, three_diff_l, three_diff_l_size, types)) {
            three_diff_l[three_diff_l_size++] = i;
        }

        if(three_diff_r_size < 3 && is_different(ir, three_diff_r, three_diff_r_size, types)) {
            three_diff_r[three_diff_r_size++] = ir;
        }
    }

    // jesli tablica nie ma trzech roznych hoteli to juz znamy odpowiedz
    if(three_diff_l_size < 3) {
        printf("0 0\n");
        return 0;
    }

    for(int k = 1; k < n; ++k) {
        int kr = n - 1 - k;

        l[k] = (types[k] == types[k - 1] ? l[k - 1] : k - 1);
        r[kr] = (types[kr] == types[kr + 1] ? r[kr + 1] : kr + 1);
    }

    int ans_min = Inf, ans_max = -1;
    for(int k = 1; k < n - 1; ++k) {
        int type = types[k];

        for(int i = 0; i < three_diff_l_size; ++i) {
            for(int j = 0; j < three_diff_r_size; ++j) {
                // bierzemy sobie po jednym z trzech roznych hoteli z koncow i sprawdzamy max tylko dla nich, bo dla innych nie ma sensu (tryw)
                int a = three_diff_l[i], b = three_diff_r[j];
                if(!(a < k && k < b)) {
                    continue;
                }

                int type_a = types[a], type_b = types[b];

                if(!pairwise_different(type, type_a, type_b)) {
                    continue;
                }

                int dist_a = distances[k] - distances[a], dist_b = distances[b] - distances[k];
                ans_max = max(ans_max, min(dist_a, dist_b));
            }
        }
        
        int left_min = l[k], right_min = r[k];

        if(is_invalid(left_min) || is_invalid(right_min)) {
            continue;
        }

        int dist_l = distances[k] - distances[left_min], dist_r = distances[right_min] - distances[k];

        // z zachlannego argumentu wiemy ze jesli najblizszy hotel innego typu na lewo i na prawo jest tego samego typu to dany hotel srodkowy nie jest czescia optymalnej trojki min
        if(types[left_min] == types[right_min]) {
            continue;
        }

        ans_min = min(ans_min, max(dist_l, dist_r));
    }

    if(ans_max == -1) {
        ans_min = ans_max = 0;
    }

    printf("%d %d\n", ans_min, ans_max);

    free(types);
    free(distances);
    free(l);
    free(r);
}
