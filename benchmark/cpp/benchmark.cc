#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hilbert_curve_sort.cc"

double calcDist(const std::array<double, 2>& point1,
                const std::array<double, 2>& point2) {
  double x1 = point1[0];
  double y1 = point1[1];
  double x2 = point2[0];
  double y2 = point2[1];
  return ceil(sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)));
}

int main(void) {
  std::ifstream pla85900("../../pla85900.tsp");
  std::string line;
  std::vector<std::array<double, 2>> points;
  while (getline(pla85900, line)) {
    std::istringstream lineStream(line);
    int index;
    double x, y;
    if (lineStream >> index >> x >> y) points.push_back({x, y});
  }
  pla85900.close();

  auto start = std::chrono::high_resolution_clock::now();
  HilbertCurveSort2D(points);
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  double dist = 0;
  for (int i = 0; i < points.size(); i++) {
    dist += calcDist(points[i], points[(i + 1) % points.size()]);
  }

  std::cout << "Sorted in: " << duration.count() << "ms. Dist: " << (int)dist
            << ", " << dist / 142382641 << "x optimal.";
  // Sorted in: (~100ms). Dist: 188465250, 1.32365x optimal.
  // 11th Gen Intel(R) Core(TM) i5-11320H
}
