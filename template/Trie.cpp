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
