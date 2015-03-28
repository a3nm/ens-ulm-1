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

char* output;

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
int left[MAXT][MAXR][MAXC]; // how many left to cover?

int solution[MAXT][MAXB];
int oldsol[MAXT][MAXB];

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

int decide_dyn(int b, int t, int a, int r, int c) {
  return dir[t][a][r][c];
}

int decide_sol(int b, int t, int a, int r, int c) {
  return solution[t][b];
}

int simulate(int b, int (*decide)(int, int, int, int, int)) {
  // return totscore
  //
  // ok now follow the best
  int totscore = 0;
  int r = rs, c = cs, a = 0;
  for (int t = 0; t < T; t++) {
    //printf("loon %d at time %d is %d %d %d\n", b, t, a, r, c);
    int bestda = (*decide)(b, t, a, r, c);
    //printf("i choose %d\n", bestda);
    // ok, apply bestda
    a += bestda;
    solution[t][b] = bestda;
    Pt next = dest[a][r][c];
    r = next.r;
    c = next.c;
    if (r < 0) {
      //printf("FAILLL\n");
      break; // loon is out
    }
    if (a > 0 && r >= 0) {
      // update covered targets
      //printf("coverage %d %d\n", r, c);
      for (unsigned int i = 0; i < coverage[r][c].size(); i++) {
        int l = coverage[r][c][i];
        totscore += covered[t+1][l] ? 0 : 1;
        covered[t+1][l] = true;
      }
    }
  }
  return totscore;
}


int planify(int b) {
  // planify one loon, b, based on the info of covered
  // return totscore
  int totscore = 0;
  // compute left
  for (int t = 0; t <= T; t++)
    for (int r = 0; r < R; r++)
      for (int c = 0; c < C; c++) {
        int cscore = 0;
        for (unsigned int i = 0; i < coverage[r][c].size(); i++) {
          int l = coverage[r][c][i];
          cscore += covered[t][l] ? 0 : 1;
        }
        left[t][r][c] = cscore;
      }
  // planify loon b, t == T is sentinel
  for (int t = 0; t <= T; t++)
    for (int a = 0; a <= A; a++)
      for (int r = 0; r < R; r++)
        for (int c = 0; c < C; c++)
          tab[t][a][r][c] = dir[t][a][r][c] = 0;
  for (int t = T-1; t >= 0; t--) {
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
            int rscore = ((a + da) > 0 ? left[t+1][next.r][next.c] : 0) +
              tab[t+1][a+da][next.r][next.c];
            if (rscore > best || (rscore == best && (rand() % 2))) {
              best = rscore;
              bestda = da;
            }
          }
          tab[t][a][r][c] = best;
          dir[t][a][r][c] = bestda;
        }
  }
  totscore += simulate(b, &decide_dyn);
  return totscore;
}

void print_sol(int totscore) {
  FILE *f = fopen(output, "w");
  
  // print solution
  fprintf(f, "score %d\n", totscore);

  for (int t = 0; t < T; t++) {
    for (int b = 0; b < B; b++)
      fprintf(f, "%d ", solution[t][b]);
    fprintf(f, "\n");
  }

  fclose(f);
}

int main(int argc, char **argv) {
  int TOREPLAN = atoi(argv[1]);
  srand(42);
  output = argv[3];

  if (argc == 1) {
    printf("syntax: ./a.out NREPLANIFY INPUT OUTPUT\n");
    return 1;
  }

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

  if (argc > 3) {
    // read existing file

    FILE *fsol = fopen(argv[2], "r");
    for (int t = 0; t < T; t++)
      for (int b = 0; b < B; b++)
        fscanf(fsol, "%d", &(solution[t][b]));
    fclose(fsol);
    for (int b = 0; b < B; b++)
      totscore += simulate(b, &decide_sol);
    printf("existing sol has score %d\n", totscore);
  } else {
    for (int b = 0; b < B; b++) {
      printf("%d\n", b);
      totscore += planify(b);
    }
    printf("score %d\n", totscore);
    print_sol(totscore);
  }

  int prevscore;
  int counter = 0;
  while(true) {
    counter++;
    counter = counter % B;
    // keep old solution
    for (int t = 0; t <= T; t++)
      for (int b = 0; b <= B; b++)
        oldsol[t][b] = solution[t][b];
    // choice of replanify
    bool toreplan[MAXB];
    for (int b = 0; b < B; b++)
      toreplan[b] = false;
    if (TOREPLAN == 1) {
      // if one to replan, do it sequentially
      toreplan[counter] = true;
    } else {
      for (int numb = 0; numb < TOREPLAN; numb++)
        toreplan[rand() % B] = true;
    }
      for (int t = 0; t <= T; t++)
        for (int l = 0; l < L; l++)
          covered[t][l] = false;
    // now simulate
    prevscore = totscore;
    totscore = 0;
    for (int b = 0; b < B; b++) {
      if (toreplan[b])
        continue;
      totscore += simulate(b, &decide_sol);
    }
    for (int b = 0; b < B; b++) {
      if (!toreplan[b])
        continue;
      printf("i replanify %d\n", b);
      totscore += planify(b);
      simulate(b, &decide_dyn);
    }

    if (totscore > prevscore) {
      printf("score was %d is %d\n", prevscore, totscore);
      print_sol(totscore);
    }
    if (totscore == prevscore) {
      printf("score was %d is still %d\n", prevscore, totscore);
    }

    if (totscore < prevscore) {
      printf("score was %d is now %d, rollback!!\n", prevscore, totscore);
      // rollback
      for (int t = 0; t <= T; t++)
        for (int b = 0; b <= B; b++)
          solution[t][b] = oldsol[t][b];
      totscore = prevscore;
    }
  }

  return 0;
}

