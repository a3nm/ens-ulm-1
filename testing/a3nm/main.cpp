#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXN 200

int R, C, H, S;
char pizz[MAXN][MAXN]; // row, col

vector<int> size;
vector<vector<int> > G;
vector<int> r1;
vector<int> c1;
vector<int> r2;
vector<int> c2;

//avant
bool compare_vect(int x, int y){
    if(G[x].size()==G[y].size()){
        return size[x]<size[y];
    } else {
      return G[x].size()<G[y].size();
    }
}
 

int main(int argc, char **argv) {
  scanf("%d%d%d%d", &R, &C, &H, &S);
  
  for (int i = 0; i < R; i++)
    scanf("%s", pizz[i]);

  for (int r = 0; r < R; r++)
    for (int c = 0; c < C; c++)
      for (int w = 1; w <= S; w++)
        for (int h = 1; w * h <= S; h++) {
          if (c + w >= C || r + h >= R)
            continue; // out of bounds
          // count
          int nham = 0;
          for (int dr = 0; dr < h; dr++)
            for (int dc = 0; dc < w; dc++)
              nham += (pizz[r + dr][c + dc] == 'H' ? 1 : 0);
          if (nham < H)
            continue; // not enough ham
          vector<int> nv;
          G.push_back(nv);
          r1.push_back(r);
          r2.push_back(r + h);
          c1.push_back(c);
          c2.push_back(c + w);
          size.push_back(h * w);
        }

  // compute adj
  for (unsigned int i = 0; i < G.size(); i++) {
    G[i].push_back(i);
    for (unsigned int j = i+1; j < G.size(); j++) {
      if (r2[i] <= r1[j] || r1[i] >= r2[j]
          || c2[i] <= c1[j] || c1[i] >= c2[j])
        continue; // not overlap
      if (r2[j] <= r1[i] || r1[j] >= r2[i]
          || c2[j] <= c1[i] || c1[j] >= c2[i])
        continue; // not overlap
      G[i].push_back(j);
      G[j].push_back(i);
    }
  }

  // output
  for (unsigned i = 0; i < G.size(); i++) {
    printf("slice %d: %d %d - %d %d: size %d\n", i, r1[i], c1[i], r2[i], c2[i],
        size[i]);
    for (unsigned j = 0; j < G[i].size(); j++) {
      printf("intersects %d: %d %d - %d %d\n", j, r1[j], c1[j], r2[j], c2[j]);
    }
  }

  //pendant
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
      taken[j]=true;
    }
  }
  
  // output
  printf("%d\n", (int) sortie.size());
  for (unsigned i = 0; i < sortie.size(); i++) {
    printf("%d %d %d %d\n", r1[i], c1[i], r2[i], c2[i]);
  }
 
  return 0;
}

