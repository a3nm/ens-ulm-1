#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

#define MAXN 1002

typedef pair<int,int> pii;
typedef pair<int,pii> piii;

struct Server{
   int id, z, c;
   Server(int id=0, z=0, c=0) : id(id), z(z), c(c) {}
   bool operator< (const Server &s) const{
      return z/c < s.z/s.c;
   }
};

int main() {
  int R, S, U, P, M;
  vector<pii> serv;
  char grid[MAXN][MAXN]; // row, col
  int capa[MAXN];
  int gcapa[MAXN][MAXN]; // row, group
  int fposr[MAXN], fposc[MAXN], fgroup[MAXN];

  scanf("%d", &R);
  scanf("%d", &S);
  scanf("%d", &U);
  scanf("%d", &P);
  scanf("%d", &M);
  for (int i = 0; i < U; i++) {
    int r, s;
    scanf("%d", &r);
    scanf("%d", &s);
    grid[r][s] = 1;
  }
  for (int i = 0; i < M; i++) {
    int z, c;
    scanf("%d", &z);
    scanf("%d", &c);
    serv.push_back(Server(i, z, c));
  }

  sort(serv.begin(), serv.end());
  reverse(serv.begin(), serv.end());

  for (int i = 0; i < M; i++)
    serv[i].first *= serv[i].second;
  
  for (int i = 0; i < M; i++) {
    fposr[i] = fposc[i] = fgroup[i] = -1;
  }

  for (int i = 0; i < M; i++) {
    // place server i
    // choose the group with lowest guaranteed
    int guar[MAXN];
    for (int j = 0; j < P; j++)
      guar[j] = capa[j];
    for (int j = 0; j < R; j++)
      for (int k = 0; k < P; k++)
        guar[k] = min(guar[k], capa[k] - gcapa[j][k]);
    int mguar = guar[0], idx = 0;
    for (int j = 0; j < P; j++)
      if (guar[j] < mguar) {
        mguar = guar[j];
        idx = j;
      }
    // idx is the group
    // choose where to place server
    vector<pii> v;
    int wherecol = -1, whererow = -1;
    for (int j = 0; j < R; j++)
      v.push_back(make_pair<int, int>(gcapa[j][idx], j));
    sort(v.begin(), v.end());
    for (int j = 0; j < R; j++) {
      // try to place in row
      int row = v[j].second;
      int contig = 0;
      int k;
      for (k = 0; k < S; k++) {
        if (!grid[row][k])
          contig++;
        else
          contig = 0;
        if (contig == serv[i].second) {
          // ok, can place
          break;
        }
      }
      if (contig == serv[i].second) {
        // do place
        wherecol = k - (serv[i].second - 1);
        whererow = row;
        break;
      }
    }
    if (wherecol >= 0 && whererow >= 0) {
      // yeah, we can place it! update
      capa[idx] += serv[i].first;
      gcapa[whererow][idx] += serv[i].first;
      for (int k = 0; k < serv[i].second; k++)
        grid[whererow][wherecol] = 1;
      fposr[i] = whererow;
      fposc[i] = wherecol;
      fgroup[i] = idx;
    }
  }

  // display sol
  for (int i= 0 ; i < M; i++) {
    if (fposr[i] >= 0)
      printf("%d %d %d\n", fposr[i], fposc[i], fgroup[i]);
    else
      printf("x\n");
  }

  return 0;
}

