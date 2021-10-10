struct Trie {
	vvi nxt;
	vb is_terminal;
	int id = 0;

	Trie() {
		nxt = vvi(1,vi(26));
		is_terminal = vb(1);
	}

	void insert(const string& s) {
		int node = 0;
		for(char c: s) {
			if(nxt[node][c-'a'] == 0) {
				nxt.pb(vi(26));
				is_terminal.pb(false);
				nxt[node][c-'a'] = ++id;

			}
			node = nxt[node][c-'a'];
		}
		is_terminal[node] = true;
	}
};



