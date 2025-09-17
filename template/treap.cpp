struct node {
    node *L, *R;
    int W, S, sm, V;
    bool F;
    node(int x) { L = R = 0; W = rand(); S = 1; sm = x; V = x; F = 0;
    }
};
int size(node *treap) {
    return (treap == 0 ? 0 : treap->S);
}
int sum(node *treap) {
    return (treap == 0 ? 0 : treap->sm);
}
void push(node *treap) {
    if (treap && treap->F) {
        treap->F = 0;
        swap(treap->L, treap->R);
        if (treap->L) treap->L->F ^= 1;
        if (treap->R) treap->R->F ^= 1;
    }
}
void split(node *treap, node *&left, node *&right, int k) {
    if (treap == 0)
        left = right = 0;
    else {
        push(treap);
        if (size(treap->L) < k) {
            split(treap->R, treap->R, right, k - size(treap->L) - 1);
            left = treap;
        }
        else {
            split(treap->L, left, treap->L, k);
            right = treap;
        }
        treap->S = size(treap->L) + size(treap->R) + 1;
        treap->sm = sum(treap->L) + sum(treap->R) + treap->V;
    }
}
void merge(node *&treap, node *left, node *right) {
    if (left == 0) treap = right;
    else if (right == 0) treap = left;
    else {
        push(left);
        push(right);
        if (left->W < right->W) {
            merge(left->R, left->R, right);
            treap = left;
        }
        else {
            merge(right->L, left, right->L);
            treap = right;
        }
        treap->S = size(treap->L) + size(treap->R) + 1;
        treap->sm = sum(treap->L) + sum(treap->R) + treap->V;
    }
}
int find_sum(node *treap, int k) {
    if (treap == 0 || k == 0)
        return 0;
    else {
        push(treap);
        if (size(treap->L) < k) { return sum(treap->L) + treap->V + 
						find_sum(treap->R, k - size(treap->L) - 1); }
        else { return find_sum(treap->L, k); }
    }
}
 
vector<node> nodes;
for(int i=0;i<n;i++)  nodes.emplace_back(node(x));
node *T=&nodes[0];
for(int i=1;i<n;i++)  merge(T, T, &nodes[i]);
/* reverse (l ,r) */
node *A, *B;
split(T, T, A, l);
split(A, A, B, r-l+1);
A->F ^= 1;
merge(T, T, A);
merge(T, T, B);
/* sum (l, r) */ 
find_sum(T, r) - find_sum(T, l-1);
