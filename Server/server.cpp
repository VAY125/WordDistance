#include<iostream>
#include<sys/types.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<string>
#include<string.h>
#include<fstream>
#include<cstdlib>
#include<bits/stdc++.h>

using namespace std;

bool lv_dis(string s, string t,int k)
{
    bool valid=true;

    int n=s.size();
    int m=t.size();

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

// serialize the vector to make it possible to send
char* serialize(vector<string>&v, int *cnt)
{

// find total character count
    int tcnt=0;
    for(int i=0;i<v.size();i++)
    {
        tcnt+=v[i].length()+1;
    }

//create a buffer to store all the values
    char* buffer=new char[tcnt];
    int idx=0;
    for(int i=0;i<v.size();i++)
    {
        string s=v[i];
        for(int j=0;j<s.size();j++)
        {
            buffer[idx ++]=s[j];
        }
        buffer[idx ++]=0;
    }
    *cnt=tcnt;

    return buffer;
}



int main()
{
    //create a socket
    int listening =socket(AF_INET , SOCK_STREAM, 0);
    if(listening==-1)
    {
        cout<<" can't create the socket";
        return -1;
    }

    //bind the socket to a IP/port
    sockaddr_in hint;
    hint.sin_family= AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    if(bind(listening,(sockaddr*)&hint, sizeof(hint))==-1)
    {
        cout<<"can't bind to the port";
        return -2;
    }

//mark the socket for listening in
    if(listen(listening , SOMAXCONN)==-1)
    {
        cout<<" can't listen";
        return -3;
    }

//accept a call
    sockaddr_in client;
    socklen_t clientsize;
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    int clientsocket=accept(listening, (sockaddr*)&client, &clientsize);
    if(clientsocket==-1)
    {
        cout<<" Problem with client connecting";
        return -4;
    }

// close the listening socket
    close(listening);
    memset(host,0, NI_MAXHOST);
    memset(svc,0, NI_MAXSERV);

    int result=getnameinfo((sockaddr*)&client, sizeof(client),host,NI_MAXHOST,svc,NI_MAXSERV,0);

    if(result)
    {
        cout<<host<<" connected on "<<svc<<endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout<<host<<" connected on "<<ntohs(client.sin_port) <<endl;
    }
//while recieving display the message
    char buf[1024];

/*recieve the lv-distance    // this section can be used if we want to search for a fixed lavensthein distance for all words
     memset(buf,0,1024);
     int krecv=recv(clientsocket,buf,1024,0);
     string ks=string(buf,0,krecv);
     int k=int(ks[0]-'0');
     cout<<"recieved distance (k) = "<<k<<"\n";
*/
  while(true)
   {

   // recieve the lv-distance
     memset(buf,0,1024);
     int krecv=recv(clientsocket,buf,1024,0);
     string ks=string(buf,0,krecv);
     int k=int(ks[0]-'0');
     cout<<"recieved distance (k) = "<<k<<"\n";

     ks.erase();
    //clear the buffer
        memset(buf,0,1024);

    //wait for a message
        int bytesrecv =recv(clientsocket, buf, 1024,0);
        if(bytesrecv==-1)
        {
            cerr<< " there was a connection issue \n";
        //    break;
        }
        if(bytesrecv==0)
        {
            cout<< " the client disconnected \n";
         //   break;
        }

    // display message
        cout<<"recieved : " <<string (buf, 0, bytesrecv)<<"\n\n";

    //the word to search in dictionary
        string word=string(buf, 0, bytesrecv);

    //import the dictionary
        ifstream indict;
        indict.open("/home/vivek/Desktop/dictionary.txt");
        if(!indict)
        {
            send(clientsocket,"reading fail",13,0);
            exit(1);
        }
        int i=0;
        bool found=false;
       // int k=1;
        string temp;
        vector<string> v; // to store possible results
        while(indict>>temp)
        {
            if(lv_dis(word,temp,k))
            {
                found=true;
                //int len=temp.size();

                v.push_back(temp);

                //send the word to client
                //send(clientsocket, temp, bytesrecv+1,0 );
            }
        }
        if(!found)
        send(clientsocket, "the word does not exist in the dictionary",42,0);
        indict.close();

        if(found)
        {
            int cnt=0;
            char* buffer=serialize(v,&cnt);
        //send this buffer to the client
            send(clientsocket,buffer,cnt*sizeof(char),0);
        }

        //clear the vector v

        v.clear();

       // send(clientsocket, buf, bytesrecv+1,0 );
        //send

    }
    // close the socket
    close(clientsocket);

    //clientsocket, buf, bytesrecv+1,0
    // command is telenet localhost 54000
}

