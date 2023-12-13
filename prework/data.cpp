#include <algorithm>
#include<bits/stdc++.h>
#include <string>
using namespace std;
#define fr(i,a,b) for(int i=a;i<=b;++i)
#define rf(i,a,b) for(int i=a;i>=b;--i)
template<typename T>
inline void read(T &X){
    X=0; bool fh = 0; char c=getchar();
    while(!isdigit(c)) fh |= (c=='-'), c=getchar();
    while(isdigit(c)) X = (X<<1) + (X<<3) + (c^48), c = getchar();
    X = (fh? -X : X);
}
int a[10005];
int main(){
    #ifdef OJ
    freopen("input.txt","r",stdin);
    #endif
    int n = 100;
    freopen("a.txt","w",stdout);
    printf("%d\n",n/2*3+1);
    std::string op = "a";
    fr(i,1,6)op+=op;

    fr(i,1,n)a[i]=i;
    random_shuffle(a+1,a+n+1);
    fr(i,1,n){
      printf("insert %s %d\n", op.c_str(), a[i]);
    }
    random_shuffle(a+1,a+n+1);
    fr(i,1,n/2) {
      printf("delete %s %d\n",op.c_str(), a[i]);
    }
    printf("find %s",op.c_str());
    
    return 0;
}