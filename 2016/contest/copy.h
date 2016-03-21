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


#include "base/commandlineflags.h"
#include "base/commandlineflags.h"
#include "base/logging.h"
#include "base/integral_types.h"
#include "base/stringprintf.h"
#include "base/map_util.h"
#include "constraint_solver/constraint_solver.h"



#define phi lat
#define lambda longi

using namespace std;
using namespace operations_research;

int nbTours, nbSat, nbCollec;

bool todoCollection[10002];
bool oldTodoCollection[10002];

struct Interv {
  int tourDebut;
  int tourFin;

  Interv() {}
  Interv(int tourDebut, int tourFin) : tourDebut(tourDebut), tourFin(tourFin) {}
};

struct Point {

  int lat;   // Phi // EN SECONDES
  int longi; // Lambda //EN SECONDES
  int id;    // pas toujours rempli
  Point() : lat(0), longi(0), id(-1) {}
  Point(int lat, int longi) : lat(lat), longi(longi), id(-1) {}
  Point(int lat, int longi, int id) : lat(lat), longi(longi), id(id) {}
  const bool operator<(const Point &other) const {
    if (lat == other.lat && longi == other.longi)
      return id < other.id;
    return lat == other.lat ? longi < other.longi : lat < other.lat;
  }
  Point operator-(const Point &o){
    Point res;
    res.lat = lat - o.lat;
    res.longi = longi-o.longi;
  }
};
struct State {
  Point pos;
  int v; // vitesse EN SECONDES D'ARC PAR TOUR

  State next() {
    State ans;
    if (pos.lat + v >= -90 * 60 * 60 && pos.lat + v <= 90 * 60 * 60) {
      ans.v = v;
      ans.pos.lat = pos.lat + v;
      ans.pos.longi = pos.longi - 15;
    } else if (pos.lat + v > 90 * 60 * 60) {
      ans.v = -v;
      ans.pos.lat = 180 * 60 * 60 - (pos.lat + v);
      ans.pos.longi = -180 * 60 * 60 + pos.longi - 15;
    } else if (pos.lat + v < -90 * 60 * 60) {
      ans.v = -v;
      ans.pos.lat = -180 * 60 * 60 - (pos.lat + v);
      ans.pos.longi = -180 * 60 * 60 + pos.longi - 15;
    }
    if (ans.pos.longi < -648000)
      ans.pos.longi += 360 * 60 * 60;
    if (ans.pos.longi > 647999)
      ans.pos.longi -= 360 * 60 * 60;

    return ans;
  }
};

struct Satellite {
  State state;
  int maxOrientChangePerTurn; /* w */
  int maxOrientChangeTotal;   /* d */
  State allStates[604800];
  vector<vector<int>> targetsAtTime;
  vector<vector<IntVar*> > targetsAtTimeIV;
  Point where_is(int t, Point target) {
    Point p;
    p.lat = target.lat - allStates[t].pos.lat;
    p.longi = target.longi - allStates[t].pos.longi;
    return p;
  }
};

Satellite satel[100];
vector<int> idLocCollec[10000];
vector<int> valCollec;
vector<Interv> toursPossibles[10000];
set<Point> allTargets;


vector<Point> listeGlobPts;
vector<vector<IntVar *> > listeGlobPtsIV;

int nbPtsTt = 0;
vector<int> idDeMaCollec;

bool isAllowed(int id_coll, int t) {
  for (int i = 0; i < toursPossibles[id_coll].size(); i++) {
    if (toursPossibles[id_coll][i].tourDebut <= t &&
        toursPossibles[id_coll][i].tourFin >= t)
      return true;
  }
  return false;
}

void listeAccessible(int idSatel, int tourPrec, int tourActuel,
                     const Point orientPrec, map<int, int> &res) {
  const int delta =
      satel[idSatel].maxOrientChangePerTurn * (tourActuel - tourPrec);
  Point posRef = satel[idSatel].allStates[tourActuel].pos;
  posRef.lat += orientPrec.lat;
  posRef.longi += orientPrec.longi;
  for (int idPt : satel[idSatel].targetsAtTime[tourActuel]) {
    Point pos = listeGlobPts[idPt];
    if (pos.longi <= posRef.longi + delta &&
        pos.longi >= posRef.longi - delta && pos.lat <= posRef.lat + delta &&
        pos.lat >= posRef.lat - delta)
      if (res.find(idPt) == res.end())
        res[idPt] = tourActuel;
  }
}

