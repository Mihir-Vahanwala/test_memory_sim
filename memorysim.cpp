//this is the real deal.
#include <iostream>
#include <vector>
#include <list>
using namespace std;

struct request
{
    char code; int pid; long int argument;
    request() {}
    request(char c, int p, long int a)
    {
        code = c; pid = p; argument = a;
    }
    request(char c, int p)
    {
        code = c; pid =p;
    }
};

struct requestA
{
    int pid; long int size; int ipnum;
    requestA() {}
    requestA(int p, long int s, int i)
    {
        pid = p; size = s; ipnum = i;
    }
};

struct interval
{
    int pid; long int start; long int end;
    interval() {}
    interval(int p, long int s, long int e)
    {
        pid = p; start = s; end = e;
    }
};



typedef list<requestA>::iterator iter_req;
typedef list<interval>::iterator iter_inter;
typedef list<iter_inter>::iterator iter_short;

long int biggest_gap(list<interval> &locations, long int M)
{
	if (locations.empty()) return M;
	long int best = (*locations.begin()).start;
	iter_inter j;
	for (j = locations.begin(); j != prev(locations.end()); j++)
	{
		long int challenge = (*(next(j))).start - (*j).end;
		if (challenge>best) best = challenge;
	}
	long int chal = M - (*(prev(locations.end()))).end;
	if (chal > best) best = chal;
	return best;
}

int main()
{
    vector <int> output;
    vector <request> input;
    int n; long int M;
    cin>>n>>M;
    char c;
    cin>>c;
    while (c != 'H')
    {
        
        if (c == 'H') break;
        if (c == 'A')
        {
            int p; long int a;
            cin>>p>>a;
            input.push_back(request(c, p, a));
            //cout<<input.size();
        }
        else if (c == 'D')
        {
            int p; long int a;
            cin>>p>>a;
            input.push_back(request(c, p, a));
        }
        else if (c == 'T')
        {
            int p;
            cin>>p;
            input.push_back(request(c, p));
        }
        else {}
        cin>>c;
    }
    output.resize(input.size());
    //cout<<input.size()<<output.size();
    for (int i =0; i<output.size(); i++)
    {
        //cout<<input[i].argument<<endl;
    }
    
    list <requestA> requests;
    list <interval> locations;
    vector <list <iter_inter> > shortcut(n);
    long int max_gap = M;
    
    for (int i=0; i<input.size(); i++)
    {
        //cout<<"Hi "<<i<<endl;
        //cout<<locations.size()<<endl;
        if (input[i].code == 'A')
        {
            if (max_gap < input[i].argument)
            {
            	requests.push_back(requestA(input[i].pid, input[i].argument, i));
            	continue;
            }
            bool possible = false;
            if (locations.empty() || (*(locations.begin())).start >= input[i].argument)
            {
                //cout<<"First case"<<endl;
                possible = true;
                locations.insert(locations.begin(), (interval(input[i].pid, 0, input[i].argument)));
                max_gap = biggest_gap(locations, M);
                output[i] = 0;
                shortcut[input[i].pid].insert(shortcut[input[i].pid].begin(), locations.begin());
                //cout<<"Ok here"<<endl;
            }
            else
            {
                //cout<<"This case"<<endl;
                for (iter_inter j= locations.begin(); j != prev(locations.end()); j++)
                {
                    possible = ((*(next(j))).start - (*j).end >= input[i].argument);
                    if (possible)
                    {
                        locations.insert(next(j), interval(input[i].pid, (*j).end, (*j).end+input[i].argument));
                        max_gap = biggest_gap(locations, M);
                        output[i] = (*j).end;
                        int index = input[i].pid;
                        iter_short foobar = shortcut[index].begin();
                        while (foobar != shortcut[index].end() && (*(*foobar)).start < output[i] ) {foobar++;}
                        shortcut[index].insert(foobar, next(j));
                        break;
                    }
                }
            }
            if (!possible)
            {
                if (M - (*(prev(locations.end()))).end >= input[i].argument)
                {
                    locations.push_back(interval(input[i].pid, (*(prev(locations.end()))).end, (*(prev(locations.end()))).end + input[i].argument));
                    max_gap = biggest_gap(locations, M);
                    possible = true;
                    output[i] = (*(prev(locations.end()))).start;
                    shortcut[input[i].pid].push_back(prev(locations.end()));
                }
            }
            if (!possible)
            {
                requests.push_back(requestA(input[i].pid, input[i].argument, i));
            }
            //cout<<"Ok"<<endl;
        }
        
        if (input[i].code == 'D')
        {
            if (shortcut[input[i].pid].empty()) {output[i] = 1; continue;}
            else
            {
                //deall = false;
                iter_short j = shortcut[input[i].pid].begin();
                //cout<<"This is ok"<<endl;
                while (input[i].argument >= (*(*j)).start )
                {
                    j++;
                    if (j == shortcut[input[i].pid].end()) break;
                    //cout<<"In the loop"<<endl;
                } 
                //cout<<"Exited loop"<<endl;
                if (j == shortcut[input[i].pid].begin())
                {
                	output[i] = 1; continue; 

                }
                //cout<<"Not yet doom"<<endl;
                if ( (*(*(prev(j)))).start != input[i].argument) {output[i] = 1; //cout<<"Here";//<<(*(j-1)).end<<input[i].argument<<endl;
                    continue; }
                //if ((*(prev(j))).pid != input[i].pid) {output[i] = 1; //cout<<"Here"<<(*(j-1)).pid<<input[i].pid<<endl;
                    //continue;}
                //(*(prev(j))).end = input[i].argument;
                output[i] = 0;
                //if ((*(prev(j))).start == (*(prev(j))).end)
                
                locations.erase(*(prev(j)));
                max_gap = biggest_gap(locations, M);
                shortcut[input[i].pid].erase(prev(j));
                
                //cout<<"Deallocation ok"<<endl;
                
                
                
            }
            if (output[i] == 0 && !requests.empty())
            {
                //allocate(requests);
                //cout<<"Need to allocate"<<endl;
                for (iter_req k = requests.begin(); k != requests.end(); k++)
                {
                    //cout<<"In allocation loop"<<endl;
                    if (max_gap < (*k).size) continue;
                    bool possible = false;
                    if (locations.empty() || (*locations.begin()).start >= (*k).size)
                    {
                        //cout<<"Case if"<<endl;
                        possible = true;
                        locations.insert(locations.begin(), (interval((*k).pid, 0, (*k).size)));
                        max_gap = biggest_gap(locations, M);
                        //cout<<(*k).ipnum<<endl;
                        output[(*k).ipnum] = 0;
                        shortcut[(*k).pid].insert(shortcut[(*k).pid].begin(), locations.begin());
                    }
                    else
                    {
                        //cout<<"Case else"<<endl;
                        for (iter_inter j= locations.begin(); j != prev(locations.end()); j++)
                        {
                            possible = ((*(next(j))).start - (*j).end >= (*k).size);
                            //cout<<"Possible assigned"<<endl;
                            if (possible)
                            {
                                locations.insert(next(j), interval((*k).pid, (*j).end, (*j).end+ (*k).size));
                                max_gap = biggest_gap(locations, M);
                                //cout<<"This worked"<<endl;
                                output[(*k).ipnum] = (*j).end;
                                int index = (*k).pid;
                                
                        		iter_short foobar = shortcut[index].begin();
                        		//cout<<"Still ok"<<endl;
                       			while ( foobar != shortcut[index].end() && (*(*foobar)).start < (*j).end ) 
                       				{
                       					//cout<<"Loop entered"<<endl;
                       					foobar++;

                       					//if (foobar == shortcut[index].end()) break;
                       				}
                        		shortcut[index].insert(foobar, next(j));
                                break;
                            }
                        }
                    }
                    if (!possible)
                    {
                        //cout<<"Still not possible"<<endl;
                        if (M - (*(prev(locations.end()))).end >= (*k).size)
                        {
                            locations.push_back(interval((*k).pid, (*(prev(locations.end()))).end, (*(prev(locations.end()))).end + (*k).size));
                            max_gap = biggest_gap(locations, M);
                            possible = true;
                            output[(*k).ipnum] = (*(prev(locations.end()))).start;
                            shortcut[(*k).pid].push_back(prev(locations.end()));
                        }
                    }
                    if (possible)
                    {
                        //iter_req l = k;
                        requests.erase(k);
                        k--;
                    }
                }
            }
            //cout<<"Ok"<<endl;
        }
        
        if (input[i].code == 'T')
        {
           	//cout<<"T"<<endl;
           	if (shortcut[input[i].pid].empty()) {output[i] = 1;}
            
            else output[i] = 0;
            //cout<<"Termination ok"<<endl;

            iter_short j = shortcut[input[i].pid].begin();
            //cout<<(*(*j)).end<<endl;
            for (j=shortcut[input[i].pid].begin(); j!=shortcut[input[i].pid].end(); j++)
            {
            	//cout<<shortcut[input[i].pid].size()<<endl;
            	locations.erase(*j);
            	//cout<<"Location erase"<<endl;
            	shortcut[input[i].pid].erase(j);
            	j--;
            	//cout<<"Loop"<<endl;
            }
            //cout<<"Purge ok"<<endl;
            //cout<<"terminating fine"<<endl;
            for (iter_req j = requests.begin(); j != requests.end(); j++)
            {
                if ((*j).pid != input[i].pid) continue;
                output[(*j).ipnum] = -1;
                requests.erase(j);
				j--;
            }
            max_gap = biggest_gap(locations, M);
            if (output[i] == 0)
            {
                //allocate(requests);
                for (iter_req k = requests.begin(); k != requests.end(); k++)
                {
                    if (max_gap < (*k).size) continue;
                    bool possible = false;
                    if (locations.empty() || (*locations.begin()).start >= (*k).size)
                    {
                        possible = true;
                        locations.insert(locations.begin(), (interval((*k).pid, 0, (*k).size)));
                        max_gap = biggest_gap(locations, M);
                        output[(*k).ipnum] = 0;
                        shortcut[(*k).pid].insert(shortcut[(*k).pid].begin(), locations.begin());
                    }
                    else
                    {
                        for (iter_inter j= locations.begin(); j != prev(locations.end()); j++)
                        {
                            possible = ((*(next(j))).start - (*j).end >= (*k).size);
                            if (possible)
                            {
                                locations.insert(next(j), interval((*k).pid, (*j).end, (*j).end+ (*k).size));
                                max_gap = biggest_gap(locations, M);
                                output[(*k).ipnum] = (*j).end;
                                int index = (*k).pid;
                        		iter_short foobar = shortcut[index].begin();
                       			while ( foobar != shortcut[index].end() && (*(*foobar)).start < (*j).end ) 
                       				{
                       					foobar++;
                       					//if (foobar == shortcut[index].end()) break;
                       				}
                        		shortcut[index].insert(foobar, next(j));
                                break;
                            }
                        }
                    }
                    if (!possible)
                    {
                        if (M - (*(prev(locations.end()))).end >= (*k).size)
                        {
                            locations.push_back(interval((*k).pid, (*(prev(locations.end()))).end, (*(prev(locations.end()))).end + (*k).size));
                            max_gap = biggest_gap(locations, M);
                            possible = true;
                            output[(*k).ipnum] = (*(prev(locations.end()))).start;
                            shortcut[(*k).pid].push_back(prev(locations.end()));
                        }
                    }
                    if (possible)
                    {
                        requests.erase(k);
                        k--;
                    }
                }
            }
            //cout<<"Ok"<<endl;
        }
        
    }
    for (iter_req k = requests.begin(); k != requests.end(); k++)
    {
        output[(*k).ipnum] = -1;
    }
    for (int i =0; i<output.size(); i++)
    {
        cout<<output[i]<<endl;
    }
    
}
