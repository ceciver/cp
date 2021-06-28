// Template {{{

/* 
 * Author: $%U%$
*/

#include <bits/stdc++.h>

#ifdef LOCAL
#include "/Library/Debug/debug.h" 
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)
#else
#define debug(x...)
#endif

using namespace std;

#define MAX                 2e9
#define MIN                 -2e9
#define INF                 1e17
#define PI                  acos(-1.0)
#define mid(s,e)            (s+(e-s)/2)
#define clz(n)              __builtin_clzll(n)
#define nbOfBits(n)         __builtin_popcount(n)
#define all(x)              (x).begin(), (x).end()
#define endl                '\n'
#define pb                  push_back
#define sz(a)               ((int)((a).size()))
#define int                 long long  
#define double              long double      

// const int dx[4] = {1,0,-1,0}, dy[4] = {0,1,0,-1};

using vi = vector<int>;
using vvi = vector<vi>;
using vb = vector<bool>;
using vvb = vector<vb>;
using vd = vector<double>;
using vvd = vector<vd>;
using vs = vector<string>;
 
using pii = pair<int, int>;
using pdd = pair<double, double>;
 
using vpii = vector<pii>;
using vpdd = vector<pdd>;

// }}}
// Modular Template {{{
template <int MOD>
class Modular {
public:
    Modular(long long v=0) {
        value = v % MOD;
        if (value < 0)
            value += MOD;
    }

    Modular(long long a, long long b) : value(0) {
        *this += a;
        *this /= b;
    }

    Modular& operator+=(Modular const& b) {
        value += b.value;
        if (value >= MOD)
            value -= MOD;
        return *this;
    }

    Modular& operator-=(Modular const& b) {
        value -= b.value;
        if (value < 0)
            value += MOD;
        return *this;
    }

    Modular& operator*=(Modular const& b) {
        value = (long long)value * b.value % MOD;
        return *this;
    }

    friend Modular power(Modular a, long long e) {
        Modular res = 1;
        while (e) {
            if (e & 1)
                res *= a;
            a *= a;
            e >>= 1;
        }
        return res;
    }

    friend Modular inverse(Modular a) {
        return power(a, MOD - 2);
    }

    Modular& operator/=(Modular const& b) {
        return *this *= inverse(b);
    }

    friend Modular operator+(Modular a, Modular const b) {
        return a += b;
    }

    friend Modular operator-(Modular a, Modular const b) {
        return a -= b;
    }

    friend Modular operator-(Modular const a) {
        return 0 - a;
    }

    friend Modular operator*(Modular a, Modular const b) {
        return a *= b;
    }

    friend Modular operator/(Modular a, Modular const b) {
        return a /= b;
    }

    friend std::ostream& operator<<(std::ostream &os, Modular const& a) {
        return os << a.value;
    }
    friend istream & operator >> (istream &in,  Modular &c) {
        in >> c.value;
        return in;
    }


    friend bool operator==(Modular const& a, Modular const& b) {
        return a.value == b.value;
    }

    friend bool operator!=(Modular const& a, Modular const& b) {
        return a.value != b.value;
    }
    int value;
    static const int MOD_value = MOD;
};
//}}}
using mdint = Modular<(int)1e9+7>;

void solve() {
}

void preprocessing() {}
bool MULTI_TEST_CASES = 0;
// {{{ MAIN 
int32_t main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    cout << setprecision(20);
    cout<<fixed;

    preprocessing();

#ifdef LOCAL
    auto begin = std::chrono::high_resolution_clock::now();
#endif

    int t=1;
    if(MULTI_TEST_CASES) cin>>t;

    while(t--) {
        solve();
    }
    

#ifdef LOCAL
    auto end = std::chrono::high_resolution_clock::now();
    cerr << endl;
    cerr << fixed <<"Execution time: " << std::chrono::duration_cast<std::chrono::duration<double>>(end - begin).count() << " seconds" << endl;
#endif

    return 0;
}
// }}}

