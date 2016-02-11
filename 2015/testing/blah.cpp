#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
int s,a,b,c,d,e,h,n;
vector<int> r1,r2,c1,c2,size,ham;
vector<vector<int> > G;

int qual(int x) {
  return (c1[x] - c2[x]) * (c1[x] - c2[x])
      +  (r1[x] - r2[x]) * (r1[x] - r2[x]);
}

//avant
bool compare_vect(int x, int y){
    if(size[x] != size[y])
      return size[x]>size[y];
    if(ham[x] != ham[y])
        return ham[x] < ham[y];
    if(r1[x]!=r1[y])
        return r1[x]<r1[y];
        return c1[x]<c1[y];
}

//bool compare_vect(int x, int y){
//    if(G[x].size()==G[y].size()){
//        return size[x]<size[y];
//    } else {
//      return G[x].size()<G[y].size();
//    }
//}

vector<int> ids;


int optimize(vector<bool> &taken, vector<int> &sortie) {
  int score = 0;
  for(unsigned int i=0;i<G.size();i++){
    if(taken[ids[i]])continue;
    //printf("je prends %d\n", ids[i]);
    score += size[ids[i]];
    sortie.push_back(ids[i]);
    for(unsigned int j = 0; j < G[ids[i]].size(); j++){
//    printf("j'exclus %d\n", G[ids[i]][j]);
      taken[G[ids[i]][j]]=true;
    }
  }
  return score;
}

int main(){
  //srand(42);
  scanf("%d",&n);
  G.resize(n);
  for(int x=0;x<n;x++){
  scanf("%d%d%d%d%d%d%d",&s, &a, &b, &c, &d, &h, &e);
  r1.push_back(a);
  c1.push_back(b);
  r2.push_back(c);
  c2.push_back(d);
  ham.push_back(h);
  size.push_back(s);
  for(int y=0;y<e;y++){
    int aa;
    scanf("%d", &aa);
    G[x].push_back(aa);
//    if (r1[x] == 0 && c1[x] == 0 && r2[x] == 1 && c2[x] == 8) {
//      printf("COUCOU %d\n", aa);
//      if (r1[aa] == 0 && c1[aa] == 0 && r2[aa] == 1 && c2[aa] == 9) {
//        printf("YESSS\n");
//      }
//    }
  }
  }
  
  vector<bool> taken(G.size(),false);
  vector<int> sortie;
  ids.resize(G.size());
  for (unsigned i = 0; i < G.size(); i++) {
  ids[i]=i;
  }
  sort(ids.begin(),ids.end(),compare_vect);

  int score = optimize(taken, sortie);

  printf("SCORE %d\n", score);

//  for (unsigned int ncase = 0; ncase < 1000; ncase++) {
//    int val = 0;
//    vector<int> rm;
//
//    for (int i= 0 ; i < 10; i++) {
//      int nn = rand() % G.size();
//      if (taken[ids[nn]]) {
//        taken[ids[nn]] = false;
//        val -= size[ids[nn]];
//        rm.push_back(nn);
//      }
//    }
//    vector<int> myn;
//    val += optimize(taken, myn);
//    if (val < 0) {
//      // baad
//      for (unsigned int j= 0; j < myn.size(); j++) {
//        taken[myn[j]] = false;
//      }
//      for (unsigned int j= 0; j < rm.size(); j++) {
//        taken[ids[rm[j]]] = true;
//      }
//    } else {
//      sortie.clear();
//      for (unsigned int j = 0; j < G.size(); j++) {
//        if (!taken[ids[j]]) {
//          sortie.push_back(ids[j]);
//        }
//      }
//    }
//  }

  // output
  printf("%d\n", (int) sortie.size());
  for (unsigned i = 0; i < sortie.size(); i++) {
    printf("%d %d %d %d\n", r1[sortie[i]], c1[sortie[i]], r2[sortie[i]]-1, c2[sortie[i]]-1);
  }
 
  return 0;  
  
  
}
