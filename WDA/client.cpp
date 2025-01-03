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
    // while loop:
    char buf[4096];
     
    string userinput; // the word which will be searched in dictionary

    //send the lv distance
    string ks;
    cout<<"distance (k) = ";
    cin>>ks;
    int sendk=send(sock,ks.c_str(),ks.size()+1,0);
    int num=1;

    do {
    //  
        cout<<"Search for > ";
        cin>>userinput;

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

    //deserialize the recieved buffer;
        vector<string > res;
        deserialise(res,buf,bytesrecv);

        cout<<userinput<<" word found with "<<res.size()<<" approximates\n";

    //Print the resulting vector
        for(int i=0;i<res.size();i++)
        {
            cout<<res[i]<<" ";
        }
        cout<<"\n";
    //clear the vector
        res.clear();
        
} while(true);

    //close the socket
    close(sock);

    return 0;
 }
