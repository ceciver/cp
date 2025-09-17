// domain: [XMIN, XMAX). For max: insert (-a,-b,-c) and negate query.
struct LiChaoQuad {
    struct F { int a,b,c; }; // f(x)=a x^2 + b x + c
    struct Node { F fn; Node *l=0,*r=0; Node():fn({0,0,INF}){} Node(F x):fn(x){} };
    static const int INF = (1LL<<62);
    int XMIN, XMAX; Node* rt=0;

    LiChaoQuad(int L,int R): XMIN(L), XMAX(R) {}
    static inline int val(const F& f, int x){
        __int128 xx=(__int128)x*x;
        __int128 v=(__int128)f.a*xx + (__int128)f.b*x + f.c;
        if(v> (__int128)INF) return INF;
        if(v<-(__int128)INF) return -INF;
        return (int)v;
    }
    void add(int a,int b,int c){ ins({a,b,c}, XMIN, XMAX, rt); }
    void add_seg(int a,int b,int c,int L,int R){ if(L<R) ins_seg({a,b,c}, L, R, XMIN, XMAX, rt); }
    int query(int x) const { return get(x, XMIN, XMAX, rt); }

private:
    static void ins(F nw, int L, int R, Node*& t){
        if(!t){ t=new Node(nw); return; }
        int M=L+((R-L)>>1);
        bool mid = val(nw,M) < val(t->fn,M);
        if(mid) swap(nw,t->fn);
        if(R-L==1) return;
        bool lef = val(nw,L)   < val(t->fn,L);
        bool rig = val(nw,R-1) < val(t->fn,R-1);
        if(lef) ins(nw, L, M, t->l);
        if(rig) ins(nw, M, R, t->r);
    }
    static void ins_seg(F nw,int ql,int qr,int L,int R,Node*& t){
        if(qr<=L||R<=ql) return;
        if(ql<=L&&R<=qr){ ins(nw,L,R,t); return; }
        if(!t) t=new Node();
        int M=L+((R-L)>>1);
        ins_seg(nw, ql, qr, L, M, t->l);
        ins_seg(nw, ql, qr, M, R, t->r);
    }
    static int get(int x,int L,int R,const Node* t){
        if(!t) return INF;
        int res=val(t->fn,x);
        if(R-L==1) return res;
        int M=L+((R-L)>>1);
        return min(res, x<M?get(x,L,M,t->l):get(x,M,R,t->r));
    }
};

