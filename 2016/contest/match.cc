typedef  int ui ;

class louis
{
  Point sat_pos[42];
  ui sat_time[42];

  vector<vector< ui> > graph ;
  vector<int> assoc_pt ;
  vector<int> assoc_sat ;
  vector<bool> seen ;

  bool sub_find_path(const ui sat )
  {
    if(seen[sat])
      return false ;
    seen[sat] = true ;
    for(auto v : graph[sat])
      {
	if(assoc_pt[v]==-1)
	  {
	    assoc_pt[v]=sat;
	    assoc_sat[sat]=v;
	    return true;
	  }
	//else
	if(sub_find_path(assoc_pt[v]))
	  {
	    assoc_pt[v]=sat;
	    assoc_sat[sat]=v;
	    return true ;
	  }
      }
    return false;
  }

  void find_path(const ui sat)
  {
    seen.clear();
    seen.resize(graph.size(),false);
    sub_find_path(sat);
  }

  void match()
  {
    assoc_pt.clear();
    assoc_pt.resize(nb_pts,-1);
    assoc_sat.clear();
    assoc_sat.resize(nbSat,-1);
    for(size_t sat = 0 ; sat < graph.size() ; sat++ )
      find_path(sat);
    return ;
  }

  map<Point,ui> cur_pts ;
  vector<vector<ui> > objs ;
  ui nb_pts = 0 ;

  ui tr(const ui id)
  {
    const Point n = listeGlobPts[id];
    if(cur_pts.find(n) == cur_pts.end())
      {
	cur_pts[n]=nb_pts++;
	objs.push_back(vector<ui>());
      }
    const ui pt_id = cur_pts[n] ;
    objs[pt_id].push_back(id);
    return pt_id;
  }

public:

  vector<bool> done ;
  vector<pair<Point,pair<int,int> > > res;
  void sol()
  {
    for(ui sat = 0 ; sat < nbSat ; sat++)
      {
	sat_time[sat]=0;
	sat_pos[sat]=Point();
      }
    done.resize(nbPtsTt,false);
    for(ui turn = 0 ; turn < nbTours ; turn++)
      {
	objs.clear();
	cur_pts.clear();
	nb_pts = 0;
	graph.clear();
	for(ui sat = 0 ; sat < nbSat ; sat++)
	  {
	    graph.push_back(vector<ui>());
	    for(const int pt : listeAccessible(sat,sat_time[sat],turn,sat_pos[sat]))
	      if(!done[pt])
		{
		  graph[sat].push_back(tr(pt));
		}
	  }
	match();
	for(ui sat = 0 ; sat < nbSat ; sat++)
	  if(assoc_sat[sat] != -1)
	    {
	      for(int v : objs[assoc_sat[sat]])
		done[v] = true ;
	      const int pt_id = objs[assoc_sat[sat]][0] ;
	      res.push_back( make_pair(listeGlobPts[pt_id],make_pair(turn,sat))) ;
	      sat_time[sat] = turn ;
	      sat_pos[sat] = listeGlobPts[pt_id] ;
	    }
      }
    
    printf("%llu\n",res.size());
    for(auto r : res)
      {
	printf("%d %d %d %d\n",r.first.lat,r.first.longi,r.second.first,r.second.second);
      }
  }
} ;
