#include <algorithm>
#include <cstdio>
#include <vector>

using namespace std;

// generous bound on all possible inputs and sizes
#define MAXN 1002

struct Server {
  int id, z, c;
  Server(int id = 0, int z = 0, int c = 0) : id(id), z(z), c(c) {}
  // sort servers by decreasing c/z then decreasing z
  bool operator< (const Server &s) const {
    // a/b < c/d iff da < bc and (e, f) < (g, h) with f,h < N iff eN+f < gN+h
    return z*s.c*MAXN + s.z < s.z*c*MAXN + z;
  }
};

// declaring those globally means we don't have to initialize them
int R, S, U, P, M;
vector<Server> serv;
char grid[MAXN][MAXN]; // filled grid cells, indexed by row, col
int capa[MAXN]; // capa[g]: total capacity allocated to group g
int gcapa[MAXN][MAXN]; // gcapa[r][g]: total capacity for group g in row r
vector<pair<int, pair<int, int> > > out; // group, row, col

int main(int argc, char **argv) {
  scanf("%d%d%d%d%d", &R, &S, &U, &P, &M);
  for (int i = 0; i < U; i++) {
    int r, s;
    scanf("%d%d", &r, &s);
    grid[r][s] = 1; // grid cell is occupied
  }
  for (int i = 0; i < M; i++) {
    int z, c;
    scanf("%d%d", &z, &c);
    serv.push_back(Server(i, z, c));
    out.push_back(make_pair(-1, make_pair(-1, -1))); // server is unused
  }

  sort(serv.begin(), serv.end());

  for (int s = 0; s < M; s++) {
    // place server s
    int guar[MAXN]; // guar[g]: guaranteed capa for group g
    for (int g = 0; g < P; g++)
      guar[g] = capa[g];
    for (int r = 0; r < R; r++)
      for (int g = 0; g < P; g++)
        guar[g] = min(guar[g], capa[g] - gcapa[r][g]); // capa if killing row r
    int mguar = guar[0], mg = 0; // min and argmin of guar over all groups
    for (int g = 0; g < P; g++)
      if (guar[g] < mguar) {
        mguar = guar[g];
        mg = g;
      }
    // mg is the group with lowest guar, assign s to mg if we can place it
    vector<pair<int, int> > rows;
    for (int r = 0; r < R; r++)
      rows.push_back(make_pair<int, int>(gcapa[r][mg], r));
    sort(rows.begin(), rows.end()); // sort rows by inc capa for group mg
    int fr = -1, fc = -1; // row and col to place s
    for (int rr = 0; rr < R; rr++) {
      int contig = 0; // count contiguous free cells
      fr = rows[rr].second;
      for (int c = 0; c < S; c++) {
        if (!grid[fr][c])
          contig++;
        else
          contig = 0;
        if (contig == serv[s].z) {
          fc = c - (serv[s].z - 1); // we can place s at (fr, fc)
          rr = R; // break out of both loops
          break;
        }
      }
    }
    if (fc >= 0) {
      // assign s to mg and place it at (fr, fc)
      capa[mg] += serv[s].c;
      gcapa[fr][mg] += serv[s].c;
      for (int d = 0; d < serv[s].z; d++)
        grid[fr][fc+d] = 1; // squares occupied by s
      out[serv[s].id] = make_pair(mg, make_pair(fr, fc));
    }
  }

  for (int i= 0 ; i < M; i++)
    if (out[i].first >= 0)
      printf("%d %d %d\n", out[i].first,
          out[i].second.first, out[i].second.second);
    else
      printf("x\n");

  return 0;
}

