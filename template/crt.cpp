// solves a*x + b*y = gcd(a,b)
// a, b can be >= 0; result gcd >= 0
int ext_gcd(int a, int b, int &x, int &y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return (a >= 0 ? a : -a);
    }
    int x1, y1;
    int g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

int safe_mod(int x, int m) {
    x %= m;
    if (x < 0) x += m;
    return x;
}

// merge:
//  x ≡ r1 (mod m1)
//  x ≡ r2 (mod m2)
//
// on success:
//  - r, m s.t. x ≡ r (mod m) is the merged system, m = lcm(m1, m2)
//  - r in [0, m)
// returns false if no solution
//
// REQUIRE: m1 > 0, m2 > 0
bool merge(int r1, int m1, int r2, int m2, int &r, int &m) {
    if (m1 <= 0 || m2 <= 0) return false;

    r1 = safe_mod(r1, m1);
    r2 = safe_mod(r2, m2);

    int x, y;
    int g = ext_gcd(m1, m2, x, y);          // m1*x + m2*y = g
    int diff = r2 - r1;
    if (diff % g != 0) return false;        // inconsistent

    int mod2 = m2 / g;
    // x is inverse of m1/g modulo m2/g
    __int128 k = (__int128)diff / g;
    __int128 t = k * x;
    t %= mod2;
    if (t < 0) t += mod2;

    __int128 m1_ = m1;
    __int128 lcm = m1_ / g * m2;           // lcm(m1, m2)
    __int128 res = r1 + m1_ * t;
    res %= lcm;
    if (res < 0) res += lcm;

    r = (int)res;
    m = (int)lcm;
    return true;
}

// merge whole system:
//   x ≡ r[i] (mod m[i])  for i = 0..k-1
//
// on success:
//   - res, mod such that x ≡ res (mod mod) is equivalent system
//   - res in [0, mod)
// returns false if system inconsistent
bool merge_all(const vector<int> &r, const vector<int> &m, int &res, int &mod) {
    int k = (int)r.size();
    if (k == 0) {
        res = 0;
        mod = 1; // everything works
        return true;
    }
    res = r[0];
    mod = m[0];
    for (int i = 1; i < k; i++) {
        if (!merge(res, mod, r[i], m[i], res, mod)) return false;
    }
    return true;
}
