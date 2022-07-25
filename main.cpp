#include "graph.h"
int main(int argc, char *argv[])
//int main()
{
	// cout<<argv[0]<<endl;// first var is ./KSP
	string keysFile = "./map/data_NY/sub/NY_sub_keys_20_200.node";	//keys file
	string graphFile = "./map/USA-road-d.NY.gr";			//graph file
	const char *indexFile ="./map/data_NY/h2h/NY_sub.index";	//H2H index file
	const char *edgeFile ="./map/data_NY/h2h/NY_sub.edge";	//edge file(graph file same)

	Graph g = Graph(graphFile);
	g.set_nodeKEYS_NodesBit(keysFile);
	g.Init_H2H_Index(indexFile, edgeFile);

	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	std::chrono::duration<double> time_span;
	std::chrono::duration<double> DA_time_totol;
	std::chrono::duration<double> GTree_time_totol;

	//vector<int>querykey;
	//string ss = "83,106,7,12,109,113, ";//,129,144,160,184"; // string ss = argv[2];
	//strToVec(querykey,ss);	//trans

	fstream infile(argv[1],ios::in);	// test file input ID1 and ID2
	// ofstream outfile(argv[4],ios::app);	// result outfile 
	// ofstream outlen("testData/NY_example100.txt",ios::app);

	int k=10;  // int k = atoi(argv[5]);
	vector<int> kPs;//Diversity distance of routes or Skyline 
	vector<vector<int>> vkPs;//diversity results routes or Skyline
	vector<int> kResults;
	//vector<int> Result;
	kResults.reserve(k);
	vector<vector<int> >vkPath; 
	kResults.clear();
	vkPath.clear();
	kPs.clear();
	vkPs.clear();
	vector<int> vPath, vPathEdge; 
	vector<int> bestpath, bestpoi; 
	srand ((unsigned)time(NULL));
	int ID1, ID2;
	string ss;
	 while(infile >> ID1 >> ID2 >> k >> ss){
	int AVEK=0,nums=0;
/*	while(nums<20){
/*
	do
	{
		//nodeid in [3,1064] because of INF value by vSPTDistance
		ID1 = rand() % (g.nodeNum-5) + 3; 
		ID2 = rand() % (g.nodeNum-5) + 3;
	}while(ID1 == ID2 || g.vbISO[ID1] || g.vbISO[ID2] || ID1<3 || ID2<3 ); 
	//ID1=18371, ID2=4787;
*/
	cout  << "ID1:" << ID1 << "\tID2:" << ID2 << endl; 
	vector<int>querykey;
	strToVec(querykey,ss);
	//createKeys(querykey, KIND, 4);
	cout<<"keywords : ";	
	for(auto kk:querykey){cout<<kk<<" ";}cout<<endl;
	kResults.clear();
	//Result.clear();
	vkPath.clear();
	bestpoi.clear();
	bestpath.clear();
	kPs.clear();
	vkPs.clear();

	t1 = std::chrono::high_resolution_clock::now();
	int size = g.DAPrune(ID1, ID2, k, querykey, kResults, vkPath, kPs, vkPs); 
	t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
	DA_time_totol = DA_time_totol + time_span;
	cout << "DAPrune Time:" << time_span.count() << endl;
	int da = *min_element(kResults.begin(),kResults.end());
	int e=0;
	// bitset<int> qu = QueryBit;

	cout<<"Top-"<<k<<" DAprune routes(totol path Set) : "<<endl;	
/*
	for(auto& d : kResults){
		cout<<d<<" ";
		//for(auto p : vkPath[e++]){
		//	cout<<p<<" ";
		//}	
		cout<<endl;
	}
	cout<<endl;
*/
/*
	for(int i=0;i<vkPath.size();i++){
		cout<<kResults[i]<<":";
		for(auto p : vkPath[e++]){
			cout<<p<<" ";
		}cout<<endl;
	}

	cout<<endl;
*/
//	cout<<"Result Routes(POI Set) : k = "<<kPs.size()<<endl;
	e=0;
	for(auto& d : kPs){
		cout<<d<<":";
		for(auto p : vkPs[e++]){
			cout<<p<<" ";		
		}
		cout<<endl;
	}
	cout<<endl;
/*
	string ss = "";	
	for(int i=1; i<querykey.size();i++){ss+=to_string(querykey[i]);ss+=",";}ss+=to_string(querykey[0]);
	if(kPs.size()==k){
		nums++;
		outlen<<ID1<<"\t"<<ID2<<"\t"<<k<<"\t"<<ss<<endl;
	}
*/
/*
	t1 = std::chrono::high_resolution_clock::now();
	int clen = g.Clen(ID1, ID2, g.QueryBit, Result);
	t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
	GTree_time_totol = GTree_time_totol + time_span;
	cout << "Clen Time:" << time_span.count()<< endl;
	// T2 += clen;

	int dij = g.distanceQuery(ID1+1,ID2+1);
	cout<<"dijkstra = "<<dij<<endl;
	cout<<"DAmrrp   = "<<da<<endl;
	cout<<"Clen     = "<<clen<<endl<<endl;
*/
/*
	if(vkPath.size()>=3){
		AVEK+=vkPath.size();
		nums++;
	}else{
		continue;	
	}	
	}
	cout<<"AVEK = "<<AVEK*1.0/nums<<endl;
*/
}
	return 0;

}

