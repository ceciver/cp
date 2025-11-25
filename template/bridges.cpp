// [NAME]: Bridge finder (Tarjan)
// [PURPOSE]: Detects all bridges in an undirected graph via DFS lowlink values.
// Typical use: identify edges whose removal increases connected components.
// [COMPLEXITY]:
//   - build adjacency: O(m)
//   - scc()/dfs traversal: O(n + m)
//   - memory: O(n + m)
// [USAGE]:
//   - Set global n and fill `adj` (0-based, undirected); call scc() to populate vector `bridges`.
//   - DFS restarts over unvisited nodes; sort/output currently happens in solve() example.
//   - Naming note: function scc() here actually finds bridges, not SCCs.
vvi adj;
vi id;
vi low;

int n;
int curr_id;
vpii bridges;

void dfs(int u, int p) {
    id[u] = low[u] = ++curr_id;
    for(int v: adj[u]) {
        if(v == p) continue;
        if (id[v] == 0) { // if V has not been discovered before
            dfs(v, u);  // recursive DFS call
            if (id[u] < low[v]) // condition to find a bridge
                bridges.push_back({u, v});
            low[u] = min(low[u], low[v]); 
        } else 
            low[u] = min(low[u], id[v]);
    }

}

void scc() {
    id = vi(n); 
    low = vi(n);
    curr_id = 0;
    bridges = vpii();

    for(int i=0;i<n;i++) {
        if(id[i] == 0) {
            dfs(i, i);
        }
    }
}


void solve() {
    adj = vvi(n);

    for(int _=0;_<n;_++) {
        int node; 
        cin>>node;
        char cc;
        cin>>cc;
        int k;
        cin>>k;
        cin>>cc;
        for(int _=0;_<k;_++) {
            int v;
            cin>>v;
            adj[node].pb(v);
        }
    }


    scc();
    for(auto& [u,v]: bridges) {
        if(u>v) swap(u,v);
    }
    sort(all(bridges));
    cout<<sz(bridges)<<" critical links"<<endl;
    for(auto [u,v]: bridges) {
        cout<<u<<" - "<<v<<endl;
    }
    cout<<endl;
}
