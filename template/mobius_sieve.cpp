
// [NAME]: Möbius function sieve (linear)
// [PURPOSE]: Computes Möbius mu(n) and least prime factors up to N using linear sieve.
// Typical use: multiplicative function sums, inclusion–exclusion over divisors.
// [COMPLEXITY]:
//   - mobius_sieve(N): O(N) time and O(N) memory
// [USAGE]:
//   - Call mobius_sieve(N); global vectors mu, lp, primes are filled (1-indexed up to N).
//   - mu[1]=1; mu[v]=0 for squares, ±1 otherwise; lp holds smallest prime factor.
vector<int> mu, lp, primes;
void mobius_sieve(int N){
  mu.assign(N+1,0); lp.assign(N+1,0);
  mu[1]=1;
  for(int i=2;i<=N;i++){
    if(!lp[i]) lp[i]=i, primes.push_back(i), mu[i]=-1;
    for(int p:primes){
      long long v=1LL*p*i; if(v> N) break;
      lp[v]=p;
      if(i%p==0){ mu[v]=0; break; }
      else mu[v]=-mu[i];
    }
  }
}

