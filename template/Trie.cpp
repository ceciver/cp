struct Trie {
    vvi nxt;
    vi childs;

    Trie() {
        nxt.push_back(vi(2,-1));
    }

    void insert(int x, bool record = true) {
        if(record) childs.pb(x);
        int curr = 0;
        for(int i=30;i>=0;i--) {
            int b = (x >> i) & 1;
            if(nxt[curr][b] == -1) {
                nxt.push_back(vi(2, -1));
                nxt[curr][b] = nxt.size() - 1;
            }
            curr = nxt[curr][b];
        }
    }

    int mx_xor(int x) {
        if (nxt.size() == 1 && nxt[0][0] == -1 && nxt[0][1] == -1) return 0;
        int curr = 0;
        int ans = 0;
        for(int i=30;i>=0;i--) {
            int b = (x >> i) & 1;
            if(nxt[curr][!b] != -1) {
                curr = nxt[curr][!b];
                ans |= (1LL << i);
            }else {
                curr = nxt[curr][b];
            }
        }
        return ans;
    }

};
// [NAME]: Binary trie for maximum XOR queries
// [PURPOSE]: Stores integers in a bitwise trie to support insertion and max-xor lookup.
// Typical use: find element maximizing x^y in O(log W) (W = bit width).
// [COMPLEXITY]:
//   - insert: O(W)
//   - mx_xor: O(W)
//   - memory: O(n * W) nodes
// [USAGE]:
//   - Bits considered from 30 down to 0; construct empty Trie, then insert values (optionally recorded in `childs`).
//   - mx_xor(x) returns maximum xor with x (0 if trie empty); indices are implicit, no deletions implemented.
