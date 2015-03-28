#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXR 77
#define MAXC 302
#define MAXL 2252
#define MAXA 10
#define MAXT 402
#define MAXB 55

int R, C, A;
int L, V, B, T;
int rs, cs;

struct Pt{
  int r, c;
  Pt(int r=0, int c=0) : r(r), c(c) {}
};

vector<int> coverage[MAXR][MAXC]; // which targets are covered in r c

Pt dest[MAXA][MAXR][MAXC]; // where does the wind take us: -1 -1 = out

Pt target[MAXL];

bool covered[MAXT][MAXL]; // does someone cover target l at time t

int solution[MAXT][MAXB];

// for dynamic
int tab[MAXT][MAXA][MAXR][MAXC];
int dir[MAXT][MAXA][MAXR][MAXC];

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
  scanf("%d%d%d%d", &L, &V, &B, &T);
  scanf("%d%d", &rs, &cs);
  for (int l = 0; l < L; l++) {
    int r, c;
    scanf("%d%d", &r, &c);
    target[l] = Pt(r, c);
  }
  for (int r = 0; r < R; r++)
    for (int c = 0; c < C; c++) {
      dest[0][r][c] = Pt(r, c);
    }
  for (int a = 1; a <= A; a++)
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
  //for (int r = 0; r < R; r++) {
  //  for (int c = 0; c < C; c++)
  //    printf("%d ", (int) coverage[r][c].size());
  //  printf("\n");
  //}

  int totscore = 0;

  for (int b = 0; b < B; b++) {
    // planify loon b
    for (int t = 0; t < T; t++)
      for (int a = 0; a <= A; a++)
        for (int r = 0; r < R; r++)
          for (int c = 0; c < C; c++)
            tab[t][a][r][c] = dir[t][a][r][c] = 0;
    for (int t = T-1; t >= 0; t--) {
      if (!(t % 50))
        printf("loon %d time %d\n", b, t);
      for (int a = 0; a <= A; a++)
        for (int r = 0; r < R; r++)
          for (int c = 0; c < C; c++) {
            int bestda = 0, best = 0;
            for (int da = -1; da <= 1; da++) {
              if (a <= 1 && da < 0)
                continue;
              if (a + da > A)
                continue;
              Pt next = dest[a + da][r][c];
              if (next.r < 0)
                break;
              int cscore = 0;
              if (a + da > 0 && next.r >= 0) {
                for (unsigned int i = 0; i < coverage[next.r][next.c].size(); i++) {
                  int l = coverage[next.r][next.c][i];
                  cscore += covered[t+1][l] ? 0 : 1;
                }
              }
              int rscore = cscore + tab[t+1][a+da][next.r][next.c];
              if (rscore > best) {
                best = rscore;
                bestda = da;
              }
            }
            tab[t][a][r][c] = best;
            dir[t][a][r][c] = bestda;
          }
    }
    // ok now follow the best
    int r = rs, c = cs, a = 0;
    for (int t = 0; t < T; t++) {
      printf("loon %d at time %d is %d %d %d\n", b, t, a, r, c);
      int bestda = dir[t][a][r][c];
      printf("i choose %d\n", bestda);
      // ok, apply bestda
      a += bestda;
      solution[t][b] = bestda;
      Pt next = dest[a][r][c];
      r = next.r;
      c = next.c;
      if (r < 0) {
        printf("FAILLL\n");
        break; // loon is out
      }
      if (a > 0 && r >= 0) {
        // update covered targets
        printf("coverage %d %d\n", r, c);
        for (unsigned int i = 0; i < coverage[r][c].size(); i++) {
          int l = coverage[r][c][i];
          totscore += covered[t+1][l] ? 0 : 1;
          covered[t+1][l] = true;
        }
      }
    }
  }

  // print solution
  printf("score %d\n", totscore);

  for (int t = 0; t < T; t++) {
    for (int b = 0; b < B; b++)
      printf("%d ", solution[t][b]);
    printf("\n");
  }
    
  return 0;
}


