#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXN 302
#define NTARGET 2252

int R, C, A;
int L, V, B, T;
int rs, cs;

vector<int> coverage[MAXN][MAXN];

int targetx[NTARGET], targety[NTARGET];

char map

vector<int> size;
vector<vector<int> > G;
vector<int> r1;
vector<int> c1;
vector<int> r2;
vector<int> c2;
vector<int> ham;

int covered(int t, int r, int c) {
  // does a loon at r c cover t?


void coverage() {
  // compute for each square which targets are covered
  for (int r = 0; r < R; r++)
    for (int c = 0; c < C; c++)
      for (int t = 0; t < T; t++)
        if (covered(t, r, c))
          coverage[r][c].push_back(t);
}

int main(int argc, char **argv) {
  scanf("%d%d%d", &R, &C, &A);
  scanf("%d%d%d", &L, &V, &B, &T);
  scanf("%d%d", &rs, &cs);
  for (int i = 0; i < L; i++) {
    scanf("%d%d", &(targetx[i]), &(targety[i]));
  }
  coverage();


  
  return 0;
}


