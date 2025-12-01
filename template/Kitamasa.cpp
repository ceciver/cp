const int MOD = 998244353;


int modpow(int a, int e) {
    int r = 1;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}

vector<int> berlekampMassey(vector<int> s) {
	int n = sz(s), L = 0, m = 0;
	vector<int> C(n), B(n), T;
	C[0] = B[0] = 1;

	int b = 1;
	for(int i=0;i<n;i++) { ++m;
		int d = s[i] % MOD;
		for(int j=1;j<L+1;j++) d = (d + C[j] * s[i - j]) % MOD;
		if (!d) continue;
		T = C; int coef = d * modpow(b, MOD-2) % MOD;
		for(int j=m;j<n;j++) C[j] = (C[j] - coef * B[j - m]) % MOD;
		if (2 * L > i) continue;
		L = i + 1 - L; B = T; b = d; m = 0;
	}

	C.resize(L + 1); C.erase(C.begin());
	for (int& x : C) x = (MOD - x) % MOD;
	return C;
}

int linear_rec(const vi &S, const vi &C, int k) {
    int L = sz(C);
    if (k < sz(S)) return (S[k] % MOD + MOD) % MOD;
    if (L == 0)    return (S[0] % MOD + MOD) % MOD;

    if (L == 1) {
        int a0 = (S[0] % MOD + MOD) % MOD;
        return a0 * modpow(C[0], k) % MOD;
    }

    vi f(L + 1); f[L] = 1;
    for (int i = 0; i < L; i++) {
        f[i] = (MOD - C[L - 1 - i]) % MOD;
    }

    auto mul = [&](const vi &a, const vi &b) { // optimize this using ntt/ftt if needed 
        vi tmp(2 * L);
        for (int i = 0; i < L; i++) if (a[i]) {
            for (int j = 0; j < L; j++) {
                tmp[i + j] = (tmp[i + j] + a[i] * b[j]) % MOD;
            }
        }
        for (int i = 2 * L - 1; i >= L; i--) if (tmp[i]) {
            int coef = tmp[i];
            for (int j = 0; j < L; j++) {
                tmp[i - L + j] = (tmp[i - L + j] - coef * f[j]) % MOD;
                if (tmp[i - L + j] < 0) tmp[i - L + j] += MOD;
            }
        }
        tmp.resize(L);
        return tmp;
    };

    vi res(L), base(L);
    res[0] = 1;
    base[1] = 1;

    int n = k;
    while (n) {
        if (n & 1) res = mul(res, base);
        base = mul(base, base);
        n >>= 1;
    }

    int ans = 0;
    for (int i = 0; i < L; i++) {
        ans = (ans + res[i] * ((S[i] % MOD + MOD) % MOD)) % MOD;
    }
    return ans;
}

int kth(const vi &S_all, int k) {
    if (k < sz(S_all)) return (S_all[k] % MOD + MOD) % MOD;
    vi C = berlekampMassey(S_all);
    int L = sz(C);
    if (L == 0) return (S_all[0] % MOD + MOD) % MOD;
    vi S(L);
    for (int i = 0; i < L; i++) S[i] = (S_all[i] % MOD + MOD) % MOD;
    return linear_rec(S, C, k);
}
