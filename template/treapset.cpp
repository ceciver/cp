// https://codeforces.com/blog/entry/46507

#ifndef _TREAP_H
#define _TREAP_H

#include <algorithm> // std::sort
#include <vector> // std::vector
#include <utility> // std::pair
#include <random> // std::mt19937_64
#include <chrono>

template <typename T>
class treap {
 private:
  struct node;
  using pnode = struct node *;

  std::mt19937_64 * rng_;
  bool rng_owner_;
  bool is_sorted_;
  bool stop_; // for priority regeneration
  pnode root_;

  using priority_t = std::mt19937_64::result_type;

  priority_t next_priority () {
    priority_t priority = (*rng_)();
    return priority;
  }

  void regenerate_priorities_recursive (std::vector <int> & new_priors, pnode & t, int l, int r) {
    if (!t)
      return;
    t->priority = new_priors[r - 1];
    regenerate_priorities_recursive(new_priors, t->l, l, l + cnt(t->l));
    regenerate_priorities_recursive(new_priors, t->r, l + cnt(t->l), r - 1);
  }

  void regenerate_priorities () {
    int sz = size();
    std::vector <int> new_priors(sz);
    for (int i = 0; i < sz; i++)
      new_priors[i] = next_priority();
    std::sort(new_priors.begin(), new_priors.end());
    for (int i = 0; i < sz; i++)
      new_priors[i] += i;
    regenerate_priorities_recursive(new_priors, root_, 0, sz);
  }

  struct node {
    priority_t priority;
    int cnt, rev;
    T key, add, fsum;
    pnode l, r;

    node (T x, priority_t p) {
      add = 0 * x;
      key = fsum = x;
      cnt = 1;
      rev = 0;
      l = r = nullptr;
      priority = p;
    }
  };

  int cnt (pnode t) {
    return t ? t->cnt : 0;
  }

  void upd_cnt (pnode t) {
    if (t)
      t->cnt = cnt(t->l) + cnt(t->r) + 1;
  }

  void upd_sum (pnode t) {
    if (t) {
      t->fsum = t->key;
      if (t->l)
        t->fsum += t->l->fsum;
      if (t->r)
        t->fsum += t->r->fsum;
    }
  }

  void update (pnode t, T add, int rev) {
    if (!t)
      return;
    t->add = t->add + add;
    t->rev = t->rev ^ rev;
    t->key = t->key + add;
    t->fsum = t->fsum + cnt(t) * add;
  }

  void push (pnode t) {
    if (!t || (t->add == 0 * T() && t->rev == 0))
      return;
    update(t->l, t->add, t->rev);
    update(t->r, t->add, t->rev);
    if (t->rev)
      std::swap(t->l, t->r);
    t->add = 0 * T();
    t->rev = 0;
  }

  void merge (pnode & t, pnode l, pnode r) {
    push(l);
    push(r);
    if (!l || !r)
      t = l ? l : r;
    else if (l->priority > r->priority) {
      merge(l->r, l->r, r);
      t = l;
    }
    else {
      merge(r->l, l, r->l);
      t = r;
    }
    upd_cnt(t);
    upd_sum(t);
  }

  void split (pnode t, pnode & l, pnode & r, int index) { // split at position
    if (!t) {
      l = r = 0;
      return;
    }
    push(t);
    if (index <= cnt(t->l)) {
      split(t->l, l, t->l, index);
      r = t;
    }
    else {
      split(t->r, t->r, r, index - 1 - cnt(t->l));
      l = t;
    }
    upd_cnt(t);
    upd_sum(t);
  }

  void split_at (pnode t, pnode & l, pnode & r, T & key, bool & eq) { // split by key
    if (!t) {
      l = r = 0;
      return;
    }
    push(t);
    if (key == t->key) {
      eq = true;
      return;
    }
    if (key < t->key) {
      split_at(t->l, l, t->l, key, eq);
      if (!eq)
        r = t;
    }
    else {
      split_at(t->r, t->r, r, key, eq);
      if (!eq)
        l = t;
    }
    if (!eq)
      upd_cnt(t);
    upd_sum(t);
  }

  void insert (pnode & t, pnode it, int index) { // insert at position
    push(t);
    if (!t)
      t = it;
    else if (it->priority == t->priority) {
      stop_ = true;
      regenerate_priorities();
    }
    else if (it->priority > t->priority) {
      split(t, it->l, it->r, index);
      t = it;
    }
    else if (index <= cnt(t->l))
      insert(t->l, it, index);
    else
      insert(t->r, it, index - cnt(t->l) - 1);
    if (stop_)
      return;
    upd_cnt(t);
    upd_sum(t);
  }

  void insert_at (pnode & t, pnode it, bool & eq) { // insert by key
    push(t);
    if (!t)
      t = it;
    else if (it->key == t->key) {
      eq = true;
      return;
    }
    else if (it->priority == t->priority) {
      stop_ = true;
      regenerate_priorities();
    }
    else if (it->priority > t->priority) {
      split_at(t, it->l, it->r, it->key, eq);
      if (!eq)
        t = it;
    }
    else if (it->key < t->key)
      insert_at(t->l, it, eq);
    else
      insert_at(t->r, it, eq);
    if (stop_)
      return;
    if (!eq)
      upd_cnt(t);
    upd_sum(t);
  }

  void erase (pnode & t, int index) {
    push(t);
    if (cnt(t->l) == index) {
      pnode l = t->l, r = t->r;
      delete t;
      t = nullptr;
      merge(t, l, r);
    }
    else if (index < cnt(t->l))
      erase(t->l, index);
    else
      erase(t->r, index - cnt(t->l) - 1);
    upd_cnt(t);
    upd_sum(t);
  }

  void erase_at (pnode & t, T key, bool & found) {
    push(t);
    if (key == t->key) {
      pnode l = t->l, r = t->r;
      delete t;
      t = nullptr;
      merge(t, l, r);
      found = true;
    }
    else if (key < t->key)
      erase_at(t->l, key, found);
    else
      erase_at(t->r, key, found);
    upd_cnt(t);
    upd_sum(t);
  }

  T get (pnode t, int index) {
    push(t);
    if (index < cnt(t->l))
      return get(t->l, index);
    else if (index > cnt(t->l))
      return get(t->r, index - cnt(t->l) - 1);
    return t->key;
  }

  int find (pnode t, T key) {
    push(t);
    if (!t || key == t->key)
      return cnt(t->l);
    if (key < t->key)
      return get(t->l, key);
    else
      return get(t->r, key) + 1 + cnt(t->l);
  }

  std::pair <T, int> lower_bound (pnode t, T key, int index) {
    push(t);
    if (!t)
      return {T(), size()};
    if (key == t->key)
      return {key, index + cnt(t->l)};
    if (key < t->key) {
      std::pair <T, int> ret = lower_bound(t->l, key, index);
      if (ret.second == size())
        ret = {t->key, index + cnt(t->l)};
      return ret;
    }
    return lower_bound(t->r, key, index + 1 + cnt(t->l));
  }

  std::pair <T, int> upper_bound (pnode t, T key, int index) {
    push(t);
    if (!t)
      return {T(), size()};
    if (key < t->key) {
      std::pair <T, int> ret = upper_bound(t->l, key, index);
      if (ret.second == size())
        ret = {t->key, index + cnt(t->l)};
      return ret;
    }
    return upper_bound(t->r, key, index + 1 + cnt(t->l));
  }

  void shift (pnode & t, int l, int r, T add) {
    pnode l1, r1;
    split(t, l1, r1, r + 1);
    pnode l2, r2;
    split(l1, l2, r2, l);
    update(r2, add, 0);
    pnode t2;
    merge(t2, l2, r2);
    merge(t, t2, r1);
  }

  void reverse (pnode & t, int l, int r) {
    pnode l1, r1;
    split(t, l1, r1, r + 1);
    pnode l2, r2;
    split(l1, l2, r2, l);
    update(r2, 0 * T(), 1);
    pnode t2;
    merge(t2, l2, r2);
    merge(t, t2, r1);
  }

  void move (pnode & t, int left, int right, int shift) {
    // [l, r) becomes [l+shift, r+shift)
    if (shift == 0)
      return;
    int l = left + std::min(0, shift);
    int r = right + std::max(0, shift);
    int m = (shift > 0) ? right : left;
    pnode prefix, tmp;
    split(root_, prefix, tmp, l);
    pnode suffix, middle;
    split(tmp, middle, suffix, r - l);
    pnode mid_prefix, mid_suffix;
    split(middle, mid_prefix, mid_suffix, m - l);
    merge(middle, mid_suffix, mid_prefix);
    merge(tmp, middle, suffix);
    merge(root_, prefix, tmp);
  }

  T get_sum (pnode & t, int l, int r) {
    pnode l1, r1;
    split(t, l1, r1, r + 1);
    pnode l2, r2;
    split(l1, l2, r2, l);
    T ret = r2->fsum;
    pnode t2;
    merge(t2, l2, r2);
    merge(t, t2, r1);
    return ret;
  }

  void clear (pnode & t) {
    if (!t)
      return;
    clear(t->l);
    clear(t->r);
    delete t;
    t = nullptr;
  }

 public:
  treap (std::mt19937_64 * rng = nullptr) {
    is_sorted_ = true;
    stop_ = false;
    root_ = nullptr;
    if (rng) {
      rng_owner_ = false;
      rng_ = rng;
    }
    else {
      rng_owner_ = true;
      rng_ = new std::mt19937_64;
      rng_->seed(std::chrono::steady_clock::now().time_since_epoch().count());
    }
  }

  ~treap () {
    if (rng_owner_)
      delete rng_;
    clear(root_);
  }

  int size () { return cnt(root_); }

  bool empty () { return (cnt(root_) == 0); }

  bool is_sorted () { return is_sorted_; }

  void srand (std::mt19937_64::result_type seed) {
    // optional
    rng_->seed(seed);
  }

  bool insert (T x) {
    bool eq = false;
    pnode t = new node(x, next_priority());
    stop_ = false;
    insert_at(root_, t, eq);
    while (stop_) {
      stop_ = false;
      eq = false;
      insert_at(root_, t, eq);
    }
    if (eq)
      delete t;
    return !eq;
  }

  void insert_at (int pos, T x) {
    if (pos > size())
      return;
    pnode t = new node(x, next_priority());
    stop_ = false;
    insert(root_, t, pos);
    while (stop_) {
      stop_ = false;
      insert(root_, t, pos);
    }
    if (pos > 0 && is_sorted_) {
      if (get(root_, pos - 1) >= get(root_, pos))
        is_sorted_ = false;
    }
    if (pos < size() - 1 && is_sorted_) {
      if (get(root_, pos) >= get(root_, pos + 1))
        is_sorted_ = false;
    }
  }

  bool erase (T x) {
    bool found = false;
    erase_at(root_, x, found);
    return found;
  }

  void erase_at (int pos) {
    if (pos >= size())
      return;
    erase(root_, pos);
  }

  void clear () {
    clear(root_);
  }

  int get_index (T key) {
    if (!is_sorted_)
      return size();
    pnode t = root_;
    int index = 0;
    while (t && t->key != key) {
      if (t->key > key)
        t = t->l;
      else {
        index += cnt(t->l) + 1;
        t = t->r;
      }
    }
    if (!t)
      return size();
    index += cnt(t->l);
    return index;
  }

  T operator[] (int index) {
    return get(root_, index);
  }

  std::pair <T, int> lower_bound (T x) {
    if (!is_sorted_)
      return {T(), size()};
    return lower_bound(root_, x, 0);
  }

  std::pair <T, int> upper_bound (T x) {
    if (!is_sorted_)
      return {T(), size()};
    return upper_bound(root_, x, 0);
  }

  void shift (int left, int right, T x) {
    left = std::max(left, 0);
    right = std::min(right, size() - 1);
    shift(root_, left, right, x);
    if (left > 0 && is_sorted_) {
      if (get(root_, left - 1) >= get(root_, left))
        is_sorted_ = false;
    }
    if (right < size() - 1 && is_sorted_) {
      if (get(root_, right) >= get(root_, right + 1))
        is_sorted_ = false;
    }
  }

  void reverse (int left, int right) {
    left = std::max(left, 0);
    right = std::min(right, size() - 1);
    reverse(root_, left, right);
    if (left != right)
      is_sorted_ = false;
  }

  void move (int left, int right, int shift) {
    move(root_, left, right, shift);
  }

  T get_sum (int left, int right) {
    return get_sum(root_, left, right);
  }
};  // class treap
// [NAME]: Treap (ordered multiset + implicit operations)
// [PURPOSE]: Randomized BST supporting ordered set operations plus range add/shift/reverse and positional inserts.
// Typical use: maintain sequence or ordered set with augmented sums in O(log n).
// [COMPLEXITY]:
//   - insert/erase/find/get_sum/shift/reverse: O(log n) expected
//   - memory: O(n)
// [USAGE]:
//   - Template on key type T; supports insert by key (unique), insert_at by position, erase by key/pos, lower/upper_bound, move segments.
//   - Lazy tags `add` (range add) and `rev` (reverse) are pushed; `fsum` stores subtree sum, `cnt` size.
//   - Nodes indexed 0-based for positional ops; randomness via std::mt19937_64 (owns RNG by default).
