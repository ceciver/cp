// [NAME]: Heavy-Light Decomposition with lazy segment tree backend
// [PURPOSE]: Decomposes a rooted tree into heavy paths to support path/subtree updates and queries.
// Typical use: translate tree path operations into O(log^2 n) segment tree operations (range add/set/query here).
// [COMPLEXITY]:
//   - build (constructor): O(n)
//   - each path/subtree op: O(log^2 n) with lazy segtree
//   - memory: O(n)
// [USAGE]:
//   - Requires global definitions of T/unit/f and Tree (lazy segment tree) matching those in LazySegTree.cpp.
//   - Construct HLD<VALS_EDGES>(adj) with 0-based adjacency of rooted tree; VALS_EDGES=false for vertex values, true for edge values.
//   - Use addPath/setPath/queryPath and addSubtree/setSubtree/querySubtree; build_from_nodes/edges build base array before queries.
//   - Root is assumed at 0; remove parent from adjacency in dfsSz; ensure Tree supports range operations used.
// Lazy Segment Tree .cpp
struct Edge { int u, v; T w; };

template <bool VALS_EDGES> struct HLD {

    int N, tim = 0;
    vector<vi> adj;
    vi par, siz, depth, rt, pos;
    Tree tree;


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

    void setPath(int u, int v, T x) {
        process(u, v, [&](int l, int r){ tree.range_set(l, r, x); });
    }

    T queryPath(int u, int v) {
        T res = unit;
        process(u, v, [&](int l, int r){ res = f(res, tree.query(l, r)); });
        return res;
    }

    void addSubtree(int v, int x) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        tree.range_add(L, R, x);
    }

    void setSubtree(int v, T x) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        tree.range_set(L, R, x);
    }

    T  querySubtree(int v) {
        int L = pos[v] + (int)VALS_EDGES, R = pos[v] + siz[v];
        return tree.query(L, R);
    }

    void build_from_nodes(const vector<T> &a) {
        vector<T> base(N);
        for (int v = 0; v < N; ++v) base[pos[v]] = a[v];
        tree = Tree(base);
    }

    void build_from_edges(const vector<Edge> &E) {
        vector<T> base(N);
        for (auto &e : E) {
            int wnode = (par[e.u] == e.v ? e.u : e.v);
            base[pos[wnode]] = e.w;
        }
        tree = Tree(base);
    }
};




