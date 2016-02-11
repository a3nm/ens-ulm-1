#include<cstdio>
#include<cmath>

#define MAXP 2002
#define MAXW 20
#define MAXD 32
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000

int R, C, D, T, L;
int P;
int Ps[MAXP];
int W;
int Wx[MAXW], Wy[MAXW];
int Store[MAXW][MAXP];
int O;
int Ox[MAXO], Oy[MAXO], On[MAXO];
int Order[MAXO][MAXON];
int Ocompl[MAXO];

int busy_until[MAXD];
int dx[MAXD], dy[MAXD];

int time_to_complete(int o) {
  return 0;
}

int dist(int xa, int ya, int xb, int yb) {
  int dx = (xa - xb);
  int dy = (ya - yb);
  return ceil(sqrt(dx*dx + dy*dy));
}

//int sacADos(int rest);

int wload(int o, int w, vector<int>& res) {
	
	/*int cpstore[MAXP];
	
	for(int i = 0; i < MAXP; i++)
		cpstore[i]=min(Store[w][i], Order[o][i]);
	
	int res = sacADos(L, 0, cpstore[0]);*/
	
	return 0;
  // return useful load than can be taken in warehouse w for order o
}

void execute(int d, int o) {
  double best_wquality = -1;
  int best_w = -1;
  // tell d to do something to help towards o
  for (int w = 0; w < W; w++) {
    int time_to_warehouse = 0; // load-unload times neglected
    time_to_warehouse += dist(dx[d], dy[d], Wx[w], Wy[w]);
    time_to_warehouse += dist(Wx[d], Wy[d], Ox[o], Oy[o]);
    int warehouse_load = wload(o, w);


  }

  // i know the warehouse where to go
}

int main() {
  scanf("%d%d%d%d%d", &R, &C, &D, &T, &L);
  scanf("%d", &P);
  for (int i = 0; i < P; i++)
    scanf("%d", &(Ps[i]));
  scanf("%d", &W);
  for (int i = 0; i < W; i++) {
    scanf("%d%d", &(Wx[i]), &(Wy[i]));
    for (int j = 0; j < P; j++) {
      scanf("%d", &(Store[i][j]));
    }
  }
  scanf("%d", &O);
  for (int i = 0; i < O; i++) {
    scanf("%d%d%d", &(Ox[i]), &(Oy[i]), &(On[i]));
    for (int j = 0; j < On[i]; j++) {
      scanf("%d", &(Order[i][j]));
    }
  }

  for (int i = 0; i < D; i++)
    busy_until[i] = 0;
  for (int o = 0; o < O; o++)
    Ocompl[o] = -1;
  for (int d = 0; d < D; d++) {
    dx[d] = Ox[0];
    dy[d] = Oy[0];
  }

  int t = 0;

  while (t <= T) {
    int first_avail = -1;
    for (int d = 0; d < D; d++)
      if (busy_until[d] <= t) {
        first_avail = d;
        break;
      }
    if (first_avail < 0)
      continue;
    int besttime = T, bestorder = -1;
    for (int o = 0; o < O; o++)
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
