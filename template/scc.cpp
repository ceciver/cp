// [NAME]: Tarjan's algorithm for Strongly Connected Components
// [PURPOSE]: Decomposes a directed graph into SCCs using DFS with lowlink values.
// Typical use: condensation DAG building, satisfiability reductions, etc.
// [COMPLEXITY]:
//   - scc(): O(n + m)
//   - memory: O(n + m)
// [USAGE]:
//   - Set global n and adjacency `adj` (0-based, directed), then call scc(); results stored in `low` as component ids (1..cnt_scc).
//   - `cnt_scc` holds number of components; reset globals if reusing on another graph.
vvi adj;
vi id;
vi low;
vb onstack;
 
int n;
int curr_id;
vi st;
int cnt_scc;
 
void dfs(int u) {
    st.pb(u);
    id[u] = low[u] = curr_id++;
    onstack[u] = 1;
 
    for(int v: adj[u]) {
        if(id[v] == -1) dfs(v);
        if(onstack[v]) {
            low[u] = min(low[u], low[v]);
        }
    }
 
    if(id[u] == low[u]) {
        while(true) {
            int node = st.back();
            onstack[node] = 0;
            st.pop_back();
            low[node] = cnt_scc+1;
            if(node == u) break; 
        }
        cnt_scc++;
    }
}
 
void scc() {
    id = vi(n,-1); 
    onstack = vb(n);
    low = vi(n);
    curr_id = 0;
    cnt_scc = 0;
 
    for(int i=0;i<n;i++) {
        if(id[i] == -1) {
            dfs(i);
        }
    }
}
