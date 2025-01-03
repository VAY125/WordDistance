#include <bits/stdc++.h>
#include "test.h"
#include "client.h"

using namespace std;

int main()
{

    string ip;
    //getchar();
    cout<<"Interface = ";cin>>ip;

    if(ip=="CMD")
    {
    do{
        int d;
        cout<<"Distance = ";cin>>d;

        test(d);
    }while(true);
    }
    if(ip=="NET")
    {
        //request to open the server
        cout<<"Waiting for remote connection \n ";
        getchar();

        client();
    }

    return 0;
}
