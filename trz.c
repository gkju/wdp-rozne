#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>

typedef enum{
    get_min,
    get_max
} min_or_max;

const int Inf = INT_MAX;

int min(int a, int b) {
    return (a < b ? a : b);
}

int max(int a, int b) {
    return (a < b ? b : a);
}

int get_min_or_max(int a, int b, int type) {
    if(!type) {
        return min(a,b);
    } else {
        return max(a,b);
    }
}

bool is_invalid(int x) {
    return x == -1 || x == Inf;
}

int invalid_type(int i) {
    return i % 2 ? -1 : Inf;
}

int main() {
    int n;
    assert(scanf("%d", &n) == 1);
    int* types = (int*) malloc(((size_t) n) * sizeof(int));
    int* distances = (int*) malloc(((size_t) n) * sizeof(int));

    // pierwsza wpolrzedna definiuje typ z dp, czy chcemy min czy max, druga typ hotelu, indeks oznacza odpowiednio min/max odl do hotelu zadanego typu na lewo/prawo
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

    for(int k = 1; k < n; ++k) {
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 3; ++j) {
                int l_prev = l[i][j][k-1];
                int dist_one = is_invalid(l_prev) ? l_prev : l_prev + distances[k] - distances[k - 1];
                int dist_two = (types[k-1] == j ? distances[k] - distances[k-1] : invalid_type(i));
                l[i][j][k] = get_min_or_max(dist_one, dist_two, i);
            }
        }
    }

    for(int k = n - 2; k >= 0; --k) {
        // i oznacza czy min czy max
        for(int i = 0; i < 2; ++i) {
            // j oznacza typ
            for(int j = 0; j < 3; ++j) {
                int r_next = r[i][j][k+1];
                int dist_one = is_invalid(r_next) ? r_next : r_next + distances[k + 1] - distances[k];
                int dist_two = (types[k+1] == j ? distances[k + 1] - distances[k] : invalid_type(i));
                r[i][j][k] = get_min_or_max(dist_one, dist_two, i);
            }
        }
    }

    int ans_min = Inf, ans_max = -1;
    for(int k = 1; k < n - 1; ++k) {
        int type = types[k];
        for(int i = 0; i < 2; ++i) {
            for(int j = 0; j < 2; ++j) {
                
                int dist_one = l[j][(type + 1 + i) % 3][k];
                int dist_two = r[j][(type + 2 - i) % 3][k];

                if(is_invalid(dist_one) || is_invalid(dist_two)) {
                    continue;
                }

                // printf("k = %d i = %d j = %d distone = %d disttwo = %d \n", k, i, j, dist_one, dist_two);
                
                if(!j) {
                    // if(max(dist_one, dist_two) < ans_min) {
                    //     printf("FOUND JD FOR k = %d\n", k);
                    // }
                    ans_min = min(ans_min, max(dist_one, dist_two));
                } else {
                    // if(min(dist_one, dist_two) > ans_max) {
                    //    printf("FOUND JD FOR k = %d\n", k);
                    // }
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