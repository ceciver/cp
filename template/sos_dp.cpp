namespace SOS {

    int op_add(int a, int b) { return a + b; }
    int op_sub(int a, int b) { return a - b; }
    int op_max(int a, int b) { return a > b ? a : b; }
    int op_min(int a, int b) { return a < b ? a : b; }

    void over_sub(vector<int> &f, int n, int (*op)(int, int)) {
        int N = 1 << n;
        for (int bit = 0; bit < n; bit++) {
            int b = 1 << bit;
            for (int mask = 0; mask < N; mask++) {
                if (mask & b) {
                    f[mask] = op(f[mask], f[mask ^ b]);
                }
            }
        }
    }

    void over_sup(vector<int> &f, int n, int (*op)(int, int)) {
        int N = 1 << n;
        for (int bit = 0; bit < n; bit++) {
            int b = 1 << bit;
            for (int mask = 0; mask < N; mask++) {
                if ((mask & b) == 0) {
                    f[mask] = op(f[mask], f[mask | b]);
                }
            }
        }
    }

    // Zeta (subsets): f[mask] = Σ_{sub ⊆ mask} original[sub]
    void sum_sub(vector<int> &f, int n) {
        over_sub(f, n, op_add);
    }

    // Zeta (supersets): f[mask] = Σ_{sup ⊇ mask} original[sup]
    void sum_sup(vector<int> &f, int n) {
        over_sup(f, n, op_add);
    }

    // Möbius inverse of sum_sub:
    // if f is Σ_{sub ⊆ mask} g[sub], after mobius_sub(f):
    //   f = g (original)
    void mobius_sub(vector<int> &f, int n) {
        over_sub(f, n, op_sub);
    }

    // Möbius inverse of sum_sup:
    void mobius_sup(vector<int> &f, int n) {
        over_sup(f, n, op_sub);
    }

    // f[mask] = max over sub ⊆ mask
    void max_sub(vector<int> &f, int n) {
        over_sub(f, n, op_max);
    }

    // f[mask] = max over sup ⊇ mask
    void max_sup(vector<int> &f, int n) {
        over_sup(f, n, op_max);
    }

    // f[mask] = min over sub ⊆ mask
    void min_sub(vector<int> &f, int n) {
        over_sub(f, n, op_min);
    }

    // f[mask] = min over sup ⊇ mask
    void min_sup(vector<int> &f, int n) {
        over_sup(f, n, op_min);
    }

    // h[mask] = Σ_{sub ⊆ mask} f[sub] * g[mask ^ sub]
    // size(f) = size(g) = 2^n
    // Uses zeta + Möbius on subsets.
    vector<int> or_convolution(vector<int> f, vector<int> g, int n) {
        int N = 1 << n;
        sum_sub(f, n);
        sum_sub(g, n);
        for (int mask = 0; mask < N; mask++) {
            f[mask] = (int)(1LL * f[mask] * g[mask]);
        }
        mobius_sub(f, n);
        return f;
    }
}
