
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


