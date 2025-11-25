
const int SIZE = 1e6;
const int BASE = 30;
vi pw(SIZE);
vi inv(SIZE);

int mdpow(int b, int p) {
    int ans = 1;
    while(p) {
        if(p&1) ans *= b, ans%=MOD;
        p/=2;
        b*=b;
        b%=MOD;
    }
    return ans;
}

struct stringHash {
    vi h; int n;

    stringHash(string s) {
        n = s.size();
        h.resize(n);
        h[0] = s[0] - 'a' + 1;
        for(int i=1;i<n;i++) {
            int c = s[i] - 'a' + 1;
            h[i] = (h[i-1]+c*pw[i])%MOD;
        }
    }

    int get(int l, int r) {
        int ans = h[r];
        if(l>0) { 
            ans -= h[l-1];
            ans %= MOD;
            if(ans<=0) ans += MOD;
            ans = (ans*inv[l])%MOD;
        }
        return ans;
    }

};
// [NAME]: Polynomial rolling hash (single mod)
// [PURPOSE]: Computes prefix hashes for a string to answer substring hash queries in O(1).
// Typical use: compare substrings, detect palindromes, etc.
// [COMPLEXITY]:
//   - preprocessing in constructor: O(n)
//   - get(l,r): O(1)
//   - memory: O(n)
// [USAGE]:
//   - Precompute pw[i]=BASE^i mod MOD and inv[i]=pw[i]^{-1} outside (BASE=30, arrays sized to SIZE).
//   - Construct stringHash(s) (0-based); call get(l,r) for hash of [l,r] inclusive.
//   - Ensure global MOD and SIZE match arrays; hashes are case-sensitive to characters 'a'+.
