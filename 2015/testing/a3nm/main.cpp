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
vector<int> ham;

int main(int argc, char **argv) {
  scanf("%d%d%d%d", &R, &C, &H, &S);
  
  for (int i = 0; i < R; i++)
    scanf("%s", pizz[i]);

  for (int r = 0; r < R; r++)
    for (int c = 0; c < C; c++)
      for (int w = 1; w <= S; w++)
        for (int h = 1; w * h <= S; h++) {
          if (c + w > C || r + h > R)
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
          ham.push_back(nham);
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
//      if (r1[i] == 0 && c1[i] == 0 && r2[i] == 1 && c2[i] == 8) {
//        if (r1[j] == 0 && c1[j] == 0 && r2[j] == 1 && c2[j] == 9) {
//          printf("YESSS %d %d\n", i, j);
//        }
//      }
      G[i].push_back(j);
      G[j].push_back(i);
    }
  }

  printf("%d\n", (int) G.size());
  for (unsigned i = 0; i < G.size(); i++) {
    printf("%d %d %d %d %d %d\n", size[i], r1[i], c1[i], r2[i], c2[i], ham[i]);
    printf("%d\n", (int) G[i].size());
    for (unsigned j = 0; j < G[i].size(); j++) {
      printf("%d\n", G[i][j]);
    }
  }
 
  return 0;
}

