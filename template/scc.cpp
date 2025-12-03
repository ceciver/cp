struct SCC {
    int n; vvi adj; vi comp; int comp_cnt;  
    vi id, low, st;
    vb onstack;
    int timer;

    SCC(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        adj.assign(n, {});
        comp.assign(n, -1);
        id.assign(n, -1);
        low.assign(n, 0);
        onstack.assign(n, false);
        st.clear();
        timer = 0;
        comp_cnt = 0;
    }

    void add_edge(int u, int v) {
        adj[u].pb(v);
    }

    void dfs(int u) {
        id[u] = low[u] = timer++;
        st.pb(u);
        onstack[u] = true;

        for (int v : adj[u]) {
            if (id[v] == -1) {
                dfs(v);
                low[u] = min(low[u], low[v]);
            } else if (onstack[v]) {
                low[u] = min(low[u], id[v]); 
            }
        }
        if (low[u] == id[u]) {
            while (true) {
                int v = st.back(); st.pop_back();
                onstack[v] = false;
                comp[v] = comp_cnt;
                if (v == u) break;
            }
            comp_cnt++;
        }
    }

    int build() {
        for (int i = 0; i < n; i++) {
            if (id[i] == -1) dfs(i);
        }
        return comp_cnt;
    }

    vvi build_dag() {
        vvi dag(comp_cnt);
        for (int u = 0; u < n; u++) {
            for (int v : adj[u]) {
                if (comp[u] == comp[v]) continue;
                dag[comp[u]].pb(comp[v]);
            }
        }
        for (int i = 0; i < comp_cnt; i++) {
            sort(all(dag[i]));
            dag[i].erase(unique(all(dag[i])), dag[i].end());
        }
        return dag;
    }
};
/*
SCC scc(n);
scc.add_edge(u, v);
scc.build();          
vvi dag = scc.build_dag();
int N = scc.comp_cnt;
vi val(N, 0);
for (int i = 0; i < n; i++) {
    val[scc.comp[i]] += a[i];
}
*/
