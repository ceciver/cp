// [NAME]: Successive Shortest Path Min-Cost Max-Flow (Dijkstra with potentials)
// [PURPOSE]: Computes min-cost flow in directed graphs with nonnegative reduced costs; supports optional Bellman-Ford init for negatives.
// Typical use: send maximum possible flow with minimal total cost; returns both flow and cost.
// [COMPLEXITY]:
//   - addEdge: O(1)
//   - maxflow(s,t): O(F * E log V) with Dijkstra (F = total flow)
//   - memory: O(V + E)
// [USAGE]:
//   - Construct with node count N; addEdge(u,v,cap,cost) (0-based, directed; ignores self loops).
//   - If some costs are negative, call setpi(s) before maxflow(); otherwise path() handles potentials on the fly.
//   - maxflow(s,t) returns {total_flow, total_cost}; edge_flows() lists flows on original forward edges.
//   - Costs summed from residual graph are halved in return (cost/2) because each edge is stored twice.
using ll = long long;
const ll INF = numeric_limits<ll>::max() / 4;
 
struct MCMF {
	struct edge {
		int from, to, rev;
		ll cap, cost, flow;
	};
	int N;
	vector<vector<edge>> ed;
	vi seen;
	vector<ll> dist, pi;
	vector<edge*> par;
 
	MCMF(int N) : N(N), ed(N), seen(N), dist(N), pi(N), par(N) {}
 
	void addEdge(int from, int to, ll cap, ll cost) {
		if (from == to) return;
		ed[from].push_back(edge{ from,to,sz(ed[to]),cap,cost,0 });
		ed[to].push_back(edge{ to,from,sz(ed[from])-1,0,-cost,0 });
	}
 
    void path(int s) {
        fill(all(seen), 0);
        fill(all(dist), INF);
        fill(all(par), nullptr);
        dist[s] = 0;
 
        using P = pair<ll,int>;
        priority_queue<P, vector<P>, greater<P>> q;
        q.push({0, s});
 
        while (!q.empty()) {
            auto [d, u] = q.top(); q.pop();
            if (d != dist[u]) continue;      // stale entry
            seen[u] = 1;
 
            ll di = dist[u] + pi[u];
            for (edge& e : ed[u]) if (!seen[e.to]) {
                if (e.cap - e.flow <= 0) continue;
                ll nd = di - pi[e.to] + e.cost;   // reduced cost relax
                if (nd < dist[e.to]) {
                    dist[e.to] = nd;
                    par[e.to] = &e;
                    q.push({nd, e.to});
                }
            }
        }
        for (int i = 0; i < N; ++i)
            if (dist[i] < INF) pi[i] = min(pi[i] + dist[i], INF);
    }
 
 
	pair<ll, ll> maxflow(int s, int t) {
		ll totflow = 0, totcost = 0;
		while (path(s), seen[t]) {
			ll fl = INF;
			for (edge* x = par[t]; x; x = par[x->from])
				fl = min(fl, x->cap - x->flow);
 
			totflow += fl;
			for (edge* x = par[t]; x; x = par[x->from]) {
				x->flow += fl;
				ed[x->to][x->rev].flow -= fl;
			}
		}
		for(int i=0;i<N;i++) for(edge& e : ed[i]) totcost += e.cost * e.flow;
		return {totflow, totcost/2};
	}
 
	// If some costs can be negative, call this before maxflow:
	void setpi(int s) { // (otherwise, leave this out)
		fill(all(pi), INF); pi[s] = 0;
		int it = N, ch = 1; ll v;
		while (ch-- && it--)
			for(int i=0;i<N;i++) if (pi[i] != INF)
			  for (edge& e : ed[i]) if (e.cap)
				  if ((v = pi[i] + e.cost) < pi[e.to])
					  pi[e.to] = v, ch = 1;
		assert(it >= 0); // negative cost cycle
	}

    vector<array<int,3>> edge_flows() {
        vector<array<int,3>> res;
        for (int u = 0; u < N; u++)
            for (auto const& e : ed[u])
                if (e.cap > 0) // original forward edges only
                    res.emplace_back(e.from, e.to, e.flow);
        return res;
    }
 
};
