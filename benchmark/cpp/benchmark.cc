#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hilbert_curve_sort.h"

auto calc_dist(const std::array<double, 2> &point1,
               const std::array<double, 2> &point2) -> double {
  const double x_1 = point1[0];
  const double y_1 = point1[1];
  const double x_2 = point2[0];
  const double y_2 = point2[1];

  return ceil(sqrt(((x_1 - x_2) * (x_1 - x_2)) + ((y_1 - y_2) * (y_1 - y_2))));
}

auto main() -> int {
  std::vector<std::array<double, 2>> points;

  std::ifstream pla85900("../../../pla85900.tsp");
  std::string line;

  while (std::getline(pla85900, line)) {
    std::istringstream lineStream(line);

    double x_coord;
    double y_coord;

    if (int index; lineStream >> index >> x_coord >> y_coord) {
      points.push_back({x_coord, y_coord});
    }
  }

  pla85900.close();

  auto start = std::chrono::high_resolution_clock::now();

  sort_2d(points);

  auto stop = std::chrono::high_resolution_clock::now();

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start)
          .count();

  double dist = 0;

  for (size_t i = 0; i < points.size() - 1; i++) {
    dist += calc_dist(points[i], points[i + 1]);
  }

  dist += calc_dist(points[points.size() - 1], points[0]);

  std::cout << "time: " << duration << ".\n"
            << "dist: " << static_cast<unsigned>(dist) << ".\n"
            << "mult: " << std::fixed << dist / 142382641 << ".\n";

  // time: 35.
  // dist: 188465250.
  // mult: 1.323653.
  // 11th Gen Intel(R) Core(TM) i5-11320H @ 3.20GHz
}