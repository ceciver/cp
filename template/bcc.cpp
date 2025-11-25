//Biconnected components

// [NAME]: Biconnected Components (edges)
// [PURPOSE]: Finds edge-biconnected components and articulation edges in an undirected graph.
// Typical use: decompose a connected graph into maximal 2-vertex-connected subgraphs.
// [COMPLEXITY]:
//   - addEdge: O(1)
//   - calc(): O(n + m)
//   - memory: O(n + m)
// [USAGE]:
//   - Construct with node count, call addEdge(u,v) for 0-based undirected edges.
//   - Call calc() to get a vector of components (each as list of edges).
//   - Assumes graph is connected and DFS roots at 0; extend calc() loop if not connected.
struct BCC{
    int n;
    vector<vi> adj;
    vector<vpii> bcc;
    vpii st;
    vi disc,low,par;
    int t;
    BCC(){}

    BCC(int n_) {
        n=n_;
        adj.resize(n);
        disc.resize(n);
        low.resize(n);
        par.resize(n,-1);
        t = 0;
    }

    void addEdge(int a,int b) {
        adj[a].pb(b);
        adj[b].pb(a);
    }

    void dfs(int x, bool root=0) {
        disc[x]=low[x]=t++;
        int children = 0;
        for(int z :adj[x]) {
            if(z!=par[x]) {
                if (disc[z]==-1) {
                    children++;
                    par[z]=x;
                    st.pb({x,z});
                    dfs(z);
                    low[x] = min(low[x],low[z]);
                    if((root && children>1)||(!root && disc[x] <= low[z])) {
                        vpii tmp;
                        while(sz(st) && st.back() != make_pair(x,z)) {
                            tmp.pb(st.back());
                            st.pop_back();
                        } 
                        tmp.pb(st.back());
                        st.pop_back();
                        bcc.pb(tmp);
                    }
                }
                else if(disc[z] < disc[x]) {
                    low[x]=min(low[x],disc[z]);
                    st.pb({x,z});
                }
            }
        }
    }

    vector<vpii> calc() {
        for(int i=0;i<n;i++)
            disc[i]=low[i]=-1;
        dfs(0,1);
        if(sz(st))
            bcc.pb(st);
        st.clear();
        return bcc;
    }
};
