#include "copy.h"

std::vector<IntVar*> allvars;

int main(int argc, char **argv) {
  Solver s("CP is fun!");
  scanf("%d%d", &nbTours, &nbSat);

  for (int i = 0; i < nbSat; i++) {
    int lat /*phi*/, longi /*lambda*/, vitesse, maxOrientChangePerTurn,
        maxOrientChangeTotal;
    scanf("%d%d%d%d%d", &lat, &longi, &vitesse, &maxOrientChangePerTurn,
          &maxOrientChangeTotal);

    satel[i].state.pos.lat = lat;
    satel[i].state.pos.longi = longi;
    satel[i].state.v = vitesse;
    satel[i].maxOrientChangePerTurn = maxOrientChangePerTurn;
    satel[i].maxOrientChangeTotal = maxOrientChangeTotal;
    satel[i].allStates[0] = satel[i].state;
    for (int t = 1; t < nbTours; t++)
      satel[i].allStates[t] = satel[i].allStates[t - 1].next();
    satel[i].targetsAtTime.resize(nbTours);
    satel[i].targetsAtTimeIV.resize(nbTours);
  }

  scanf("%d", &nbCollec);

  for (int i = 0; i < nbCollec; i++) {
    int valCol, nbLoc, nbRange;
    scanf("%d%d%d", &valCol, &nbLoc, &nbRange);

    valCollec.push_back(valCol);

    for (int j = 0; j < nbLoc; j++) {
      int lat /*phi*/, longi /*lambda*/;

      scanf("%d%d", &lat, &longi);

      listeGlobPts.push_back(Point(lat, longi));

      idLocCollec[i].push_back(nbPtsTt);
      allTargets.insert(Point(lat, longi, nbPtsTt));

      idDeMaCollec.push_back(i);

      nbPtsTt++;
    }
    listeGlobPtsIV.resize(listeGlobPts.size());

    for (int j = 0; j < nbRange; j++) {
      int tourDebut, tourFin; // inclus
      scanf("%d%d", &tourDebut, &tourFin);
      toursPossibles[i].push_back(Interv(tourDebut, tourFin));
    }
  }

  if (argc >= 2) {
    FILE *f = fopen(argv[1], "r");
    int i, t, n_truc, truc;
    while (2 < fscanf(f, "%d%d%d", &i, &t, &n_truc)) {
      for (int k = 0; k < n_truc; k++) {
        fscanf(f, "%d", &truc);
        satel[i].targetsAtTime[t].push_back(truc);
        IntVar* xxx = s.MakeBoolVar();
        allvars.push_back(xxx);
        satel[i].targetsAtTimeIV[t].push_back(xxx);
        listeGlobPtsIV[truc].push_back(xxx);
        for(int dt=1;t-dt>=0 && dt-1<=(2*satel[i].maxOrientChangeTotal / satel[i].maxOrientChangePerTurn);dt++ ){
          Point posi = satel[i].allStates[t].pos-listeGlobPts[truc];
          for(int j=0;j<satel[i].targetsAtTime[t-dt].size();j++){
            Point posj = satel[i].allStates[t-dt].pos - listeGlobPts[satel[i].targetsAtTime[t][j]];
            Point delta = posi - posj;
            if(delta.lat > satel[i].maxOrientChangePerTurn * dt || delta.longi > satel[i].maxOrientChangePerTurn * dt){
              std::vector<IntVar*> x;
              x.push_back(satel[i].targetsAtTimeIV[t][k]);
              x.push_back(satel[i].targetsAtTimeIV[t-dt][j]);
              s.AddConstraint(s.MakeSumLessOrEqual(x,1));
            }  
          }
        }
      }//end for (n_truc)
      if(satel[i].targetsAtTimeIV[t].size()>1)
        s.AddConstraint(s.MakeSumLessOrEqual(satel[i].targetsAtTimeIV[t],1));
    }
  } else { // precalcul

    for (int i = 0; i < nbSat; i++) {
      // printf("%d\n",i);
      for (int t = 0; t < nbTours; t++) {
        Point pos = satel[i].allStates[t].pos;
        int delta = min(satel[i].maxOrientChangePerTurn * (t + 1),
                        satel[i].maxOrientChangeTotal);
        set<Point>::iterator iter =
            allTargets.lower_bound(Point(pos.lat - delta, pos.longi - delta));
        set<Point>::iterator iterMax = allTargets.lower_bound(
            Point(pos.lat + delta, pos.longi + delta + 1));
        for (; iter != iterMax && iter != allTargets.end(); iter++) {
          if (abs(iter->lat - pos.lat) <= delta &&
              abs(iter->longi - pos.longi) <= delta)
            if (isAllowed(idDeMaCollec[iter->id],
                          t)) { // if iter is allowed at time t
              satel[i].targetsAtTime[t].push_back(iter->id);
              // printf("someone can do %d\n", iter->id);
            }
        }
        // PRECALCUL
        if (satel[i].targetsAtTime[t].size())
          printf("%d %d %d\n", i, t, satel[i].targetsAtTime[t].size());
        for (int k = 0; k < satel[i].targetsAtTime[t].size(); k++)
          printf("%d\n", satel[i].targetsAtTime[t][k]);
      }
    }
    return 0;
  }


vector<IntVar*> collecsIV;



for (int i = 0; i < nbCollec; i++) {
  vector<IntVar*> collecPhotosIV;
  for (int j = 0; j < idLocCollec[i].size(); j++) {
    collecPhotosIV.push_back(s.MakeMax(listeGlobPtsIV[idLocCollec[i][j]])->Var());
  }
  collecsIV.push_back(s.MakeMin(collecPhotosIV)->Var());
}
IntVar* score = s.MakeScalProd(collecsIV,valCollec)->Var();

OptimizeVar* const result = s.MakeMaximize(score,1000);

DecisionBuilder* const db = s.MakePhase(allvars, Solver::CHOOSE_FIRST_UNBOUND, Solver::ASSIGN_MIN_VALUE);

cerr << "start search" << endl;
s.Solve(db, result);


cerr << s.DebugString() << endl;
  // fin de la lecture d'entree
  ///////////////////////////////////////////////

  /////////////////////////////////////////////////
  return 0;
}
