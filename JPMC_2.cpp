#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <math.h>
#include <algorithm>

using namespace std;
#define ll long long
#define inf 1000000000
#define mod 1000000009

ll factorial(ll n)  {
    if(n==0)
        return 1;
    ll res=1;
    for(ll i = 2; i<=n; i++)
        res*=i;
    return res;
}

ll nCr(ll n, ll r)  {
    if(n<r)
        return 0;
    ll res=1;
    for(ll i = max(r,n-r)+1; i<=n; i++)
        res*=i;
    return res/factorial(min(r,n-r));
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    long long testCases=1;
    // cin>>testCases;
    while(testCases--)  {
        //take input

        //solve

        //print output
    }
    return 0;
  }