// [NAME]: Lazy Segment Tree with range add + range set (min query)
// [PURPOSE]: Supports point update, range add, range assignment, and range query using lazy propagation.
// Typical use: maintain minimum of pairs while applying increments/overwrites on intervals.
// [COMPLEXITY]:
//   - build from array: O(n)
//   - point update/range_add/range_set/query: O(log n)
//   - memory: O(n)
// [USAGE]:
//   - Value type T = pair<int,int>, f = min, unit = {INT_MAX, INT_MAX}; adjust if you swap to other ops.
//   - Construct with size n or base vector; indices 0-based, ranges are half-open [l, r).
//   - Call range_add(l,r,delta), range_set(l,r,val), query(l,r); push/build handle lazy tags.

using T = pair<int,int>;
static constexpr T unit = {INT_MAX, INT_MAX};   
inline T f(const T& a, const T& b) {
    return min(a, b);
}

struct Tree {

    using A = long long; // lazy

    int n, h;
    vector<T> s;
    vector<A> add;          
    vector<T> setv;        
    vector<uint8_t> has_set;
    vector<int> len;       


    inline void node_apply_add(T& x, A v, int seglen) const {
        (void)seglen;
        x.first += v;
    }

    inline void node_apply_set(T& x, const T& val, int seglen) const {
        (void)seglen;
        x = val;

    }

    void init_lengths() {
        if (!n) return;
        len.assign(2*n, 0);
        for (int i = n; i < 2*n; ++i) len[i] = 1;
        for (int i = n - 1; i > 0; --i) len[i] = len[i<<1] + len[i<<1|1];
    }

    inline void pull(int p) {
        s[p] = f(s[p<<1], s[p<<1|1]);
        if (has_set[p]) node_apply_set(s[p], setv[p], len[p]);
        if (add[p] != 0) node_apply_add(s[p], add[p], len[p]);
    }

    inline void apply_add(int p, A v) {
        node_apply_add(s[p], v, len[p]);
        if (p < n) add[p] += v;
    }

    inline void apply_set(int p, const T& x) {
        node_apply_set(s[p], x, len[p]);
        if (p < n) {
            setv[p] = x;
            has_set[p] = 1;
            add[p] = 0; // set kills add
        }
    }

    void push(int p) {
        for (int k = h; k > 0; --k) {
            int i = p >> k;
            if (has_set[i]) {
                apply_set(i<<1,     setv[i]);
                apply_set(i<<1 | 1, setv[i]);
                has_set[i] = 0;
            }
            if (add[i] != 0) {
                apply_add(i<<1,     add[i]);
                apply_add(i<<1 | 1, add[i]);
                add[i] = 0;
            }
        }
    }

    void build(int p) {
        for (p >>= 1; p; p >>= 1) pull(p);
    }

    Tree(int n_ = 0, T def = unit)
        : n(n_), h(n ? (32 - __builtin_clz(n)) : 0),
          s(2 * n, def), add(n, 0), setv(n, def), has_set(n, 0) {
        init_lengths();
        for (int i = n - 1; i > 0; --i) pull(i);
    }

    Tree(const vector<T>& a) : n((int)a.size()),
        h(n ? (32 - __builtin_clz(n)) : 0),
        s(2 * n, unit), add(n, 0), setv(n, unit), has_set(n, 0) {
        init_lengths();
        for (int i = 0; i < n; ++i) s[i + n] = a[i];
        for (int i = n - 1; i > 0; --i) pull(i);
    }

    void update(int pos, const T& val) {
        int p = pos + n;
        push(p);
        s[p] = val;
        build(p);
    }

    void range_add(int l, int r, A v) {
        if (l >= r) return;
        int L = l + n, R = r + n;
        push(L); push(R - 1);
        int l2 = L, r2 = R;
        while (l2 < r2) {
            if (l2 & 1) apply_add(l2++, v);
            if (r2 & 1) apply_add(--r2, v);
            l2 >>= 1; r2 >>= 1;
        }
        build(L); build(R - 1);
    }

    void range_set(int l, int r, const T& x) {
        if (l >= r) return;
        int L = l + n, R = r + n;
        push(L); push(R - 1);
        int l2 = L, r2 = R;
        while (l2 < r2) {
            if (l2 & 1) apply_set(l2++, x);
            if (r2 & 1) apply_set(--r2, x);
            l2 >>= 1; r2 >>= 1;
        }
        build(L); build(R - 1);
    }

    T query(int l, int r) {
        if (l >= r) return unit;
        int L = l + n, R = r + n;
        push(L); push(R - 1);
        T ra = unit, rb = unit;
        while (L < R) {
            if (L & 1) ra = f(ra, s[L++]);
            if (R & 1) rb = f(s[--R], rb);
            L >>= 1; R >>= 1;
        }
        return f(ra, rb);
    }
};
