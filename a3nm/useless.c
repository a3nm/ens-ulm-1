
  int did = 1;
  while (did) {
    did = 0;
    int fguar[MAXN];
    for (int j = 0; j < P; j++)
      fguar[j] = capa[j];
    for (int j = 0; j < R; j++)
      for (int k = 0; k < P; k++)
        fguar[k] = min(fguar[k], capa[k] - gcapa[j][k]);
    int mfguar = fguar[0];
    int g = 0;
    for (int j = 0; j < P; j++)
      if (fguar[j] < mfguar) {
        mfguar = fguar[j];
        g = j;
      }
    printf("guar is %d, optimize worst group: %d\n", mfguar, g);
    // optimize g
    vector<pii> vv;
    for (int r = 0; r < R; r++)
      vv.push_back(make_pair(gcapa[r][g], r));
    sort(vv.begin(), vv.end());
    for (int rh = R - 1; rh >= 0; rh--) {
    if (vv[rh].first < vv[R-1].first)
      break;
      for (int rl = 0; rl < rh; rl++) {
        //printf("swap in rl %d rh %d\n", rl, rh);
        vector<int> sl, sh;
        for (int s = 0; s < M; s++) {
          if (fgroup[s] != g)
            continue;
          //printf("aoeu servera %d is in %d\n", s, fposr[s]);
          if (fposr[s] == vv[rh].second)
            sh.push_back(s);
          if (fposr[s] == vv[rl].second)
            sl.push_back(s);
        }
        int tcapal = 0;
        for (unsigned int ssl = 0; ssl < sl.size(); ssl++)
          tcapal += oserv[sl[ssl]].c;
        printf("calculated capa of l as %d and should be %d\n", tcapal, gcapa[vv[rl].second][g]);
        //printf("aoeu i have %d %d servers\n", sl.size(), sh.size());
        for (unsigned int ssl = 0; ssl < sl.size(); ssl++) {
          for (unsigned int ssh = 0; ssh < sh.size(); ssh++) {
            int rsh = sh[ssh];
            int rsl = sl[ssl];
            //printf("aoeu soeu wap consider rsl %d rsh %d\n", rsl, rsh);
            // try to swap rsl and rsh
            if (oserv[rsl].z != oserv[rsh].z)
              continue;
            if (oserv[rsh].c <= oserv[rsl].c)
              continue;
            if (gcapa[vv[rl].second][g] + oserv[rsh].c - oserv[rsl].c >= gcapa[vv[rh].second][g])
              continue;
            // can swap
            did = 1;
            printf("swap servers %d %d\n", rsl, rsh);
            printf("swap rows %d %d\n", vv[rl].second, vv[rh].second);
            printf("capas on rows are %d %d\n", gcapa[rl][g], gcapa[rh][g]);
            printf("capas on rows will be %d %d\n", gcapa[rl][g] + oserv[rsh].c - oserv[rsh].c,
                gcapa[rh][g] + oserv[rsl].c - oserv[rsh].c);
            printf("their groups are: %d %d\n", fgroup[rsl], fgroup[rsh]);
            printf("their capas are: %d %d\n", oserv[rsl].c, oserv[rsh].c);
            printf("their sizes are: %d %d\n", oserv[rsl].z, oserv[rsh].z);
            // do swap
            int tmp;
            tmp = fposr[rsh];
            fposr[rsh] = fposr[rsl];
            fposr[rsl] = tmp;
            tmp = fposc[rsh];
            fposc[rsh] = fposc[rsl];
            fposc[rsl] = tmp;
            gcapa[rh][g] += oserv[rsl].c - oserv[rsh].c;
            gcapa[rl][g] += oserv[rsh].c - oserv[rsl].c;
            break;
          }
          if (did)
            break;
        }
        if (did)
          break;
      }
      if (did)
        break;
    }
  }
