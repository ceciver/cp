struct DynamicBridges {
    int n;
    vi dsu_2ecc, dsu_cc, parent_cc, cc_size, last_visit;
    int bridges_cnt, lca_it;

    DynamicBridges(int _n = 0) { if (_n) init(_n); }

    void init(int _n) {
        n = _n;
        dsu_2ecc.resize(n);
        dsu_cc.resize(n);
        parent_cc.resize(n);
        cc_size.resize(n);
        last_visit.assign(n, 0);
        for (int i = 0; i < n; i++) {
            dsu_2ecc[i] = dsu_cc[i] = i;
            parent_cc[i] = -1;
            cc_size[i] = 1;
        }
        bridges_cnt = 0;
        lca_it = 0;
    }

    int find_2ecc(int v) {
        if (v == -1) return -1;
        if (v == dsu_2ecc[v]) return v;
        return dsu_2ecc[v] = find_2ecc(dsu_2ecc[v]);
    }

    int find_cc(int v) {
        v = find_2ecc(v);
        if (v == dsu_cc[v]) return v;
        return dsu_cc[v] = find_cc(dsu_cc[v]);
    }

    void make_root(int v) {
        v = find_2ecc(v);
        int root = v;
        int child = -1;
        while (v != -1) {
            int p = find_2ecc(parent_cc[v]);
            parent_cc[v] = child;
            dsu_cc[v] = root;
            child = v;
            v = p;
        }
        cc_size[root] = cc_size[child];
    }

    void merge_path(int a, int b) {
        a = find_2ecc(a);
        b = find_2ecc(b);
        if (a == b) return;

        ++lca_it;
        int lca = -1;
        vi path_a, path_b;

        while (lca == -1) {
            if (a != -1) {
                a = find_2ecc(a);
                path_a.pb(a);
                if (last_visit[a] == lca_it) {
                    lca = a;
                    break;
                }
                last_visit[a] = lca_it;
                a = parent_cc[a];
            }
            if (b != -1) {
                b = find_2ecc(b);
                path_b.pb(b);
                if (last_visit[b] == lca_it) {
                    lca = b;
                    break;
                }
                last_visit[b] = lca_it;
                b = parent_cc[b];
            }
        }

        for (int v : path_a) {
            dsu_2ecc[v] = lca;
            if (v == lca) break;
            --bridges_cnt;
        }
        for (int v : path_b) {
            dsu_2ecc[v] = lca;
            if (v == lca) break;
            --bridges_cnt;
        }
    }

    void add_edge(int a, int b) {
        if (a == b) return;
        a = find_2ecc(a);
        b = find_2ecc(b);
        if (a == b) return;

        int ca = find_cc(a);
        int cb = find_cc(b);

        if (ca != cb) {
            ++bridges_cnt;
            if (cc_size[ca] > cc_size[cb]) {
                swap(ca, cb);
                swap(a, b);
            }
            make_root(a);
            parent_cc[a] = b;
            dsu_cc[a] = b;
            cc_size[cb] += cc_size[a];
        } else {
            merge_path(a, b);
        }
    }

    int bridges() const {
        return bridges_cnt;
    }

    bool same_cc(int a, int b) {
        return find_cc(a) == find_cc(b);
    }

    bool same_2ecc(int a, int b) {
        return find_2ecc(a) == find_2ecc(b);
    }

    int get_cc(int v) {
        return find_cc(v);
    }

    int get_2ecc(int v) {
        return find_2ecc(v);
    }
};
