#include <bits/stdc++.h>
#include "nanoflann.hpp"
#include "KDTreeVectorOfVectorsAdaptor.h"
using namespace nanoflann;

// typedef double double;
typedef KDTreeVectorOfVectorsAdaptor<vector<vector<double> >, double> spatial_tree;

class data{
public:
    vector<double> v;
    bool visited = false;
    double reachability_distance = 1e9;
    double core_distance = 0;
    int ID = -1;
};
class CompareDouble{
public: 
    bool operator()(pair<double, data> n1, pair<double, data> n2){
        return n1.first>n2.first;
    }
};
vector<data> SetofObjects; 
vector<data> Orderedfile;
vector<vector<double> > A;                           // MAIN
priority_queue<pair <double , data>, vector<pair<double, data> >, CompareDouble > orderseed;
double epsilon;
int minpoint;
int dimensions=0;
vector<pair<size_t ,double > > ret_matches;

vector<data> Find_Neighbors(data &object, double epsilon, spatial_tree &tree){
    vector<data> abc3;
    // cout<<object.ID<<endl;
    const double distance = static_cast<double>(epsilon*epsilon);
    ret_matches.clear();
    nanoflann::SearchParams params;
    params.sorted = true;
    double query[dimensions];
    for(int i = 0; i<dimensions; i++) query[i] = object.v[i];
    const size_t nMatches = tree.index->radiusSearch(query, distance, ret_matches, params);
     // cout<<ret_matches.size()<<"HH"<<endl;
    int p=0;
    if(ret_matches.size()>=1){
    for(int i=1;i<ret_matches.size();i++) {
        p=ret_matches[i].first;
        // cout<<p<<"P"<<endl;
        abc3.push_back(SetofObjects[p]);
    }
    }
    if(minpoint>ret_matches.size()) {
           
        object.core_distance = epsilon+1;
        
    }
    else {
         // cout<<sqrt(ret_matches[minpoint-1].second)<<"l"<<ret_matches[minpoint-1].first<<endl;
        object.core_distance= sqrt(ret_matches[minpoint-1].second);

    }
    return(abc3);
}

void List_update(vector<int> NBD, data &CenterObject ) {
    double temp=CenterObject.core_distance;
    //cout<<"LLLP "<<SetofObjects[NBD[0]].ID<<endl;   
    for(int i=0;i<NBD.size();i++) {
        if(!SetofObjects[NBD[i]].visited) {
            double temp4=0;
            double temp2=0;
            temp4= sqrt(ret_matches[i+1].second);
            // for(int j=0;j<CenterObject.v.size();j++) {
            //     // cout<<SetofObjects[NBD[i]].v[j]<<"love"<<endl;
            //     temp4=temp4+((CenterObject.v[j]-SetofObjects[NBD[i]].v[j])*(CenterObject.v[j]-SetofObjects[NBD[i]].v[j]));
            // }
        
            // temp4=sqrt(temp4);
            if(temp4>=temp) {
                temp2=temp4;
            }
            else {
                temp2=temp;
            }
        
            if(SetofObjects[NBD[i]].reachability_distance==1e9){
                SetofObjects[NBD[i]].reachability_distance=temp2;
                orderseed.push({temp2,SetofObjects[NBD[i]]});
               
            }    
            else {
                if(temp2<SetofObjects[NBD[i]].reachability_distance){
                SetofObjects[NBD[i]].reachability_distance=temp2;
                orderseed.push({temp2,SetofObjects[NBD[i]]});
                

                }
            }

        }
    }
}

void Expand_Cluster(data &object, double epsilon, int minpoint, spatial_tree &tree){
    vector<data> abc;
    vector<int> NBD;
    int p=0;
    abc = Find_Neighbors(object , epsilon, tree);
    NBD.resize(abc.size());
    for(int i=0;i<abc.size();i++) {
      NBD[i]=abc[i].ID;
       
    }
    object.visited= true;
    object.reachability_distance=1e9;
    Orderedfile.push_back(object);
    if(object.core_distance<=epsilon){
        List_update(NBD, object);
    }
    
    while (orderseed.size()>0) {
        int l=0;
            
        pair<double, data> k = orderseed.top();
        l=k.second.ID;
        if(SetofObjects[l].visited){
            orderseed.pop();
            continue;
        }
        vector<int> NBD2;
        vector<data> abc2;
        abc2= Find_Neighbors(SetofObjects[l],epsilon, tree);
        NBD2.resize(abc2.size());
        for(int i=0;i<abc2.size();i++) {
            
            NBD2[i]=abc2[i].ID;
        }
        
        SetofObjects[l].visited=true;
        Orderedfile.push_back(SetofObjects[l]);
        orderseed.pop();
        if(SetofObjects[l].core_distance<=epsilon){
            List_update(NBD2, SetofObjects[l]);
        }
    }
    
}

int lines;

int READ(char const *name){
    string s;
    ifstream inFile;
    inFile.open(name);
    int count = 0;
    while(getline(inFile, s)){
        count = 0;
         for(int i = 0; i<s.length(); i++)
             if(s[i] == ' ') count++;
         lines++;
    }
    inFile.close();
   // cout<<count<<", "<<lines<<endl;
    return (count+1);                              // convert it to count if space at the end
}

void read_data(char const* name, int dimensions){
    for(int i = 0; i<lines; i++){
        data vicky;
        A[i].resize(dimensions);
        vicky.v.resize(dimensions);
        for(int j = 0; j<dimensions; j++){
            cin>>A[i][j];
            vicky.v[j] = A[i][j];
        }
        vicky.ID = i;
        SetofObjects[i]=vicky;
    }
}

int main(int argc, char const *argv[]){
    epsilon = atof(argv[3]);
    minpoint = atoi(argv[2]);
    dimensions = READ(argv[1]);
    //cout<<dimensions<< endl;
    A.resize(lines);
    SetofObjects.resize(lines);
    read_data(argv[1], dimensions);
  
    

    spatial_tree tree(dimensions, A, 200);
    tree.index->buildIndex();
    for(int i=0;i<SetofObjects.size();i++) {
        if(!SetofObjects[i].visited) {    
            Expand_Cluster(SetofObjects[i],epsilon,minpoint, tree);
        }
    }
        ofstream out_data("output.txt");
    for(int i = 0; i<Orderedfile.size(); i++) {
       if(Orderedfile[i].reachability_distance==1e9)
        Orderedfile[i].reachability_distance=1.5*epsilon;
        out_data<<Orderedfile[i].reachability_distance<<endl;
    }
    return 0;
}