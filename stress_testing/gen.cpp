/// Template {{{

#include <bits/stdc++.h>

#ifdef LOCAL
#include "/Library/debug/debug.h"
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

using namespace std;

#define MAX                 2e9
#define MIN                 -2e9
#define by(x)				[](const auto& a, const auto& b) { return a.x < b.x; }
#define INF                 1e17
#define PI                  acos(-1.0)
#define mid(s,e)            (s+(e-s)/2)
#define clz(n)              __builtin_clzll(n)
#define nbOfBits(n)         __builtin_popcountll(n)
#define all(x)              (x).begin(), (x).end()
#define endl                '\n'
#define pb                  push_back
#define sz(a)               ((int)((a).size()))
#define int                 long long
#define double              long double
#define fi					first
#define se					second
#define getunique(v)		{sort(v.begin(), v.end()); v.erase(unique(v.begin(), v.end()), v.end());}

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

int rnd(int a, int b){
    return a + rand() % (b - a + 1);
}

void solve() {
    int n = rnd(5,10) , k = rnd(0, n);
    cout<<n<<" "<<k<<endl;

    for(int i=0;i<n;i++) {
        char c = 'a' + rnd(0,25);
        cout<<c;
    }
    cout<<endl;
}

void preprocessing() {
    srand(time(0));
}

bool MULTI_TEST_CASES = 0;

// MAIN {{{
int32_t main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout << setprecision(20);
	cout << fixed;

	preprocessing();

	int t = 1;
	if (MULTI_TEST_CASES) cin >> t;

	while (t--) {
		solve();
	}

	return 0;
}





