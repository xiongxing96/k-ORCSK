#include "graph.h"
// test for H2H
// int main(int argc, char *argv[])
// {
// 	// cout<<argv[0]<<endl;// first var is ./KSP
// 	// string keysFile = "./map/data_cal/sub/cal_sub_keys_10_150.node";	//keys file
// 	string keysFile = argv[1];
// 	string graphFile = "./map/USA-road-d.CAL.gr";			//graph file
// 	const char *indexFile ="./map/data_cal/h2h/cal_sub.index";	//H2H index file
// 	const char *edgeFile ="./map/data_cal/h2h/cal_sub.edge";	//edge file(graph file same)

// 	Graph g = Graph(graphFile);
// 	g.set_nodeKEYS_NodesBit(keysFile);
// 	g.Init_H2H_Index(indexFile, edgeFile);

// 	std::chrono::high_resolution_clock::time_point t1;
// 	std::chrono::high_resolution_clock::time_point t2;
// 	std::chrono::duration<double> time_span;
// 	std::chrono::duration<double> DA_time_totol;
// 	std::chrono::duration<double> GTree_time_totol;

// 	vector<int>querykey;
// 	string ss = argv[2];
// 	strToVec(querykey,ss);	//trans
// 	fstream infile(argv[3],ios::in);	// test file input ID1 and ID2
// 	ofstream outfile(argv[4],ios::app);	// result outfile                                                                                                                                                                                                        

// 	int k = 200000;       
// 	vector<int> kResults;
// 	vector<int> Result;
// 	kResults.reserve(k);
// 	vector<vector<int> >vkPath; 
// 	kResults.clear();
// 	vkPath.clear();
// 	vector<int> vPath, vPathEdge; 
// 	vector<int> bestpath, bestpoi; 
// 	int num = 0, num1 = 0, num2 = 0, num3 = 0, num4 = 0;
// 	int res = 0;
// 	long costd1 = 0, costg1 = 0, costd2 = 0, costg2 = 0;
// 	srand (time(NULL));
// 	int ID1, ID2;
// 	while(infile >> ID1 >> ID2 >> res){
// 	if(num == 50)break;

// 	// while(num==2){
// 	// do
// 	// {
// 	// 	//nodeid in [3,1064] because of INF value by vSPTDistance
// 	// 	ID1 = rand() % (g.nodeNum-5) + 3; 
// 	// 	ID2 = rand() % (g.nodeNum-5) + 3;
// 	// }while(ID1 == ID2 || g.vbISO[ID1] || g.vbISO[ID2] || ID1<3 || ID2<3 ); 
// 	// ID1=978, ID2=758;
// 	cout  << "ID1:" << ID1 << "\tID2:" << ID2 << endl; 

// 	kResults.clear();
// 	Result.clear();
// 	vkPath.clear();
// 	bestpoi.clear();
// 	bestpath.clear();

// 	t1 = std::chrono::high_resolution_clock::now();
// 	int da = g.eKSPNew(ID1, ID2, k, querykey, kResults, vkPath); 
// 	t2 = std::chrono::high_resolution_clock::now();
// 	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
// 	cout << "eKSP Time:" << time_span.count() <<" cost = "<<res<< endl;
// 	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
// 	DA_time_totol = DA_time_totol + time_span;
// 	cout << "DAmrrp Time:" << time_span.count() << endl;

// 	t1 = std::chrono::high_resolution_clock::now();
// 	int clen = g.Clen(ID1, ID2, g.QueryBit, Result);
// 	t2 = std::chrono::high_resolution_clock::now();
// 	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
// 	GTree_time_totol = GTree_time_totol + time_span;
// 	cout << "Clen Time:" << time_span.count()<< endl;

// 	int dij = g.distanceQuery(ID1+1,ID2+1);
// 	cout<<"dijkstra = "<<dij<<endl;
// 	cout<<"DAmrrp   = "<<da<<endl;
// 	cout<<"Clen     = "<<clen<<endl<<endl;
// 	num++;
	
// 	if(da == clen && da == dij)continue;
// 	if(da == clen){
// 		num1++;
// 	}else if(da < clen){
// 		num2++;
// 		costd1 += da;
// 		costg1 += clen;
// 	}else{
// 		// g.PruneRepeatedPoiPath(vkPath[0],bestpoi);
// 		num3++;
// 		costd2 += da;
// 		costg2 += clen;
// 		// outest << ID1 << "\t" << ID2 <<"\t\tda = "<<da<<" | ";
// 		// for(int p : bestpoi ){outest<<p<<" ";}
// 		// outest << "\t\t | clen = "<< clen <<" | ";
// 		// for(int p : Result){outest<<p<<" "; }
// 		// outest<<endl; 
// 	}


// 	}

// 	outfile<<keysFile<<endl;
// 	outfile<<"keywords = "<<querykey.size()<<"\t"<<ss<<endl;
// 	outfile<<"Test for "<<num<<" times query set"<<endl;
// 	outfile<<"num1 = "<<num1<<"	(DAmrrp = GTree) equal per = "<<(double)num1/num*100<<"%"<<endl;
// 	outfile<<"num2 = "<<num2<<"	(DAmrrp < GTree) small per = "<<(double)num2/num*100<<"%"<<endl;
// 	outfile<<"num3 = "<<num3<<"	(DAmrrp > GTree) large per = "<<(double)num3/num*100<<"%"<<endl;
// 	num4 = num-num1-num2-num3;
// 	outfile<<"rest = "<<num4<<"	(dijkstra  path) satis per = "<<(double)num4/num*100<<"%"<<endl;
// 	outfile<<"(DAmrrp < GTree) totol cost  : "<<costd1<<"     "<<costg1<<"     "<<(double)costd1/costg1<<endl;
// 	outfile<<"(DAmrrp > GTree) totol cost  : "<<costd2<<"     "<<costg2<<"     "<<(double)costg2/costd2<<endl;
// 	// outfile<<"same = "<<same<<endl;
// 	// outfile<<"less = "<<less<<endl;
// 	outfile<<"DAmrrp : totol time = "<<DA_time_totol.count()<<"\t"<<"average time = "<<DA_time_totol.count()/num<<endl;
// 	outfile<<"GTree : totol time = "<<GTree_time_totol.count()<<"\t"<<"average time = "<<GTree_time_totol.count()/num<<endl;
// 	outfile<<"-------------------------(P1NN Result)"<<endl<<endl;
// 	outfile.close();
// 	infile.close();

// 	// int k = 20000;       
// 	// vector<int> kResults;
// 	// kResults.reserve(k);
// 	// vector<vector<int> >vkPath; 
// 	// kResults.clear();
// 	// vkPath.clear();
// 	// t1 = std::chrono::high_resolution_clock::now();
// 	// int res = g.eKSPNew(ID1, ID2, k, querykey, kResults, vkPath); 
// 	// t2 = std::chrono::high_resolution_clock::now();
// 	// time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
// 	// cout << "eKSP Time:" << time_span.count() <<" cost = "<<res<< endl;
// 	// g.FindRepeatedPath(vkPath);
// 	// kResults.clear();
// 	// vkPath.clear();
// 	return 0;

// }
int main(){
	// string keysFile = "./map/data_cal/sub/cal_sub_keys_5_100.node";	//keys file
	// string graphFile = "./map/USA-road-d.CAL.gr";			//graph file
	// const char *indexFile ="./map/data_cal/h2h/cal_sub.index";	//H2H index file
	// const char *edgeFile ="./map/data_cal/h2h/cal_sub.edge";	//edge file(graph file same)

	// Graph g = Graph(graphFile);
	// g.set_nodeKEYS_NodesBit(keysFile);
	// g.Init_H2H_Index(indexFile, edgeFile);
	
	// cout << "--------> this is for test:"<< endl;
	// srand (time(NULL));
	// int ID1, ID2;
	// int num = 0, dist = 0, Lon = 15e5;

	// vector<int>querykey;
	// string ss = "12,45,23,56,89";
	// strToVec(querykey,ss);	//trans
	// int k = 10;   
	// vector<int> kResults;
	// vector<int> Result;
	// kResults.reserve(k);
	// vector<vector<int> >vkPath; 
	// kResults.clear();
	// vkPath.clear();

	// // while(num < 100){
	// 	// num ++;
	// do
	// {
	// 	//nodeid in [3,1064] because of INF value by vSPTDistance
	// 	ID1 = rand() % (g.nodeNum-5) + 3; 
	// 	ID2 = rand() % (g.nodeNum-5) + 3;
	// 	dist = g.distanceQuery(ID1+1, ID2+1);
	// }while(ID1 == ID2 || g.vbISO[ID1] || g.vbISO[ID2] || ID1<3 || ID2<3 || dist > Lon*0.5 );
 
 // 	g.DAPrune(ID1, ID2, k, querykey, kResults, vkPath);
 // 	for(auto p:kResults){
 // 		cout<<p<<":"<<endl;
 // 		// for()
 // 	}
// }
}