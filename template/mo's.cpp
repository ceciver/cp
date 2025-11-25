// [NAME]: Mo's algorithm on arrays
// [PURPOSE]: Offline algorithm to answer range queries by reordering them to minimize add/remove operations.
// Typical use: queries requiring amortized O(1) add/remove; total O((n+q) * sqrt(n)) adjustments.
// [COMPLEXITY]:
//   - sort queries: O(q log q)
//   - traversal: O((n + q) * sqrt(n) * cost(add/remove))
//   - memory: O(q)
// [USAGE]:
//   - Implement add(idx), remove(idx), get_answer() for your data structure; set block_size ≈ sqrt(n).
//   - Fill vector<Query> with 0-based inclusive endpoints (l,r) and original idx; call mo_s_algorithm to get answers.
//   - Ensure add/remove maintain state for current [cur_l, cur_r].
void remove(idx);  // TODO: remove value at idx from data structure
void add(idx);     // TODO: add value at idx from data structure
int get_answer();  // TODO: extract the current answer of the data structure

int block_size;

struct Query {
    int l, r, idx;
    bool operator<(Query other) const
    {
        return make_pair(l / block_size, r) <
               make_pair(other.l / block_size, other.r);
    }
};

vector<int> mo_s_algorithm(vector<Query> queries) {
    vector<int> answers(queries.size());
    sort(queries.begin(), queries.end());

    // TODO: initialize data structure

    int cur_l = 0;
    int cur_r = -1;
    // invariant: data structure will always reflect the range [cur_l, cur_r]
    for (Query q : queries) {
        while (cur_l > q.l) {
            cur_l--;
            add(cur_l);
        }
        while (cur_r < q.r) {
            cur_r++;
            add(cur_r);
        }
        while (cur_l < q.l) {
            remove(cur_l);
            cur_l++;
        }
        while (cur_r > q.r) {
            remove(cur_r);
            cur_r--;
        }
        answers[q.idx] = get_answer();
    }
    return answers;
}
