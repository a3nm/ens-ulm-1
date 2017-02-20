#include <cstdio>
#include <vector>
#include <map>
#include <set>

using namespace std;
bool taken[1000][1000];
int tot = 0;

class Point{
    public:
    int x,y;
    Point(int x,int y):x(x),y(y){}
    bool operator<(const Point o) const{
    return (x==o.x) ? y<o.y : x<o.x;
    }
};


class Rect{
    public:
    int r1,c1,r2,c2;
    Rect(int r1, int c1,int r2,int c2):r1(r1),r2(r2),c1(c1),c2(c2){}
    void print(){
        printf("%d %d %d %d\n",r1,c1,r2,c2);
    }
    bool operator<(const Rect o) const{
        return (r1==o.r1)?(c1==o.c1)?(r2==o.r2)?(c2==o.c2)?false:(c2<o.c2):(r2<o.r2):(c1<o.c1):(r1<o.r1);
    }
    
    bool canTake(){
        for(int k1=r1;k1<=r2;k1++)
        for(int k2=c1;k2<=c2;k2++)
           if(taken[k1][k2]) return false;
        return true;    
    }

    void take(){
        for(int k1=r1;k1<=r2;k1++)
        for(int k2=c1;k2<=c2;k2++)
           taken[k1][k2] = true;
        print();
        tot += (r2-r1+1)*(c2-c1+1);
    }
    
};


int r,c,l,h;
char tab[1000][1000];
int validwidth[] = {1,2,3,4,6,12,1,13,1,2,7,14};
int validheight[] = {12,6,4,3,2,1,13,1,14,7,2,1};
vector<Rect> rectAtPoint[1000][1000];
vector<Rect> allRects;
map<Rect, vector<Rect> > conflicts;
vector<vector<Point> >pointsParCtr(200);

bool isvalid(int r1,int c1,int r2,int c2){
    int nt=0;
    int nm=0;
    if(r2>=r || c2>=c) return false;
    for(int i=r1;i<=r2;i++)for(int j=c1;j<=c2;j++){
        if(tab[i][j]=='T')nt++;
        if(tab[i][j]=='M')nm++;
    }
    if(nt<6 || nm<6) return false;
}


int main(){
    scanf("%d%d%d%d",&r,&c,&l,&h);//h is max #trucs
    for(int i=0;i<r;i++)
        scanf("%s\n",tab[i]);
    for(int i=0;i<r;i++)for(int j=0;j<c;j++)
    for(int piece=0;piece<12;piece++){
        if(isvalid(i,j,i+validheight[piece]-1,j+validwidth[piece]-1)){
            Rect a(i,j,i+validheight[piece]-1,j+validwidth[piece]-1);
            allRects.push_back(a);
            for(int k1=i;k1<=i+validheight[piece]-1;k1++)
            for(int k2=j;k2<=j+validwidth[piece]-1;k2++)
                rectAtPoint[k1][k2].push_back(a);
        }
    }
    

    for(int i=0;i<r;i++)for(int j=0;j<c;j++){
        Point a(i,j);
        pointsParCtr[rectAtPoint[i][j].size()].push_back(a);
        //printf("%d\n",rectAtPoint[i][j].size());
    }
    
    for (int ctr = 1; ctr < 124; ctr++){
        for(Point a:pointsParCtr[ctr]){
            for(Rect re:rectAtPoint[a.x][a.y]){
                if(re.canTake())
                    re.take();
            }
        }
    }
    printf("%d\n",tot);
    return 0;
}
