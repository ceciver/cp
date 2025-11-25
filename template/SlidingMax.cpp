// [NAME]: Sliding window maximum deque
// [PURPOSE]: Maintains maximum over a moving window with amortized O(1) updates.
// Typical use: process array windows while moving pointers (two-pointer/Mo's).
// [COMPLEXITY]:
//   - pushRight/popLeft: O(1) amortized
//   - getMax: O(1)
//   - memory: O(window size)
// [USAGE]:
//   - pushRight(val) when right endpoint increases; popLeft() when left endpoint moves right.
//   - getMax() returns current maximum; indices tracked internally via lef/rig counters (0-based progression).
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
