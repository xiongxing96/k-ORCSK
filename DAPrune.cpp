#include "graph.h"

int Graph::DAPrune(int ID1, int ID2, int k, vector<int>& query, vector<int>& kResults, vector<vector<int> >& vkPath, vector<int>& kPs, vector<vector<int>>& vkPs){
	std::chrono::high_resolution_clock::time_point t1;
	std::chrono::high_resolution_clock::time_point t2;
	std::chrono::duration<double> time_span;
	std::chrono::duration<double> time_lb;
	std::chrono::duration<double> time_pnn;

	// Initialize global var
	QueryWord.clear();// not include query keywords in both ID1 and ID2 
	QueryBit.reset();// not include query keywords in both ID1 and ID2 
	Qu.reset();		// include query keywords in both ID1 and ID2
	Cache.clear();

	//set QueryBit and QueryWord
	for(int k:query){
		QueryBit.set(k);
		Qu.set(k);
		if( !NodesBit[ID1].test(k) && !NodesBit[ID2].test(k) ){
			QueryWord.push_back(k);
		}
	}
	QueryBit ^= (QueryBit & ( NodesBit[ID1] | NodesBit[ID2] ));//delete keywords from ID1 and ID2

	//Shortest Path Tree Info
	vector<int> vSPTDistance(nodeNum, INF); //record dist from node to root ID2 
	vector<int> vSPTParent(nodeNum, -1);  //record node's prenode in shorest path
	vector<int> vSPTParentEdge(nodeNum, -1); 
	vector<int> vTmp;
	vector<vector<int> > vSPT(nodeNum, vTmp); //Tree from root  ID2
	vector<bitset<KIND> > vSPTBit(NodesBit);  //path binary from node to root ID2
	SPT(ID2, vSPTDistance, vSPTParent, vSPTParentEdge, vSPT);

	//avoid of node's distance equal to INF by SPT algorithm 1 and 2
	// int nn=0;//nn=2 means two node (id=1 or 2)both not able
	// 	for(int i=0;i<nodeNum;i++){
	// 		if(vSPTDistance[i]==INF)
	// 	{nn++;cout<<i<<"\t"<<vSPTDistance[i]<<endl;}
	// }
	// cout<<"nn = "<<nn<<endl;
	// if(nn>0)return 0;

	// SPTree for compute path binary from node to ID2
	queue<int>SPTree;
	SPTree.push(ID2);
	while(!SPTree.empty()){
		int node = SPTree.front();
		if(vSPTParent[node] != -1){
			vSPTBit[node] |= vSPTBit[vSPTParent[node]];
		}
		SPTree.pop();
		if(vSPT[node].size()>0){
			for(int i=0;i<vSPT[node].size();i++){
				SPTree.push(vSPT[node][i]);
			}
		}
	}

	//maintain the path in PT tree
 	vector<int> vDistance;		//distance
	vector<int> vPathParent;	//Deviated from
	vector<int> vPathParentPos;	//Deviated Pos from Parent
 	vector< vector<int> > vvPathCandidate;	 //nodes
	vector< vector<int> > vvPathCandidateEdge;//edges 	
	vector< bitset<KIND> > vPathBit;  //curpath keywords binary 
	vector< bitset<KIND> > vPathParentBit;  //curpath keywords binary 
	vector<int> vPathLB;
	vector<int> vPath; 
	vector<int> vPathEdge;
	vector<int> best;	//return result path
	int bestcost;	//result path cost

	//push the shortest path into queue
	vPath.push_back(ID1);
	int p = vSPTParent[ID1]; 
	int e = vSPTParentEdge[ID1];
	while(p != -1)
	{
		vPath.push_back(p); 
		vPathEdge.push_back(e);
		e = vSPTParentEdge[p];
		p = vSPTParent[p];
	}
	
	bitset<KIND>testBit(QueryBit);
	testBit &= vSPTBit[ID1];
	if(QueryBit.count()==0||QueryBit.count()==testBit.count()){
		vkPath.push_back(vPath);
		kResults.push_back(vSPTDistance[ID1]);
		return  1;
	}

	benchmark::heap<2, int, int> qPath(nodeNum*3000);  //size
	vvPathCandidate.push_back(vPath); 
	vvPathCandidateEdge.push_back(vPathEdge);
	vDistance.push_back(vSPTDistance[ID1]);  
	vPathParent.push_back(ID1);
	vPathParentPos.push_back(0);

	vPathBit.push_back( QueryBit & (NodesBit[ID1] | NodesBit[ID2]));
	bitset<KIND>vbit(QueryBit);
	int lb = ComputeLB(vPath,vPathBit[0], 0);
	vPathLB.push_back(lb);
	qPath.update(vvPathCandidate.size()-1, vSPTDistance[ID1]+vPathLB[0]);  //push operater 
	int topPathID, topPathDistance; //flag of top
	int pcount = 0;
	int UB = INF;
	int m = 0;
	unordered_map<int,int> DIST;//filter different POI set routes(simple with distance)
	while( (int)kResults.size() < k && qPath.size()>0 )
	{
		// pcount++;
		qPath.extract_min(topPathID, topPathDistance); 
		//break return
        	bitset<KIND>bestBit(QueryBit);
        	bestBit &= (vPathBit[topPathID] | vSPTBit[vPathParent[topPathID]]); 
		if(bestBit.count()==QueryBit.count() && vPathLB[topPathID]==0){
			vector<int>bestpoi;
			int result = PruneRepeatedPoiPath(vvPathCandidate[topPathID], bestpoi);
			if(kResults.back()!=result){
				//Distinct POI set by distance 
				if(DIST.count(result)){
					continue;	
				}else{
					DIST.insert(make_pair(result,1));
				} 
				m++;
				kResults.push_back(result);
				vkPath.push_back(vvPathCandidate[topPathID]);

				kPs.push_back(result);
				vkPs.push_back(bestpoi);

				// to compute Diversity
				/*if(kPs.size()==0){
					kPs.push_back(result);
					vkPs.push_back(bestpoi);	
				}else{// filter divesity results 
					bool fg = false;
					for(int i=0;i<vkPs.size();i++){
						if(ComputeSim(vkPs[i], bestpoi) > 0.5){//Set similarity value = 0.75 
							fg = true;					
						}								
					}
					if(!fg){
						kPs.push_back(result);
						vkPs.push_back(bestpoi);
					}
				}
				*/
			}		
			continue;
		}
		// prune1 by UB(uppperbound)
		if(vDistance[topPathID]+vPathLB[topPathID] > UB && m >= k ) continue;

		// deviate by path deviation 
			int pos = vPathParentPos[topPathID]+1;
			int predist = 0; //compute predist before pos; 
			bitset<KIND>_tmpBit(vPathBit[topPathID]);
			int pnn=-1;
			for(int i = pos-1;i < vvPathCandidate[topPathID].size()-1;i++){
				vPath.clear();
				int deviation = vvPathCandidate[topPathID][i];
				int numkey = _tmpBit.count(); //curentpos  keybit
				_tmpBit|=(QueryBit & NodesBit[deviation]);
				//	accumulate predist if deviate from pos then nodo else do++
				//	<1>insert p[ID1,pos]
				vector<int>tmPath(vvPathCandidate[topPathID].begin(),vvPathCandidate[topPathID].begin()+i);
				//	<2>insert sp(pos,PNN]
				int PNN=-1;
				pair<int,int> TwoPNN(-1,-1);
				int PNNdist = FindPNN(deviation ,ID2,_tmpBit, TwoPNN);
				PNN = TwoPNN.first;
				// Prune2 Dominance relationship, prune the path, compare with before deviation PNN 
				if(PNN == pnn && numkey==_tmpBit.count())continue;
				vector<int>vPathPNN;
				vector<int>vPathPNNEdge;
				int distn = H2HPath(deviation, PNN, vPathPNN, vPathPNNEdge);
				pnn = PNN;
				bitset<KIND>_bit;
				_bit.reset();
				for(auto &v:vPathPNN){_bit |= NodesBit[v];}
				_bit&=QueryBit;
				if(distn != 0){
					tmPath.insert(tmPath.end(), vPathPNN.begin(), vPathPNN.end()-1);
				}
				// or bit for new node PNN
				bitset<KIND>tmpBit(QueryBit);
				tmpBit &= (NodesBit[PNN] | _tmpBit | _bit);				
				// update deviation node pos = PNN 
				int tmpos = i + vPathPNN.size() - 1;
				//	<3>insert sp(PNN->ID2]
				int p = vSPTParent[PNN];
				tmPath.push_back(PNN);
				vector<int>tp;
				int da = PruneRepeatedPoiPath(tmPath);
				while(p != -1){
					tmPath.push_back(p);
					p = vSPTParent[p];
				}
				// int tmpdist = predist + distn + vSPTDistance[PNN]; 
				int tmpdist = da + vSPTDistance[PNN];
				t1 = std::chrono::high_resolution_clock::now();
				int tmplb = ComputeLB(tmPath,tmpBit,tmpos);
				t2 = std::chrono::high_resolution_clock::now();
				time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
				//cout<<"lb : "<<time_span.count()<<endl; 
				time_lb+=time_span;
				//find potetial valid paths, update UB 
				bitset<KIND>_Bit(QueryBit);
        			_Bit &= (tmpBit | vSPTBit[PNN]); 
				if(_Bit.count() == QueryBit.count()){
					m++;
					if(m <= k){
						UB = max(UB, tmpdist + tmplb);					
					}else{
				 		UB = min(UB, tmpdist + tmplb);
					}
				}
				vvPathCandidate.push_back(tmPath);
				vPathParent.push_back(PNN);
				vPathParentPos.push_back(tmpos);
				vPathBit.push_back(tmpBit);
				vDistance.push_back(tmpdist);
				vPathLB.push_back(tmplb);
				qPath.update(vvPathCandidate.size()-1,tmpdist+tmplb);
		}
	}
	//cout<<"LB : "<<time_lb.count()<<endl; 
	return qPath.size();
}
//	compute Divesity result routes   
float Graph::ComputeSim(vector<int> A, vector<int> B){
	vector<int>n;
	vector<int>m;
	sort(A.begin(), A.end());
	sort(B.begin(), B.end());
	set_intersection(A.begin(),A.end(),B.begin(),B.end(),back_inserter(n));
	set_union(A.begin(),A.end(),B.begin(),B.end(),back_inserter(m));
	return (n.size()-2)*1.0/(m.size()-2);//delete vertexes pair(s,t)
}
//	compute Skyline result routes
bool Graph::CompteSkyline(vector<int> &A, int disA, vector<int> &B, int disB){
	if((B.size() <= A.size() && disB < disA) || (B.size() < A.size() && disB <= disA)){
		return true;
	}
	return false;
} 

// no need do EuclideanDistance for prune because the same time with distanceQuery; 

// int Graph::FindPNN(int deviation, bitset<KIND> &vPathBit, int &PNN ){
// 	// cout<<"--------------(1)FindPNN:-------------------------"<<endl;
// 	bitset<KIND>tbit(vPathBit);
// 	tbit &= QueryBit;
// 	tbit ^= QueryBit;
// 	// cout<<tbit.count()<<endl;
// 	if(vPathBit.count()==QueryBit.count()){
// 		PNN = -1;
// 		return 0;
// 	}
// 	int Min_val = INF;
// 	int Min_node;
// 	for(auto &k : QueryWord){
// 		if(tbit.test(k)){
// 			// cout<<"k = "<<k<<" ->";
// 			int M = INF,N=-1;
// 			for(int i=0;i<RSP[k].size();i++){
// 				int d = distanceQuery(deviation+1,RSP[k][i]+1);
// 				if(d<Min_val){
// 					Min_node = RSP[k][i];
// 					Min_val = d;
// 				}
// 				if(d<M){
// 					N = RSP[k][i];
// 					M = d;
// 				}
// 			}
// 			// cout<<"mincost = "<<M<<"     node = "<<N<<endl;
// 		}
// 	}
// 	PNN = Min_node;
// 	// cout<<"res : "<<Min_val<<"\tPNN : "<<Min_node<<endl;
// 	return Min_val;
// }

// find P1NN and P2NN
int Graph::FindPNN(int deviation, int t,  bitset<KIND> &vPathBit, pair<int,int> &PNN ){
	bitset<KIND>tbit(vPathBit);
	tbit &= QueryBit;
	tbit ^= QueryBit;
	if(vPathBit.count()==QueryBit.count()){
		PNN.first = -1;
		PNN.second = -1;
		cout<<" -----------error !"<<endl;
		return 0;
	}

	int firstmin = INF;
	int secondmin = INF;
	int firstnode = -1;
	int secondnode = -1;
	for(auto &k : QueryWord){
		if(tbit.test(k)){
			for(int i = 0; i < RSP[k].size(); i++){
				int d = distanceQuery(deviation+1,RSP[k][i]+1);
				if(d < firstmin){
					firstmin = d;
					firstnode = RSP[k][i];
				}else if(d < secondmin && d != firstmin){
					secondmin = d;
					secondnode = RSP[k][i];
				}
			}
		}
	}
	// cout<<"firstmin : "<<firstmin <<"\tPNN1 : "<<firstnode<<endl;
	// cout<<"secondmin : "<<secondmin<<"\tPNN2 : "<<secondnode<<endl;
	PNN.first = firstnode;
	PNN.second = secondnode;      
	return firstmin;
}

// just pos and t, compute maxLB with Improvement
int Graph::ComputeLB(vector<int> &vpath, bitset<KIND> &vpathposBit, int pos){
	bitset<KIND> Bit(vpathposBit ^ QueryBit);// get not satisfied keyword and set 1
	int s = vpath[pos], t = vpath[vpath.size() - 1];
	int st = distanceQuery(s + 1, t + 1);
	if(Bit.count() == 0)return 0;//all keyword satisfied
	int Max_val = 0, _advance = -1;
	int kp = -1, node = -1, farnode = -1;
	bool flag = false;
	for(auto &k : QueryWord){//foreach keyword
		if(Bit.test(k)){
			int Min_val = INF;
			for(int i = 0; i < RSP[k].size(); i++){//foreach node contain keyword k
				int d = distanceQuery(s + 1, RSP[k][i] + 1) + distanceQuery(RSP[k][i] + 1, t + 1) - st;
				if(d <_advance){	// for advance prune
					flag = true;
					break;
				}
				if(d < Min_val){
					Min_val = d;
					node = RSP[k][i];
				}

			}
			if(!flag && Min_val > Max_val){
				Max_val = Min_val;
				_advance = Max_val;
				kp = k;
				farnode = node;
				flag = false;
			}
		}	
	} 
	return Max_val;
}

// change bestpoi set result by RFM(refinement)
int Graph::PruneRepeatedPoiPath(vector<int> bestpath, vector<int>& bestpoi){
	reverse(bestpath.begin(),bestpath.end());
	//vector<int> bestpoi;
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

//only return distance by RFM(refinement)
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
/*
// just pos and t, compute maxLB with Naive
 int Graph::ComputeLB(vector<int> &vpath, bitset<KIND> &vpathposBit, int pos){
 	bitset<KIND>Bit(vpathposBit^QueryBit);// get not satisfied keyword and set 1
 	int s = vpath[pos],t = vpath[vpath.size()-1];
 	int st = distanceQuery(s+1, t+1);
 	if(Bit.count()==0)return 0;//all keyword satisfied
 	int Max_val = 0;
 	int kp=-1 ,node=-1,farnode=-1;
 	for(auto &k : QueryWord){//foreach keyword
 		if(Bit.test(k)){
 			int Min_val = INF;
 			for(int i=0;i<RSP[k].size();i++){//foreach node contain keyword k
 				int d = distanceQuery(s+1,RSP[k][i]+1)+distanceQuery(RSP[k][i]+1,t+1)-st;
 				if(d < Min_val){
 					Min_val = d;
 					node = RSP[k][i];
 				}
 			}
 			if(Min_val > Max_val){
 				Max_val = Min_val;
 				kp = k;
 				farnode = node;
 			}
 		}	
 	} 
 	return Max_val;
 }
*/





