struct Tree {
    using T = int;
    static constexpr T unit = 0;       
    T f(T a, T b) { return (a + b); }      

    int n, h;             
    vector<T> s;         
    vector<T> d;        

    Tree(int n = 0, T def = unit)
        : n(n), h(n ? (32 - __builtin_clz(n)) : 0),
          s(2 * n, def), d(n, 0) {}

    void apply(int p, T val) {
        s[p] += val;
        if (p < n) d[p] += val;   
    }

    void build(int p) {
        for (p >>= 1; p; p >>= 1)
            s[p] = f(s[p << 1], s[p << 1 | 1]) + d[p];
    }

    void push(int p) {
        for (int k = h; k > 0; --k) {
            int i = p >> k;
            if (d[i] != 0) {
                apply(i << 1,     d[i]);
                apply(i << 1 | 1, d[i]);
                d[i] = 0;
            }
        }
    }

    void update(int pos, T val) {
        int p = pos + n;
        push(p);            
        s[p] = val;
        build(p);          
    }

    void range_add(int l, int r, T val) {
        if (l >= r) return;
        int L = l + n, R = r + n;
        push(L);
        push(R - 1);
        for (int l2 = L, r2 = R; l2 < r2; l2 >>= 1, r2 >>= 1) {
            if (l2 & 1) apply(l2++, val);
            if (r2 & 1) apply(--r2, val);
        }
        build(L);
        build(R - 1);
    }

    T query(int l, int r) {
        if (l >= r) return unit;
        int L = l + n, R = r + n;
        push(L);
        push(R - 1);
        T ra = unit, rb = unit;
        for (; L < R; L >>= 1, R >>= 1) {
            if (L & 1) ra = f(ra, s[L++]);
            if (R & 1) rb = f(s[--R], rb);
        }
        return f(ra, rb);
    }
};
