#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string>
#include<string.h>
#include<bits/stdc++.h>

using namespace std;

//deserialize the recieved buffer
void deserialise(vector<string >&res, char* buffer, int cnt)
{
    for(int i=0;i<cnt;i++)
    {
        const char*begin= &buffer[i];
        int siz=0;
        while(buffer[i++])
        {
            siz +=1;
        }
        res.push_back(string(begin, siz));
    }
}

 int client()
 {
    //create a socket
    int sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock==-1)
    {
        cout<<"socket not created"<<endl;
        return 1;
    }

    //create a hint structure for the server we are connecting with
    int port=54000;
    string ipaddress= "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port=htons (port);
    inet_pton(AF_INET, ipaddress.c_str(),&hint.sin_addr);

    //connect to the server on the socket
    int connectres=connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(connectres==-1)
    {
        cout<<"Client not connected"<<endl;
        return 1;
    }

    // while loop if sending inputs in a stream:
    char buf[4096];
    string userinput;

    /*send the lv distance ...// in case if we want to use same lavensthein distance for all words in one go.
    string ks;
    cout<<"distance (k) = ";
    cin>>ks;
    int sendk=send(sock,ks.c_str(),ks.size()+1,0);
    int num=1;
    */

    do {

        //send the lv distance
        string ks;
        cout<<"distance (k) = ";
        cin>>ks;
        int sendk=send(sock,ks.c_str(),ks.size()+1,0);
        int num=1;
        ks.erase();

    //   enter the query to find its approximate;
        cout<<"Search for > ";
        cin>>userinput;

    //  time start
        clock_t strt,en;

        strt=clock();
    //   send to the server
        int sendres=send(sock,userinput.c_str(),userinput.size()+1, 0);
        if(sendres==-1)
        {
             cout<<" could not send to the server "<<endl;
        //     continue;
        }

    //   wait to response
        memset(buf,0,4096);
        int bytesrecv=recv(sock,buf,4096,0);

        en=clock();

    //deserialize the recieved buffer;
        vector<string > res;
        deserialise(res,buf,bytesrecv);

    //the time taken by program
        double time_taken = double(en - strt)/double(CLOCKS_PER_SEC);

    //check the size of the resulting vector
        int sz=res.size();
        if(sz>1)
        {
            cout<<userinput<<" word found with "<<res.size()<<" approximates, time taken "<<fixed<<time_taken*1000<<" ms\n";
        }
        else if(sz==1&&res[0]==userinput)
        {
            cout<<userinput<<" word found, time taken "<<fixed<<time_taken*1000<<" ms\n";
        }
        else
        {
            //.the word does not exist in the dictionary.
        }



    //Print the resulting vector
        for(int i=0;i<res.size();i++)
        {
            cout<<res[i]<<" ";
        }
        cout<<"\n\n";

    //clear the vector./
        res.clear();

    //cout<<"result is > "<<string(buf,bytesrecv)<<"\r\n";
} while(true);

    //close the socket
    close(sock);

    return 0;
 }
