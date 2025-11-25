// [NAME]: Manacher's algorithm for palindromic substrings
// [PURPOSE]: Computes longest palindrome radius centered at each position to answer palindrome queries quickly.
// Typical use: precompute palindromic lengths in O(n) for substring checks.
// [COMPLEXITY]:
//   - manacher_odd / manacher: O(n)
//   - memory: O(n)
// [USAGE]:
//   - Call manacher_odd(s) for odd-length radii on original string; manacher(s) returns radii on interleaved string for even/odd.
//   - Use returned vector `pal` with helper `is_pal(l,r)` example (0-based, inclusive r) to test palindromes in O(1).
vector<int> manacher_odd(string s) {
    int n = s.size();
    s = "$" + s + "^";
    vector<int> p(n + 2);
    int l = 1, r = 1;
    for(int i = 1; i <= n; i++) {
        p[i] = max(0LL, min(r - i, p[l + (r - i)]));
        while(s[i - p[i]] == s[i + p[i]]) {
            p[i]++;
        }
        if(i + p[i] > r) {
            l = i - p[i], r = i + p[i];
        }
    }
    return vector<int>(begin(p) + 1, end(p) - 1);
}
vector<int> manacher(string s) {
    string t;
    for(auto c: s) {
        t += string("#") + c;
    }
    auto res = manacher_odd(t + "#");
    return vector<int>(begin(res) + 1, end(res) - 1);
}

/*
    vi pal = manacher(s);
    auto is_pal = [&](int l, int r) {
        int len = r - l + 1;
        return pal[l + r] >= len;
    };
*/
