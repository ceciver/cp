// [NAME]: Edmonds–Karp Max Flow (BFS augmenting paths)
// [PURPOSE]: Simple max-flow implementation using shortest augmenting paths in residual graph.
// Typical use: small graphs where clarity outweighs performance.
// [COMPLEXITY]:
//   - bfs() per augment: O(E)
//   - maxflow(): O(V * E^2) worst-case
//   - memory: O(V^2) if using full capacity matrix
// [USAGE]:
//   - Set global n, fill `adj` (directed edges) and `capacity` matrix before calling maxflow(s, t).
//   - Nodes are 0-based; capacity[cur][next] must hold residual capacity.
//   - Function returns total flow and updates residual capacities; adjacency should include reverse edges for residual graph.
int n;
vector<vector<int>> capacity;
vector<vector<int>> adj;

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next]) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxflow(int s, int t) {
    int flow = 0;
    vector<int> parent(n);
    int new_flow;

    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}
