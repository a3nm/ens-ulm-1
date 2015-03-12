#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

#define MAXN 1002

typedef pair<int,int> pii;

int main() {
  int R, S, U, P, M;
  vector<pii> serv;
  char grid[MAXN][MAXN];
  int capa[MAXN];
  int gcapa[MAXN][MAXN];
  int fposx[MAXN], fposy[MAXN];

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
    serv.push_back(make_pair<int, int>(c/z, z));
  }

  sort(serv.begin(), serv.end());
  reverse(serv.begin(), serv.end());

  for (int i = 0; i < M; i++)
    serv[i].first *= serv.second;

  for (int i = 0; i < M; i++) {
    // place serv[i]
  }

  // display sol
  
  for (int i= 0 ; i < M; i++) {

  }

  return 0;
}

