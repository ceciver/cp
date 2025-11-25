// [NAME]: Dinic Max Flow with capacity scaling (iterative)
// [PURPOSE]: Computes maximum s-t flow on directed graphs using layered BFS + DFS blocking flows with bit scaling.
// Typical use: max flow / min cut problems with better performance on larger capacities.
// [COMPLEXITY]:
//   - addEdge: O(1)
//   - calc(s,t): O(E * V * log C) with scaling (C = max capacity)
//   - memory: O(V + E)
// [USAGE]:
//   - Construct with node count n; add directed edges via addEdge(u,v,cap,rcap=0).
//   - Call calc(source, sink) to obtain max flow; use leftOfMinCut(x) to test min-cut side afterward.
//   - Graph is 0-based; capacities are ints (adjust type if needed).
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
