#include<bits/stdc++.h>
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
bool mp[10005];
int main(){
    #ifdef OJ
    freopen("input.txt","r",stdin);
    #endif
    freopen("a222.txt","r",stdin);
    int a;
    int cnt=0;
    while(scanf("%d",&a)==1) {
      mp[a]=1;
      ++cnt;
    }
cout<<cnt;
    return 0;
}