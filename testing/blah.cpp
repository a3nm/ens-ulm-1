#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
int s,a,b,c,d,e,n;
vector<int> r1,r2,c1,c2,size;
vector<vector<int> > G;

int qual(int x) {
  return (c1[x] - c2[x]) * (c1[x] - c2[x])
      +  (r1[x] - r2[x]) * (r1[x] - r2[x]);
}

//avant
bool compare_vect(int x, int y){
    if(size[x] == size[y]){
      return G[x].size()<G[y].size();
    } else {
      return size[x]>size[y];
    }
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
  scanf("%d",&n);
  G.resize(n);
  for(int x=0;x<n;x++){
  scanf("%d%d%d%d%d%d",&s, &a, &b, &c, &d, &e);
  r1.push_back(a);
  c1.push_back(b);
  r2.push_back(c);
  c2.push_back(d);
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

//  while (true) {
//
//  }

  // output
  printf("%d\n", (int) sortie.size());
  for (unsigned i = 0; i < sortie.size(); i++) {
    printf("%d %d %d %d\n", r1[sortie[i]], c1[sortie[i]], r2[sortie[i]]-1, c2[sortie[i]]-1);
  }
 
  return 0;  
  
  
}
