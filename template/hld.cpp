const int INF = 1e9;

struct Tree {
    using T = int;               
    static constexpr T unit = -INF;

    int n, h;
    vector<T> s;
    vector<T> add;
    vector<T> setv;
    vector<uint8_t> has_set; 
    vector<int> len;          

    inline T f(T a, T b) const { return max(a , b); }

    void init_lengths() {
        if (!n) return;
        len.assign(2*n, 0);
        for (int i = n; i < 2*n; ++i) len[i] = 1;
        for (int i = n - 1; i > 0; --i) len[i] = len[i<<1] + len[i<<1|1];
    }

    inline void pull(int p) {
        if (has_set[p]) { 
            s[p] = setv[p] * len[p] + add[p] * (T)len[p];
        } else {
            s[p] = f(s[p<<1], s[p<<1|1]) + add[p] * (T)len[p];
        }
    }

    inline void apply_add(int p, T v) {
        s[p] += v * (T)len[p];
        if (p < n) add[p] += v;
    }

    inline void apply_set(int p, T x) {
        s[p] = x * (T)len[p];
        if (p < n) {
            setv[p] = x;
            has_set[p] = 1;
            add[p] = 0;   
        }
    }

    void push(int p) {
        for (int k = h; k > 0; --k) {
            int i = p >> k;
            if (has_set[i]) {
                apply_set(i<<1, setv[i]);
                apply_set(i<<1|1, setv[i]);
                has_set[i] = 0;
            }
            if (add[i] != 0) {
                apply_add(i<<1, add[i]);
                apply_add(i<<1|1, add[i]);
                add[i] = 0;
            }
        }
    }

    void build(int p) {
        for (p >>= 1; p; p >>= 1) pull(p);
    }

    Tree(int n_ = 0, T def = unit)
        : n(n_), h(n ? (32 - __builtin_clz(n)) : 0),
          s(2 * n, def), add(n, 0), setv(n, 0), has_set(n, 0) {
        init_lengths();
        for (int i = n - 1; i > 0; --i) pull(i);
    }

    Tree(const vector<T>& a) : n((int)a.size()),
        h(n ? (32 - __builtin_clz(n)) : 0),
        s(2 * n, 0), add(n, 0), setv(n, 0), has_set(n, 0) {
        init_lengths();
        for (int i = 0; i < n; ++i) s[i + n] = a[i];
        for (int i = n - 1; i > 0; --i) pull(i);
    }

    void update(int pos, T val) {  
        int p = pos + n;
        push(p);
        s[p] = val;
        build(p);
    }

    void range_add(int l, int r, T v) {
        if (l >= r) return;
        int L = l + n, R = r + n;
        push(L);
        push(R - 1);
        int l2 = L, r2 = R;
        while (l2 < r2) {
            if (l2 & 1) apply_add(l2++, v);
            if (r2 & 1) apply_add(--r2, v);
            l2 >>= 1; r2 >>= 1;
        }
        build(L);
        build(R - 1);
    }

    void range_set(int l, int r, T x) {
        if (l >= r) return;
        int L = l + n, R = r + n;
        push(L);
        push(R - 1);
        int l2 = L, r2 = R;
        while (l2 < r2) {
            if (l2 & 1) apply_set(l2++, x);
            if (r2 & 1) apply_set(--r2, x);
            l2 >>= 1; r2 >>= 1;
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
        while (L < R) {
            if (L & 1) ra = f(ra, s[L++]);
            if (R & 1) rb = f(s[--R], rb);
            L >>= 1; R >>= 1;
        }
        return f(ra, rb);
    }
};

struct Edge { int u, v; int w; };

template <bool VALS_EDGES> struct HLD {

    int N, tim = 0;
    vector<vi> adj;
    vi par, siz, depth, rt, pos;
    Tree tree;
    using T   = typename Tree::T;

    HLD(vector<vi> adj_)
        : N((int)adj_.size()), adj(adj_), par(N,-1), siz(N,1), depth(N,0),
          rt(N,0), pos(N,0), tree(N) { dfsSz(0); dfsHld(0); }

    void dfsSz(int v) {
        if (par[v] != -1)
            adj[v].erase(find(adj[v].begin(), adj[v].end(), par[v]));
        for (int &u : adj[v]) {
            par[u] = v, depth[u] = depth[v] + 1;
            dfsSz(u);
            siz[v] += siz[u];
            if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
        }
    }
    void dfsHld(int v) {
        pos[v] = tim++;
        for (int u : adj[v]) {
            rt[u] = (u == adj[v][0] ? rt[v] : u);
            dfsHld(u);
        }
    }
    template <class B> void process(int u, int v, B op) {
        for (; rt[u] != rt[v]; v = par[rt[v]]) {
            if (depth[rt[u]] > depth[rt[v]]) swap(u, v);
            op(pos[rt[v]], pos[v] + 1);
        }
        if (depth[u] > depth[v]) swap(u, v);
        op(pos[u] + (int)VALS_EDGES, pos[v] + 1);
    }

    void addPath(int u, int v, int x) {
        process(u, v, [&](int l, int r){ tree.range_add(l, r, x); });
    }
    void setPath(int u, int v, int x) {
        process(u, v, [&](int l, int r){ tree.range_set(l, r, x); });
    }
    int  maxPath(int u, int v) {
        int res = -(int)1e9;
        process(u, v, [&](int l, int r){ res = max(res, (int)tree.query(l, r)); });
        return res;
    }
    void addSubtree(int v, int x) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        tree.range_add(L, R, x);
    }
    void setSubtree(int v, int x) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        tree.range_set(L, R, x);
    }
    int  maxSubtree(int v) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        return tree.query(L, R);
    }

    void build_from_nodes(const vector<T> &base_on_nodes) {
        vector<T> base(N);
        for (int v = 0; v < N; ++v) base[pos[v]] = base_on_nodes[v];
        tree = Tree(base);
    }

    void build_from_edges(const vector<Edge> &E) {
        vector<T> base(N, 0);
        for (auto &e : E) {
            int wnode = (par[e.u] == e.v ? e.u : e.v);
            base[pos[wnode]] = e.w;
        }
        tree = Tree(base);
    }

};

