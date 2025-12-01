// [NAME]: Formal Power Series over mod 998244353 (NTT-based)
// [PURPOSE]: Supports polynomial multiplication, coefficient queries of product/division via offline techniques.
// Typical use: convolutions and retrieving k-th coefficient of P/Q in O(n log n).
// [COMPLEXITY]:
//   - ntt/conv on size n: O(n log n)
//   - operator*= on degree d polys: O(d log d)
//   - convAt/divAt single coefficient: O(d log d) per query (using divide-and-conquer)
//   - memory: O(d)
// [USAGE]:
//   - Uses global MOD=998244353, primitive root=62; vi/Poly are 0-based.
//   - Build Poly from coefficients, multiply via operator*, conv via conv(), or query convAt/divAt.
//   - Ensure leading term of divisor is non-zero for divAt; shrink() trims trailing zeros.
const int MOD = 998244353, root = 62;

static inline int modpow(int a, long long e){
    int r = 1%MOD; a%=MOD;
    while(e){ if(e&1) r = (r%MOD*a%MOD)%MOD; a = (a%MOD*a%MOD)%MOD; e >>= 1; }
    return r;
}

void ntt(vi &a) {
	int n = sz(a), L = 31 - __builtin_clz(n);
	static vi rt(2, 1);
	for (static int k = 2, s = 2; k < n; k *= 2, s++) {
		rt.resize(n);
		int z[] = {1, modpow(root, MOD >> s)};
		for(int i=k;i<2*k;i++) rt[i] = rt[i / 2] * z[i & 1] % MOD;
	}
	vi rev(n);
	for(int i=0;i<n;i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for(int i=0;i<n;i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) for(int j=0;j<k;j++) {
			int z = rt[j + k] * a[i + j + k] % MOD, &ai = a[i + j];
			a[i + j + k] = ai - z + (z > ai ? MOD : 0);
			ai += (ai + z >= MOD ? z - MOD : z);
		}
}
vi conv(const vi &a, const vi &b) {
	if (a.empty() || b.empty()) return {};
	int s = sz(a) + sz(b) - 1, B = 32 - __builtin_clz(s),
	    n = 1 << B;
	int inv = modpow(n, MOD - 2);
	vi L(a), R(b), out(n);
	L.resize(n), R.resize(n);
	ntt(L), ntt(R);
	for(int i=0;i<n;i++)
		out[-i & (n - 1)] = (int)L[i] * R[i] % MOD * inv % MOD;
	ntt(out);
	return {out.begin(), out.begin() + s};
}


template<typename T> struct FormalPowerSeries : vector<T> {
    using vector<T>::vector;
    using P = FormalPowerSeries;

    void shrink() { while (!this->empty() && this->back() == T(0)) this->pop_back(); }

    P operator * (const P& r) { return P(*this) *= r; }

    P& operator *= (const P& r) {
        if (this->empty() || r.empty()) {
            this->clear();
        } else {
            auto res = conv(*this, r);
            *this = P(res.begin(), res.end());
        }
        return *this;
    }

    T convAt(const P& q_, int k){
        P p = *this, q = q_;
        T s = 0;
        int L = max(0LL, k - sz(q) + 1);
        int R = min(k, sz(p)-1);
        for (int i = L; i <= R; ++i){
            s = (s + p[i] * q[k - i]) % MOD;
        }
        return s;
    }

    P pow(long long e, int lim = -1) const {
        assert(e >= 0);

        P base = *this;
        P res{T(1)};

        auto shrink_to = [&](P& f) { if (lim >= 0 && sz(f) > lim) f.resize(lim); };

        shrink_to(base); shrink_to(res);
        if (e == 0) return res;
        while (e) {
            if (e & 1) {
                res *= base;
                shrink_to(res);
            }
            e >>= 1;
            if (!e) break; 
            base *= base;
            shrink_to(base);
        }
        return res;
    }


    T divAt(const P& q_, int k) const {
        P p = *this, q = q_;
        p.shrink(); q.shrink();

        assert(!q.empty() && q[0] != T(0));

        auto negsubst = [](P a){   
            for (int i = 1; i < sz(a); i += 2) a[i] = -a[i];
            a.shrink(); return a;
        };
        auto even = [](const P& a){ 
            P r((a.size() + 1) / 2);
            for(int i = 0; i < sz(r); i++) r[i] = a[i * 2];
            r.shrink(); return r;
        };
        auto odd  = [](const P& a){     
            P r(a.size() / 2);
            for (int i = 0; i < sz(r); i++) r[i] = a[i * 2 + 1];
            r.shrink(); return r;
        };

        while (k > 0) {
            P qm = negsubst(q);
            auto Sv = conv(p, qm);
            auto Tv = conv(q, qm);
            P S(Sv.begin(), Sv.end()), Den(Tv.begin(), Tv.end()); 
            q = even(Den);                       
            p = (k & 1) ? odd(S) : even(S);  
            k >>= 1;
        }
        return p.empty() ? T(0) : p[0] / q[0];
    }
};


using Poly = FormalPowerSeries<int>;

