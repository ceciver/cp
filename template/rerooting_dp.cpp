

namespace reroot {
	const auto exclusive = [](const auto& a, auto baseAgg, const auto& merge_into, int vertex) {
		int n = a.size();
		using Agg = decltype(baseAgg);
		vector<Agg> b(n, baseAgg);
		auto rec = [&](const auto& self, int l, int r, Agg agg) -> void {
			if (l == r) return;
			if (l + 1 == r) { b[l] = agg; return; }
			int m = (l + r) >> 1;
			Agg L = agg;
			for (int i=m; i<r; i++) L = merge_into(L, a[i], vertex, i);
			self(self, l, m, L);
			Agg R = agg;
			for (int i=l; i<m; i++) R = merge_into(R, a[i], vertex, i);
			self(self, m, r, R);
		};
		rec(rec, 0, n, baseAgg);
		return b;
	};

	const auto rerooter = [](const auto& g, const auto& base, const auto& merge_into, const auto& finalize_merge) {
		int n = g.size();
		using Agg = decltype(base(0));
		using Val = decltype(finalize_merge(base(0), 0, -1));

		vector<int> parent(n, -1), bfs; bfs.reserve(n); bfs.push_back(0);
		for (int i=0; i<sz(bfs); i++) {
			int u = bfs[i];
			for (int v: g[u]) if (v != parent[u]) parent[v] = u, bfs.push_back(v);
		}

		vector<Val> dp(n);
		for (int i=n-1; i>=0;i--) {
			int u = bfs[i];
			Agg agg = base(u);
			for (int j=0; j<sz(g[u]); j++) {
				int v = g[u][j];
				if (v == parent[u]) continue;
				agg = merge_into(agg, dp[v], u, j);
			}
			dp[u] = finalize_merge(agg, u, -1);
		}

		vector<Val> up(n);
		up[0] = finalize_merge(base(0), 0, -1);  

		vector<Val> root_dp(n);
		vector<vector<Val>> edge_dp(n), redge_dp(n);

		for (int u : bfs) {
			int m = g[u].size();
			edge_dp[u].resize(m);
			for (int j = 0; j<m; j++) {
				int v = g[u][j];
				edge_dp[u][j] = (v == parent[u]) ? up[u] : dp[v];
			}

			auto excl = exclusive(edge_dp[u], base(u), merge_into, u);
			redge_dp[u].resize(m);
			for (int j=0; j<m; j++) redge_dp[u][j] = finalize_merge(excl[j], u, j);
			Agg all = base(u);
			for (int j=0; j<m; j++) all = merge_into(all, edge_dp[u][j], u, j);
			root_dp[u] = finalize_merge(all, u, -1);

			for (int j=0; j<m; j++) {
				int v = g[u][j];
				if (v == parent[u]) continue;
				up[v] = redge_dp[u][j];
			}
		}
		return make_tuple(root_dp, edge_dp, redge_dp);
	};
}

/*
    
    using Aggregate = array<int, 2>;
    using Value = array<int, 2>;
 
    auto base = [](int vertex) -> Aggregate {
        (void)vertex;
        return {1, 1};
    };

    auto merge_into = [](Aggregate vertex_dp, Value neighbor_dp, int vertex, int edge_index) -> Aggregate {
        (void)vertex;
        (void)edge_index;
        vertex_dp[0] = vertex_dp[0] * (neighbor_dp[0] + neighbor_dp[1]);
        vertex_dp[0] %= MOD;

        vertex_dp[1] = vertex_dp[1] * (neighbor_dp[1]);
        vertex_dp[1] %= MOD;

        return vertex_dp;
    };
    auto finalize_merge = [](Aggregate vertex_dp, int vertex, int edge_index) -> Value {
        (void)vertex;
        (void)edge_index;
        return vertex_dp;
    };
 
    auto [reroot_result, edge_dp, redge_dp] = reroot::rerooter(adj, base, merge_into, finalize_merge);
    
    for(auto dp : reroot_result)
        cout<<dp[0]<<" ";
    cout<<endl;

*/

// [NAME]: Generic rerooting DP template
// [PURPOSE]: Computes values for every tree root by aggregating child contributions with customizable merge/finalize lambdas.
// Typical use: subtree/path DP that needs answers for all roots or edges in O(n).
// [COMPLEXITY]:
//   - rerooter(): O(n * deg) overall (~O(n))
//   - memory: O(n)
// [USAGE]:
//   - Provide adjacency list g (0-based tree) and functions: base(vertex), merge_into(agg, childVal, v, edgeIdx), finalize_merge(agg, v, edgeIdx).
//   - Call reroot::rerooter(...) to get {root_dp, edge_dp, redge_dp}; see example below for colorings count.
//   - exclusive() helper builds per-child prefix/suffix merges for reroot step.
