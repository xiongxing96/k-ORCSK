#include "graph.h"
int main(int argc, char *argv[])
{
	// cout<<argv[0]<<endl;// first var is ./KSP
	// string keysFile = "./map/data_cal/sub/cal_sub_keys_10_150.node";	//keys file
	string keysFile = argv[1];
	string graphFile = "./map/USA-road-d.CAL.gr";			//graph file
	const char *indexFile ="./map/data_cal/h2h/cal_sub.index";	//H2H index file
	const char *edgeFile ="./map/data_cal/h2h/cal_sub.edge";	//edge file(graph file same)

	Graph g = Graph(graphFile);
	g.set_nodeKEYS_NodesBit(keysFile);
	g.Init_H2H_Index(indexFile, edgeFile);
	return 0;

	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	std::chrono::duration<double> time_span;
	std::chrono::duration<double> DA_time_totol;
	std::chrono::duration<double> GTree_time_totol;

	vector<int>querykey;
	string ss = argv[2];
	strToVec(querykey,ss);	//trans
	fstream infile(argv[3],ios::in);	// test file input ID1 and ID2
	ofstream outfile(argv[4],ios::app);	// result outfile 
	ofstream outlen("testData/compare_cal_10.txt");

	// int k = 10;  
	int k = atoi(argv[5]);     
	vector<int> kResults;
	vector<int> Result;
	kResults.reserve(k);
	vector<vector<int> >vkPath; 
	kResults.clear();
	vkPath.clear();
	vector<int> vPath, vPathEdge; 
	vector<int> bestpath, bestpoi; 
	int num = 0, num1 = 0, num2 = 0, num3 = 0, num4 = 0;
	int res = 0;
	long costd1 = 0, costg1 = 0, costd2 = 0, costg2 = 0;
	long T1 = 0, T2 = 0, T3 = 0 ;
	srand (time(NULL));
	int ID1, ID2;
	while(infile >> ID1 >> ID2 >> res){
	if(num == 100)break;

	// while(num==2){
	// do
	// {
	// 	//nodeid in [3,1064] because of INF value by vSPTDistance
	// 	ID1 = rand() % (g.nodeNum-5) + 3; 
	// 	ID2 = rand() % (g.nodeNum-5) + 3;
	// }while(ID1 == ID2 || g.vbISO[ID1] || g.vbISO[ID2] || ID1<3 || ID2<3 ); 
	// ID1=978, ID2=758;
	cout  << "ID1:" << ID1 << "\tID2:" << ID2 << endl; 

	kResults.clear();
	Result.clear();
	vkPath.clear();
	bestpoi.clear();
	bestpath.clear();

	t1 = std::chrono::high_resolution_clock::now();
	int size = g.DAPrune(ID1, ID2, k, querykey, kResults, vkPath); 
	t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
	DA_time_totol = DA_time_totol + time_span;
	cout << "DAmrrp Time:" << time_span.count() << endl;
	int da = *min_element(kResults.begin(),kResults.end());
	T1 += da;
	T3 += size;
	outlen << ID1 <<"\t"<<ID2<<"\t"<<da<<endl;
	// for(auto& d : kResults){
	// 	cout<<d<<" ";
	// }
	// cout<<endl;
	


	t1 = std::chrono::high_resolution_clock::now();
	int clen = g.Clen(ID1, ID2, g.QueryBit, Result);
	t2 = std::chrono::high_resolution_clock::now();
	time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
	GTree_time_totol = GTree_time_totol + time_span;
	cout << "Clen Time:" << time_span.count()<< endl;
	T2 += clen;

	int dij = g.distanceQuery(ID1+1,ID2+1);
	cout<<"dijkstra = "<<dij<<endl;
	cout<<"DAmrrp   = "<<da<<endl;
	cout<<"Clen     = "<<clen<<endl<<endl;
	num++;
	
	if(da == clen && da == dij)continue;
	if(da == clen){
		num1++;
	}else if(da < clen){
		num2++;
		costd1 += da;
		costg1 += clen;
	}else{
		// g.PruneRepeatedPoiPath(vkPath[0],bestpoi);
		num3++;
		costd2 += da;
		costg2 += clen;
		// outest << ID1 << "\t" << ID2 <<"\t\tda = "<<da<<" | ";
		// for(int p : bestpoi ){outest<<p<<" ";}
		// outest << "\t\t | clen = "<< clen <<" | ";
		// for(int p : Result){outest<<p<<" "; }
		// outest<<endl; 
	}


	}

	outfile <<keysFile<< "\tk = "<<argv[5]<<endl;
	outfile <<"keywords = "<<querykey.size()<<"\t"<<ss<<endl;
	outfile <<"Test for "<<num<<" times query set"<<endl;
	outfile <<"num1 = "<<num1<<"	(DAmrrp = GTree) equal per = "<<(double)num1/num*100<<"%"<<endl;
	outfile <<"num2 = "<<num2<<"	(DAmrrp < GTree) small per = "<<(double)num2/num*100<<"%"<<endl;
	outfile <<"num3 = "<<num3<<"	(DAmrrp > GTree) large per = "<<(double)num3/num*100<<"%"<<endl;
	num4 = num-num1-num2-num3;
	outfile <<"rest = "<<num4<<"	(dijkstra  path) satis per = "<<(double)num4/num*100<<"%"<<endl;
	outfile <<"(DAmrrp < GTree) totol cost  : "<<costd1<<"     "<<costg1<<"     "<<(double)costd1/costg1<<endl;
	outfile <<"(DAmrrp > GTree) totol cost  : "<<costd2<<"     "<<costg2<<"     "<<(double)costg2/costd2<<endl;
	// outfile<<"same = "<<same<<endl;
	// outfile<<"less = "<<less<<endl;
	outfile <<"DAmrrp : totol time = "<<DA_time_totol.count()<<"\t"<<"average time = "<<DA_time_totol.count()/num<<endl;
	outfile <<"GTree : totol time = "<<GTree_time_totol.count()<<"\t"<<"average time = "<<GTree_time_totol.count()/num<<endl;
	outfile <<"DAmrrp : totol length = "<<T1<<"\taverager length = "<<T1/num<<endl;
	outfile <<"GTree : totol length = "<<T1<<"\taverager length = "<<T2/num<<endl;
	outfile <<"DAPrune : totol nums = "<<T3<<"\taverager routes = "<<T3/num<<endl;
	outfile <<"-------------------------(P1NN Result)"<<endl<<endl;
	outfile.close();
	infile.close();

	// int k = 20000;       
	// vector<int> kResults;
	// kResults.reserve(k);
	// vector<vector<int> >vkPath; 
	// kResults.clear();
	// vkPath.clear();
	// t1 = std::chrono::high_resolution_clock::now();
	// int res = g.eKSPNew(ID1, ID2, k, querykey, kResults, vkPath); 
	// t2 = std::chrono::high_resolution_clock::now();
	// time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
	// cout << "eKSP Time:" << time_span.count() <<" cost = "<<res<< endl;
	// int e=0;
	// for(auto& d : kResults){
	// 	cout << d << " : ";
	// 	for(auto &p : vkPath[e++])
	// 		cout<<p<<" ";
	// 	cout<<endl;
	// }
	// cout << endl;
	
	// g.FindRepeatedPath(vkPath);
	// kResults.clear();
	// vkPath.clear();
	return 0;

}
int Graph::PruneRepeatedPoiPath(vector<int> bestpath){
	reverse(bestpath.begin(),bestpath.end());
	vector<int> bestpoi;
	bestpoi.push_back(bestpath.front());
	bitset<KIND> qu(Qu);
	for(auto &p : bestpath){
		bitset<KIND>testpoi(NodesBit[p]);
		testpoi &= qu;
		qu ^= testpoi;
		if(testpoi.count() > 0){bestpoi.push_back(p);};//keyword unique
	}
	bestpoi.push_back(bestpath.back());
	int da = 0;
	for(int i = 0;i < bestpoi.size() - 1; i++){
		da += distanceQuery(bestpoi[i] + 1, bestpoi[i+1] + 1);
	}
	return da;
}
int Graph::PruneRepeatedPoiPath(vector<int>& bestpath, vector<int>& bestpoi){
	reverse(bestpath.begin(),bestpath.end());//prune 3
	bitset<KIND> t1;
	bitset<KIND> t2;
	for(auto &v : bestpath){
		t2.reset();
		t2 = QueryBit;
		t2 &= NodesBit[v];
		// cout<<v<<" ";
		// if(t2.count() > 0)cout<<"(Y)";
		t1 |= NodesBit[v];
	}
	// cout<<endl;
	t1 &= QueryBit;
	bestpoi.push_back(bestpath[0]);
	bitset<KIND> qu(Qu);
	for(auto p : bestpath){
		bitset<KIND>testpoi(NodesBit[p]);
		testpoi &= qu;
		qu ^= testpoi;
		if(testpoi.count() > 0){bestpoi.push_back(p);};//keyword unique
	}
	bestpoi.push_back(bestpath[bestpath.size()-1]);
	int da = 0;
	for(int i = 0;i < bestpoi.size() - 1; i++){
		cout<<bestpoi[i]<<" ";
		da += distanceQuery(bestpoi[i] + 1, bestpoi[i+1] + 1);
	}
	// cout<<endl;
	cout<<bestpoi.back();
	reverse(bestpoi.begin(),bestpoi.end());
	return da;

}

void Graph::FindRepeatedPath(vector<vector<int> >& vvPath)
{
	for(int i = 0; i < (int)vvPath.size()-1; i++)
	{
		vector<int> vSame;
		for(int j = i + 1; j < (int)vvPath.size(); j++)
		{
			if(vvPath[i].size() != vvPath[j].size())
				continue;
			
			bool bSame = true;
			for(int k = 0; k < vvPath[i].size(); k++)
			{
				if(vvPath[i][k] != vvPath[j][k]) 
				{
					bSame = false;
					break;
				}
			}

			if(bSame)
				vSame.push_back(j);
		}

		if(!vSame.empty()) 
		{
			cout << "Same path of " << i << ":" << endl;
			for(auto& sID : vSame)
				cout << sID << "\t";
			cout << endl;
		}
	}
}
