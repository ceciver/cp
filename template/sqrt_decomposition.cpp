    // preprocessing
    int len = (int) sqrt (n + .0) + 1; // size of the block and the number of blocks

    vector<int> b (len);
    for (int i=0; i<n; ++i)
        b[i / len] += a[i];

    // answering the queries
    for (;;) {
        int l, r;
      
        int sum = 0;
        int c_l = l / len,   c_r = r / len;
        if (c_l == c_r)
            for (int i=l; i<=r; ++i)
                sum += a[i];
        else {
            for (int i=l, end=(c_l+1)*len-1; i<=end; ++i)
                sum += a[i];
            for (int i=c_l+1; i<=c_r-1; ++i)
                sum += b[i];
            for (int i=c_r*len; i<=r; ++i)
                sum += a[i];
        }
    }
// [NAME]: Sqrt decomposition for range sum (template snippet)
// [PURPOSE]: Precomputes block sums of an array to answer range queries in ~O(sqrt n) with simple updates.
// Typical use: static/rarely-updated arrays with moderate constraints where segment tree is overkill.
// [COMPLEXITY]:
//   - preprocessing: O(n)
//   - query(l,r): O(sqrt n)
//   - memory: O(sqrt n)
// [USAGE]:
//   - Choose block size len ≈ sqrt(n); fill block sums b[i/len] over base array a.
//   - For a query [l,r], sum partial blocks then full blocks as shown; add update logic if needed.
//   - Variables n,a must be defined externally; snippet uses 0-based inclusive ranges.
