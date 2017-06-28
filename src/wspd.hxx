#pragma once

template <typename T>
HyperRect<T>::HyperRect(const std::vector<Point<T>>& s) {
  if (s.size() == 1) {
    /* HyperRect has no meaning if sz == 1 it's a leaf */
    intervals.push_back(std::make_pair(s[0].x, s[0].y));
    intervals.push_back(std::make_pair(s[0].x, s[0].y));
    return;
  }

  const auto min = std::numeric_limits<T>::min();
  const auto max = std::numeric_limits<T>::max();
  std::pair<T, T> range_x(max, min);
  std::pair<T, T> range_y(max, min);

  for (const auto p : s) {
    if (p.x < range_x.first)
      range_x.first = p.x;
    else if (p.x > range_x.second)
      range_x.second = p.x;
    if (p.y < range_y.first)
      range_y.first = p.y;
    else if (p.y > range_y.second)
      range_y.second = p.y;
  }

  intervals.push_back(range_x);
  intervals.push_back(range_y);
}

/* Split rectangle on maximum dimension */
template <typename T>
std::pair<HyperRect<T>, HyperRect<T>> HyperRect<T>::split() const {
  size_t max_idx = 0;
  auto max = std::numeric_limits<T>::min();
  for (size_t i = 0; i < intervals.size(); ++i)
    if (intervals[i].second - intervals[i].first > max)
      max_idx = i;

  auto v1 = std::vector<std::pair<T, T>>();
  auto v2 = std::vector<std::pair<T, T>>();
  auto new_max = (intervals[max_idx].first + intervals[max_idx].second) / 2;

  for (size_t i = 0; i < intervals.size(); ++i)
    if (i != max_idx) {
      v1.push_back(intervals[i]);
      v2.push_back(intervals[i]);
    } else {
      v1.push_back(std::make_pair(intervals[i].first, new_max));
      v2.push_back(std::make_pair(new_max, intervals[i].second));
    }

  return std::make_pair(HyperRect(v1), HyperRect(v2));
}

template <typename T>
bool HyperRect<T>::is_in(const Point<T>& p) const {
  auto dim_x = intervals[0];
  auto dim_y = intervals[1];
  return (p.x >= dim_x.first && p.x <= dim_x.second)
	 && (p.y >= dim_y.first && p.y <= dim_y.second);
}

template <typename T>
Sphere<T>::Sphere(const HyperRect<T>& rect) {
  Point<T> up_left(rect.intervals[0].first, rect.intervals[1].second);
  Point<T> down_right(rect.intervals[1].first, rect.intervals[0].second);

  if (down_right.y - down_right.x == 0)
    down_right.y = 0;
  this->center = Point<T>((up_left.x + down_right.x) / 2,
			  (up_left.y + down_right.y) / 2);
  this->rayon = up_left.euclidean_distance(this->center);
}

template <typename T>
tree_ptr<T> WSPD::split_tree(const std::vector<Point<T>>& s) {
  if (s.size() == 0)
    return tree_ptr<T>(nullptr);

  HyperRect<T> rect(s);	/* box that contains all points */
  if (s.size() == 1)
    return tree_ptr<T>(new Tree<Point<T>, T>(s[0], rect));

  auto pr = rect.split();
  auto rect_left = pr.first;
  auto rect_right = pr.second;
  auto left = std::vector<Point<T>>();
  auto right = std::vector<Point<T>>();

  for (const auto p : s)
    if (rect_left.is_in(p))
      left.push_back(p);
    else
      right.push_back(p);

  auto tree = new Tree<Point<T>, T>(left[0] /* unused */, rect);
  tree->l = split_tree(left);
  tree->r = split_tree(right);

  return tree_ptr<T>(tree);
}
