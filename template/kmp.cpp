vi pi(const string& s) {
	vi p(sz(s));
	for(int i=1;i<sz(s);i++) {
		int g = p[i-1];
		while (g && s[i] != s[g]) g = p[g-1];
		p[i] = g + (s[i] == s[g]);
	}
	return p;
}

vi match(const string& s, const string& pat) {
	vi p = pi(pat + '\0' + s), res;
	for(int i=sz(p)-sz(s);i<sz(p);i++)
		if (p[i] == sz(pat)) res.push_back(i - 2 * sz(pat));
	return res;
}

vvi advance(n+1, vi(26));
vi ps = pi(s);
{
    for(int c='a';c<='z';c++) {
        advance[0][c-'a'] = s[0] == c;
    }
    for(int k=1;k<=n;k++) {
        for(char c='a';c<='z';c++) {
            if(k < n && s[k] == c) {
                advance[k][c-'a'] = k + 1;
                continue;
            }
            advance[k][c-'a'] = advance[ps[k-1]][c-'a'];
        }
    }
}
