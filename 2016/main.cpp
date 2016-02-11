#include<cstdio>
#include<cstring>
#include<cmath>
#include<cassert>
#include<vector>
#include<algorithm>

#define MAXP 2002
#define MAXW 20
#define MAXD 32
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000
#define MAXL 201

#define MAXEXAM 5

// estimated useful capa per roundtrip
#define ESTIMATED_CAPA 60.

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

int dist(int xa, int ya, int xb, int yb) {
  int mydx = (xa - xb);
  int mydy = (ya - yb);
  return ceil(sqrt(mydx*mydx + mydy*mydy));
}


int order_score(int o, int d) {
  // return a score depending on the distance to the currently unoccupied drone
  // and the number of travels required
  
  int load = 0;
  for (int p = 0; p < P; p++) {
    load += Order[o][p] * Ps[p];
  }
  //int x = dx[d];
  //int y = dy[d];
  int x = Wx[0];
  int y = Wy[0];
  double ntrips = ceil(((double) load) / ESTIMATED_CAPA);
  return 2*ntrips*dist(x, y, Ox[o], Oy[o]);
}

int dyn[MAXL][MAXP][MAXL];
int cpstore[MAXP+1];
int vu[MAXL][MAXP][MAXL];

int TOUR=1;

const int LOL = 5;

int sacADos(int cap, int pos, int rp)
{
	if(pos==MAXP)
		return 0;
	if(rp==0)
		return sacADos(cap, pos+1, cpstore[pos+1]);
	
	int& vc = dyn[cap][pos][rp];
	
	if(vu[cap][pos][rp]!=TOUR)
	{
		vu[cap][pos][rp]=TOUR;
		vc=0;
		vc = max(vc,sacADos(cap, pos+1, cpstore[pos+1]));
		if(cap >= Ps[pos])
			vc = max(vc, sacADos(cap-Ps[pos], pos, rp-1)+Ps[pos]);//-(rp==cpstore[pos])*LOL);
	}
	
	return vc;
}

int wload2(int o, int w, vector<int>& res, bool withvect) {

for(int i = 0; i < MAXP; i++)
		cpstore[i]=min(L, min(Store[w][i], Order[o][i]));
	
	int curL = L;
	int rv=0;

        // todo sort decreasing
	
	for(int i = 0; i < MAXP; i++)
	{
		if(cpstore[i] > 0 && Ps[i] <= curL)
		{
                        if (withvect)
			  res.push_back(i);
			rv+=Ps[i];
			curL-=Ps[i];
		}
	}
	return rv;

}

//withvect ignored
int wload(int o, int w, vector<int>& res, bool withvect) {
	TOUR++;
	//memset(dyn, 0, sizeof(dyn));
	
	
	for(int i = 0; i < MAXP; i++)
		cpstore[i]=min(L, min(Store[w][i], Order[o][i]));
		/*
	
	int curL = L;
	int rv=0;
	
	for(int i = 0; i < MAXP; i++)
	{
		if(cpstore[i] > 0 && Ps[i] <= curL)
		{
			res.push_back(i);
			rv+=Ps[i];
			curL-=Ps[i];
		}
	}
	return rv;*/
	
	
	int rv = sacADos(L, 0, cpstore[0]);
	
	int cap=L,pos=0,rp=cpstore[0];
	
	while(pos!=MAXP)
	{
		//printf("wtf %d\n", pos);
		if(rp==0)
		{
			pos++;
			rp=cpstore[pos];
			continue;
		}
		
		int notTake = sacADos(cap, pos+1, cpstore[pos+1]);
		int take=-9999;
		if(cap >= Ps[pos])
			take = sacADos(cap-Ps[pos], pos, rp-1)+Ps[pos];//-(rp==cpstore[pos])*LOL;
		if(take > notTake && take >-9999)
		{
			res.push_back(pos);
			rp--;
			cap=cap-Ps[pos];
		}
		else
		{
			pos++;
			rp=cpstore[pos];
		}
	}
	
	return rv;
  // return useful load than can be taken in warehouse w for order o
}

bool order_is_complete(int o) {
  for (int p = 0; p < P; p++)
    if (Order[o][p] > 0)
      return false;
  return true;
}

// tell d to do something to help towards o
// real = really execute?
int execute(int d, int o, bool real, int myloadf(int, int, vector<int> &, bool)) {
  double best_wquality = -1;
  int best_w = -1;
  for (int w = 0; w < W; w++) {
    int time_to_warehouse = 0; // load-unload times neglected
    time_to_warehouse += dist(dx[d], dy[d], Wx[w], Wy[w]);
    time_to_warehouse += dist(Wx[w], Wy[w], Ox[o], Oy[o]);
    vector<int> v; // décoratif
    int warehouse_load = (*myloadf)(o, w, v, false);
    double w_quality = ((double) warehouse_load) / time_to_warehouse;
    if (w_quality > best_wquality) {
      best_wquality = w_quality;
      best_w = w;
    }
  }

  // i know the warehouse where to go
  vector<int> objects;
  (*myloadf)(o, best_w, objects, true);
  sort(objects.begin(), objects.end());
  objects.push_back(MAXP); // sentinel

  // objects sorted, RLE

  int last_type = objects[0], n_last_type = 0;
  assert(last_type != P);
  int drone_time = 0;
  drone_time += dist(dx[d], dy[d], Wx[best_w], Wy[best_w]);

  vector<pair<int, int> > act;

  //printf("drone %d warehouse %d objective %d will load:\n", d, best_w, o);
  //for (unsigned int i = 0; i < objects.size(); i++) {
  //  printf("%d\n", objects[i]);
  //}

  for (unsigned int i = 0; i < objects.size(); i++) {
    // go there and load
    if (objects[i] != last_type) {
      // we changed type
      if (real)
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
    if (real)
      printf("%d D %d %d %d\n", d, o, myp.first, myp.second);
    Order[o][myp.first] -= myp.second;
   // printf("order %d for type %d now wants %d\n", o, myp.first, Order[o][myp.first]);
    drone_time += 1;
  }

  dx[d] = Ox[o];
  dy[d] = Oy[o];
  busy_until[d] = t + drone_time;

  // is order complete?
  if (order_is_complete(o)) {
    Ocompl[o] = busy_until[d];
    return Ocompl[o];
  } else {
    return -1;
  }
}



int time_to_complete(int o, int d) {
  // backup the crap
  assert(Ocompl[o] == -1);
  int bakorder[MAXP];
  for (int p = 0; p < P; p++)
    bakorder[p] = Order[o][p];
  int bakdx[MAXD], bakdy[MAXD];
  for (int d = 0; d < D; d++) {
    bakdx[d] = dx[d];
    bakdy[d] = dy[d];
  }
  int bakStore[MAXW][MAXP];
  for (int w = 0; w < W; w++)
    for (int p = 0; p < P; p++)
      bakStore[w][p] = Store[w][p];
  int bakbusy_until[MAXD];
  for (int d = 0; d < D; d++)
    bakbusy_until[d] = busy_until[d];
  int bakt = t;

  int att = -1;
  
  // simulate assigning everyone to that
  while (t <= T) {
    //printf("at time %d\n", t);
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
    att = execute(first_avail, o, 0, &wload2);
    if (att > 0)
      break;
  }

  assert(att > 0);
 
  // restore
  for (int p = 0; p < P; p++)
    Order[o][p] = bakorder[p];
  for (int d = 0; d < D; d++) {
    dx[d] = bakdx[d];
    dy[d] = bakdy[d];
  }
  t = bakt;
  for (int w = 0; w < W; w++)
    for (int p = 0; p < P; p++)
      Store[w][p] = bakStore[w][p];
  for (int d = 0; d < D; d++)
    busy_until[d] = bakbusy_until[d];
  Ocompl[o] = -1;

  return att;
}


int main() {
  scanf("%d%d%d%d%d", &R, &C, &D, &T, &L);
  scanf("%d", &P);
  for (int p = 0; p < P; p++)
    scanf("%d", &(Ps[p]));
  scanf("%d", &W);
  //printf("%d\n", W);
  for (int w = 0; w < W; w++) {
    scanf("%d%d", &(Wx[w]), &(Wy[w]));
    for (int p = 0; p < P; p++) {
      scanf("%d", &(Store[w][p]));
    }
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
    //printf("at time %d\n", t);
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
    //printf("O is %d\n", O);
    int examd = 0;

    vector<pair<int, int> > vheur;
    for (int o = 0; o < O; o++) {
      if (Ocompl[o] < 0) {
        int myt = order_score(o, first_avail);
        vheur.push_back(make_pair(myt, o));
      }
    }
    sort(vheur.begin(), vheur.end());
    for (unsigned int i = 0; i < vheur.size(); i++) {
      int o = vheur[i].second;
      //printf("considering incompl order %d\n", o);
      int torder = time_to_complete(o, first_avail);
      //int torder = order_score(o, first_avail);
      if (torder < besttime) {
        bestorder = o;
        besttime = torder;
      }
      examd++;
      if (examd > MAXEXAM)
        break;
    }
    if (bestorder == -1) {
      // we finished?!
      //printf("finished!!\n");
      break;
    }
    // assign d to help towards o
   // printf("drone %d will help for order %d\n", first_avail, bestorder);
    execute(first_avail, bestorder, 1, &wload);
  }
  return 0;
}
