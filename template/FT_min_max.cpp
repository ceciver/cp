const int N = 1 << 18; // has to be power of 2
const int inf = 1 << 28;

struct RMQ {
    int a[N * 2];
    RMQ() {
        for(int i=0;i<N * 2;i++)
            a[i] = -inf;
    }
    void setMax(int pos, int x) {
        for (int i = pos + N; i; i >>= 1)
            a[i] = max(a[i], x);
    }

    int getMax(int L, int R) const {
        int res = -inf;
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
