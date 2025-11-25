// [NAME]: Berlekamp–Massey algorithm
// [PURPOSE]: Finds the shortest linear recurrence for a sequence over a field (works for mod primes or doubles with care).
// Typical use: predict future terms, compute n-th term via linear recurrences.
// [COMPLEXITY]:
//   - run on sequence length m: O(m^2)
//   - memory: O(m)
// [USAGE]:
//   - Call berlekampMassey(sequence) to obtain coefficients c so that s[k] = sum c[i]*s[k-i-1].
//   - Works with a type T supporting +, -, *, / and equality to 0.
template<typename T>
vector<T> berlekampMassey(const vector<T> &s) {
    vector<T> c;    
    vector<T> oldC; 
    int f = -1;    
    for (int i=0; i<(int)s.size(); i++) {
        T delta = s[i];
        for (int j=1; j<=(int)c.size(); j++)
            delta -= c[j-1] * s[i-j]; 
        if (delta == 0)
            continue;   
        if (f == -1) {
            c.resize(i + 1);
            mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
            for (T &x : c)
                x = rng();  
            f = i;
        } else {
            vector<T> d = oldC;
            for (T &x : d)
                x = -x;
            d.insert(d.begin(), 1);
            T df1 = 0;  // d(f + 1)
            for (int j=1; j<=(int)d.size(); j++)
                df1 += d[j-1] * s[f+1-j];
            assert(df1 != 0);
            T coef = delta / df1;   
            for (T &x : d)
                x *= coef;
            vector<T> zeros(i - f - 1);
            zeros.insert(zeros.end(), d.begin(), d.end());
            d = zeros;
            vector<T> temp = c; 
            c.resize(max(c.size(), d.size()));
            for (int j=0; j<(int)d.size(); j++)
                c[j] += d[j];
            if (i - (int) temp.size() > f - (int) oldC.size()) {
                oldC = temp;
                f = i;
            }
        }
    }
    return c;
}
