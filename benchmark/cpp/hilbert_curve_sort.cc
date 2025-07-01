// hilbert_curve_sort.cc
// refer to ..\ts\hilbert-curve-sort.ts for documentation

#include "hilbert_curve_sort.h"

#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <set>

void run_sort_2d(std::vector<std::array<double, 2>> &vec2s, double side);
auto is_base_2d(const std::vector<std::array<double, 2>> &vec2s) -> bool;

void sort_2d(std::vector<std::array<double, 2>> &vec2s) {
  double min_x = std::numeric_limits<double>::max();
  double max_x = std::numeric_limits<double>::lowest();
  double min_y = std::numeric_limits<double>::max();
  double max_y = std::numeric_limits<double>::lowest();

  for (const auto &vec2 : vec2s) {
    min_x = std::min(min_x, vec2[0]);
    max_x = std::max(max_x, vec2[0]);
    min_y = std::min(min_y, vec2[1]);
    max_y = std::max(max_y, vec2[1]);
  }

  const double side_x = max_x - min_x;
  const double side_y = max_y - min_y;
  const double bound = std::max(side_x, side_y);

  const double scale_x = (side_x == 0 || bound == 0) ? 1 : bound / side_x;
  const double scale_y = (side_y == 0 || bound == 0) ? 1 : bound / side_y;

  for (auto &vec2 : vec2s) {
    vec2[0] = scale_x * (vec2[0] - min_x);
    vec2[1] = scale_y * (vec2[1] - min_y);
  }

  run_sort_2d(vec2s, bound);

  for (auto &vec2 : vec2s) {
    vec2[0] = vec2[0] / scale_x + min_x;
    vec2[1] = vec2[1] / scale_y + min_y;
  }
};

auto gray(const uint8_t n) {
  std::vector<uint8_t> code;

  for (uint8_t bit = 0; bit < (1 << n); bit++) {
    code.push_back(bit ^ (bit >> 1));
  }

  return code;
};

void run_sort_2d(std::vector<std::array<double, 2>> &vec2s, const double side) {
  if (is_base_2d(vec2s)) {
    return;
  }

  const double mid = side / 2;

  std::array<std::function<void(std::array<double, 2> &)>, 4> maps;

  maps[0b00] = [](std::array<double, 2> &vec2) {
    vec2 = {vec2[1], vec2[0]};
  };
  maps[0b01] = [mid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0], vec2[1] - mid};
  };
  maps[0b11] = [mid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0] - mid, vec2[1] - mid};
  };
  maps[0b10] = [side, mid](std::array<double, 2> &vec2) {
    vec2 = {mid - vec2[1], side - vec2[0]};
  };

  std::array<std::function<void(std::array<double, 2> &)>, 4> invs;

  invs[0b00] = [](std::array<double, 2> &vec2) {
    vec2 = {vec2[1], vec2[0]};
  };
  invs[0b01] = [mid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0], vec2[1] + mid};
  };
  invs[0b11] = [mid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0] + mid, vec2[1] + mid};
  };
  invs[0b10] = [side, mid](std::array<double, 2> &vec2) {
    vec2 = {side - vec2[1], mid - vec2[0]};
  };

  std::array quads = {
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{}
  };

  for (auto &vec2 : vec2s) {
    const bool bit_x = vec2[0] > mid;
    const bool bit_y = vec2[1] > mid;

    const uint8_t quad = (static_cast<int>(bit_x) << 1) +
                         static_cast<int>(bit_y);

    maps[quad](vec2);
    quads[quad].push_back(vec2);
  }

  for (auto &quad_vec2s : quads) {
    run_sort_2d(quad_vec2s, mid);
  }

  std::vector<std::array<double, 2>> result;

  for (const auto &quad : gray(2)) {
    for (auto &vec2 : quads[quad]) {
      invs[quad](vec2);
    }

    result.insert(result.end(),
                  make_move_iterator(quads[quad].begin()),
                  make_move_iterator(quads[quad].end()));
  }

  vec2s = result;
}

auto is_base_2d(const std::vector<std::array<double, 2>> &vec2s) -> bool {
  if (vec2s.size() < 2) {
    return true;
  }

  const std::set unique_vec2s(
      make_move_iterator(vec2s.begin()), make_move_iterator(vec2s.end()));

  return unique_vec2s.size() == 1;
}