struct BridgeTree {
    int n;
    vector<vpii> adj;   // (to, edge_id)
    vpii edges;         // edges[id] = {u, v}
    vi tin, low, comp;
    vector<char> is_bridge;
    int timer, comp_cnt;

    BridgeTree(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n, {});
        edges.clear();
        tin.assign(n, -1);
        low.assign(n, 0);
        comp.assign(n, -1);
        is_bridge.clear();
        timer = 0;
        comp_cnt = 0;
    }

    int add_edge(int u, int v) {
        int id = sz(edges);
        edges.pb({u, v});
        adj[u].pb({v, id});
        adj[v].pb({u, id});
        return id;
    }

    void dfs_bridges(int u, int pe) {
        tin[u] = low[u] = timer++;
        for (auto [v, id] : adj[u]) {
            if (id == pe) continue;
            if (tin[v] == -1) {
                dfs_bridges(v, id);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) is_bridge[id] = 1;
            } else {
                low[u] = min(low[u], tin[v]);
            }
        }
    }

    void find_bridges() {
        int m = sz(edges);
        is_bridge.assign(m, 0);
        timer = 0;
        fill(all(tin), -1);
        for (int i = 0; i < n; i++) {
            if (tin[i] == -1) dfs_bridges(i, -1);
        }
    }

    void dfs_comp(int u, int c) {
        comp[u] = c;
        for (auto [v, id] : adj[u]) {
            if (comp[v] != -1) continue;
            if (is_bridge[id]) continue;
            dfs_comp(v, c);
        }
    }

    int build_components() {
        comp.assign(n, -1);
        comp_cnt = 0;
        for (int i = 0; i < n; i++) {
            if (comp[i] == -1) {
                dfs_comp(i, comp_cnt);
                comp_cnt++;
            }
        }
        return comp_cnt;
    }

    int build() {
        find_bridges();
        return build_components();
    }

    vvi build_tree() {
        vvi tree(comp_cnt);
        for (int id = 0; id < sz(edges); id++) {
            auto [u, v] = edges[id];
            int a = comp[u];
            int b = comp[v];
            if (a == b) continue;
            tree[a].pb(b);
            tree[b].pb(a);
        }
        for (int i = 0; i < comp_cnt; i++) {
            sort(all(tree[i]));
            tree[i].erase(unique(all(tree[i])), tree[i].end());
        }
        return tree;
    }

    vpii get_bridges() {
        vpii res;
        for (int id = 0; id < sz(edges); id++) {
            if (is_bridge[id]) res.pb(edges[id]);
        }
        return res;
    }
};
