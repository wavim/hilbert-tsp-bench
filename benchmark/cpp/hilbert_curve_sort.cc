// hilbert_curve_sort.cc
// refer to ..\ts\hilbert-curve-sort.ts for documentation

#include "hilbert_curve_sort.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iterator>

void run_sort_2d(std::vector<std::array<double, 2>> &vec2s, double side);
auto is_base_2d(const std::vector<std::array<double, 2>> &vec2s) -> bool;

constexpr std::array<uint8_t, 4> gray2 = {0b00, 0b01, 0b11, 0b10};

void sort_2d(std::vector<std::array<double, 2>> &vec2s) {
  const auto [min_x_it, max_x_it] = std::ranges::minmax_element(
      vec2s, {}, [](const auto &vec) { return vec[0]; });
  const double min_x = (*min_x_it)[0];
  const double max_x = (*max_x_it)[0];

  const auto [min_y_it, max_y_it] = std::ranges::minmax_element(
      vec2s, {}, [](const auto &vec) { return vec[1]; });
  const double min_y = (*min_y_it)[1];
  const double max_y = (*max_y_it)[1];

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

void run_sort_2d(std::vector<std::array<double, 2>> &vec2s, const double side) {
  if (is_base_2d(vec2s)) {
    return;
  }

  const double mid = side / 2;

  const std::array<std::function<void(std::array<double, 2> &)>, 4> maps{
      [](auto &vec2) { vec2 = {vec2[1], vec2[0]}; },
      [mid](auto &vec2) { vec2 = {vec2[0], vec2[1] - mid}; },
      [side, mid](auto &vec2) { vec2 = {mid - vec2[1], side - vec2[0]}; },
      [mid](auto &vec2) { vec2 = {vec2[0] - mid, vec2[1] - mid}; }};

  std::array quads = {std::vector<std::array<double, 2>>{},
                      std::vector<std::array<double, 2>>{},
                      std::vector<std::array<double, 2>>{},
                      std::vector<std::array<double, 2>>{}};

  for (auto &vec2 : vec2s) {
    const bool bit_x = vec2[0] > mid;
    const bool bit_y = vec2[1] > mid;

    const uint8_t quad =
        (static_cast<int>(bit_x) << 1) + static_cast<int>(bit_y);

    maps[quad](vec2);
    quads[quad].push_back(vec2);
  }

  for (auto &quad_vec2s : quads) {
    run_sort_2d(quad_vec2s, mid);
  }

  std::vector<std::array<double, 2>> result;

  const std::array<std::function<void(std::array<double, 2> &)>, 4> invs{
      [](auto &vec2) { vec2 = {vec2[1], vec2[0]}; },
      [mid](auto &vec2) { vec2 = {vec2[0], vec2[1] + mid}; },
      [side, mid](auto &vec2) { vec2 = {side - vec2[1], mid - vec2[0]}; },
      [mid](auto &vec2) { vec2 = {vec2[0] + mid, vec2[1] + mid}; }};

  for (const auto quad : gray2) {
    for (auto &vec2 : quads[quad]) {
      invs[quad](vec2);
    }

    result.insert(result.end(), make_move_iterator(quads[quad].begin()),
                  make_move_iterator(quads[quad].end()));
  }

  vec2s = result;
}

auto is_base_2d(const std::vector<std::array<double, 2>> &vec2s) -> bool {
  if (vec2s.size() < 2) {
    return true;
  }

  const auto &first = vec2s[0];

  return std::all_of(vec2s.begin() + 1, vec2s.end(),
                     [&first](const auto &vec2) { return vec2 == first; });
}
