#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

#include <set>

#define phi lat
#define lambda longi

using namespace std;
int nbTours, nbSat, nbCollec;

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
        if(pos.lat + v >= -90*60*60 && pos.lat <= 90*60*60 ){
            ans.v=v;
            ans.pos.lat=pos.lat+v;ans.pos.longi=pos.longi-15;
        }else if(pos.lat+v>90*60*60){
            ans.v=-v;
            ans.pos.lat=180*60*60-(pos.lat+v);ans.pos.longi=-180*60*60+pos.longi-15;
        }else if(pos.lat+v < -90*60*60){
            ans.v=-v;
            ans.pos.lat=-180*60*60-(pos.lat+v);ans.pos.longi=-180*60*60+pos.longi-15;
        }

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

vector<int> listeAccessible(int idSatel, int tourPrec, int tourActuel, Point orientPrec)
{
	vector<int> res;
	int delta = min(satel[idSatel].maxOrientChangeTotal, satel[idSatel].maxOrientChangePerTurn * (tourActuel - tourPrec));
	Point posRef = satel[idSatel].allStates[tourActuel].pos;
	
	for(int i = 0; i < satel[idSatel].targetsAtTime[tourActuel].size(); i++)
	{
		int idPt = satel[idSatel].targetsAtTime[tourActuel][i];
		Point pos = listeGlobPts[idPt];
		
		if(pos.longi <= posRef.longi + delta && pos.longi >= posRef.longi - delta
			&& pos.lat <= posRef.lat + delta && pos.lat >= posRef.lat)
			res.push_back(idPt);
	}
	return res;
}

int main()
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
	
FILE *f=fopen("precalc_
for(int i=0;i<nbSat;i++){
   // printf("%d\n",i);
    for(int t=0;t<nbTours;t++){
        Point pos = satel[i].allStates[t].pos;
        int delta=min(satel[i].maxOrientChangePerTurn * t, satel[i].maxOrientChangeTotal);
        set<Point>::iterator iter = allTargets.lower_bound(Point(pos.lat-delta,pos.longi-delta));
        set<Point>::iterator iterMax = allTargets.lower_bound(Point(pos.lat+delta,pos.longi+delta+1));
        for(;iter!=iterMax && iter != allTargets.end() ;iter++){
            if(abs(iter->lat - pos.lat) <= delta && abs(iter->longi - pos.longi) <= delta )
               satel[i].targetsAtTime[t].push_back(iter->id);
        }
    //printf("%d %d\n",satel[i].allStates[t].pos.lat,satel[i].allStates[t].pos.longi);
    //if(satel[i].targetsAtTime[t].size() != 0)
    //printf("%d\n",satel[i].targetsAtTime[t].size());
    }
}




	return 0;
}
