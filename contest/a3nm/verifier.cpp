#include <vector>
#include <cstdio>
#include <algorithm>
#include <map>
#include <time.h>

using namespace std;

#define MAXN 302
#define MAXL 2252
#define NALT 10
#define MAXT 402

int R, C, A;
int L, V, B, T;
int rs, cs;

struct Pt{
	int r, c;
	Pt(int r=0, int c=0) : r(r), c(c) {}
};

vector<int> coverage[MAXN][MAXN];

Pt dest[NALT][MAXN][MAXN]; // where does the wind take us: -1 -1 = out

Pt target[MAXL];

bool covered[MAXL][MAXT]; // does someone cover target l at time t

int columndist(int c1, int c2) {
	return min(abs(c1 - c2), C - abs(c1 - c2));
}

int iscovered(int l, int r, int c) {
	// does a loon at r c cover l?
	int u = target[l].r, v = target[l].c;
	return ((r - u) * (r - u) + columndist(c, v) * columndist(c, v) <= V*V);
}


void calccoverage() {
	// compute for each square which targets are covered
	for (int r = 0; r < R; r++)
		for (int c = 0; c < C; c++)
			for (int l = 0; l < L; l++)
				if (iscovered(l, r, c))
					coverage[r][c].push_back(l);
}

Pt position[MAXL];
int alt[MAXL];

int main(int argc, char **argv) {
	FILE *input = fopen("final_round.in", "r");
	fscanf(input, "%d%d%d", &R, &C, &A);
	fscanf(input, "%d%d%d%d", &L, &V, &B, &T);
	fscanf(input, "%d%d", &rs, &cs);
	for (int i = 0; i < L; i++) {
		int r, c;
		fscanf(input, "%d%d", &r, &c);
		target[i] = Pt(r, c);
	}
	for (int a = 0; a < A; a++)
		for (int r = 0; r < R; r++)
			for (int c = 0; c < C; c++) {
				int dr, dc;
				fscanf(input, "%d%d", &dr, &dc);
				int destr, destc;
				destr = r + dr;
				destc = (c + dc + C) % C;
				if (destr < 0 || destr >= R)
					destr = destc = -1; // out
				dest[a][r][c] = Pt(destr, destc);
			}
	if (argc == 1) {
		printf("Pas de sortie.\n");
		return 0;
	}
	FILE *out = fopen(argv[1], "r");
	int score = 0;
	for (int loon = 0; loon < B; loon++)
		position[loon] = Pt(rs, cs);
	for (int iTour = 0; iTour < T; iTour++) {
		for (int loon = 0; loon < B; loon++) {
			int delta;
			fscanf(out, "%d", delta);
			alt[loon] += delta;
			position[loon] = dest[alt[loon]][position[loon].r][position[loon].c];
		}	
		for (int target = 0; target < L; target++) {
			bool cov = false;
			for (int loon = 0; loon < B; loon++)
				if (iscovered(target, position[loon].r, position[loon].c)) {
					cov = true;
					break;
				}
			if (cov)
				score++;
		}
	}
	printf("%d\n", score);
	return 0;
}

