/**
 * @Author: Michel Andy <andy>
 * @Date:   2017-04-04T08:10:46-04:00
 * @Email:  Andymic12@gmail.com
 * @Project: Anycast
 * @Filename: worm_similation.cc
 * @Last modified by:   andy
 * @Last modified time: 2017-04-04T21:26:32-04:00
 */
#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<sys/wait.h>
using namespace std;
/*
    1) Address space: 100,000
    2) Clusters: 10
    3) Infected Ips per cluster: 10

    Simulations:
    1) Random-scanning worm propagation
    2) Local preference: with 60% probability an ip in the neighborhood is chosen -10 or +10
                         with 40% probability a number from the entire space (100,000) is chosen

    Tips:
    1) x = rand()%m+1 generates a number from 1 to (m+1)-1
    2)

*/

enum Node{
    empty,
    susceptible,
    infectious
};

vector<int> simulation(vector<Node> &nodes, int scan_rate, int &vulnerable_nodes){
    cout<<"nodes: "<<nodes.size()<<" scan_rate: "<<scan_rate<<" vulnerable_nodes: "<<vulnerable_nodes<<endl;
    vector<int> infected(1000);
    int tick = 0;
    while(vulnerable_nodes > 0){
      tick++;
      for(int i=1; i<=scan_rate; i++){
        int rand_ip = rand()%100000+1;
        if(nodes[rand_ip] == susceptible){
           nodes[rand_ip] = infectious;
           scan_rate+=2;
           vulnerable_nodes--;
           cout<<"new scan rate: "<<scan_rate<<endl;
        }

        infected[tick]= 1000 - vulnerable_nodes;
      }
    }
    return infected;
}

void dump_to_file(vector<vector<int>> v){
  int len = 1000;

  std::ofstream ofs;
  ofs.open ("worm_random.txt", ofstream::out);
  for(int i=0; i<len; i++){
      ofs<<v[0][i]<<","<<v[1][i]<<","<<v[2][i]<<"\n";
  }

  ofs.close();
}

vector<Node> init(const int omega, int &scan_rate, int &vulnerable_nodes){
    vector<Node> nodes(omega);
    for(int i=1; i<=omega; i++) nodes[i] = empty;

    for(int j=0; j<100; j++){
        for(int i=1; i<=10; i++)
            nodes[i+j*1000] = susceptible;
    }

    int rand_cluster = rand()%100;
    int rand_ip = rand()%10+1;
    nodes[rand_cluster*1000+rand_ip] = infectious;
    scan_rate = 2;
    vulnerable_nodes--;

    int s=0, e=0, i=0;
    for(Node n: nodes){
        if(n == susceptible)
            s++;
        else if(n==infectious)
            i++;
        else
            e++;
    }
    cout<<"susceptible: "<<s<<" infectious: "<<i<<" empty: "<<e<<endl;
    return nodes;
}


int main(void){
   const int sim_iteration = 3;
   const int omega = 100000;
   srand(time(NULL));
   vector<vector<int>> infected_per_iteration(0, vector<int>(0));
   infected_per_iteration.push_back(vector<int>(1000));
   infected_per_iteration.push_back(vector<int>(1000));
   infected_per_iteration.push_back(vector<int>(1000));
   vector<int> sims;
   for(int i=0; i<sim_iteration; i++){
     int vulnerable_nodes = 1000;
     int init_scan_rate=0;

     vector<Node> nodes = init(omega, init_scan_rate, vulnerable_nodes);

     sims = simulation(nodes, init_scan_rate, vulnerable_nodes);
     infected_per_iteration[i] = sims;

   }

   dump_to_file(infected_per_iteration);

    return 0;
}
