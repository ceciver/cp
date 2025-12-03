const int MOD1 = 1000000007;
const int MOD2 = 1000000009;
const int B1   = 911382323;
const int B2   = 972663749;

vi pw1(1, 1), pw2(1, 1);

void ensure_pw(int n) {
    while (sz(pw1) <= n) {
        pw1.pb(pw1.back() * B1 % MOD1);
        pw2.pb(pw2.back() * B2 % MOD2);
    }
}

struct HashNode {
    int h1, h2;
    int len;   
    HashNode(int _h1 = 0, int _h2 = 0, int _len = 0) : h1(_h1), h2(_h2), len(_len) {}
};

HashNode make_char_node(char c) {
    int x = (unsigned char)c + 1; // avoid zero
    return HashNode(x % MOD1, x % MOD2, 1);
}

HashNode merge_node(const HashNode &L, const HashNode &R) {
    if (!L.len) return R;
    if (!R.len) return L;
    ensure_pw(R.len);
    HashNode res;
    res.len = L.len + R.len;
    res.h1 = (L.h1 * pw1[R.len] + R.h1) % MOD1;
    res.h2 = (L.h2 * pw2[R.len] + R.h2) % MOD2;
    return res;
}

struct StrHash {
    string s;
    vi h1, h2; 

    StrHash() {}
    StrHash(const string &str) { build(str); }

    void build(const string &str) {
        s = str;
        int n = sz(s);
        ensure_pw(n);
        h1.assign(n + 1, 0);
        h2.assign(n + 1, 0);
        for (int i = 0; i < n; i++) {
            int x = (unsigned char)s[i] + 1;
            h1[i + 1] = (h1[i] * B1 + x) % MOD1;
            h2[i + 1] = (h2[i] * B2 + x) % MOD2;
        }
    }

    HashNode get(int l, int r) const { // [l, r) 
        int len = r - l;
        ensure_pw(len);
        int x1 = (h1[r] - h1[l] * pw1[len] % MOD1 + MOD1) % MOD1;
        int x2 = (h2[r] - h2[l] * pw2[len] % MOD2 + MOD2) % MOD2;
        return HashNode(x1, x2, len);
    }

    HashNode full() const {
        return get(0, sz(s));
    }

    bool equal(int l1, int r1, int l2, int r2) const { 
        if (r1 - l1 != r2 - l2) return 0;
        HashNode A = get(l1, r1);
        HashNode B = get(l2, r2);
        return A.h1 == B.h1 && A.h2 == B.h2;
    }

    bool equal(const StrHash &o, int l1, int r1, int l2, int r2) const {
        if (r1 - l1 != r2 - l2) return 0;
        HashNode A = get(l1, r1);
        HashNode B = o.get(l2, r2);
        return A.h1 == B.h1 && A.h2 == B.h2;
    }
};

typedef HashNode T;
T unit = HashNode();
