struct RMQ {

    vi a;
    int N;

    RMQ(int n) {
        int p = 31 - __builtin_clz(n);
        N = 1 << (p + 1);
        a = vi(2 * n, -INF);
    }

    void setMax(int pos, int x) {
        for (int i = pos + N; i; i >>= 1)
            a[i] = max(a[i], x);
    }

    int getMax(int L, int R) const {
        int res = -INF;
        for (L += N, R += N; L < R; L >>= 1, R >>= 1) {
            if (L & 1) {
                res = max(res, a[L]);
                L++;
            }
            if (R & 1) {
                R--;
                res = max(res, a[R]);
            }
        }
        return res;
    }
};
