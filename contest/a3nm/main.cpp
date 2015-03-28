#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXN 302
#define NTARGET 2252
#define NALT 10

int R, C, A;
int L, V, B, T;
int rs, cs;

struct Pt{
  int r, c;
  Pt(int r=0, int c=0) : r(r), c(c) {}
};

vector<int> coverage[MAXN][MAXN];

Pt dest[NALT][MAXN][MAXN]; // where does the wind take us: -1 -1 = out

Pt target[NTARGET];

char map

vector<int> size;
vector<vector<int> > G;
vector<int> r1;
vector<int> c1;
vector<int> r2;
vector<int> c2;
vector<int> ham;

int columndist(int c1, int c2) {
  return min(abs(c1 - c2), C - abs(c1 - c2));
}

int covered(int t, int r, int c) {
  // does a loon at r c cover t?
  int u = target[t].r, v = target[t].c;
  return ((r - u) * (r - u) + columndist(c, v) * columndist(c, v) <= V*V);
}


void calccoverage() {
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
    int r, c;
    scanf("%d%d", &r, &c);
    target[i] = Pt(r, c);
  }
  for (int a = 0; a < A; a++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        int dr, dc;
        scanf("%d%d", &dr, &dc);
        int destr, destc;
        destr = r + dr;
        destc = (c + dc + C) % C;
        if (destr < 0 || destr >= R)
          destr = destc = -1; // out
        dest[a][r][c] = Pt(destr, destc);
      }
  calccoverage();
  
  return 0;
}


