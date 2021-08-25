#include "graph.h"

int Graph::DAPrune(int ID1, int ID2, int k, vector<int>& query, vector<int>& kResults, vector<vector<int> >& vkPath){
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
		// cout<<k<<" ";
	}
	// cout<<"\t before QueryNums = "<<QueryBit.count()<<endl;
	// cout<<"keywords : ";
	QueryBit ^= (QueryBit & ( NodesBit[ID1] | NodesBit[ID2] ));//delete keywords from ID1 and ID2
	// for(auto &k:QueryWord){cout<<k<<" ";}
	// cout<<"\t after QueryNums = "<<QueryBit.count()<<endl;
	// cout<<QueryBit.to_string()<<endl;

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

	// cout<<"start heap while: "<<endl;
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
	while( (int)kResults.size() < k && qPath.size()>0 )
	{
		// pcount++;
		qPath.extract_min(topPathID, topPathDistance); 
	   	// cout << endl << pcount << "\t" <<topPathID<<" \t"<< topPathDistance  << "\t" <<  endl; 
		
		//break return
        bitset<KIND>bestBit(QueryBit);
        bestBit &= (vPathBit[topPathID] | vSPTBit[vPathParent[topPathID]]); 
		if(bestBit.count()==QueryBit.count() && vPathLB[topPathID]==0){
			// bestPath = vvPathCandidate[topPathID];
			// cout<<"........................qPath.size() = "<<qPath.size()<<endl;
			// cout << "ComputeLB Time:" << time_lb.count() << endl;
			// cout << "UB = "<<UB<<endl;
			// return vDistance[topPathID]+vPathLB[topPathID];
			int size = kResults.size();
			int result = PruneRepeatedPoiPath(vvPathCandidate[topPathID]);
			if(kResults.back()==result)
				continue;
			kResults.push_back(result);
			vkPath.push_back(vvPathCandidate[topPathID]);
			continue;
		}
		// prune1 by UB
		// if(vDistance[topPathID]+vPathLB[topPathID] > UB)continue;
		
		// deviate by path deviation 
			int pos = vPathParentPos[topPathID]+1;
			int predist = 0; //compute predist before pos; 
			bitset<KIND>_tmpBit(vPathBit[topPathID]);
			// if(pos>1){
			// 	for(int i=0;i<pos;i++){
			// 		predist+=distanceQuery(vvPathCandidate[topPathID][i]+1,vvPathCandidate[topPathID][i+1]+1);	
			// 	}
			// }

			// cout<<"dist to pos = "<<predist<<endl;
			int pnn=-1;
			for(int i = pos-1;i < vvPathCandidate[topPathID].size()-1;i++){
				vPath.clear();
				int deviation = vvPathCandidate[topPathID][i];

				int numkey = _tmpBit.count(); //curentpos  keybit
				_tmpBit|=(QueryBit & NodesBit[deviation]);
				//	accumulate predist if deviate from pos then nodo else do++
				// if(i>0)predist+=distanceQuery(vvPathCandidate[topPathID][i-1]+1,vvPathCandidate[topPathID][i]+1);
				

				//	<1>insert p[ID1,pos]
				vector<int>tmPath(vvPathCandidate[topPathID].begin(),vvPathCandidate[topPathID].begin()+i);

				//	<2>insert sp(pos,PNN]
				int PNN=-1;
				pair<int,int> TwoPNN(-1,-1);
				int PNNdist = FindPNN(deviation ,ID2,_tmpBit, TwoPNN);
				PNN = TwoPNN.first;

				// Prune2, prune the path, compare with before deviation PNN 
				// cout<<PNN<<" "<<pnn<<" "<<numkey<<" "<<_tmpBit.count()<<endl;
				if(PNN == pnn && numkey==_tmpBit.count())continue;

				vector<int>vPathPNN;
				vector<int>vPathPNNEdge;
				int distn = H2HPath(deviation, PNN, vPathPNN, vPathPNNEdge);
				pnn = PNN;
				bitset<KIND>_bit;
				_bit.reset();
				for(auto &v:vPathPNN){_bit|=NodesBit[v];}
				_bit&=QueryBit;
				if(distn!=0){
					tmPath.insert(tmPath.end(), vPathPNN.begin(), vPathPNN.end()-1);
				}
				
				// or bit for new node PNN
				bitset<KIND>tmpBit(QueryBit);
				tmpBit &= (NodesBit[PNN] | _tmpBit | _bit);				

				// update deviation node pos = PNN 
				int tmpos = i+vPathPNN.size()-1;

				//	<3>insert sp(PNN->ID2]
				int p = vSPTParent[PNN];
				tmPath.push_back(PNN);
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
				time_lb+=time_span;
				
				// if(QueryBit.count()-tmpBit.count() <= 1){
				// 	UB = min(UB,tmpdist + tmplb);
				// }
				
				vvPathCandidate.push_back(tmPath);
				vPathParent.push_back(PNN);
				vPathParentPos.push_back(tmpos);
				vPathBit.push_back(tmpBit);
				vDistance.push_back(tmpdist);
				vPathLB.push_back(tmplb);
				qPath.update(vvPathCandidate.size()-1,tmpdist+tmplb);
		}
	}
	// return -1;
	return qPath.size(); 
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
	// cout<<"--------------(1)FindPNN:-------------------------"<<endl;

	// ofstream outfile("./testdata1.txt",ios::app);	// result outfile  
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
	// string ss="";
	for(auto &k : QueryWord){
		if(tbit.test(k)){
			// cout<<"k = "<<k<<" ->";
			// ss+=to_string(k);
			// ss+=" ";
			for(int i=0;i<RSP[k].size();i++){
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

	// vector<int>vPathPNN;
	// vector<int>vPathPNNEdge;
	// int distn = H2HPath(deviation, t, vPathPNN, vPathPNNEdge);
	// string st="";
	// for(int &p : vPathPNN){
	// 	st+=to_string(p);
	// 	st+=" ";
	// }        
	// outfile << st <<endl << ss << "\t" << PNN.first << endl<<endl;
	return firstmin;
}

// // just pos and t
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

// just pos and t
// int Graph::ComputeLB(vector<int> &vpath, bitset<KIND> &vpathposBit, int pos){
// 	// ofstream outfile("./testdata2.txt",ios::app);	// result outfile 
// 	// cout<<"================(2)computeLB:========================"<<endl;
// 	bitset<KIND>Bit(vpathposBit^QueryBit);// get not satisfied keyword and set 1
// 	int s = vpath[pos],t = vpath[vpath.size()-1];
// 	int st = distanceQuery(s+1, t+1);
// 	if(Bit.count()==0)return 0;//all keyword satisfied
// 	int Max_val = 0;
// 	int kp=-1 ,node=-1,farnode=-1;
// 	// string ss="";
// 	for(auto &k : QueryWord){//foreach keyword
// 		if(Bit.test(k)){
// 			// cout<<"k = "<<k<<" ";
// 			// ss+=to_string(k);
// 			// ss+=" ";
// 			int Min_val = INF;
// 			for(int i=0;i<RSP[k].size();i++){//foreach node contain keyword k
// 				int d = distanceQuery(s+1,RSP[k][i]+1)+distanceQuery(RSP[k][i]+1,t+1)-st;
// 				if(d < Min_val){
// 					Min_val = d;
// 					node = RSP[k][i];
// 				}
// 			}
// 			if(Min_val > Max_val){
// 				Max_val = Min_val;
// 				kp = k;
// 				farnode = node;
// 			}
// 		}	
// 	} 
	// cout<<"Max_val = "<<Max_val<<" K="<<kp<<" farnode = "<<farnode<<endl;
	// vector<int>vPathPNN;
	// vector<int>vPathPNNEdge;
	// int distn = H2HPath(s, t, vPathPNN, vPathPNNEdge);
	// string s_t="";
	// for(int &p : vPathPNN){
	// 	s_t+=to_string(p);
	// 	s_t+=" ";
	// }        
	// outfile << s_t <<endl << ss << "\t" << farnode << endl<<endl;
// 	return Max_val;
// }

// after pos pair 
// int Graph::ComputeLB(vector<int> &vpath, bitset<KIND> &vpathposBit, int pos){
// 	// cout<<"================(2)computeLB:"<<endl;
// 	bitset<KIND>tbit;
// 	vector<int>poi;
// 	bitset<KIND>Bit(vpathposBit^QueryBit);
// 	bitset<KIND>bit(QueryBit);
// 	for(int i=pos;i<vpath.size();i++){
// 		int p = vpath[i];
// 		tbit|=NodesBit[p];
// 		if(i==pos||i==(vpath.size()-1)){poi.push_back(p);continue;}//push ID1 and ID2
// 		bit&=NodesBit[p];
// 		if(bit.count()!=0){ poi.push_back(p);} 
// 		bit^=Bit;
// 	}
// 	tbit &= Bit;
// 	tbit ^= Bit;
// 	// not satisfied keywords set 1

// 	vector<int>pathDist;
// 	for(int i=0;i<poi.size()-1;i++){
// 		pathDist.push_back(distanceQuery(poi[i]+1,poi[i+1]+1));
// 	}

// 	if(tbit.count()==0)return 0;//all keyword satisfied
// 	int Max_val = 0;
// 	int kp=-1 ,node=-1,farnode=-1;
// 	for(auto &k : QueryWord){//foreach keyword
// 		if(tbit.test(k)){
// 			// cout<<"k = "<<k<<" ";
// 			int Min_P = INF;
// 			for(int i=0;i<RSP[k].size();i++){//foreach node contain keyword k
// 				int Min_val = INF;
// 				int tn;
// 				for(int j=0;j<poi.size()-1;j++){//foreach pair node of path_poi
// 					int d = distanceQuery(poi[j]+1,RSP[k][i]+1)+distanceQuery(RSP[k][i]+1,poi[j+1]+1)-pathDist[j];
// 					if(d<Min_val)
// 						Min_val=d;
// 				}
// 				if(Min_val < Min_P){
// 					Min_P = Min_val;
// 					node = RSP[k][i];
// 				}
// 			}
// 			if(Min_P > Max_val){
// 				Max_val = Min_P;
// 				kp = k;
// 				farnode = node;
// 			}
// 			// cout<<"			Min_P = "<<Min_P<<"    node = "<<node<<endl;
// 		}	
// 	} 
// 	// cout<<"Max_val = "<<Max_val<<" K="<<kp<<" farnode = "<<farnode<<endl;
// 	return Max_val;
// }

//totol poi pair
// int Graph::ComputeLB(vector<int> &vpath, bitset<KIND> &vpathposBit, int pos){
// 	// cout<<"================(2)computeLB:"<<endl;
// 	// already have keyword poi
// 	bitset<KIND>tbit;
// 	vector<int>poi;
// 	bitset<KIND>bit(QueryBit);
// 	for(int i=0;i<vpath.size();i++){
// 		int p = vpath[i];
// 			// cout<<p<<" ";
// 		tbit|=NodesBit[p];
// 		if(i==0||i==(vpath.size()-1)){poi.push_back(p);continue;}//push ID1 and ID2

// 		bit&=NodesBit[p];
// 		if(bit.count()!=0){ poi.push_back(p);} 
// 		bit^=QueryBit;
// 		// if(bit.count()!=0){ poi.push_back(p);} 
// 	}
// 	// cout<<endl;
// 	// cout<<"QueryWord = "<<QueryWord.size()<<endl;
// 	// for(auto &k : QueryWord)cout<<k<<" ";cout<<endl;
// 	tbit &= QueryBit;
// 	tbit ^= QueryBit;
// 	// not satisfied keywords
// 	// cout<<tbit.count()<<endl;
// 	// cout<<tbit.to_string()<<endl;
	
// 	// cout<<"poi.size()="<<poi.size()<<endl;
// 	vector<int>pathDist;
// 	for(int i=0;i<poi.size()-1;i++){
// 		pathDist.push_back(distanceQuery(poi[i]+1,poi[i+1]+1));
// 	}
// 	// cout<<"pathDist = "<<pathDist.size()<<endl;

// 	if(tbit.count()==0)return 0;//all keyword satisfied
// 	int Max_val = 0;
// 	int kp=-1 ,node=-1,farnode=-1;
// 	for(auto &k : QueryWord){//foreach keyword
// 		if(tbit.test(k)){
// 			// cout<<"k = "<<k<<" ";
// 			int Min_P = INF;
// 			for(int i=0;i<RSP[k].size();i++){//foreach node contain keyword k
// 				int Min_val = INF;
// 				int tn;
// 				for(int j=0;j<poi.size()-1;j++){//foreach pair node of path_poi
// 					int d = distanceQuery(poi[j]+1,RSP[k][i]+1)+distanceQuery(RSP[k][i]+1,poi[j+1]+1)-pathDist[j];
// 					if(d<Min_val)
// 						Min_val=d;
// 				}
// 				if(Min_val < Min_P){
// 					Min_P = Min_val;
// 					node = RSP[k][i];
// 				}
// 			}
// 			if(Min_P > Max_val){
// 				Max_val = Min_P;
// 				kp = k;
// 				farnode = node;
// 			}
// 			// cout<<"			Min_P = "<<Min_P<<"    node = "<<node<<endl;
// 		}	
// 	} 
// 	// cout<<"Max_val = "<<Max_val<<" K="<<kp<<" farnode = "<<farnode<<endl;
// 	return Max_val;
// }

/*
//cache
int Graph::ComputeLB(vector<int> &vpath){
	cout<<"================(2)computeLB:"<<endl;

	// already have keyword poi
	bitset<KIND>tbit;
	vector<int>poi;
	bitset<KIND>bit(QueryBit);
	for(int i=0;i<vpath.size();i++){
		int p = vpath[i];
			cout<<p<<" ";
		tbit|=NodesBit[p];
		if(i==0||i==(vpath.size()-1)){poi.push_back(p);continue;}//push ID1 and ID2
		bit&=NodesBit[p];
		if(bit.count()!=0){ poi.push_back(p);} 
		bit^=QueryBit;
		// if(bit.count()!=0){ poi.push_back(p);} 
	}
	cout<<endl;
	cout<<"QueryWord = "<<QueryWord.size()<<endl;
	for(auto &k : QueryWord)cout<<k<<" ";cout<<endl;
	tbit &= QueryBit;
	tbit ^= QueryBit;
	// not satisfied keywords
	cout<<tbit.count()<<endl;
	cout<<tbit.to_string()<<endl;
	
	cout<<"poi.size()="<<poi.size()<<endl;
	vector<int>pathDist;
	for(int i=0;i<poi.size()-1;i++){
		pathDist.push_back(distanceQuery(poi[i]+1,poi[i+1]+1));
	}
	cout<<"pathDist = "<<pathDist.size()<<endl;

	if(tbit.count()==0)return 0;//all keyword satisfied
	int Max_val = 0;
	int kp=-1 ,node=-1,farnode=-1;

	unordered_map<int,int>tmap;
	for(auto &k : QueryWord){
		if(tbit.test(k)){
			tmap[k] = INF;
		}
	}
	int count = 0;
	for(int j=0;j<poi.size()-1;j++){				//foreach pair node of path_poi
		int a = poi[j],b = poi[j+1];
		if(poi[j]>poi[j+1]){a = poi[j+1];b = poi[j];}//swap
		string id_str = to_string(a)+"to"+to_string(b);
		for(auto &k : QueryWord){					//foreach Query KIND K
			if(!tbit.test(k))continue;
			if(Cache.count(id_str) && Cache[id_str].count(k)){//judge if there is history value 
				tmap[k] = Cache[id_str][k];
				count++;
				continue;
			}
			int Min_val = INF;
			for(int i = 0;i < RSP[k].size(); i++){	//foreach RSP[k] node
				int d = distanceQuery( poi[j] + 1, RSP[k][i] + 1) + distanceQuery( RSP[k][i] + 1, poi[j+1] + 1) - pathDist[j];
					if(d < Min_val){
						Min_val = d;
						node = RSP[k][i];
						// farnode = node;
					}
			}
			if(Min_val < tmap[k]){
				tmap[k] = Min_val;
				farnode = node;
				Cache[id_str][k] = Min_val;
				farnode = node;	
			}
			cout<<"       k = "<<k<<"	Min_val = "<<Min_val<<"    node = "<<node<<endl;
		}
	}

	for(auto &[k,v] : tmap){
		if(v >= Max_val){
			Max_val = v;
			kp = k;
		}
	}
	cout<<"\t -----------count = "<<count<<endl;
	 cout<<"Max_val = "<<Max_val<<" K="<<kp<<" farnode = "<<farnode<<endl;
	return Max_val;
}


//cache
int Graph::ComputeLB(vector<int> &vpath){
	cout<<"================(2)computeLB:"<<endl;

	// already have keyword poi
	bitset<KIND>tbit;
	vector<int>poi;
	bitset<KIND>bit(QueryBit);
	for(int i=0;i<vpath.size();i++){
		int p = vpath[i];
			cout<<p<<" ";
		tbit|=NodesBit[p];
		if(i==0||i==(vpath.size()-1)){poi.push_back(p);continue;}//push ID1 and ID2
		bit&=NodesBit[p];
		if(bit.count()!=0){ poi.push_back(p);} 
		bit^=QueryBit;
		// if(bit.count()!=0){ poi.push_back(p);} 
	}
	cout<<endl;
	cout<<"QueryWord = "<<QueryWord.size()<<endl;
	for(auto &k : QueryWord)cout<<k<<" ";cout<<endl;
	tbit &= QueryBit;
	tbit ^= QueryBit;
	// not satisfied keywords
	cout<<tbit.count()<<endl;
	cout<<tbit.to_string()<<endl;
	
	cout<<"poi.size()="<<poi.size()<<endl;
	vector<int>pathDist;
	for(int i=0;i<poi.size()-1;i++){
		pathDist.push_back(distanceQuery(poi[i]+1,poi[i+1]+1));
	}
	cout<<"pathDist = "<<pathDist.size()<<endl;

	if(tbit.count()==0)return 0;//all keyword satisfied
	int Max_val = 0;
	int kp=-1 ,node=-1,farnode=-1;

	unordered_map<int,int>tmap;
	for(auto &k : QueryWord){
		if(tbit.test(k)){
			tmap[k] = INF;
		}
	}
	int count = 0;
	for(int j=0;j<poi.size()-1;j++){				//foreach pair node of path_poi
		int a = poi[j],b = poi[j+1];
		if(poi[j]>poi[j+1]){a = poi[j+1];b = poi[j];}//swap
		string id_str = to_string(a)+"to"+to_string(b);
		for(auto &k : QueryWord){					//foreach Query KIND K
			if(!tbit.test(k))continue;
			if(Cache.count(id_str) && Cache[id_str].count(k)){//judge if there is history value 
				tmap[k] = Cache[id_str][k];
				count++;
				continue;
			}
			int Min_val = INF;
			for(int i = 0;i < RSP[k].size(); i++){	//foreach RSP[k] node
				int pnode = RSP[k][i];
				int d = 0;
				if(Cache_node.count(id_str)&&Cache_node[id_str].count(pnode)){d = Cache_node[id_str][pnode];}
				else{
						d = distanceQuery( a + 1, pnode + 1) + distanceQuery( pnode + 1, b + 1) - pathDist[j];
						Cache_node[id_str][pnode] = d;
					}
						if(d < Min_val){
							Min_val = d;
							node = pnode;
							// farnode = node;
						}
				
			}
			if(Min_val < tmap[k]){
				tmap[k] = Min_val;
				farnode = node;
				Cache[id_str][k] = Min_val;
				farnode = node;	
			}
			cout<<"       k = "<<k<<"	Min_val = "<<Min_val<<"    node = "<<node<<endl;
		}
	}

	for(auto &[k,v] : tmap){
		if(v >= Max_val){
			Max_val = v;
			kp = k;
		}
	}
	cout<<"\t -----------count = "<<count<<endl;
	 cout<<"Max_val = "<<Max_val<<" K="<<kp<<" farnode = "<<farnode<<endl;
	return Max_val;
}
*/