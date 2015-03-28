#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXN 302
#define MAXL 2252
#define NALT 10
#define MAXT 402

int R, C, A;
int L, V, B, T;
int rs, cs;

struct Pt{
  int r, c;
  Pt(int r=0, int c=0) : r(r), c(c) {}
};

vector<int> coverage[MAXN][MAXN];

Pt dest[NALT][MAXN][MAXN]; // where does the wind take us: -1 -1 = out

Pt target[MAXL];

bool covered[MAXL][MAXT]; // does someone cover target l at time t

int columndist(int c1, int c2) {
  return min(abs(c1 - c2), C - abs(c1 - c2));
}

int iscovered(int l, int r, int c) {
  // does a loon at r c cover l?
  int u = target[l].r, v = target[l].c;
  return ((r - u) * (r - u) + columndist(c, v) * columndist(c, v) <= V*V);
}


void calccoverage() {
  // compute for each square which targets are covered
  for (int r = 0; r < R; r++)
    for (int c = 0; c < C; c++)
      for (int l = 0; l < L; l++)
        if (iscovered(l, r, c))
          coverage[r][c].push_back(l);
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

  for (int b = 0; b < B; b++) {
    // planify loon b
    for (int n = 0; 
  }
  
  return 0;
}


