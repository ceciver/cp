struct Tree2D {
    typedef int T;
    static constexpr T unit = 0;
    T f(T a, T b) { return a + b; }

    int n, m;
    vector<vector<T>> s;

    Tree2D(int n = 0, int m = 0, T def = unit) : n(n), m(m), s(2*n, vector<T>(2*m, def)) {}

    void update(int x, int y, T val) {
        int xi = x + n, yj = y + m;
        s[xi][yj] = val;
        for (int j = yj; j > 1; j >>= 1)
            s[xi][j >> 1] = f(s[xi][j], s[xi][j ^ 1]);

        for (xi >>= 1; xi; xi >>= 1) {
            s[xi][yj] = f(s[xi << 1][yj], s[xi << 1 | 1][yj]);
            for (int j = yj; j > 1; j >>= 1)
                s[xi][j >> 1] = f(s[xi][j], s[xi][j ^ 1]);
        }
    }

    T query_y(int xi, int y1, int y2) { // [y1, y2)
        T rl = unit, rr = unit;
        for (int l = y1 + m, r = y2 + m; l < r; l >>= 1, r >>= 1) {
            if (l & 1) rl = f(rl, s[xi][l++]);
            if (r & 1) rr = f(s[xi][--r], rr);
        }
        return f(rl, rr);
    }

    T query(int x1, int y1, int x2, int y2) {
        T ans = unit;
        for (int l = x1 + n, r = x2 + n; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ans = f(ans, query_y(l++, y1, y2));
            if (r & 1) ans = f(ans, query_y(--r, y1, y2));
        }
        return ans;
    }
};

