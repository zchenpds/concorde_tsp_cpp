extern "C" {
  #include <concorde.h>
}

#include <iostream>
#include <vector>
#include <numeric>

int solve_tsp_concorde (const std::vector<std::vector<int>>& distance, std::vector<int>& tour)
{
  int rval = 0;
  const int ncount = distance.size();
  if (tour.size() != ncount) {
    tour.resize(ncount);
  }
  if (ncount < 5) {
    std::cerr << "Less than 5 nodes.";
    return 1;
  }

  int seed = rand();
  CCrandstate rstate;
  CCutil_sprand(seed, &rstate);
  const int ecount = (ncount * (ncount - 1)) / 2;
  std::vector<int> elist(ecount * 2);
  std::vector<int> elen(ecount);
  int edge = 0;
  int edgeWeight = 0;
  for (int i = 0; i < ncount; i++) {
    for (int j = i + 1; j < ncount; j++) {
      elist[edge++] = i;
      elist[edge++] = j;
      elen[edgeWeight++] = distance[i][j];
    }
  }
  char *name = CCtsp_problabel(" ");
  CCdatagroup dat;
  CCutil_init_datagroup (&dat);
  rval = CCutil_graph2dat_matrix (ncount, ecount, elist.data(), elen.data(), 1, &dat);

  double  optval, *timebound = nullptr;
  int success, foundtour, hit_timebound = 0;
  rval = CCtsp_solve_dat (ncount, &dat, nullptr, tour.data(), NULL, &optval, &success, 
    &foundtour, name, timebound, &hit_timebound, 1, &rstate);

  return rval;
}

int main(int argc, char** argv)
{

  auto print_solution = [](const std::vector<int>& tour, int rval) {
    std::cout << "\nSolution: ";
    if (rval) {
      std::cerr << "Error occurred.\n" << std::endl;
    }
    else {
      for (int i = 0; i <= tour.size(); ++i) {
        std::cout << " " << tour[i % tour.size()];
      }
      std::cout << "\n\n" << std::endl;
    }
  };

  // ERROR: ncount < 5
  {
    std::vector<std::vector<int>> distance{
      {0, 11, 8, 4},
      {0, 0, 7, 2},
      {0, 0, 0, 4},
      {0, 0, 0, 0}
    };
    std::vector<int> tour;
    int rval = solve_tsp_concorde(distance, tour);
    print_solution(tour, rval);
  }

  // ncount: 5
  {
    std::vector<std::vector<int>> distance{
      {0, 11, 8, 4, 9},
      {0, 0, 7, 2, 2},
      {0, 0, 0, 4, 5},
      {0, 0, 0, 0, 6},
      {0, 0, 0, 0, 0}
    };
    std::vector<int> tour;
    int rval = solve_tsp_concorde(distance, tour);
    print_solution(tour, rval);
  }

  // ncount: 13
  {
    std::vector<std::vector<int>> distance{
        {0, 2451, 713, 1018, 1631, 1374, 2408, 213, 2571, 875, 1420, 2145, 1972},
        {2451, 0, 1745, 1524, 831, 1240, 959, 2596, 403, 1589, 1374, 357, 579},
        {713, 1745, 0, 355, 920, 803, 1737, 851, 1858, 262, 940, 1453, 1260},
        {1018, 1524, 355, 0, 700, 862, 1395, 1123, 1584, 466, 1056, 1280, 987},
        {1631, 831, 920, 700, 0, 663, 1021, 1769, 949, 796, 879, 586, 371},
        {1374, 1240, 803, 862, 663, 0, 1681, 1551, 1765, 547, 225, 887, 999},
        {2408, 959, 1737, 1395, 1021, 1681, 0, 2493, 678, 1724, 1891, 1114, 701},
        {213, 2596, 851, 1123, 1769, 1551, 2493, 0, 2699, 1038, 1605, 2300, 2099},
        {2571, 403, 1858, 1584, 949, 1765, 678, 2699, 0, 1744, 1645, 653, 600},
        {875, 1589, 262, 466, 796, 547, 1724, 1038, 1744, 0, 679, 1272, 1162},
        {1420, 1374, 940, 1056, 879, 225, 1891, 1605, 1645, 679, 0, 1017, 1200},
        {2145, 357, 1453, 1280, 586, 887, 1114, 2300, 653, 1272, 1017, 0, 504},
        {1972, 579, 1260, 987, 371, 999, 701, 2099, 600, 1162, 1200, 504, 0},
    };
    std::vector<int> tour;
    int rval = solve_tsp_concorde(distance, tour);
    print_solution(tour, rval);
  }

  return 0;
}