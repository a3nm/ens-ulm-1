#include<cstdio>
#include<vector>
#include <math.h>

#define MAXP 2002
#define MAXW 20
#define MAXD 32
#define MAXON 10002
#define MAXO 1252
#define MAXT 150000

using namespace std;
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

vector<int> store_of[MAXP];

int car(int d)
{
  return d*d;
}

int dist(int d, int w,int o)
{
  return sqrt(car(dx[d]-Wx[w])+car(dy[d]-Wy[d])) + sqrt(car(Ox[o]-Wx[w])+car(Oy[o]-Wy[d])) ;
}
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
      if(Store[i][j])
	store_of[j].push_back(i);
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

  for(int o = 0 ; o < O ; o++)
    for(int j = 0 ; j < On[o] ; j++ )
      {
	int prod = Order[o][j] ;
	int best_t = T ;
	int best_w = -1 ;
	int best_d = -1 ;
	for(int w : store_of[prod])
	  if(Store[w][prod])
	    {
	      for(int d = 0 ; d < D ; d++)
		{
		  int tps = busy_until[d]+dist(d,w,o)+2;
		  if(tps<best_t)
		    {
		      best_t = tps ;
		      best_w = w;
		      best_d = d;
		    }
		}
	    }
	if(best_t<T)
	  {
	    busy_until[best_d] = best_t;
	    dx[best_d] = Ox[o] ;
	    dy[best_d] = Oy[o] ;
	    printf("%d L %d %d 1\n",best_d,best_w,prod);
	    printf("%d W %d %d 1\n",best_d,best_w,prod);
	    Store[best_w][prod]--;
	  }
      }
  return 0;
}
