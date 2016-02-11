#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>

// Compile WITHOUT OPTIMS!!

using namespace std;

#define MAXN 1002

typedef pair<int,int> pii;
typedef pair<int,pii> piii;

struct Server{
   int id, z, c;
   Server(int id=0, int z=0, int c=0) : id(id), z(z), c(c) {}
   bool operator< (const Server &s) const{
      if (z*s.c == s.z*c) return z > s.z;
      return z*s.c < s.z*c;
   }
};

struct comp{
   bool operator() (const Server &a, const Server &b){
      if (a.z == b.z)
        return a.c > b.c;
      return a.z > b.z;
   }
};

int main(int argc, char **argv) {
  int R, S, U, P, M;
  vector<Server> serv;
  char grid[MAXN][MAXN]; // row, col
  int capa[MAXN];
  int space[MAXN]; // free space
  int gcapa[MAXN][MAXN]; // row, group
  int fposr[MAXN], fposc[MAXN], fgroup[MAXN];

  scanf("%d", &R);
  scanf("%d", &S);
  scanf("%d", &U);
  scanf("%d", &P);
  scanf("%d", &M);
  for (int i = 0; i < R; i++)
    space[i] = S;
  for (int i = 0; i < U; i++) {
    int r, s;
    scanf("%d", &r);
    scanf("%d", &s);
    grid[r][s] = 1;
    space[r] -= 1;
  }
  for (int i = 0; i < M; i++) {
    int z, c;
    scanf("%d", &z);
    scanf("%d", &c);
    serv.push_back(Server(i, z, c));
  }

  sort(serv.begin(), serv.end());

  // now keep only the servers we will use
  int free = R * S - U;
  int besttcapa = 0;

  int i;
  for (i = 0; i < M; i++) {
    free -= serv[i].c; // should be .z, but???!
    if (free >= 0)
      besttcapa += serv[i].c;
    if (free <= 0)
      break;
  }

  // for i in `seq 500`; do echo -n "$i "; ./a.out $i < ../dc.in  | grep FINAL | cut -d ' ' -f2 ; done | sort -k2,2n
  // USE 470
  sort(serv.begin(), serv.begin() + i + atoi(argv[1]), comp());

  int ftcapa = 0;
  for (int i = 0; i < M; i++) {
    fposr[i] = fposc[i] = fgroup[i] = -1;
  }

  for (int i = 0; i < M; i++) {
    // place server i
    printf("i want to place server %d id %d c %d z %d, c/z %f\n", i, serv[i].id, serv[i].c, serv[i].z, 1. * serv[i].c/serv[i].z);
    // choose the group with lowest guaranteed
    int guar[MAXN];
    for (int j = 0; j < P; j++)
      guar[j] = capa[j];
    for (int j = 0; j < R; j++)
      for (int k = 0; k < P; k++)
        guar[k] = min(guar[k], capa[k] - gcapa[j][k]);
    int mguar = guar[0];
    int idx = 0;
    for (int j = 0; j < P; j++)
      if (guar[j] < mguar) {
        mguar = guar[j];
        idx = j;
      }
    // idx is the group
    // choose where to place server
    vector<pii> v;
    int wherecol = -1, whererow = -1;
    for (int j = 0; j < R; j++) {
      printf("gcapa of row is %d and space is %d\n", gcapa[j][idx], space[j]);
      v.push_back(make_pair<int, int>(MAXN * gcapa[j][idx] + (MAXN-1) - space[j], j));
    }
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
        if (contig == serv[i].z) {
          // ok, can place
          break;
        }
      }
      if (contig == serv[i].z) {
        // do place
        wherecol = k - (serv[i].z - 1);
        whererow = row;
        break;
      }
    }
    if (wherecol >= 0 && whererow >= 0) {
      // yeah, we can place it! update
      capa[idx] += serv[i].c;
      gcapa[whererow][idx] += serv[i].c;
      for (int k = 0; k < serv[i].z; k++)
        grid[whererow][wherecol+k] = 2;
      fposr[serv[i].id] = whererow;
      fposc[serv[i].id] = wherecol;
      fgroup[serv[i].id] = idx;
      space[whererow] -= serv[i].z;
      ftcapa += serv[i].c;
    } else {
      printf("CANNOT PLACE!!\n");
    }
  }

  
  int fguar[MAXN];
  for (int j = 0; j < P; j++)
    fguar[j] = capa[j];
  for (int j = 0; j < R; j++)
    for (int k = 0; k < P; k++)
      fguar[k] = min(fguar[k], capa[k] - gcapa[j][k]);
  int mfguar = fguar[0];
  for (int j = 0; j < P; j++)
    if (fguar[j] < mfguar) {
      mfguar = fguar[j];
    }

  printf("best %d placed %d\n", besttcapa, ftcapa);
  for (int i = 0; i < P; i++)
    printf("guar for %d: %d\n", i, fguar[i]);
  printf("FINAL: %d\n", mfguar);
  
  for (int i = 0; i < R; i++) {
    for (int j = 0; j < S; j++)
      putchar(grid[i][j] == 1? 'X' : (grid[i][j] == 2 ? 'O' : ' '));
    putchar('\n');
  }

  printf("CUT\n");

  // display sol
  for (int i= 0 ; i < M; i++) {
    if (fposr[i] >= 0)
      printf("%d %d %d\n", fposr[i], fposc[i], fgroup[i]);
    else
      printf("x\n");
  }

  return 0;
}

