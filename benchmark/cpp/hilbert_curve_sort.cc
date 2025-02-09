// hilbert_curve_sort.cc
// refer to ..\ts\hilbert-curve-sort.ts for documentation

#include "hilbert_curve_sort.h"

void HilbertCurveSort2D(std::vector<std::array<double, 2>> &vec2s);
void RunHilbertCurveSort2D(std::vector<std::array<double, 2>> &vec2s,
                           const double kSide);
bool IsBaseHilbertCurveSort2D(const std::vector<std::array<double, 2>> &kVec2s);

const auto kGrayCode = [](const int n) {
  std::vector<char> result;
  for (char bit = 0; bit < (1 << n); bit++) {
    result.push_back(bit ^ (bit >> 1));
  }
  return result;
};
const std::vector<char> kGrayCode2 = kGrayCode(2);
const std::vector<char> kGrayCode3 = kGrayCode(3);

void HilbertCurveSort2D(std::vector<std::array<double, 2>> &vec2s) {
  double minX = std::numeric_limits<double>::max();
  double maxX = std::numeric_limits<double>::min();
  double minY = std::numeric_limits<double>::max();
  double maxY = std::numeric_limits<double>::min();

  for (const auto &kVec2 : vec2s) {
    minX = std::min(minX, kVec2[0]);
    maxX = std::max(maxX, kVec2[0]);
    minY = std::min(minY, kVec2[1]);
    maxY = std::max(maxY, kVec2[1]);
  }

  const double kSideX = maxX - minX;
  const double kSideY = maxY - minY;
  const double kMaxSide = std::max(kSideX, kSideY);

  const double kScaleX = (kSideX == 0 || kMaxSide == 0) ? 1 : kMaxSide / kSideX;
  const double kScaleY = (kSideY == 0 || kMaxSide == 0) ? 1 : kMaxSide / kSideY;

  for (auto &vec2 : vec2s) {
    vec2[0] = kScaleX * (vec2[0] - minX);
    vec2[1] = kScaleY * (vec2[1] - minY);
  }

  RunHilbertCurveSort2D(vec2s, kMaxSide);

  for (auto &vec2 : vec2s) {
    vec2[0] = vec2[0] / kScaleX + minX;
    vec2[1] = vec2[1] / kScaleY + minY;
  }
};

void RunHilbertCurveSort2D(std::vector<std::array<double, 2>> &vec2s,
                           const double kSide) {
  if (IsBaseHilbertCurveSort2D(vec2s)) return;

  const double kMid = kSide / 2;

  std::array<std::function<void(std::array<double, 2> &)>, 4> maps;
  maps[0b00] = [](std::array<double, 2> &vec2) { vec2 = {vec2[1], vec2[0]}; };
  maps[0b01] = [kMid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0], vec2[1] - kMid};
  };
  maps[0b11] = [kMid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0] - kMid, vec2[1] - kMid};
  };
  maps[0b10] = [kSide, kMid](std::array<double, 2> &vec2) {
    vec2 = {kMid - vec2[1], kSide - vec2[0]};
  };

  std::array<std::function<void(std::array<double, 2> &)>, 4> invMaps;
  invMaps[0b00] = [](std::array<double, 2> &vec2) {
    vec2 = {vec2[1], vec2[0]};
  };
  invMaps[0b01] = [kMid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0], vec2[1] + kMid};
  };
  invMaps[0b11] = [kMid](std::array<double, 2> &vec2) {
    vec2 = {vec2[0] + kMid, vec2[1] + kMid};
  };
  invMaps[0b10] = [kSide, kMid](std::array<double, 2> &vec2) {
    vec2 = {kSide - vec2[1], kMid - vec2[0]};
  };

  std::array<std::vector<std::array<double, 2>>, 4> quads = {
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{},
      std::vector<std::array<double, 2>>{}};

  for (auto &vec2 : vec2s) {
    const bool kBitX = vec2[0] > kMid;
    const bool kBitY = vec2[1] > kMid;
    const char kQuad = (kBitX << 1) + kBitY;

    maps[kQuad](vec2);
    quads[kQuad].push_back(std::move(vec2));
  }

  for (auto &vec2s : quads) RunHilbertCurveSort2D(vec2s, kMid);

  std::vector<std::array<double, 2>> result;

  for (const auto &kQuad : kGrayCode2) {
    for (auto &vec2 : quads[kQuad]) invMaps[kQuad](vec2);

    result.insert(result.end(), make_move_iterator(quads[kQuad].begin()),
                  make_move_iterator(quads[kQuad].end()));
  }

  vec2s = result;
}

bool IsBaseHilbertCurveSort2D(
    const std::vector<std::array<double, 2>> &kVec2s) {
  if (kVec2s.size() < 2) return true;
  const std::set<std::array<double, 2>> kUniqueVec2s(
      make_move_iterator(kVec2s.begin()), make_move_iterator(kVec2s.end()));
  return kUniqueVec2s.size() == 1;
}
