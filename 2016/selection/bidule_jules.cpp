#include<cstdio>

#define MAXP 2002
#define MAXW 20
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000

int nbLig, nbCol, nbDrones, T, maxLoad;
int P;
int Ps[MAXP];
int W;
int Wx[MAXW], Wy[MAXW];
int Store[MAXW][MAXP];
int O;
int Ox[MAXO], Oy[MAXO], On[MAXO];
int Order[MAXO][MAXON];


int main() {
  scanf("%d%d%d%d%d", &nbLig, &nbCol, &nbDrones, &T, &maxLoad);
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



  return 0;
}
