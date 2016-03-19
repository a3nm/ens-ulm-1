#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>

#define phi lat
#define lambda longi

using namespace std;
int nbTours, nbSat, nbCollec;

struct Point
{
        int lat;//Phi // EN SECONDES
        int longi;//Lambda //EN SECONDES
};


struct State
{
    Point pos;
    int v;//vitesse EN SECONDES D'ARC PAR TOUR

State next(){
    State ans;
    if(pos.lat + v >= 90*60*60 && pos.lat <= -90*60*60 ){
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
        State start;
	int maxOrientChangePerTurn; /* w */
	int maxOrientChangeTotal; /* d */
};

Satellite satel[100];

int main()
{
	scanf("%d%d", &nbTours, &nbSat);
	
	for(int i = 0; i < nbSat; i++)
	{
		int lat /*phi*/, longi /*lambda*/, vitesse, maxOrientChangePerTurn, maxOrientChangeTotal;
		scanf("%d%d%d%d%d", &lat, &longi, &vitesse, &maxOrientChangePerTurn, &maxOrientChangeTotal);
	}
	
	scanf("%d", &nbCollec);
	
	for(int i = 0; i < nbCollec; i++)
	{
		int valCol, nbLoc, nbRange;
		scanf("%d%d%d", &valCol, &nbLoc, &nbRange);
		
		for(int j = 0; j < nbLoc; j++)
		{
			int lat /*phi*/, longi /*lambda*/;
			
			scanf("%d%d", &lat, &longi);
		}
		
		for(int j = 0; j < nbRange; j++)
		{
			int tourDebut, tourFin; //inclus
			scanf("%d%d", &tourDebut, &tourFin);
		}
	}
	
	return 0;
}
