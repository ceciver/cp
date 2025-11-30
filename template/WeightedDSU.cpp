struct WeightedDSU {
    vi par, d, rank;
    #define PARITY 3

    WeightedDSU(int n) {
        par.resize(n);
        d.assign(n, 0);
        rank.assign(n, 1);
        for (int i = 0; i < n; i++) par[i] = i;
    }

    int find(int x) {
        if (x == par[x]) return x;
        int p = par[x];
        par[x] = find(p);
        d[x] += d[p];
        return par[x];
    }

    bool join(int a, int b, int r) {
        int ra = find(a);
        int rb = find(b);
        if (ra == rb) {
            int tmp = ((d[a] - d[b]) % PARITY + PARITY) % PARITY;
            return tmp == r;
        } else {
            if (rank[ra] < rank[rb]) {
                par[ra] = rb;
                d[ra] = ((d[b] + r - d[a]) % PARITY + PARITY) % PARITY;
                rank[rb] += rank[ra];
            } else {
                r = (PARITY - r) % PARITY;
                par[rb] = ra;
                d[rb] = ((r + d[a] - d[b]) % PARITY + PARITY) % PARITY;
                rank[ra] += rank[rb];
            }
            return true;
        }
    }
};
