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

void solve() {
    int n, k;
    cin>>n>>k;

    string s; cin>>s;

    vector<set<int>> bank(26);
    multiset<pii> available;

    for(int i=0;i<n;i++) {
        bank[s[i]-'a'].insert(i);
        available.insert({s[i] - 'a', i});
    }

    string t = "";

    multiset<int> free;

    multiset<int> used;


    for(int i=0;i<n;i++) {
        debug(i, k);
        int mn = INF;

        if(!free.empty()) mn = min(mn, *free.begin());
        if(!available.empty()) mn = min(mn, available.begin()->fi);

        if(!available.empty() && available.begin()->se == i && available.begin()->fi == mn) { // this one is free
            t += char(mn + 'a');
            available.erase(available.begin());
            bank[mn].erase(i);
            continue;
        }

        if((k == 0 && used.count(i) == 0)) { // this one should be fixed
            t += s[i];
            bank[s[i]-'a'].erase(i);
            available.erase({s[i]-'a', i});
            continue;
        }



        if(used.count(i) == 0) {
            k--;
            free.insert(s[i]-'a');
            bank[s[i]-'a'].erase(i);
            available.erase({s[i]-'a', i});
        }


        if(!free.empty() && *free.begin() == mn) {
            t += *free.begin() + 'a';
            free.erase(free.begin());
            continue;
        }

        if(k == 0) {
            assert(!free.empty());
            if(*free.begin() == (s[i]-'a')) k++;
            t += *free.begin() + 'a';
            free.erase(free.begin());
            continue;
        }


        k--;
        int j = *bank[mn].rbegin();
        assert(j > i);

        t += char(mn + 'a');

        available.erase({mn, j});
        bank[mn].erase(j);

        used.insert(j);

    }

    cout<<t<<endl;


}

void preprocessing() {}

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






