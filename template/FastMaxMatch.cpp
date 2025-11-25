// [NAME]: Hopcroft–Karp Maximum Bipartite Matching
// [PURPOSE]: Computes maximum matching in bipartite graphs using layered BFS + DFS for O(E sqrt(V)) time.
// Typical use: faster alternative to Kuhn for larger graphs with many edges.
// [COMPLEXITY]:
//   - build adjacency: O(E)
//   - Solve loop: O(E * sqrt(V))
//   - memory: O(V + E)
// [USAGE]:
//   - Call maxMatch(L, R, edges) with counts of left/right vertices and list of 0-based edges (u in [0,L), v in [0,R)).
//   - Returns matching size; internal `nxt` (left->right) and `prv` (right->left) can be inspected inside function.
//   - Graph stored temporarily; adjust if you need the match arrays outside.
int maxMatch(int L, int R, const vpii& edges) {
	vvi adj = vvi(L);
	vi nxt(L,-1), prv(R,-1), lev, ptr;
	for(int i=0;i<sz(edges);i++) adj.at(edges[i].fi).pb(edges[i].se);

	while (true) {
		lev = ptr = vi(L); int max_lev = 0;
		queue<int> q; for(int i=0;i<L;i++) if (nxt[i]==-1) lev[i]=1, q.push(i);
		while (sz(q)) {
			int x = q.front(); q.pop();
			for (int y: adj[x]) {
				int z = prv[y];
				if (z == -1) max_lev = lev[x];
				else if (!lev[z]) lev[z] = lev[x]+1, q.push(z);
			}
			if (max_lev) break;
		}
		if (!max_lev) break;
		for(int i=0;i<L;i++) if (lev[i] > max_lev) lev[i] = 0;
		auto dfs = [&](auto self, int x) -> bool {
			for (;ptr[x] < sz(adj[x]);++ptr[x]) {
				int y = adj[x][ptr[x]], z = prv[y];
				if (z == -1 || (lev[z] == lev[x]+1 && self(self,z))) 
					return nxt[x]=y, prv[y]=x, ptr[x]=sz(adj[x]), 1;
			}
			return 0;
		};
		for(int i=0;i<L;i++) if (nxt[i] == -1) dfs(dfs,i);
	}
	int ans=0;
    for(int i=0;i<L;i++) if (nxt[i] != -1) ans++;
	return ans;
}
