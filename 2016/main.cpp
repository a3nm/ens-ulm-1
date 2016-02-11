#include<cstdio>

#define MAXP 2002
#define MAXW 20
#define MAXD 32
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000

unsigned int R, C, D, T, L;
unsigned int P;
int Ps[MAXP];
unsigned int W;
int Wx[MAXW], Wy[MAXW];
int Store[MAXW][MAXP];
unsigned int O;
unsigned int Ox[MAXO], Oy[MAXO], On[MAXO];
int Order[MAXO][MAXON];
int Ocompl[MAXO];

unsigned int busy_until[MAXD];
int dx[MAXD], dy[MAXD];

int time_to_complete(int o) {
  return 0;
}

void execute(int d, int o) {
  // tell d to do something to help towards o
}

int main() {
  scanf("%d%d%d%d%d", &R, &C, &D, &T, &L);
  scanf("%d", &P);
  for (unsigned int i = 0; i < P; i++)
    scanf("%d", &(Ps[i]));
  scanf("%d", &W);
  for (unsigned int i = 0; i < W; i++) {
    scanf("%d%d", &(Wx[i]), &(Wy[i]));
    for (unsigned int j = 0; j < P; j++) {
      scanf("%d", &(Store[i][j]));
    }
  }
  scanf("%d", &O);
  for (unsigned int i = 0; i < O; i++) {
    scanf("%d%d%d", &(Ox[i]), &(Oy[i]), &(On[i]));
    for (unsigned int j = 0; j < On[i]; j++) {
      scanf("%d", &(Order[i][j]));
    }
  }

  for (unsigned int i = 0; i < D; i++)
    busy_until[i] = 0;
  for (unsigned int o = 0; o < O; o++)
    Ocompl[o] = -1;
  for (unsigned int d = 0; d < D; d++) {
    dx[d] = Ox[0];
    dy[d] = Oy[0];
  }

  unsigned int t = 0;

  while (t <= T) {
    int first_avail = -1;
    for (unsigned int d = 0; d < D; d++)
      if (busy_until[d] <= t) {
        first_avail = d;
        break;
      }
    if (first_avail < 0)
      continue;
    int besttime = T, bestorder = -1;
    for (unsigned int o = 0; o < O; o++)
      if (Ocompl[o] < 0) {
        int torder = time_to_complete(o);
        if (torder < besttime) {
          bestorder = o;
        }
      }
    // assign d to help towards o
    execute(first_avail, bestorder);
  }
  return 0;
}
