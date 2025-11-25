// [NAME]: FFT-based convolution modulo arbitrary M (split-coefficient trick)
// [PURPOSE]: Multiplies two integer polynomials modulo M by embedding coefficients into complex numbers and FFT.
// Typical use: convolution when M is not NTT-friendly but fits in int (cut ~ sqrt(M) splitting).
// [COMPLEXITY]:
//   - fft length n: O(n log n)
//   - convMod(a,b): O((n+m) log(n+m))
//   - memory: O(n)
// [USAGE]:
//   - Provide vectors a,b (can be empty); template parameter M sets modulus.
//   - convMod<M>(a,b) returns result mod M; internally chooses n as next power of two.
//   - Requires double precision care; coefficients should be small enough to avoid rounding issues.
using ll = long long;
typedef complex<double> C;

void fft(vector<C>& a) {
	int n = sz(a), L = 31 - __builtin_clz(n);
	static vector<complex<long double>> R(2, 1);
	static vector<C> rt(2, 1);  // (^ 10% faster if double)
	for (static int k = 2; k < n; k *= 2) {
		R.resize(n); rt.resize(n);
		auto x = polar(1.0L, acos(-1.0L) / k);
		for(int i=k;i<2*k;i++) rt[i] = R[i] = i&1 ? R[i/2] * x : R[i/2];
	}
	vi rev(n);
	for(int i=0;i<n;i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
	for(int i=0;i<n;i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) for(int j=0;j<k;j++) {
			// C z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)  /// include-line
			auto x = (double *)&rt[j+k], y = (double *)&a[i+j+k];        /// exclude-line
			C z(x[0]*y[0] - x[1]*y[1], x[0]*y[1] + x[1]*y[0]);           /// exclude-line
			a[i + j + k] = a[i + j] - z;
			a[i + j] += z;
		}
}

template<int M> vi convMod(const vi &a, const vi &b) {
	if (a.empty() || b.empty()) return {};
	vi res(sz(a) + sz(b) - 1);
	int B=32-__builtin_clz(sz(res)), n=1<<B, cut=(int)(sqrt(M));
	vector<C> L(n), R(n), outs(n), outl(n);
	for(int i=0;i<sz(a);i++) L[i] = C((int)a[i] / cut, (int)a[i] % cut);
	for(int i=0;i<sz(b);i++) R[i] = C((int)b[i] / cut, (int)b[i] % cut);
	fft(L), fft(R);
	for(int i=0;i<n;i++) {
		int j = -i & (n - 1);
		outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
		outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
	}
	fft(outl), fft(outs);
	for(int i=0;i<sz(res);i++) {
		ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
		ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
		res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
	}
	return res;
}
