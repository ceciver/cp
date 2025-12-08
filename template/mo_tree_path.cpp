const int MAXN = 40000 + 5;
const int LOGN = 17; // 2^17 > 40000


int N, M;
vector<int> adj[MAXN];
// this is just euler part
int tin[MAXN], tout[MAXN];
int depthv[MAXN];
int up[MAXN][LOGN];

int eulerArr[2 * MAXN];
int timer_;


// example color # of distinct colors in path
int node_color[MAXN];     
int color_cnt_arr[MAXN]; 
bool vis_arr[MAXN];     
int cur_answer;        


int block_size;
void toggle_node(int u) {
    int c = node_color[u];
    if (vis_arr[u]) {
        vis_arr[u] = false;
        if (--color_cnt_arr[c] == 0) cur_answer--;
    } else {
        vis_arr[u] = true;
        if (color_cnt_arr[c]++ == 0) cur_answer++;
    }
}

void add(int idx) {
    int u = eulerArr[idx];
    toggle_node(u);
}

void remove(int idx) { 
    int u = eulerArr[idx];
    toggle_node(u);
}

int get_answer() {
    return cur_answer;
}

struct Query {
    int l, r, idx, extra; // extra = LCA(u,v) if != -1

    bool operator<(Query other) const {
        int bl = l / block_size;
        int br = other.l / block_size;
        if (bl != br) return bl < br;
        if (bl & 1) return r > other.r;
        return r < other.r;
    }
};

vector<int> mo_s_algorithm(vector<Query> &queries) {
    vector<int> answers(queries.size());
    sort(queries.begin(), queries.end());

    int cur_l = 0;
    int cur_r = -1; // current range is empty

    for (const Query &q : queries) {
        while (cur_l > q.l) {
            --cur_l;
            add(cur_l);
        }
        while (cur_r < q.r) {
            ++cur_r;
            add(cur_r);
        }
        while (cur_l < q.l) {
            remove(cur_l);
            ++cur_l;
        }
        while (cur_r > q.r) {
            remove(cur_r);
            --cur_r;
        }

        if (q.extra != -1) add(tin[q.extra]);      // temporarily add LCA
        answers[q.idx] = get_answer();
        if (q.extra != -1) remove(tin[q.extra]);   // remove LCA again
    }

    return answers;
}


// build up, and euler
void dfs(int u, int p) {
    up[u][0] = p;
    for (int k = 1; k < LOGN; ++k)
        up[u][k] = up[ up[u][k - 1] ][k - 1];

    tin[u] = timer_;
    eulerArr[timer_] = u;
    ++timer_;

    for (int v : adj[u]) if (v != p) {
        depthv[v] = depthv[u] + 1;
        dfs(v, u);
    }

    tout[u] = timer_;
    eulerArr[timer_] = u;
    ++timer_;
}

bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
}

int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int k = LOGN - 1; k >= 0; --k) {
        int uu = up[u][k];
        if (!is_ancestor(uu, v))
            u = uu;
    }
    return up[u][0];
}
/*
 // euler + lca 
timer_ = 0;
int root = 0; // node 0 == original node 1
depthv[root] = 0;
dfs(root, root);


vector<Query> queries;
queries.reserve(M);
for (int i = 0; i < M; ++i) {
    int u, v;
    cin >> u >> v;
    --u; --v;
    if (tin[u] > tin[v]) swap(u, v);
    int p = lca(u, v);
    if (p == u) {
        queries.push_back({ tin[u], tin[v], i, -1 });
    } else {
        queries.push_back({ tout[u], tin[v], i, p });
    }
}

cur_answer = 0;
block_size = max(1, (int) sqrt((double) timer_));
vector<int> ans = mo_s_algorithm(queries);

*/

