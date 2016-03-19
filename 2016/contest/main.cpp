#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <cmath>

#include <set>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define phi lat
#define lambda longi

using namespace std;
int nbTours, nbSat, nbCollec;

bool todoCollection[10002];
bool oldTodoCollection[10002];

struct Interv
{
	int tourDebut;
	int tourFin;
	
	Interv() { }
	Interv(int tourDebut, int tourFin) : tourDebut(tourDebut), tourFin(tourFin) { }
};

struct Point
{
	
        int lat;//Phi // EN SECONDES
        int longi;//Lambda //EN SECONDES
        int id; //pas toujours rempli
    Point() : lat(0), longi(0), id(-1){ }
    Point(int lat, int longi) : lat(lat), longi(longi), id(-1) { }
    Point(int lat, int longi, int id) : lat(lat), longi(longi), id(id) { }
    const bool operator< (const Point &other ) const{
    	if (lat==other.lat && longi == other.longi)
    		return id < other.id;
        return lat==other.lat ? longi<other.longi : lat<other.lat;
        }
};

struct State
{
    Point pos;
    int v;//vitesse EN SECONDES D'ARC PAR TOUR

    State next(){
        State ans;
        if(pos.lat + v >= -90*60*60 && pos.lat+v <= 90*60*60 ){
            ans.v=v;
            ans.pos.lat=pos.lat+v;ans.pos.longi=pos.longi-15;
        }else if(pos.lat+v>90*60*60){
            ans.v=-v;
            ans.pos.lat=180*60*60-(pos.lat+v);ans.pos.longi=-180*60*60+pos.longi-15;
        }else if(pos.lat+v < -90*60*60){
            ans.v=-v;
            ans.pos.lat=-180*60*60-(pos.lat+v);ans.pos.longi=-180*60*60+pos.longi-15;
        }
        if(ans.pos.longi < -648000)ans.pos.longi+=360*60*60;
        if(ans.pos.longi > 647999)ans.pos.longi-=360*60*60;

    return ans;
    }
};


struct Satellite
{
    State state;
	int maxOrientChangePerTurn; /* w */
	int maxOrientChangeTotal; /* d */
        State allStates[604800];
        vector<vector< int > > targetsAtTime; 
        Point where_is(int t, Point target){
            Point p;
            p.lat=target.lat - allStates[t].pos.lat;
            p.longi=target.longi - allStates[t].pos.longi;
            return p;
        }


};

Satellite satel[100];
vector<int> idLocCollec[10000];
int valCollec[10000];
vector<Interv> toursPossibles[10000];
set<Point> allTargets;

vector<Point> listeGlobPts;
int nbPtsTt;
vector<int> idDeMaCollec;

bool isAllowed(int id_coll, int t){
    for(int i=0;i<toursPossibles[id_coll].size();i++){
        if(toursPossibles[id_coll][i].tourDebut <= t && toursPossibles[id_coll][i].tourFin>=t)
            return true;
    }
    return false;
}





void listeAccessible(int idSatel, int tourPrec, int tourActuel, const Point orientPrec, map<int,int> & res )
{
	const int delta = satel[idSatel].maxOrientChangePerTurn * (tourActuel - tourPrec);
	Point posRef = satel[idSatel].allStates[tourActuel].pos ;
	posRef.lat+= orientPrec.lat;
	posRef.longi+= orientPrec.longi;
	for(int idPt : satel[idSatel].targetsAtTime[tourActuel])
	{
	  Point pos = listeGlobPts[idPt];
	  if(pos.longi <= posRef.longi + delta && pos.longi >= posRef.longi - delta
	     && pos.lat <= posRef.lat + delta && pos.lat >= posRef.lat - delta)
	    if(res.find(idPt) == res.end() )
	      res[idPt]=tourActuel ;
	}
}

#include "match.cc"

int readsol(const char* file, bool print, unsigned int nbDone[10002], vector<pair<Point, pair<int, int> > > result);

int glouton(void);
//int glouton2(void);

int main(int argc, char **argv)
{
	scanf("%d%d", &nbTours, &nbSat);
	
	for(int i = 0; i < nbSat; i++)
	{
		int lat /*phi*/, longi /*lambda*/, vitesse, maxOrientChangePerTurn, maxOrientChangeTotal;
		scanf("%d%d%d%d%d", &lat, &longi, &vitesse, &maxOrientChangePerTurn, &maxOrientChangeTotal);
		
		satel[i].state.pos.lat=lat;
		satel[i].state.pos.longi=longi;
		satel[i].state.v=vitesse;
		satel[i].maxOrientChangePerTurn=maxOrientChangePerTurn;
		satel[i].maxOrientChangeTotal=maxOrientChangeTotal;
                satel[i].allStates[0]=satel[i].state;
                for(int t=1;t<nbTours;t++)satel[i].allStates[t]=satel[i].allStates[t-1].next();
                satel[i].targetsAtTime.resize(nbTours);
	}
	
	scanf("%d", &nbCollec);
	
	for(int i = 0; i < nbCollec; i++)
	{
		int valCol, nbLoc, nbRange;
		scanf("%d%d%d", &valCol, &nbLoc, &nbRange);
		
		valCollec[i]=valCol;
		
		for(int j = 0; j < nbLoc; j++)
		{
			int lat /*phi*/, longi /*lambda*/;
			
			scanf("%d%d", &lat, &longi);
			
			listeGlobPts.push_back(Point(lat,longi));
			
			idLocCollec[i].push_back(nbPtsTt);
            allTargets.insert(Point(lat,longi,nbPtsTt));
            
            idDeMaCollec.push_back(i);
            
            nbPtsTt++;
		}
		
		for(int j = 0; j < nbRange; j++)
		{
			int tourDebut, tourFin; //inclus
			scanf("%d%d", &tourDebut, &tourFin);
			toursPossibles[i].push_back(Interv(tourDebut, tourFin));
		}
	}
	
if(argc>=2){
    FILE *f = fopen(argv[1],"r");
    int i, t, n_truc, truc;
    while(2<fscanf(f,"%d%d%d",&i,&t,&n_truc)){
            for(int k=0;k<n_truc;k++){fscanf(f,"%d",&truc);satel[i].targetsAtTime[t].push_back(truc);}
    }
    }else{

for(int i=0;i<nbSat;i++){
   // printf("%d\n",i);
    for(int t=0;t<nbTours;t++){
        Point pos = satel[i].allStates[t].pos;
        int delta=min(satel[i].maxOrientChangePerTurn * t, satel[i].maxOrientChangeTotal);
        set<Point>::iterator iter = allTargets.lower_bound(Point(pos.lat-delta,pos.longi-delta));
        set<Point>::iterator iterMax = allTargets.lower_bound(Point(pos.lat+delta,pos.longi+delta+1));
        for(;iter!=iterMax && iter != allTargets.end() ;iter++){
            if(abs(iter->lat - pos.lat) <= delta && abs(iter->longi - pos.longi) <= delta )
                if( isAllowed(idDeMaCollec[iter->id],t)) {//if iter is allowed at time t
                    satel[i].targetsAtTime[t].push_back(iter->id);
            //printf("someone can do %d\n", iter->id);
                }
        }
        //PRECALCUL
        if(satel[i].targetsAtTime[t].size()) 
            printf("%d %d %d\n",i,t,satel[i].targetsAtTime[t].size());
        for(int k=0;k<satel[i].targetsAtTime[t].size();k++)
            printf("%d\n",satel[i].targetsAtTime[t][k]);

    }

 
 }
 return 0;
}


        if (argc == 4) {
          unsigned int nbDone[10002];
          vector<pair<Point, pair<int, int> > > result;
          readsol(argv[3], true, nbDone, result);
          return 0;
        }
    if (!strcmp(argv[0], "./glouton"))
      return glouton();

    unsigned int nbDone[10002];
    int score = 0;
      
    for (int c = 0; c < nbCollec; c++) {
      todoCollection[c] = true;
      oldTodoCollection[c] = true;
    }

    while (true) {
      louis l;
      l.sol();
      int new_score = readsol("", false, nbDone, l.res);
      printf("old score was %d new score is %d\n", score, new_score);
      if (score > new_score) {
        // crap; reset and retry
        printf("backtrack\n");
        for (int c = 0; c < nbCollec; c++)
          todoCollection[c] = oldTodoCollection[c];
        continue;
      }
      // best so far
      for (int c = 0; c < nbCollec; c++)
        oldTodoCollection[c] = todoCollection[c];
      score = new_score;
      l.print(string(argv[2]));
      printf("SAVED SOLUTION WITH SCORE %d\n", score);
      // decide based on nbDone which tasks to do or not
      int ndone= 0, ntodo = 0;
      printf("we did collections: ");
      for (int c = 0; c < nbCollec; c++) {
        if (nbDone[c] == idLocCollec[c].size()) {
          ndone++;
          ntodo++;
          todoCollection[c] = true; // keep it
          printf("%d ", c);
        } else {
          // not fully done, keep with low proba
          //todoCollection[c] = (!(rand() % 150));
          todoCollection[c] = false;
          ntodo += todoCollection[c];
        }
      }
      printf("\n");
      printf("we will do collections ");
      for (int c = 0; c < nbCollec; c++) {
        if (todoCollection[c]) {
          printf("%d ", c);
        }
      }
      printf("\n");
      printf("done %d TODO %d\n", ndone, ntodo);
    }

    return 0;
}


int readsol(const char* file, bool print, unsigned int nbDone[10002], vector<pair<Point, pair<int, int> > > result)
    {
  if (print)
    printf("CHECK SOLUTION in %s\n", file);
  FILE* f;
  if (result.size() == 0)
    f = fopen(file, "r");
  int N;
  vector<pair<int, pair<int, int> > > V[42];
  if (result.size() == 0)
    fscanf(f, "%d", &N);
  else
    N = result.size();
  for (int i = 0; i < N; i++) {
    int phi, lambda, t, id;
    if (result.size() == 0)
      fscanf(f, "%d%d%d%d", &phi, &lambda, &t, &id);
    else {
      phi = result[i].first.lat;
      lambda = result[i].first.longi;
      t = result[i].second.first;
      id = result[i].second.second;
    }
    V[id].push_back(make_pair(t, make_pair(phi, lambda)));
  }
  if (result.size() == 0)
    fclose(f);
  map<Point, vector<int> > pointToCollec;
  // done points for collection
  set<Point> isDone[10002];
  // nb done for collection

  for (int i = 0; i < nbCollec; i++) {
    nbDone[i] = 0;
    for (unsigned int j = 0; j < idLocCollec[i].size(); j++) {
      Point pt = listeGlobPts[idLocCollec[i][j]];
      pointToCollec[pt].push_back(i);
    }
  }
  for (int i = 0; i < nbSat; i++) {
    sort(V[i].begin(), V[i].end());
    int posx = 0, posy = 0, t = 0;
    for (unsigned int j = 0; j < V[i].size(); j++) {
      int phi = V[i][j].second.first;
      int lambda = V[i][j].second.second;
      Point rel = satel[i].where_is(V[i][j].first, Point(phi, lambda));
      if (abs(rel.lat) > satel[i].maxOrientChangeTotal || abs(rel.longi) > satel[i].maxOrientChangeTotal) {
        // we have problem
        printf("problem with satel %d\n", i);
        printf("at time %d posx was %d and posy was %d\n", t, posx, posy);
        printf("at time %d you want to take picture at %d %d\n", V[i][j].first, phi, lambda);
        printf("which is OUT OF VIEW: %d %d\n", rel.lat, rel.longi);
        return 42;
      }
      int dt = V[i][j].first - t;
      int mdelta = dt * satel[i].maxOrientChangePerTurn;
      int dx = rel.lat - posx;
      int dy = rel.longi - posy;
      if (abs(dx) > mdelta || abs(dy) > mdelta) {
        // we have problem
        printf("problem with satel %d\n", i);
        printf("at time %d posx was %d and posy was %d\n", t, posx, posy);
        printf("at time %d you want to take picture at %d %d\n", V[i][j].first, phi, lambda);
        printf("which is in view at %d %d\n", rel.lat, rel.longi);
        printf("so the relative motion is %d %d in %d turns with velocity %d: PROBLEM\n",
            dx, dy, dt, satel[i].maxOrientChangePerTurn);
        return 42;
      }
      // now mark the done collecs
      Point mypt = Point(phi, lambda);
      for (unsigned int k = 0; k < pointToCollec[mypt].size(); k++) {
        int collec = pointToCollec[mypt][k];
        if (isDone[collec].find(mypt) != isDone[collec].end())
          continue; // already good
        // check if t is in right range
        for (unsigned int l = 0; l < toursPossibles[collec].size(); l++) {
          if (t >= toursPossibles[collec][l].tourDebut && t <= toursPossibles[collec][l].tourFin) {
            // we are good!
            isDone[collec].insert(mypt);
            nbDone[collec]++;
            break;
          }
        }
      }
      t = V[i][j].first;
      posx = rel.lat;
      posy = rel.longi;
    }
  }
  if (print)
    printf("solution ok\n");
  long score = 0;
  for (int c = 0; c <nbCollec; c++) {
    if(print)
      printf("for collec %d done %d points of %d\n", c, nbDone[c], (int) idLocCollec[c].size());
    if (nbDone[c] == idLocCollec[c].size()) {
      score += valCollec[c];
    }
  }
  if (print)
  printf("FINAL SCORE %ld\n", score);
  return score;
}

int glouton() {
  vector<pair<Point, pair<int, int> > > result;
  int satposx[50], satposy[50], satfree[50];
  set<int> doneObj;
  for (int i = 0; i < nbSat; i++)
    satposx[i] = satposy[i] = satfree[i] = 0;
  for (int t = 0; t < nbTours; t++) {
    for (int s = 0; s < nbSat; s++) {
      if (satfree[s] > t)
        continue;
      // choose an objective for s
      vector<int> targets = satel[s].targetsAtTime[t];
      // assuming that the objectives are OK
      //if (t == 373)
        //printf("sat %d at time %d can do %d targets\n", s, t, targets.size());
      for (unsigned int no = 0; no < targets.size(); no++) {
        int o = targets[no];
        if (doneObj.find(o) != doneObj.end())
          continue; // done already
        // can we get there?
        Point rel = satel[s].where_is(t, listeGlobPts[o]);
        int dt = t - satfree[s];
        int w = satel[s].maxOrientChangePerTurn;
        int maxdelta = dt * w;
        int dx = abs(satposx[s] - rel.lat);
        int dy = abs(satposy[s] - rel.longi);
        //printf("sat %d at time %d can do %d and dx dy is %d %d!!\n", s, t, o, dx, dy);
        if (dx > maxdelta || dy > maxdelta)
          continue;  // objective not doable
        // ok, go there
        satfree[s] = t;
        satposx[s] = rel.lat;
        satposy[s] = rel.longi;
        doneObj.insert(o);
        result.push_back(make_pair(listeGlobPts[o], make_pair(t, s)));
        // TODO may have done other things
        break;
      }
    }
  }
  printf("%d\n", result.size());
  for (unsigned int i = 0; i < result.size(); i++)
    printf("%d %d %d %d\n", result[i].first.lat, result[i].first.longi, result[i].second.first, result[i].second.second);
  return 0;
}


