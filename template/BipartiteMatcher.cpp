// [NAME]: Kuhn's Algorithm for Maximum Bipartite Matching
// [PURPOSE]: Finds a maximum cardinality matching in a bipartite graph using DFS-based augmenting paths.
// Typical use: assignment/selection problems when |L| and edges are moderate.
// [COMPLEXITY]:
//   - add edge: O(1)
//   - Solve(): O(|L| * (|L|+|E|)) in practice; faster with shuffled adjacency
//   - memory: O(|L| + |R| + |E|)
// [USAGE]:
//   - Construct with left/right sizes (n,m); add edges via AddEdge(u,v) with 0-based endpoints.
//   - Call Solve() to return matching size; matched vertices stored in L (left->right) and R (right->left).
//   - Optionally shuffle adjacency lists before Solve to reduce worst-case behavior (see note at bottom of file).
struct BipartiteMatcher {
  vector<vector<int>> G;
  vector<int> L, R, Viz;
  
  BipartiteMatcher(int n, int m) :
  G(n), L(n, -1), R(m, -1), Viz(n) {}
  
  void AddEdge(int a, int b) {
    G[a].push_back(b);
  }
  
  bool Match(int node) {
    if (Viz[node])
      return false;
    Viz[node] = true;
    
    for (auto vec : G[node]) {
      if (R[vec] == -1) {
        L[node] = vec;
        R[vec] = node;
        return true;
      }
    }
    
    for (auto vec : G[node]) {
      if (Match(R[vec])) {
        L[node] = vec;
        R[vec] = node;
        return true;
      }
    }
    
    return false;
  }
  
  int Solve() {
    int ok = true;
    while (ok--) {
      fill(Viz.begin(), Viz.end(), 0);
      for (int i = 0; i < (int)L.size(); ++i)
        if (L[i] == -1)
          ok |= Match(i);
    }
    
    int ret = 0;
    for (int i = 0; i < (int)L.size(); ++i)
      ret += (L[i] != -1);
    return ret;
  }
};
/*
do this
    mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
    for (int i = 0; i < n; i++) {
        shuffle(bm.G[i].begin(), bm.G[i].end(), rng);
    }

*/
