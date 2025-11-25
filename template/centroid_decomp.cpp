// [NAME]: Centroid Decomposition of a tree
// [PURPOSE]: Recursively splits a tree into centroids to enable divide-and-conquer queries (e.g., distance updates/queries).
// Typical use: handle path/radius queries with O(log n) update/query after preprocessing.
// [COMPLEXITY]:
//   - get_subtree_size / get_centroid per level: O(subtree size)
//   - build_centroid_decomp: O(n log n) overall
//   - memory: O(n)
// [USAGE]:
//   - Prepare global adjacency `adj` (0-based tree); resize `subtree_size`, `is_removed`, `par`.
//   - Call build_centroid_decomp(root, parent=-1); `par[v]` stores centroid parent in decomposition tree.
//   - Use example lambdas for updates/queries; you must supply a distance function on the original tree.
vector<vector<int>> adj;
vector<int> subtree_size;
vector<bool> is_removed;


int get_subtree_size(int node, int parent = -1) {
	subtree_size[node] = 1;
	for (int child : adj[node]) {
		if (child == parent || is_removed[child]) { continue; }
		subtree_size[node] += get_subtree_size(child, node);
	}
	return subtree_size[node];
}

int get_centroid(int node, int tree_size, int parent = -1) {
	for (int child : adj[node]) {
		if (child == parent || is_removed[child]) { continue; }
		if (subtree_size[child] * 2 > tree_size) {
			return get_centroid(child, tree_size, node);
		}
	}
	return node;
}


vi par;

void build_centroid_decomp(int node, int p=-1) {
	int centroid = get_centroid(node, get_subtree_size(node));

	is_removed[centroid] = true;
    par[centroid] = p;

	for (int child : adj[centroid]) {
		if (is_removed[child]) { continue; }
		build_centroid_decomp(child, centroid);
	}
}
/*

    build_centroid_decomp(0, -1);

    auto upd = [&](int u) {
        int curr = u;
        while(curr != -1) {
            ans[curr] = min(ans[curr], dist(u, curr));
            //debug(u, curr, dist(u, curr));
            curr = par[curr];
        }
    };
    auto query = [&](int u) {
        int best = INF;
        int curr = u;
        while(curr != -1) {
            best = min(best, ans[curr] + dist(u, curr));
            //debug(curr, best, ans[curr], dist(u, curr));
            curr = par[curr];
        }
        return best;
    };

*/
