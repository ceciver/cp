// [NAME]: Link-Cut Tree (splay-based)
// [PURPOSE]: Dynamic tree data structure supporting link/cut queries and path aggregate (sum) updates in amortized logarithmic time.
// Typical use: maintain forests with queries on arbitrary paths as edges are added/removed online.
// [COMPLEXITY]:
//   - link/cut/make_root/access: O(log n) amortized
//   - query_path/set_val/add_val: O(log n) amortized
//   - memory: O(n)
// [USAGE]:
//   - init(N) prepares nodes 1..N (0 is null); set val[x] before queries if needed.
//   - link(a,b) connects different trees; cut(a,b) or cut_parent(x) removes edges; connected() checks same tree.
//   - query_path(a,b) returns sum along path; set_val/add_val modify node values; lca(a,b) available after connectivity check.
using ll = long long;

struct LCT {
    int n;
    vector<array<int,2>> ch;
    vector<int> p;         
    vector<int> rev;      
    vector<ll> val, sum; 
    vector<int> sz;
    vector<int> vir;

    LCT(int N=0){ init(N); }

    void init(int N){
        n = N;
        ch.assign(n+1, {0,0});
        p.assign(n+1, 0);
        rev.assign(n+1, 0);
        val.assign(n+1, 0);
        sum.assign(n+1, 0);
        sz.assign(n+1, 1);   // each node counts as 1
        vir.assign(n+1, 0);  // no virtual children initially
    }


    bool is_root(int x){
        int f = p[x];
        return f==0 || (ch[f][0]!=x && ch[f][1]!=x);
    }
    int dir(int x){ int f=p[x]; return (ch[f][1]==x); } 
    void push(int x){
        if(!x || !rev[x]) return;
        swap(ch[x][0], ch[x][1]);
        if(ch[x][0]) rev[ch[x][0]] ^= 1;
        if(ch[x][1]) rev[ch[x][1]] ^= 1;
        rev[x] = 0;
    }

    void pull(int x){
        sum[x] = val[x];
        if(ch[x][0]) sum[x] += sum[ch[x][0]];
        if(ch[x][1]) sum[x] += sum[ch[x][1]];
        sz[x] = 1;
        if(ch[x][0]) sz[x] += sz[ch[x][0]];
        if(ch[x][1]) sz[x] += sz[ch[x][1]];
        sz[x] += vir[x];            // add sizes of virtual children
    }


    void push_path(int x) {
        static vector<int> stk; stk.clear();
        int y=x; stk.push_back(y);
        while(!is_root(y)){ y=p[y]; stk.push_back(y); }
        for(int i=(int)stk.size()-1;i>=0;i--) push(stk[i]);
    }
    void rotate(int x){
        int y = p[x], z = p[y];
        int dx = dir(x);
        int b = ch[x][dx ^ 1];
        if (!is_root(y)) { ch[z][dir(y)] = x; }
        p[x] = z;
        ch[x][dx ^ 1] = y;   p[y] = x;
        ch[y][dx]     = b;   if (b) p[b] = y;
        pull(y); pull(x);
    }

    void splay(int x){
        push_path(x);
        while(!is_root(x)){
            int y=p[x];
            if(!is_root(y)){
                if(dir(x)==dir(y)) rotate(y);
                else rotate(x);
            }
            rotate(x);
        }
    }

    int access(int x){
        int last = 0;
        for(int y = x; y; y = p[y]){
            splay(y);
            if (ch[y][1]) vir[y] += sz[ch[y][1]];  // old preferred becomes virtual
            ch[y][1] = last;
            if (last)     vir[y] -= sz[last];      // new preferred stops being virtual
            pull(y);
            last = y;
        }
        splay(x);
        return last;
    }

    void make_root(int x){
        access(x);
        rev[x] ^= 1;
        push(x);
    }
    int find_root(int x){
        access(x);
        while(push(x), ch[x][0]) x = ch[x][0];
        splay(x);
        return x;
    }
    bool connected(int a, int b){
        if(a==0 || b==0) return false;
        return find_root(a) == find_root(b);
    }

    bool link(int a, int b){
        make_root(a);
        if (find_root(b) == a) return false; // would create cycle
        access(b);                // splay(b), ch[b][1] becomes 0
        p[a] = b;
        ch[b][1] = a;             // attach as preferred child
        pull(b);
        return true;
    }

    bool cut(int a, int b){
        make_root(a); access(b);
        if(ch[b][0]==a && p[a]==b && ch[a][1]==0){
            ch[b][0] = 0; p[a] = 0;
            pull(b);
            return true;
        }
        return false; 
    }
    bool cut_parent(int A) {
        access(A);   
        if (!ch[A][0]) return false;
        int U = ch[A][0];
        ch[A][0] = 0;   
        p[U] = 0;      
        pull(A);
        return true;
    }

    int lca(int a, int b){
        if(!connected(a,b)) return 0;
        access(a);
        return access(b);
    }

    void split(int a, int b){
        make_root(a);
        access(b); 
    }

    ll query_path(int a, int b){
        split(a,b);
        return sum[b];
    }

    void set_val(int x, ll v){
        access(x);
        val[x] = v;
        pull(x);
    }

    void add_val(int x, ll delta){
        access(x);
        val[x] += delta;
        pull(x);
    }

    int component_size(int x){
        make_root(x);
        access(x);  
        return sz[x];
    }

};
