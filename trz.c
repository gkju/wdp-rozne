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

int main() {
    int n;
    assert(scanf("%d", &n) == 1);
    int* types = (int*) malloc(((size_t) n) * sizeof(int));
    int* distances = (int*) malloc(((size_t) n) * sizeof(int));

    // pierwsza wpolrzedna definiuje typ z dp, czy chcemy min czy max, druga typ hotelu, k-ty element to odpowiednio min/max odl k-tego hotelu do hotelu zadanego typu na lewo/prawo
    // pytamy sie o odleglosc do hotelu na lewo, typ x nie wplywa na l[i][j][k]
    int* l[2][3];
    int* r[2][3];

    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 3; ++j) {
            l[i][j] = (int*) malloc(((size_t) n) * sizeof(int));
            r[i][j] = (int*) malloc(((size_t) n) * sizeof(int));

            l[i][j][0] = invalid_type(i);
            r[i][j][n - 1] = invalid_type(i);
        }
    }

    for(int i = 0; i < n; ++i) {
        assert(scanf("%d %d", &types[i], &distances[i]) == 2);
        types[i]--;
    }

    for(min_or_max_t min_or_max = 0; min_or_max < 2; ++min_or_max) {
        for(int typ = 0; typ < 3; ++typ) {
            // dla czytelności osobno rozważamy przypadek lewy i prawy
            for(int k = 1; k < n; ++k) {
                int l_prev = l[min_or_max][typ][k - 1];
                int dist_one = sum_or_invalid(l_prev, distances[k] - distances[k - 1]);
                int dist_two = (types[k - 1] == typ ? distances[k] - distances[k - 1] : invalid_type(min_or_max));
                l[min_or_max][typ][k] = get_min_or_max(dist_one, dist_two, min_or_max);
            }

            for(int k = n - 2; k >= 0; --k) {
                int r_next = r[min_or_max][typ][k + 1];
                int dist_one = sum_or_invalid(r_next, distances[k + 1] - distances[k]);
                int dist_two = (types[k + 1] == typ ? distances[k + 1] - distances[k] : invalid_type(min_or_max));
                r[min_or_max][typ][k] = get_min_or_max(dist_one, dist_two, min_or_max);
            }
        }
    }

    int ans_min = Inf, ans_max = -1;
    for(int k = 1; k < n - 1; ++k) {
        int type = types[k];
        
        for(min_or_max_t  min_or_max = 0; min_or_max < 2; ++min_or_max) {
            // i wyznacza typ hotelu na lewo/prawo, który chcemy znaleźć
            for(int i = 0; i < 2; ++i) {
                int dist_one = l[min_or_max][(type + 1 + i) % 3][k];
                int dist_two = r[min_or_max][(type + 2 - i) % 3][k];

                if(is_invalid(dist_one) || is_invalid(dist_two)) {
                    continue;
                }

                if(!min_or_max) {
                    ans_min = min(ans_min, max(dist_one, dist_two));
                } else {
                    ans_max = max(ans_max, min(dist_one, dist_two));
                }
            }
        }
    }

    if(ans_max == -1) {
        ans_min = ans_max = 0;
    }

    printf("%d %d\n", ans_min, ans_max);

    free(types);
    free(distances);
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 3; ++j) {
            free(l[i][j]);
            free(r[i][j]);
        }
    }
}