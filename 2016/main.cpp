#include<cstdio>
#include<cmath>
#include<vector>
#include<algorithm>

#define MAXP 2002
#define MAXW 20
#define MAXD 32
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000

using namespace std;

int R, C, D, T, L;
int P;
int Ps[MAXP];
int W;
int Wx[MAXW], Wy[MAXW];
int Store[MAXW][MAXP];
int O;
int Ox[MAXO], Oy[MAXO], On[MAXO];
int Order[MAXO][MAXP];
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

int wload(int o, int w) {
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
    vector<int> v; // décoratif
    int warehouse_load = wload(o, w, v);
    double w_quality = ((double) warehouse_load) / time_to_warehouse;
    if (w_quality > best_wquality) {
      best_wquality = w_quality;
      best_w = w;
    }
  }

  // i know the warehouse where to go
  vector<int> objects;
  wload(o, best_w, objects);
  sort(objects.begin(), objects.end());

  // objects sorted, RLE

  int last_type = -1, n_last_type = 0;

  for (unsigned int i = 0; i < objects.size(); i++) {
    // go there and load
    if (objects[i] != last_type) {
      // we changed type
      printf("%d L %d %d %d\n", d, best_w, last_type, n_last_type);
    }
  }

  // write that order is fulfilled
  // write that order components are fulfilled
  // write that capas are used
  // write that the drone is busy
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
  for (int i = 0; i < O; i++)
    for (int p = 0; p < P; p++)
      Order[i][p] = 0;
  for (int o = 0; o < O; o++) {
    scanf("%d%d%d", &(Ox[i]), &(Oy[i]), &(On[i]));
    for (int j = 0; j < On[i]; j++) {
      int p;
      scanf("%d", p);
      Order[o][p]++;
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
