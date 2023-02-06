#include <iostream>
#include <queue>
#include <cstdio>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include<map>
#include <math.h>
#include <utility>
#include<iomanip>
#include <numeric>


using namespace std;
//modification for prioprity queue sorting used in aging
struct less_then_greater {
    template<typename T, typename U>
    bool operator()(T const& lhs, U const& rhs) const {
        if (lhs.first < rhs.first)
            return true;
        else
        return false;
    }
};
int main ()
{
char * line { new char { '\0' } };
vector<string> lines;
vector <string> processes;
map<string, vector<int>> mix;
vector <string> rr;
queue <string> mainProcesses;
queue <string> spare;
vector <int> arrive;
vector <int> service;
vector <int> back_up;
vector <int> finish;
vector <int> turnaround;
vector <double> normturn;
int counter=0;
int trace = 0;
vector <int> cpu;
vector <int> quantum;
int flag=0;
string name;

// taking input from user

cin >> line;
if(strcasecmp(line, "trace") == 0)
{
    trace = 1;
}

char *sav1 = NULL;
char *sav2 = NULL;
cin >> line;

// tokenize input to get required implementation
char * token = strtok_r(line, ",", &sav1);
while(token != NULL)
{

    if(strstr(token, "-") != NULL)
    {
// tokanize to get quantum for RR and Aging
         char * token2  = strtok_r(token, "-", &sav2);

        if(stoi(token2) == 2)
        {
            cpu.push_back(2);
            token2 = strtok_r(NULL, "-", &sav2);

            quantum.push_back(stoi(token2));

        }
        else
        {
            cpu.push_back(8);
            token2 = strtok_r(NULL, "-", &sav2);

            quantum.push_back(stoi(token2));
        }

    }
    else
    {
        cpu.push_back(stoi(token));

    }
    token = strtok_r(NULL, ",", &sav1);
}



cin >> line;
int timespan = stoi(line);

cin >> line;
int num = stoi(line);

//take in input process with arrival and service time 

for(int j = 0; j < num; j++)
{
    cin >> line;
    string T;
    string P;
    stringstream s(line);
    getline(s, P, ',');
  processes.push_back(P);
    getline(s, T, ',');
    mix[P].push_back(stoi(T));
  arrive.push_back(stoi(T));
    getline(s, T, ',');
    mix[P].push_back(stoi(T));
    service.push_back(stoi(T));
    back_up.push_back(stoi(T));
}
    arrive.push_back(timespan);
    
    // loop over required implementations
	for(int u = 0; u < cpu.size(); u++)
    {
    
    //reset for data structures used in each implementation
        finish.clear();
        turnaround.clear();
        normturn.clear();
        rr.clear();
        mix.clear();
        for(int h=0; h<processes.size();h++)
        {

            mix[processes.at(h)].push_back(arrive.at(h));
             mix[processes.at(h)].push_back(back_up.at(h));
        }
        
        
        if(cpu.at(u) == 1)
        {
            name="FCFS";
            //each process pushed by it's service time sequentially
            for(int m = 0; m < processes.size(); m++)
            {
                for(int j = 0; j < back_up.at(m); j++)
                {
                    rr.push_back(processes.at(m));
                }
            }

        }
        if(cpu.at(u) == 2)
        {
            name="RR-"+to_string(quantum.at(counter));
            int t;
            int j=0;
            int k =0;
           
           //loop over timespan
            while(k<timespan)
            {
                // check if there is a process arrived before or at this time k
                while (arrive.at(j)<=k)
                {
                       // push process in queue by quantum or by service time if it is smaller than quantum
                    if (mix[processes.at(j)].at(1)<quantum.at(counter))
                    {

                        for(t =0;t<back_up.at(j);t++)
                        {

                            spare.push(processes.at(j));
                            
                             mix[processes.at(j)].at(1)--;  //decrement service time when pushed 

                        }

                    }
                    else{
                            for(t =0;t<quantum.at(counter);t++)
                        {

                            spare.push(processes.at(j));
                             mix[processes.at(j)].at(1)--;
                        }


                        }
                     j++;

                }

                    //push currently running process in ready queue if service time !=0
                     if(rr.size()!=0){
                        if(mix[rr.back()].at(1)<quantum.at(counter))
                        {
                               for(t =0;t<mix[rr.back()].at(1);t++)
                        {
                             spare.push(rr.back());


                        }

                             mix[rr.back()].at(1)=0;

                        }
                         else{
                             for(t =0;t<quantum.at(counter);t++)
                        {
                            spare.push(rr.back());
                             mix[rr.back()].at(1)--;

                        }

                    }
                     }

                      // pop process from ready queue and increment time k by quantum or by service time if it is smaller than quantum
                      for(t=0;t<quantum.at(counter); t++)
                      {
                          if(t==0){
                            rr.push_back(spare.front());
                            spare.pop();
                            k++;
                          }else
                          {
                          if(rr.back()==spare.front())
                          {
                              rr.push_back(spare.front());
                              spare.pop();
                              k++;
                          } else
                          {
                              break;
                          }
                          }
                      }


                }


                 counter++;
            }
        if(cpu.at(u) == 3)
        {
            name="SPN";
            int t;
            int j=0;
            int k =0;
            while(k<timespan){
            auto it = mix.begin();
            string mini= it->first;
            for(;it!= mix.end();it++)
            { 
            // check which process has minimum service time
                if(mix[it->first].at(0)<=k)    // check if process arrived before or at time k
                {
                    if((mix[it->first].at(1)<mix[mini].at(1)))
                    {
                        mini=it->first;
                    }
                }
            }
             // run process with shortest sevice time
            for(t=0;t<mix[mini].at(1);t++)
            {
                rr.push_back(mini);
                k++;
            }
            mix.erase(mini);
            }


        }
        if(cpu.at(u) == 4)
        {
            name="SRT";
            int t;
            int j=0;
            int k =0;
            for(k=0;k<timespan;k++){
            auto it = mix.begin();
            string mini= it->first;
            
            // check which process has minimum remaining time
            for(;it!= mix.end();it++)
            {
                if(mix[it->first].at(0)<=k)
                {
                    if((mix[it->first].at(1)<mix[mini].at(1)))
                    {
                        mini=it->first;
                    }
                }
            }

                rr.push_back(mini);  // run process with smallest remaining time
                mix[mini].at(1)--;  // decrement sercive time of currently running process
                //remove process when it is done
                if(mix[mini].at(1)==0)  
                {
                    mix.erase(mini);
                }



            }


        }
        if(cpu.at(u) == 5)
        {
            name="HRRN";
            int t;
            int j=0;
            int k =0;
            while(k<timespan){
            auto it = mix.begin();
            string maxi= it->first;
            for(;it!= mix.end();it++)
            {
            // check which process with highest response ratio
                if(mix[it->first].at(0)<=k)
                {
                   if((((k-mix[it->first].at(0))+mix[it->first].at(1))/(float)mix[it->first].at(1))>(((k-mix[maxi].at(0))+mix[maxi].at(1))/(float)mix[maxi].at(1)))
                    {
                        maxi=it->first;
                    }
                }
            }

            for(t=0;t<mix[maxi].at(1);t++)
            {
                rr.push_back(maxi);
                k++;
            }
            mix.erase(maxi);
            }

        }
        if(cpu.at(u) == 6)
        {
            name="FB-1";
            vector <queue<string>> queues;
            queues.push_back(queue<string>());
            map <string,int> indices;
            int t;
            int j;
            int k;
            for(k = 0; k < timespan; k++)
            {
                auto it = mix.begin();
                for(;it!= mix.end();it++)
                {   // push process arrived at time k into Q0
                    if(mix[it->first].at(0)==k)
                    {
                        queues.at(0).push(it->first);
                        indices[it->first] = 0;   // save index of queue holding process
                    }
                }

                for(t = 0; t < queues.size(); t++)
                {
                    // loop over queues untill finding 1st unempty queue
                    
                    if(!queues.at(t).empty())
                        {
                        
                        //push currently running process in it's next queue and run process in 1st unempty queue 
                        
                            if(rr.size()!=0)
                        {
                            if(mix[rr.back()].at(1)!=0)
                            {
                                if(indices[rr.back()] + 1 == queues.size())
                                {
                                    queues.push_back(queue<string>());
                                    queues.at(queues.size()-1).push(rr.back());

                                }
                                else
                                {
                                    queues.at(indices[rr.back()] + 1).push(rr.back());
                                }

                                indices[rr.back()]+=1;
                            }

                                rr.push_back(queues.at(t).front());
                                mix[queues.at(t).front()].at(1) --;
                                queues.at(t).pop();

                                break;


                        }

                        }
                }
              
                if(t == queues.size())  // if all queues are empty run currently running process again

                {
                     if(rr.size()!=0)
                        {
                     rr.push_back(rr.back());

                    }
                    else
                    {
                        rr.push_back(queues.at(0).front());
                        queues.at(0).pop();

                    }

                     mix[rr.back()].at(1) --;
            
                }
            }
        }
        if(cpu.at(u) == 7)
        {
            name="FB-2i";
 vector <queue<string>> queues;
            queues.push_back(queue<string>());
            map <string,int> indices;
            int t;
            int j=0;
            int k=0;
            auto it = mix.begin();
            while(k < timespan)
            {

                while(it!= mix.end())
                {
                    if(mix[it->first].at(0)<=k)
                    {
                        queues.at(0).push(it->first);
                        indices[it->first] = 0;
                    }
                    else
                    {
                        break;
                    }

                    it++;
                }

                for(t = 0; t < queues.size(); t++)
                {

                    if(!queues.at(t).empty())
                        {
                            if(rr.size()!=0)
                        {
                            if(mix[rr.back()].at(1)!=0)
                            {
                                if(indices[rr.back()] + 1 == queues.size())
                                {
                                    queues.push_back(queue<string>());
                                    queues.at(queues.size()-1).push(rr.back());

                                }
                                else
                                {
                                    queues.at(indices[rr.back()] + 1).push(rr.back());
                                }

                                indices[rr.back()]+=1;
                            }
                                    if(mix[queues.at(t).front()].at(1) > pow(2,indices[queues.at(t).front()]))
                                   {
                                      for(int x = 0; x < pow(2,indices[queues.at(t).front()]); x++)   // change quantum by 2^ number of queue currently holding process
                                        {
                                           rr.push_back(queues.at(t).front());
                                           mix[queues.at(t).front()].at(1) --;
                                           k++;
                                        }
                                   }
                                   else
                                   {
                                       for(int x = 0; x < mix[queues.at(t).front()].at(1); x++)
                                        {
                                            rr.push_back(queues.at(t).front());
                                            k++;

                                        }
                                         mix[queues.at(t).front()].at(1)=0;
                                   }

                                queues.at(t).pop();

                                break;
                        }

                        }
                }
          

                if(t == queues.size())

                {
                    
                     if(rr.size()!=0)
                        {
                            if(mix[rr.back()].at(1) > pow(2,indices[rr.back()]))
                                   {
                                      for(int x = 0; x < pow(2,indices[rr.back()]); x++)
                                        {
                                           rr.push_back(rr.back());
                                           mix[rr.back()].at(1) --;
                                           k++;
                                        }
                                   }
                                   else
                                   {
                                       for(int x = 0; x < mix[rr.back()].at(1); x++)
                                        {
                                            rr.push_back(rr.back());
                                            k++;

                                        }
                                         mix[rr.back()].at(1)=0;
                                   }
                           }
                    else
                    {
                        rr.push_back(queues.at(0).front());
                        queues.at(0).pop();
                        k++;
                         mix[rr.back()].at(1) --;

                    }


                }
            }
        }
        if(cpu.at(u) == 8)
        {
            name="Aging"; // service time in aging is priority
            flag=1;
              std::priority_queue<std::pair<int, string>,
                    std::vector<std::pair<int, string>>,
                    less_then_greater
                   > pq;
             int t;
            int j=0;
            int k=0;
            auto it = mix.begin();
            while(k<timespan)
            {
                  while(it!= mix.end())
                {
                    if(mix[it->first].at(0)<=k)
                    {
                        pq.push(make_pair(mix[it->first].at(1), it->first));
                    }
                    else
                    {
                        break;
                    }

                    it++;
                }
                if(rr.size()!=0)
                {
                    for(t=0;t< pq.size();t++) // increment priority of processes in queue
                    {
                         pair<int, string> top = pq.top();
                         top.first++;    
                         pq.pop();
                         pq.push(make_pair(top.first,top.second));
                    }
                    pq.push(make_pair(mix[rr.back()].at(1), rr.back()));  // push currently running process in queue with it's intial priority

                    for(t=0;t<quantum.at(counter);t++)  // run process with highest priority by quantum period
                    {
                        rr.push_back(pq.top().second);
                        k++;
                    }
                    pq.pop();
                }else
                {
                    for(t=0;t< pq.size();t++) // same as previous algorithm but without pushing any processs as this is the 1st process 
                    {
                         pair<int, string> top = pq.top();
                         top.first++;
                         pq.pop();
                         pq.push(make_pair(top.first,top.second));
                    }
                     for(t=0;t<quantum.at(counter);t++)
                    {
                        rr.push_back(pq.top().second);
                        k++;
                    }
                    pq.pop();

                }

            }

            counter++;
        }


           if(trace == 1)
    {
        cout<<name;
        int d = 6-name.length();
        for(int i=0; i<d ; i++)
        {
            cout<<" ";
        }
        for(int i =0; i<=timespan;i++)
        {
            cout<<i%10<<" ";
        }
            cout<<endl;
            for(int i =0;i<(6+((timespan+1)*2));i++)
            {
                cout<<"-";
            }
            cout<<endl;

        //printing

        if(flag==0)
        {

        int k=0;
        for(int h=0; h < processes.size();h++){
                cout<<processes.at(h)<<"     ";
        while(k!=timespan) // loop over final vector rr which contain processes sequence for each implementation 
            {

                string x= rr.at(k);  
                string y=processes.at(h);


                if(k>= arrive.at(h)&& service.at(h)!=0)
                {
                    if(x == y)
                    {
                        printf("|*");
                        service.at(h)--;
                    }else
                    printf("|.");
                } else
                    printf("| ");

                k++;
                }
                printf("| \n");
                k=0;
        }


    }
    else
    {   
    // printing of aging 
        int k=0;
        for(int h=0; h < processes.size();h++){
                cout<<processes.at(h)<<"     ";
        while(k!=timespan)
            {

                string x= rr.at(k);

                string y=processes.at(h);


                if(k>= arrive.at(h))
                {
                    if(x == y)
                    {
                        printf("|*");
                    }else
                    printf("|.");
                } else
                    printf("| ");

                k++;
                }
                printf("|\n");
                k=0;
        }

    }
    for(int i =0;i<(6+((timespan+1)*2));i++)
            {
                cout<<"-";
            }
            cout<<endl;
    }
    else
    {
        cout<<name<<endl;
        cout<<"Process    ";
        for(int h=0; h < processes.size();h++)
            {
                cout<<"|  "<<processes.at(h)<<"  ";
            }
            cout<<"|"<<endl;

             cout<<"Arrival    ";
        for(int h=0; h < processes.size();h++)
            {
                if(arrive.at(h)<10)
                cout<<"|  "<<arrive.at(h)<<"  ";
                else
                    cout<<"| "<<arrive.at(h)<<"  ";
            }
            cout<<"|"<<endl;

                    cout<<"Service    ";
        for(int h=0; h < processes.size();h++)
            {
                if(back_up.at(h)<10)
                cout<<"|  "<<back_up.at(h)<<"  ";
                else
                    cout<<"| "<<back_up.at(h)<<"  ";
            }
            cout<<"| Mean|"<<endl;


                   cout<<"Finish     ";
      for(int h=0; h < processes.size();h++)
            {
                int k=rr.size()-1;
                while(k>=0)
                      {
                         if(rr.at(k)== processes.at(h))
                            {
                                if(k+1<10)
                                cout<<"|  "<<k+1<<"  ";
                                else
                                     cout<<"| "<<k+1<<"  ";
                                finish.push_back(k+1);
                                break;
                            }
                            k--;
                      }

            }
            cout<<"|-----|"<<endl;
            cout<<"Turnaround ";
      for(int h=0; h < processes.size();h++)
            {
                turnaround.push_back(finish.at(h)-arrive.at(h));
                if(finish.at(h)-arrive.at(h)<10)
            cout<<"|  "<<finish.at(h)-arrive.at(h)<<"  ";
            else
             cout<<"| "<<finish.at(h)-arrive.at(h)<<"  ";
            }
            if(accumulate(turnaround.begin(), turnaround.end(), 0.0) / turnaround.size()<10.0)
            cout<<"| "<<fixed<<setprecision(2)<<accumulate(turnaround.begin(), turnaround.end(), 0.0) / turnaround.size()<<"|"<<endl;
              else
              cout<<"|"<<fixed<<setprecision(2)<<accumulate(turnaround.begin(), turnaround.end(), 0.0) / turnaround.size()<<"|"<<endl;

            cout<<"NormTurn   ";
      for(int h=0; h < processes.size();h++)
            {
                normturn.push_back(turnaround.at(h)/(float)back_up.at(h));
            cout<<"| "<<fixed<<setprecision(2)<<turnaround.at(h)/(float)back_up.at(h);

            }
                if(accumulate(normturn.begin(), normturn.end(), 0.0) /(float) normturn.size()<10.0)
            cout<<"| "<<fixed<<setprecision(2)<<accumulate(normturn.begin(), normturn.end(), 0.0) /(float) normturn.size()<<"|"<<endl;
            else
            cout<<"|"<<fixed<<setprecision(2)<<accumulate(normturn.begin(), normturn.end(), 0.0) /(float) normturn.size()<<"|"<<endl;


    }


    cout<<endl;
    for(int h=0; h<back_up.size();h++) // reset sevice time as it changed in printing
        {

            service.at(h) = back_up.at(h);
        }


}

}

