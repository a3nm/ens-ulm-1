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

int t;

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

bool order_is_complete(int o) {
  for (int p = 0; p < P; p++)
    if (Order[o][p] > 0)
      return false;
  return true;
}

void execute(int d, int o) {
  double best_wquality = -1;
  int best_w = -1;
  // tell d to do something to help towards o
  for (int w = 0; w < W; w++) {
    int time_to_warehouse = 0; // load-unload times neglected
    time_to_warehouse += dist(dx[d], dy[d], Wx[w], Wy[w]);
    time_to_warehouse += dist(Wx[w], Wy[w], Ox[o], Oy[o]);
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
  objects.push_back(MAXP); // sentinel

  // objects sorted, RLE

  int last_type = -1, n_last_type = 0;
  int drone_time = 0;
  drone_time += dist(dx[d], dy[d], Wx[best_w], Wy[best_w]);

  vector<pair<int, int> > act;

  for (unsigned int i = 0; i < objects.size(); i++) {
    // go there and load
    if (objects[i] != last_type) {
      // we changed type
      printf("%d L %d %d %d\n", d, best_w, last_type, n_last_type);
      act.push_back(make_pair(last_type, n_last_type));
      Store[best_w][last_type] -= n_last_type;
      drone_time++;
      if (objects[i] == MAXP) {
        // sentinel
        break;
      }
      last_type = objects[i];
      n_last_type = 1;
    } else {
      n_last_type++;
    }
  }

  drone_time += dist(Wx[best_w], Wy[best_w], Ox[o], Oy[o]);
  for (unsigned int i = 0; i < act.size(); i++) {
    pair<int, int> myp = act[i];
    printf("%d U %d %d %d\n", d, o, myp.first, myp.second);
    Order[o][last_type] -= n_last_type;
    drone_time += 1;
  }

  dx[d] = Ox[o];
  dy[d] = Oy[o];
  busy_until[d] = t + drone_time;

  // is order complete?
  if (order_is_complete(o))
    Ocompl[o] = busy_until[d];
}

int main() {
  scanf("%d%d%d%d%d", &R, &C, &D, &T, &L);
  scanf("%d", &P);
  for (int p = 0; p < P; p++)
    scanf("%d", &(Ps[p]));
  scanf("%d", &W);
  printf("%d\n", W);
  for (int w = 0; w < W; w++) {
    scanf("%d%d", &(Wx[w]), &(Wy[w]));
  }
  scanf("%d", &O);
  for (int i = 0; i < O; i++)
    for (int p = 0; p < P; p++)
      Order[i][p] = 0;
  for (int o = 0; o < O; o++) {
    scanf("%d%d%d", &(Ox[o]), &(Oy[o]), &(On[o]));
    for (int j = 0; j < On[o]; j++) {
      int p;
      scanf("%d", &p);
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

  t = 0;

  while (t <= T) {
    printf("at time %d\n", t);
    int first_avail = -1;
    for (int d = 0; d < D; d++)
      if (busy_until[d] <= t) {
        first_avail = d;
        break;
      }
    if (first_avail < 0) {
      t++;
      continue;
    }
    int besttime = T, bestorder = -1;
    printf("O is %d\n", O);
    for (int o = 0; o < O; o++) {
      printf("ocompl %d %d\n", o, Ocompl[o]);
      if (Ocompl[o] < 0) {
        printf("considering incompl order %d\n", o);
        int torder = time_to_complete(o);
        if (torder < besttime) {
          bestorder = o;
        }
      }
    }
    // assign d to help towards o
    printf("drone %d will help for order %d\n", first_avail, bestorder);
    execute(first_avail, bestorder);
  }
  return 0;
}
