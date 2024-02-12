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
