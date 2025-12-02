template<typename T = int, int B = 31>
struct XorBasis {
    T b[B]; 
    int sz;

    XorBasis() { clear(); }

    void clear() {
        memset(b, 0, sizeof b);
        sz = 0;
    }

    bool insert(T x) {
        for (int i = B - 1; i >= 0; --i) {
            if (!(x >> i & 1)) continue;
            if (!b[i]) {
                b[i] = x;
                ++sz;
                return true;
            }
            x ^= b[i];
        }
        return false; 
    }


    void merge(const XorBasis &o) {
        for (int i = 0; i < B; ++i) {
            if (o.b[i]) insert(o.b[i]);
        }
    }

    bool can(T x) {
        for (int i = B - 1; i >= 0; --i) {
            if (!b[i]) continue;
            x = min(x, x ^ b[i]);
        }
        return x == 0;
    }

    T reduce(T x) {
        for (int i = B - 1; i >= 0; --i) {
            if (!b[i]) continue;
            x = min(x, x ^ b[i]);
        }
        return x;
    }

    T max_xor(T x = 0) {
        for (int i = B - 1; i >= 0; --i) {
            if (!b[i]) continue;
            x = max(x, x ^ b[i]);
        }
        return x;
    }

    T get_max() const { return max_xor(0); }
    int rank() const { return sz; }
    bool empty() const { return sz == 0; }
};
