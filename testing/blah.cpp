#include <cstdio>
#include <vector>
#include <algorithm>

using namespace std;
int s,a,b,c,d,e,n;
vector<int> r1,r2,c1,c2,size;
vector<vector<int> > G;

//avant
bool compare_vect(int x, int y){
    if(G[x].size()==G[y].size()){
        return size[x]<size[y];
    } else {
      return G[x].size()<G[y].size();
    }
}


int main(){
  scanf("%d",&n);
  G.resize(n);
  for(int x=0;x<n;x++){
  scanf("%d%d%d%d%d",&s, &a, &b, &c, &d, &e);
  r1.push_back(a);
  r2.push_back(b);
  c1.push_back(c);
  c2.push_back(d);
  size.push_back(s);
  for(int y=0;y<e;y++){
    scanf("%d", &a);
    G[x].push_back(y);
  }
  }
  
  vector<int> ids(G.size());
  for (unsigned i = 0; i < G.size(); i++) {
  ids[i]=i;
  }
  
  sort(ids.begin(),ids.end(),compare_vect);
   
  vector<bool> taken(G.size(),false);
  vector<int> sortie;
   
  vector<bool> choix;
  for(unsigned int i=0;i<G.size();i++){
    if(taken[ids[i]])continue;
    sortie.push_back(ids[i]);
    for(unsigned int j = 0; j < G[ids[i]].size(); j++){
      taken[G[ids[i]][j]]=true;
    }
  }
  
  // output
  printf("%d\n", (int) sortie.size());
  for (unsigned i = 0; i < sortie.size(); i++) {
    printf("%d %d %d %d\n", r1[i], c1[i], r2[i], c2[i]);
  }
 
  return 0;  
  
  
}
