// [NAME]: Kitamasa algorithm for linear recurrences
// [PURPOSE]: Computes n-th term of order-k linear recurrence using polynomial exponentiation.
// Typical use: fast exponentiation of recurrences when k is moderate.
// [COMPLEXITY]:
//   - kitamasa(c, a, n): O(k^2 log n) with naive convolution
//   - memory: O(k)
// [USAGE]:
//   - Recurrence: A_i = sum_{j=1..k} c_{j-1} * A_{i-j}; pass coefficients `c` (size k) and initial terms `a` (size k).
//   - Call kitamasa(c, a, n) to get A_n (0-based indexing of terms inside code).
//   - MOD set to 1e9+7; adjust Mod()/operations if modulus changes.
/*
Solve A[n] = c1 * A[n-1] + .... + ck * A[k-2] in (K^2 log N)

*/

const int MOD = 1e9+7;
using ll = long long;
using poly = vector<ll>;

int Mod(ll x){
    return (x %= MOD) < 0 ? x + MOD : x;
}

poly Mul(const poly &a, const poly &b){
    poly ret(a.size() + b.size() - 1);
    for(int i=0; i<(int)a.size(); i++) for(int j=0; j<(int)b.size(); j++) {
        ret[i+j] = (ret[i+j] + a[i] * b[j]) % MOD;
    }
    return ret;
}

poly Div(const poly &a, const poly &b){
    poly ret(all(a));
    for(int i=ret.size()-1; i>=(int)b.size()-1; i--) for(int j=0; j<(int)b.size(); j++) {
        ret[i+j-b.size()+1] = Mod(ret[i+j-b.size()+1] - ret[i] * b[j]);
    }
    ret.resize(b.size()-1);
    return ret;
}

/// A_{n} = \sum c_{i}A_{n-i} = \sum d_{i}A_{i}
ll kitamasa(poly c, poly a, ll n){
    poly d = {1}; // result
    poly xn = {0, 1}; // xn = x^1, x^2, x^4, ...
    poly f(c.size()+1); // f(x) = x^K - \sum c_{i}x^{i}
    f.back() = 1;
    for(int i=0; i<(int)c.size(); i++) f[i] = Mod(-c[i]);
    while(n){
        if(n & 1) d = Div(Mul(d, xn), f);
        n >>= 1; xn = Div(Mul(xn, xn), f);
    }

    ll ret = 0;
    for(int i=0; i<(int)a.size(); i++) ret = Mod(ret + a[i] * d[i]);
    return ret;
}


void solve() {
    int k,n;
    cin>>k>>n;
    vi dp(k,1);
    vi rec(k, 1);
    cout<<kitamasa(rec, dp, n-1)<<endl;
}
