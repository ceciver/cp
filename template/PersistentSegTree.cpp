using T = int;                
const T ID = 0;               
inline T merge(const T &a, const T &b) {
    return a + b;              
}

struct PersistentSegTree {
    struct Node {
        T val;
        int l, r;
        Node(): val(ID), l(-1), r(-1) {}
    };

    int n;
    vector<Node> st;

    PersistentSegTree(int _n = 0) { init(_n); }

    void init(int _n) {
        n = _n;
        st.clear();
        st.reserve(n * 40 + 10);   // rough upper bound
    }

    int build_rec(int L, int R, const vector<T>* a) {
        int id = sz(st);
        st.pb(Node());
        if (L + 1 == R) {
            st[id].val = a ? (*a)[L] : ID;
            return id;
        }
        int M = (L + R) >> 1;
        st[id].l = build_rec(L, M, a);
        st[id].r = build_rec(M, R, a);
        st[id].val = merge(st[st[id].l].val, st[st[id].r].val);
        return id;
    }

    int build_empty() { return build_rec(0, n, nullptr); }

    int build(const vector<T> &a) { return build_rec(0, n, &a); }

    int update_rec(int id, int L, int R, int pos, const T &val) {
        int new_id = sz(st);
        st.pb(st[id]); // clone

        if (L + 1 == R) {
            st[new_id].val = val;
            return new_id;
        }
        int M = (L + R) >> 1;
        if (pos < M)
            st[new_id].l = update_rec(st[id].l, L, M, pos, val);
        else
            st[new_id].r = update_rec(st[id].r, M, R, pos, val);

        st[new_id].val = merge(st[st[new_id].l].val, st[st[new_id].r].val);
        return new_id;
    }

    int update(int root, int pos, const T &val) {
        return update_rec(root, 0, n, pos, val);
    }

    T query_rec(int id, int L, int R, int ql, int qr) const {
        if (qr <= L || R <= ql) return ID;
        if (ql <= L && R <= qr) return st[id].val;
        int M = (L + R) >> 1;
        T leftv  = query_rec(st[id].l, L, M, ql, qr);
        T rightv = query_rec(st[id].r, M, R, ql, qr);
        return merge(leftv, rightv);
    }

    T query(int root, int l, int r) const {
        return query_rec(root, 0, n, l, r);
    }

    T query_point(int root, int pos) const {
        return query(root, pos, pos + 1);
    }
};
