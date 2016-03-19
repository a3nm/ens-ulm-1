#include<cstdio>
#include<vector>
#include<algorithm>
#include <math.h>
#include <map>

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
int inStore[MAXW][MAXP];
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
  return ceil(sqrt(car(dx[d]-Wx[w])+car(dy[d]-Wy[w])) + sqrt(car(Ox[o]-Wx[w])+car(Oy[o]-Wy[w]))) ;
}

int dist_direct(int d, int o)
{
  return ceil(sqrt(car(Ox[o]-dx[d])+car(Oy[o]-dy[d]))) ;
}

int time_to_complete(int o) {
  return 0;
}

void execute(int d, int o) {
  // tell d to do something to help towards o
}

void read_in ()
{
  scanf("%d%d%d%d%d", &R, &C, &D, &T, &L);
  scanf("%d", &P);
  for (unsigned int i = 0; i < P; i++)
    scanf("%d", &(Ps[i]));
  scanf("%d", &W);
  for (unsigned int i = 0; i < W; i++) {
    scanf("%d%d", &(Wx[i]), &(Wy[i]));
    for (unsigned int j = 0; j < P; j++) {
      scanf("%d", &(inStore[i][j]));
      if(inStore[i][j])
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

}


struct step
{
  int client;
  int warehouse;
  int weight ;
  int deliver ;
  vector<int> load ;
};

typedef pair<int,int> pii ;

struct sol
{

  vector<step> parcours [ MAXD ] ;
  vector<pair<int,int> > sort_dist ;

  int score ;
  
  void optim_order()
  {
    for(int o = 0 ; o < O ; o++)
      {
	sort_dist.push_back(make_pair(dist_direct(0,o)+On[o]*5,o));
      }
    sort(sort_dist.begin(),sort_dist.end());
  }


  void eval()
  {
    for(int d = 0 ; d < D ; d++)
      parcours[d].clear();
    for(size_t i = 0 ; i < W  ; i++ )
      for(size_t j = 0 ; j < P ; j++ )
	Store[i][j] = inStore[i][j]; 

    score = 0 ;
    for(int id_o = 0 ; id_o < O ; id_o++)
      {
	int o = sort_dist[id_o].second ;
	int tps_o = -1 ;
	for(int j = 0 ; j < On[o] ; j++ )
	  {
	    int prod = Order[o][j] ;
	    int best_t = T ;
	    int best_w = -1 ;
	    int best_d = -1 ;
	    for(int d = 0 ; d < D ; d++)
	      {
		for(int i = parcours[d].size()-1 ; i>=0 ; i--)
		  {
		    step & p =parcours[d][i];
		    if(p.weight+Ps[prod] > L)
		      break ;
		    if(p.warehouse >= 0 && Store[p.warehouse][prod])
		      {
			int tps = busy_until[d]+dist_direct(d,o)+2;
			for( int l : p.load)
			  if(l==prod)
			    {
			      tps-- ;
			      break ;
			    }
			if(tps<best_t)
			  {
			    best_t = tps ;
			    best_w = -1 ;
			    best_d = d;
			  }
			break;
		      }
		  }
		for(int w : store_of[prod])
		  if(Store[w][prod])
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
		if(tps_o<best_t)
		  tps_o = best_t ;
		if(best_w>= 0)
		  {
		    //fprintf(stderr,"DRONE %d WARE %d ORDER %d\n",best_d,best_w,o);
		    busy_until[best_d] = best_t;
		    dx[best_d] = Ox[o] ;
		    dy[best_d] = Oy[o] ;
		    Store[best_w][prod]--;
		    step s ;
		    s.client = o;
		    s.warehouse = best_w ;
		    s.deliver = prod;
		    s.load.push_back(prod);
		    s.weight = Ps[prod] ;
		    parcours[best_d].push_back(s);
		  }
		else
		  {
		    //fprintf(stderr,"SUROCCUPY %d WARE %d ORDER %d TIME %d \n",best_d,best_w,o,best_t);
		    busy_until[best_d] = best_t;
		    dx[best_d] = Ox[o] ;
		    dy[best_d] = Oy[o] ;
		    step s ;
		    s.client = o;
		    s.warehouse = best_w ;
		    s.deliver = prod;
		    s.weight = Ps[prod] ;
		
		    for(int i = parcours[best_d].size()-1 ; i>=0 ; i--)
		      {
			step & p =parcours[best_d][i];
			p.weight+=Ps[prod] ;
			if(p.weight > L)
			  exit(0);
			if(p.warehouse >= 0 && Store[p.warehouse][prod])
			  {
			    p.load.push_back(prod);
			    Store[p.warehouse][prod]--;
			    break;
			  }
		      }
		    parcours[best_d].push_back(s);
		  }
	      }
	  }
	if(tps_o>=0)
	  {
	    score+= ((T-tps_o)*100+T-1)/T ;
	  }
      }
  }

  int print()
  {
    for(int d = 0 ; d < D ; d++)
      for(step & s : parcours[d])
	{
	  map<int,int> to_load ;
	  for(int p : s.load)
	    to_load[p]++;
	  for(int p : s.load)
	    if(to_load[p]>0)
	      {
		printf("%d L %d %d %d\n",d,s.warehouse,p,to_load[p]);
		to_load[p] = 0 ;
	      }
	  printf("%d D %d %d 1\n",d,s.client,s.deliver);
	}
  }

  
};

int main() {
  read_in();
  sol s ;
  s.optim_order();
  s.eval();
  fprintf(stderr,"SCORE: %d\n",s.score);
  sol s2 ;
  
  for(int i = 0 ; i < 00 ; i++ )
    {
      s2.sort_dist = s.sort_dist ;
      int a = rand()%O;
      int b = rand()%O;
      if(a!=b)
	swap(s2.sort_dist[a],s2.sort_dist[b]);
      s2.eval();
      if(s2.score>s.score)
	{
	  fprintf(stderr,"UPDATE SCORE: %d\n",s2.score);
	  s = s2 ;
	}
    }
  s.print();
  
  return 0;
}
