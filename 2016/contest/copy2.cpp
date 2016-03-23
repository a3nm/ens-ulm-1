#include "copy2.h"
#include <csignal>

std::vector<MPVariable*> allvars;

int main(int argc, char **argv) {
  MPSolver s("Solver",MPSolver::GLPK_LINEAR_PROGRAMMING);
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

    for (int j = 0; j < nbRange; j++) {
      int tourDebut, tourFin; // inclus
      scanf("%d%d", &tourDebut, &tourFin);
      toursPossibles[i].push_back(Interv(tourDebut, tourFin));
    }
  }
  listeGlobPtsIV.resize(listeGlobPts.size());

  if (argc >= 2) {
    FILE *f = fopen(argv[1], "r");
    int i, t, n_truc, truc;
    while (2 < fscanf(f, "%d%d%d", &i, &t, &n_truc)) {
        //vector<MPVariable*> vvv;
        s.MakeBoolVarArray(n_truc,"_"+to_string(i)+"_"+to_string(t)+"_",&(satel[i].targetsAtTimeIV[t]));
        //satel[i].targetsAtTimeIV[t]=vvv;
      for (int k = 0; k < n_truc; k++) {
        fscanf(f, "%d", &truc);
        //MPVariable* xxx = s.MakeBoolVar(to_string(i)+"_"+to_string(t)+"_"+to_string(k));
        //satel[i].targetsAtTimeIV[t].push_back(vvv[k]);
        allvars.push_back(satel[i].targetsAtTimeIV[t][k]);
        listeGlobPtsIV[truc].push_back(satel[i].targetsAtTimeIV[t][k]);
        Point posi = satel[i].allStates[t].pos-listeGlobPts[truc];
        for(int dt=0;(t-dt>=0) && (dt <= 2+((2*satel[i].maxOrientChangeTotal) / satel[i].maxOrientChangePerTurn));dt++ ){
            //if(t==81803 && t-dt==81802)std::raise(SIGINT);
          for(int j=0;j<satel[i].targetsAtTime[t-dt].size();j++){
            Point posj = satel[i].allStates[t-dt].pos - listeGlobPts[satel[i].targetsAtTime[t-dt][j]];
            Point delta = posi - posj;
            if( (abs(delta.lat) > (satel[i].maxOrientChangePerTurn * dt)) || (abs(delta.longi) > (satel[i].maxOrientChangePerTurn * dt)) || satel[i].targetsAtTime[t-dt][j] == truc){
              MPConstraint* x = s.MakeRowConstraint(0.,1.);
              x->SetCoefficient(satel[i].targetsAtTimeIV[t][k],1);
              x->SetCoefficient(satel[i].targetsAtTimeIV[t-dt][j],1);
              //printf("%d\n",s.NumConstraints());
              //cerr<< "no" <<i << " " << t-dt << " "<< listeGlobPts[satel[i].targetsAtTime[t-dt][j]].lat << " " << t << " " << listeGlobPts[truc].lat << endl;
            }  
          }
        }
          satel[i].targetsAtTime[t].push_back(truc);
      }//end for (n_truc)
      /*if(satel[i].targetsAtTimeIV[t].size()>1){
          MPConstraint* x = s.MakeRowConstraint(0,1);
          for(auto j:satel[i].targetsAtTimeIV[t])x->SetCoefficient(j,1);
      }*/
    }//end read file
    //begin constraints












    fclose(f);
    
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


vector<MPVariable*> collecsIV;
s.MakeBoolVarArray(nbCollec,"collec",&collecsIV);

for (int i = 0; i < nbCollec; i++) {
  vector<MPVariable*> collecPhotosIV;
  s.MakeBoolVarArray(idLocCollec[i].size(),"photo"+to_string(i)+"_",&collecPhotosIV);
  for (int j = 0; j < idLocCollec[i].size(); j++) {
      //MPVariable * x = s.MakeBoolVar("photo"+to_string(i)+"_"+to_string(j));
      //collecPhotosIV.push_back(x);
      MPConstraint* y = s.MakeRowConstraint(0,0);
      y->SetCoefficient(collecPhotosIV[j],-1);
      for(auto k:listeGlobPtsIV[idLocCollec[i][j]])y->SetCoefficient(k,1);
  }
/*
  MPConstraint* y = s.MakeRowConstraint(0,0);
  //MPVariable * x = s.MakeBoolVar("collec"+to_string(i));
  //collecsIV.push_back(x);
  y->SetCoefficient(collecsIV[i],-1*idLocCollec[i].size());
  for(auto k:collecPhotosIV)y->SetCoefficient(k,1);*/
  for(auto k:collecPhotosIV){
      MPConstraint* y = s.MakeRowConstraint(0,0);
      y->SetCoefficient(k,1);
      y->SetCoefficient(collecsIV[i],-1);
  }
}

MPObjective* obj = s.MutableObjective();
for(int k=0;k<collecsIV.size();k++)obj->SetCoefficient(collecsIV[k],valCollec[k]);
obj->SetMaximization();


//s.set_time_limit(3600000);
s.EnableOutput(),
cerr << "start search "<< s.NumVariables() << " " << s.NumConstraints() << endl;
auto result = s.Solve();

switch(result) {
      case MPSolver::OPTIMAL:
          cout << "Optimal result found ! Score: " << obj->Value() << endl;
          break;
      case MPSolver::FEASIBLE:
          cout << "Sub-optimal solution found; score: " << obj->Value() << " (upper bound: " << obj->BestBound() << ")" << endl;
          break;
      default:
          cout << "The solver failed to solve the problem.";
          exit(1);
}
cout << obj->Value() << endl;

for (int i = 0; i < nbSat; i++) {
  for (int t = 0; t < nbTours; t++) {
    for(int j=0;j<satel[i].targetsAtTimeIV[t].size();j++){
      if(satel[i].targetsAtTimeIV[t][j]->solution_value()> 0.5){
        if(satel[i].targetsAtTimeIV[t][j]->solution_value()>0.001 && satel[i].targetsAtTimeIV[t][j]->solution_value()<0.999)printf("WAAAAAAA\n");  
        printf("%d %d %d %d\n",listeGlobPts[satel[i].targetsAtTime[t][j]].lat,listeGlobPts[satel[i].targetsAtTime[t][j]].longi,t,i);
      }
    }
  }
}
      //
//for(auto i:allvars)cout << i->solution_value() << endl;
//cerr << s.DebugString() << endl;

//cout << result->best()<< endl;

  // fin de la lecture d'entree
  ///////////////////////////////////////////////

  /////////////////////////////////////////////////
  return 0;
}
