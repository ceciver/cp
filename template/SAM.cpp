// [NAME]: Suffix Automaton (SAM)
// [PURPOSE]: Compact automaton of all substrings of a string; supports occurrence counts, substring checks, k-th substring queries.
// Typical use: substring existence, counting distinct/all substrings, longest common substring, etc.
// [COMPLEXITY]:
//   - extend/build over length n: O(n * alphabet)
//   - queries (is_substring, count_occurrences, kth_substring...): O(len) or O(answer length)
//   - memory: O(n * alphabet)
// [USAGE]:
//   - Template parameters: ALPH (alphabet size) and MINC (minimum char code). Call build(s) or extend_char per character.
//   - mark_terminals() called inside build; prepare_occ()/prepare_paths_dp() lazily compute occurrence/path DP.
//   - Methods assume 0-based string indices; invalid chars reset to root. Occurrence functions require prepare_occ() first.
template<int ALPH = 26, int MINC = 'a'>
struct SAM {
    struct State {
        int link, len;
        int next[ALPH];
        long long occ;    
        int firstpos;    
        State(): link(-1), len(0), occ(0), firstpos(-1) {
            for (int i = 0; i < ALPH; ++i) next[i] = -1;
        }
    };

    vector<State> st;
    int last;
    vector<char> is_term;
    bool occ_ready = false;
    bool dp_ready  = false;
    vector<int> dp_paths;

    SAM(int maxlen = 0) {
        st.reserve(max(2LL, 2 * maxlen));
        st.push_back(State());
        last = 0;
        is_term.assign(1, false);
    }

    static inline bool okc(int c) { return 0 <= c && c < ALPH; }

    void extend_idx(int c, int pos) {
        int cur = (int)st.size();
        st.push_back(State());
        st[cur].len = st[last].len + 1;
        st[cur].firstpos = pos;
        st[cur].occ = 1;
        int p = last;
        for (; p != -1 && st[p].next[c] == -1; p = st[p].link) st[p].next[c] = cur;

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = (int)st.size();
                st.push_back(st[q]);     
                st[clone].len = st[p].len + 1;
                st[clone].occ = 0;           
                // redirect
                for (; p != -1 && st[p].next[c] == q; p = st[p].link) st[p].next[c] = clone;
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;
    }

    void extend_char(char ch, int pos) {
        int c = (int)ch - MINC;
        if (!okc(c)) {
            last = 0;
            return;
        }
        extend_idx(c, pos);
    }

    void build(const string &s) {
        for (int i = 0; i < (int)s.size(); ++i) extend_char(s[i], i);
        mark_terminals();
    }

    void mark_terminals() {
        is_term.assign(st.size(), false);
        int p = last;
        while (p != -1) { is_term[p] = true; p = st[p].link; }
    }

    vector<int> order_by_len() const {
        int mx = 0;
        for (auto &x : st) if (x.len > mx) mx = x.len;
        vector<int> cnt(mx + 1, 0);
        for (auto &x : st) ++cnt[x.len];
        for (int i = 1; i <= mx; ++i) cnt[i] += cnt[i - 1];
        vector<int> ord(st.size());
        for (int i = (int)st.size() - 1; i >= 0; --i) ord[--cnt[st[i].len]] = i;
        return ord;
    }

    void prepare_occ() {
        if (occ_ready) return;
        auto ord = order_by_len();
        for (int i = (int)ord.size() - 1; i > 0; --i) {
            int v = ord[i], p = st[v].link;
            if (p != -1) st[p].occ += st[v].occ;
        }
        occ_ready = true;
    }

    bool is_substring(const string &w) const {
        int v = 0;
        for (char ch : w) {
            int c = (int)ch - MINC;
            if (!okc(c) || st[v].next[c] == -1) return false;
            v = st[v].next[c];
        }
        return true;
    }

    bool is_suffix(const string &w) const {
        int v = 0;
        for (char ch : w) {
            int c = (int)ch - MINC;
            if (!okc(c) || st[v].next[c] == -1) return false;
            v = st[v].next[c];
        }
        return is_term[v];
    }

    long long count_distinct_substrings() const {
        long long ans = 0;
        for (int v = 1; v < (int)st.size(); ++v) ans += st[v].len - st[st[v].link].len;
        return ans;
    }

    long long count_occurrences(const string &w) {
        prepare_occ();
        int v = 0;
        for (char ch : w) {
            int c = (int)ch - MINC;
            if (!okc(c) || st[v].next[c] == -1) return 0;
            v = st[v].next[c];
        }
        return st[v].occ;
    }

    int first_occurrence(const string &w) const {
        int v = 0;
        for (char ch : w) {
            int c = (int)ch - MINC;
            if (!okc(c) || st[v].next[c] == -1) return -1;
            v = st[v].next[c];
        }
        return st[v].firstpos - (int)w.size() + 1;
    }

    void prepare_paths_dp() {
        if (dp_ready) return;
        auto ord = order_by_len();
        dp_paths.assign(st.size(), 1); // empty path
        for (int i = (int)ord.size() - 1; i >= 0; --i) {
            int v = ord[i];
            long long sum = 1;
            for (int c = 0; c < ALPH; ++c) {
                int to = st[v].next[c];
                if (to != -1) sum += dp_paths[to];
            }
            dp_paths[v] = sum;
        }
        dp_ready = true;
    }

    string kth_distinct_substring(long long k) {
        prepare_paths_dp();
        long long total = dp_paths[0] - 1;
        if (k < 1 || k > total) return string();
        string ans;
        int v = 0;
        while (k > 0) {
            for (int c = 0; c < ALPH; ++c) {
                int to = st[v].next[c];
                if (to == -1) continue;
                if (dp_paths[to] >= k) {
                    ans.push_back(char(MINC + c));
                    v = to;
                    --k; 
                    break;
                } else {
                    k -= dp_paths[to];
                }
            }
        }
        return ans;
    }

    vector<long long> dp_all;
    bool all_ready = false;

    void prepare_all_multiplicity_dp() {
        prepare_occ();            
        if (all_ready) return;
        auto ord = order_by_len(); 
        dp_all.assign(st.size(), 0);
        for (int i = (int)ord.size() - 1; i >= 0; --i) {
            int v = ord[i];
            long long sum = 0;
            for (int c = 0; c < ALPH; ++c) {
                int to = st[v].next[c];
                if (to == -1) continue;
                sum += st[to].occ + dp_all[to];
            }
            dp_all[v] = sum;
        }
        all_ready = true;
    }

    string kth_substring_all(long long k) {
        prepare_all_multiplicity_dp();
        if (k < 1 || k > dp_all[0]) return string();
        string ans;
        int v = 0;
        while (k > 0) {
            for (int c = 0; c < ALPH; ++c) {
                int to = st[v].next[c];
                if (to == -1) continue;
                long long block = st[to].occ + dp_all[to]; // <-- long long
                if (k > block) { k -= block; continue; }
                ans.push_back(char(MINC + c));
                if (k <= st[to].occ) return ans;
                k -= st[to].occ;
                v = to;
                break;
            }
        }
        return ans;
    }


    pair<int,int> longest_repeated() {
        prepare_occ();
        int best = 0, idx = 0;
        for (int v = 1; v < (int)st.size(); ++v) {
            if (st[v].occ >= 2 && st[v].len > best) {
                best = st[v].len;
                idx = v;
            }
        }
        return {best, st[idx].firstpos};
    }

    pair<int,int> lcs_against(const string &t) const {
        int v = 0, l = 0, best = 0, pos = -1;
        for (int i = 0; i < (int)t.size(); ++i) {
            int c = (int)t[i] - MINC;
            if (!okc(c)) { v = 0; l = 0; continue; }
            while (v && st[v].next[c] == -1) { v = st[v].link; l = st[v].len; }
            if (st[v].next[c] != -1) { v = st[v].next[c]; ++l; }
            if (l > best) { best = l; pos = i; }
        }
        return {best, pos};
    }
};

