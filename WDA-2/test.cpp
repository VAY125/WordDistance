#include <iostream>
#include<cstdlib>
#include<string>
#include<fstream>
#include<chrono>
#include<bits/stdc++.h>

using namespace std;

// function to check if word/ approximates exist
bool l_dis(string s, string t,int k)
{
// valid is a boolean value to check if the word in the dictionary falls in the range
// if it falls in range, then we proceed with dp algorithms, otherwise return false.
    bool valid=true;

    int n=s.size();  //size of the query word.
    int m=t.size();  //size of the word in dictionary.

// condition to check if the word in dictionary is in range
    if((m>n+k)||(m<n-k))
    {
        valid=false;
    }

    if(valid)
    {
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
	return false;
}


int test( int k)
{

    //ios_base::sync_with_stdio(false);
    //cin.tie(NULL);
    //time variables
    clock_t strt,en;

    //the word to search in dictionary
    string query;

    ifstream indict;
    indict.open("/home/vivek/Desktop/dictionary.txt"); // this path is different in different systems, write correct path where the dictionary.txt is located in your system
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

    //start of the search function
    strt=clock();

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

    en=clock();

    // time taken by the function
    double time_taken = double(en - strt)/double(CLOCKS_PER_SEC);

    indict.close();
    if(k==0&&found)
    {
        cout<<"Search result  : "<<query<<" word found, time taken "<<fixed<<time_taken<< " ms "<<setprecision(5)<<"\n";
        cout<<" Search outcome  : "<<query<<"\n";
    }
    if(k>0&&found)
    {
        int sz=res.size();
        cout<<"Search result  : "<<query<<" word found with "<<sz<<" approximates, time taken "<<fixed<<time_taken<< " ms "<<setprecision(5)<<"\n";
        cout<<"search outcome  : "<<query<<"\n";
        cout<<"Approximate ("<<sz<<")  : ";
        for (int j=0;j<sz;j++)
        {
            cout<<res[j]<<" ";
        }
        cout<<"\n\n\n";
    }
    res.clear();

    //cout<<endl;
    //double time_taken=double(end-start);
    return 0;
}
