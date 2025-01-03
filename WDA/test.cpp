#include <iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<bits/stdc++.h>

using namespace std;

// function to check if word/ approximates exist
bool l_dis(string s, string t,int k)
{
    int n=s.size();
    int m=t.size();
	int dp[n+1][m+1];
	for(int i=0;i<=n;i++) dp[i][0]=i;
	for(int j=0;j<=m;j++) dp[0][j]=j;
	int cost;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<=m;j++)
		{
			if(s[i-1]==t[j-1]) cost=0;
			else cost=1;

			dp[i][j]=min(dp[i-1][j-1]+cost,min(dp[i-1][j]+1,dp[i][j-1]+1));
		}
	}
	return (dp[n][m]<=k);
}
int test( int k)
{
   //time_t s,e;
   // time(&s);
    //ios_base::sync_with_stdio(false);
    //cin.tie(NULL);


    //the word to search in dictionary
    string query;

    ifstream indict;
    indict.open("/home/vivek/Desktop/dictionary.txt");
    if(!indict)
    {
        cout<<"reading fail";
        exit(1);
    }
    string temp;

    //// vector to store the resulting words
    vector<string > res;

    cout<<"search word  : ";cin>>query;
    //int i=0;
    bool found=false;
   // int k;
   // cout<<"Enter the allowed Lavenstein distance ";cin>>k;
    while(indict>>temp)
    {
        if(l_dis(query,temp,k))
        {
            found=true;
            res.push_back(temp);
        }
    }
    if(!found)
    cout<<"the word does not exist in the dictionary \n";
    indict.close();
    if(k==0&&found)
    {
        cout<<"Search result  : "<<query<<" word found"<<"\n";
        cout<<" Search outcome  : "<<query<<"\n";
    }
    if(k>0&&found)
    {
        int sz=res.size();
        cout<<"Search result  : "<<query<<" word found with "<<sz<<" approximates \n";
        cout<<"search outcome  : "<<query<<"\n";
        cout<<"Approximate ("<<sz<<")  : ";
        for (int j=0;j<sz;j++)
        {
            cout<<res[j]<<" ";
        }
        cout<<"\n\n\n";
    }
    res.clear();

    //time(&e);
    //cout<<endl;
    //double time_taken=double(e-s);
    return 0;
}
