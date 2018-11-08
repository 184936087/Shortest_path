#include <iostream>
#include <climits>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <cmath>

using namespace std;

typedef struct Point
{
    int length;
    int ways;
}node;

void shortest_path(){
    ///////////////////////PART 3/////////////////////////////////
    string ipt = "edges.txt";
    int i,j,k;
    vector<int> edge_data;
    ifstream file;
    file.open(ipt.c_str());
    if (file.good()){
        // Push items into a vector
        int current_number = 0;
        while (file >> current_number)
            edge_data.push_back(current_number);
        file.close();
    }
    int len = a.size();
    int edges = a[0];
    //Initialized 2d array:
    Point **shortest_path = new Point *[edges];
    for (i = 0; i < edges; ++i){
        shortest_path[i] = new Point[edges];
    }
    bool *finished = new bool[edges];
    string *v = new string[edges*edges];

    for(i = 0; i < edges; ++i){
        for (j = 0; j < edges; ++j){
            v[i*edges+j] = "";
        }
    }

    //Assign original shortest path
    for(i = 1; i < len - 2; i += 3){
        shortest_path[a[i]-1][a[i+1]-1].length =a[i+2];
        shortest_path[a[i+1]-1][a[i]-1].length =a[i+2];
    }
    int start = a[len-2];
    int end = a[len-1];
    vector<int>(a).swap(a); 
    for (i = 0; i < edges; ++i){
        for (j = 0; j < edges; ++j){
            if(shortest_path[i][j].length == 0 && i != j)
                shortest_path[i][j].length = 20;
            shortest_path[i][j].ways = 1;
        }
    }

    //Loop for each point and find minimum point, which is not 0
    for(i = 0; i < edges; ++i){
        int min = 20;
        int minIdx;
        for (j = i; j < edges; ++j){
            if(shortest_path[i][j].length < min && i != j){
                minIdx = j;
                min = shortest_path[i][j].length;
            }
        }

        //Initialized finished[] and set all index to 0
        for (int s = 0; s < edges; ++s){
            finished[s] = 0;
        }
        finished[i] = 1;
        int allDone = 1;
        for (int s = 0; s < edges; ++s){
            allDone *= finished[s]; 
        }
        
        //Now shortest_path[i][minIdx].length is the shortest.
        //Begin with shortest_path[i][minIdx].length and finding shortestpath
        while(allDone == 0){
            int localmin = 20;
            int localminIdx;
            //cout << "i=" << i << "minIDX:" << minIdx << endl;
            for(k = 0; k < edges; ++k){
                if(k == minIdx || finished[k] == 1)
                    continue;
                //Check if i->min->k is shorter than i->k
                if(shortest_path[i][minIdx].length + shortest_path[k][minIdx].length == shortest_path[i][k].length && i < k){
                    string pt = v[edges*i+k];
                    string nd = to_string(minIdx + 1);
                    int idx = pt.find(nd);
                    if(idx == string::npos){
                        v[edges*i+k] += v[edges*i+minIdx] + v[edges*minIdx + k] + nd;
                        v[edges*k+i] = v[edges*i+k];
                        shortest_path[i][k].ways ++;
                        shortest_path[k][i].ways ++;
                    }
                }
                else if(shortest_path[i][minIdx].length + shortest_path[k][minIdx].length < shortest_path[i][k].length){
                    shortest_path[i][k].length = shortest_path[i][minIdx].length + shortest_path[k][minIdx].length;
                    shortest_path[k][i].length = shortest_path[i][k].length;
                    shortest_path[i][k].ways = shortest_path[i][minIdx].ways * shortest_path[k][minIdx].ways;
                    shortest_path[k][i].ways = shortest_path[i][k].ways;
                    v[edges*i+k] = v[edges*i+minIdx] + v[edges*minIdx + k];
                    for(int myways = 0; myways < shortest_path[i][k].ways; ++myways)
                        v[edges*i+k] += to_string(minIdx + 1); 
                    v[edges*k+i] = v[edges*i+k];
                }

                if(shortest_path[i][k].length < localmin && i != k){
                    localminIdx = k;
                    localmin = shortest_path[i][k].length;
                }
            }

            //Check if local min is infinite to avoid infinite loop
            if(localmin == 20)
                break;

            finished[minIdx] = 1;
            min = localmin;
            minIdx = localminIdx;
            for (int s = 0; s < edges; ++s){
                allDone *= finished[s]; 
            }
        }
    }

    //First: sort shortest_path
    int *sourseSearch = new int[edges];
    int *destinationSearch = new int[edges];
    int startIdx = start - 1;
    int endIdx = end - 1;
    int *sortedStartEdge = new int[edges];
    int *sortedEndEdge = new int[edges];
    int *doneStart = new int[edges];
    int *doneEnd = new int[edges];
    for (i = 0; i < edges; ++i){
        doneStart[i] = 0;
        doneEnd[i] = 0;
    }
    int t = 0;
    i = 0;
    while(t < edges){
        while(doneStart[i] == 1)
            i++;
        int min = shortest_path[startIdx][i].length;
        int minIdx = i;
        for (j = 0; j < edges; j++){
            if (doneStart[j]!= 1 && j != i){
                if (shortest_path[startIdx][j].length < min){
                    min = shortest_path[startIdx][j].length;
                    minIdx = j;
                }
            }
        }
        sortedStartEdge[t] = minIdx;
        doneStart[minIdx] = 1;
        t++;
    }

    t = 0;
    i = 0;
    int Sourcelength;
    int Destinationlength;
    int meetNode;
    while(t < edges){
        while(doneEnd[i] == 1)
            i++;                            
        int min = shortest_path[endIdx][i].length;
        int minIdx = i;
        for (j = 0; j < edges; j++){
            if (doneEnd[j]!= 1 && j != i){
                if (shortest_path[endIdx][j].length < min){
                    min = shortest_path[endIdx][j].length;
                    minIdx = j;
                }
            }
        }
        sortedEndEdge[t] = minIdx;
        doneEnd[minIdx] = 1;
        t++;
    }
    bool found = 0;
    Destinationlength = 0;
    Sourcelength = 0;
    for (i = 0; i < edges; ++i){
        //Sourse move:
        sourseSearch[i] = sortedStartEdge[i] + 1;
        for (j = 0; j < Destinationlength; ++j){
            if(sourseSearch[i] == destinationSearch[j]){
                meetNode = sourseSearch[i];
                found = 1;
            }
        }
        Sourcelength++;
        if(found == 1){
            break;
        }
        //Destination move:
        destinationSearch[i] = sortedEndEdge[i] + 1;
        for (j = 0; j < Sourcelength; ++j){
            if(destinationSearch[i] == sourseSearch[j]){
                meetNode = destinationSearch[i];
                found = 1;
            }
        }
        Destinationlength++;
        if(found == 1){
            break;
        }
    }
    //Output for part 3:
    ofstream outf;
    outf.open("pa2_op3.txt"); 
    for(int x = 0; x < Sourcelength; ++x){
        outf << sourseSearch[x] << " ";
    }
    outf << endl;

    for(int x = 0; x < Destinationlength; ++x){
        outf << destinationSearch[x] << " ";
    }
    outf << endl;

    if(startIdx + 1 != meetNode)
        outf << startIdx + 1 << " ";
    for (i = 0; i < v[edges*startIdx + meetNode - 1].length(); ++i){
        outf << v[edges*startIdx + meetNode - 1][i] << " ";
    }
    outf << meetNode;
    if(meetNode != endIdx + 1)
        outf << " " << endIdx + 1;
    outf << endl;
    outf.close();


    delete []finished;
    for (i = 0; i < edges; ++i){
        delete []shortest_path[i];
    }
    delete []shortest_path;
    delete []v;
    delete []sourseSearch;
    delete []destinationSearch;
    delete []doneStart;
    delete []doneEnd;
    delete []sortedEndEdge;
    delete []sortedStartEdge;
}


int main(int argc, char* argv[]){
    shortest_path();
    return 0;
}