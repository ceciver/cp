struct Dinic {
    struct Edge {
        int to, rev;
        int c, oc;
        int flow() { return max(oc - c, 0LL); }
    };

    vi lvl, ptr, q;
    vector<vector<Edge>> adj;
    Dinic(int n): lvl(n), ptr(n), q(n), adj(n) {}
    void addEdge(int a, int b, int c, int rcap=0) {
        adj[a].pb({b, sz(adj[b]), c, c});
        adj[b].pb({a, sz(adj[a]) - 1, rcap, rcap});
    }
    int dfs(int v, int t, int f) {
        if(v == t || !f) return f;
        for(int& i = ptr[v]; i < sz(adj[v]); i++) { 
            Edge& e = adj[v][i];
            if(lvl[e.to] == lvl[v] + 1) {
                if(int p = dfs(e.to, t, min(f, e.c))) {
                    e.c -= p, adj[e.to][e.rev].c += p;
                    return p;
                }
            }
        }
        return 0;
    }
    int calc(int s, int t) {
        int flow = 0; q[0] = s;
        for(int L=0;L<31;L++) {
            do {
                lvl = ptr = vi(sz(q));
                int qi = 0, qe = lvl[s] = 1;
                while(qi < qe && !lvl[t]) {
                    int v = q[qi++];
                    for(Edge e: adj[v]) 
                        if(!lvl[e.to] && e.c >> (30 - L))
                            q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
                }
                while(int p = dfs(s, t, LLONG_MAX)) flow += p;
            }while(lvl[t]);
        }
        return flow;
    }
    bool leftOfMinCut(int a) { return lvl[a] != 0; }
};
