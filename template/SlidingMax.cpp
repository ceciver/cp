struct SlidingMax {
	deque<pair<int, int>> deq;
	int lef = 0, rig = -1;
	void pushRight(int val) {
		while (!deq.empty() && deq.back().first <= val) deq.pop_back();
		deq.emplace_back(val, ++rig);
	}
	void popLeft() {
		assert(lef <= rig);
		if (deq.front().second == lef) deq.pop_front();
		++lef;
	}
	inline int size() { return rig - lef + 1; }
	inline int getMax() { assert(!deq.empty()); return deq.front().first; }
};
