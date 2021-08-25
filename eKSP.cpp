#include "graph.h"

int Graph::eKSPNew(int ID1, int ID2, int k, vector<int>& query, vector<int>& kResults, vector<vector<int> >& vkPath) 
{
	// cout<<"start eKSPNew processing...................."<<endl;
	// Initialize global var
	QueryWord.clear();
	QueryBit.reset();
	Qu.reset();

	//set QueryBit and QueryWord
	// cout<<"keywords : ";
	for(int k:query){
		QueryBit.set(k);
		Qu.set(k);
		if(!NodesBit[ID1].test(k)&&!NodesBit[ID2].test(k)){
			QueryWord.push_back(k);
		}
		// cout<<k<<" ";
	}
	// cout<<"\t before QueryNums = "<<QueryBit.count()<<endl;
	// cout<<"keywords : ";
	QueryBit ^= (QueryBit & ( NodesBit[ID1] | NodesBit[ID2] ));//delete keywwords from ID1 and ID2
	// for(auto &k:QueryWord){cout<<k<<" ";}
	// cout<<"\t after QueryNums = "<<QueryBit.count()<<endl;
	// cout<<QueryBit.to_string()<<endl;

	//Shortest Path Tree Info
	vector<int> vSPTDistance(nodeNum, INF); //record dist from node to root ID1 
	vector<int> vSPTParent(nodeNum, -1);//record fatherid of current node 
	vector<int> vSPTParentEdge(nodeNum, -1); 
	vector<int> vTmp;
	vector<vector<int> > vSPT(nodeNum, vTmp); //Tree from root  ID1
	SPT(ID1, vSPTDistance, vSPTParent, vSPTParentEdge, vSPT);

	// cout<<"Shortest path cost = "<<vSPTDistance[ID2]<<endl;

	vector<int> vEdgeReducedLength(vEdge.size(), INF); //record sidecost of curNode 
	for(int i = 0; i < (int)vEdge.size(); i++)
		vEdgeReducedLength[i] = vEdge[i].length + vSPTDistance[vEdge[i].ID1] - vSPTDistance[vEdge[i].ID2];  
	
	vector<vector<int> > vvResult;	//Exact Path
	vvResult.reserve(k);
   	vector<int> vDistance;
	vector<int> vPathParent;	//Deviated from
	vector<int> vPathParentPos;	//Deviated Pos from Parent
    vector<vector<int> > vvPathCandidate;	 //nodes
	vector<vector<int> > vvPathCandidateEdge;//edges 
	vector<unordered_map<int, int> > vmPathNodePos;	//Position of the non-fixed vertices

	//The size of mvPathNodePos indicates the fixed pos
	vector<vector<pair<int, int> > > vvPathNonTree; 
	vector<multimap<int, int> > vmArc;
	vector<int> vFather; 
	vFather.push_back(-1);
	
	vector<int> vPath; 
	vector<int> vPathEdge;
	vPath.push_back(ID2); 
	vector<pair<int, int> > vPathNonTree; 

	int oldP = ID2;
	int p = vSPTParent[ID2]; 
	int e = vSPTParentEdge[ID2];
   	 multimap<int, int> mArc; 
	while(p != -1)
	{
		vPath.push_back(p); 
		for(int i = 0; i < (int)adjListEdgeR[oldP].size(); i++) 
		{
			int eID = adjListEdgeR[oldP][i].second;  
			if(eID != e)  
                mArc.insert(make_pair(vEdgeReducedLength[eID], eID)); 
		//	else
		//		cout << "Skip " << e << endl;
		}
		oldP = p;
		vPathEdge.push_back(e);
		e = vSPTParentEdge[p];
		p = vSPTParent[p];
	}
	vmArc.push_back(mArc);

	reverse(vPath.begin(), vPath.end()); 
	reverse(vPathEdge.begin(), vPathEdge.end()); 

	unordered_map<int, int> mPos;
	for(int i = 0; i < (int)vPath.size(); i++)
		mPos[vPath[i]] = i;
	vmPathNodePos.push_back(mPos);

	benchmark::heap<2, int, int> qPath(nodeNum*100);  //control size 
	vvPathCandidate.push_back(vPath); 
	vvPathCandidateEdge.push_back(vPathEdge);
	vDistance.push_back(vSPTDistance[ID2]);  
	vPathParent.push_back(-1);
	vPathParentPos.push_back(0);
	qPath.update(vvPathCandidate.size()-1, vSPTDistance[ID2]); 
	
	vector<int> vResultID;
	int topPathID, topPathDistance; 
	int pcount = 0;
	int oldDistance = -1;
	while((int)kResults.size() < k)
	{
		pcount++;
		qPath.extract_min(topPathID, topPathDistance); 
		// cout<<pcount<<endl;
		bitset<KIND>_bit(Qu);
		vector<int>tmpPath(vvPathCandidate[topPathID]);
		vector<int>poi;
		poi.clear();
		for(auto& id : tmpPath){
			for(auto& [k,v] : KEYS[id]){
				if(_bit[k] == 1 && v == 1){_bit.reset(k);poi.push_back(id);}
			}
		}
		if(_bit.none()){
			// cout<<"topPDist = "<<topPathDistance<<"  topPathID = "<<topPathID<<endl;
			// for(auto p:poi)cout<<p<<" ";cout<<endl;
			// for(auto p:tmpPath)cout<<p<<" ";cout<<endl;
			return topPathDistance;
		}
		
		if(topPathDistance < oldDistance)
			cout<< "Error" <<endl;

		oldDistance = topPathDistance;
	   // cout << endl << pcount << "\t" << topPathDistance  << "\t" << kResults.size() << endl; 
        
		//Loop Test
		unordered_set<int> us; 
		bool bTopLoop = false;
		for(auto& v : vvPathCandidate[topPathID]) 
		{
			if(us.find(v) == us.end())
				us.insert(v);	  
			else
			{
				bTopLoop = true; 
			//	cout << "Top Loop!" << endl;
				break;
			}
		}

        if(!bTopLoop)
		{
			kResults.push_back(topPathDistance);  
			vkPath.push_back(vvPathCandidate[topPathID]);
			vResultID.push_back(topPathID);
			// cout << kResults.size()  << endl;
		}

		vector<int> vTwo;
		vTwo.push_back(topPathID);
        if(vFather[topPathID] != -1 &&  !vmArc[vFather[topPathID]].empty())  
			vTwo.push_back(vFather[topPathID]);
		// cout<<"zzzz"<<endl;
		int count = 0;
		for(auto& pID : vTwo)
		{
			count++;
			// cout<<count<<endl;
			bool bLoop = true;  
			while(bLoop) 
			{
				//No More Candidate from current class
				if(vmArc[pID].empty())   
				{
					vvPathCandidate[pID].clear();
					vvPathCandidateEdge[pID].clear();
					vmPathNodePos[pID].clear();
					break;
				}

				int mineID;
				int eReducedLen; 
        		        auto it = vmArc[pID].begin();
				eReducedLen = (*it).first;
				mineID = (*it).second;
				vmArc[pID].erase(it);
				
				int eNodeID1 = vEdge[mineID].ID1;
				int eNodeID2 = vEdge[mineID].ID2;
				
				//Loop in the fixed part, prune
				bool bFixedLoop = false; 
				unordered_set<int> sE;
				for(int i = vmPathNodePos[pID][eNodeID2]; i < (int)vvPathCandidate[pID].size(); i++)  
				{
					if(sE.find(vvPathCandidate[pID][i]) == sE.end())  
						sE.insert(vvPathCandidate[pID][i]); 
					else
					{
						bFixedLoop = true;
						break;
					}
				}

				if(bFixedLoop)
					continue;

				int distTmp = vDistance[pID] - vSPTDistance[eNodeID2] + vEdge[mineID].length;     
				bLoop = false;
				

//				cout << "eNodeID1:" << eNodeID1 << "\t" << "eNodeID2:" << eNodeID2 << endl; 
//				cout << "Previous:" << vvPathCandidate[pID][vmPathNodePos[pID][eNodeID2]-1] << endl;

				//Traverse the non-fixed part: the tree
               			multimap<int, int> mArcTmp; 
				vPath.clear();
				vPathEdge.clear();
				int p = eNodeID1;
				int e = vSPTParentEdge[p];
				while(p != -1)
				{
                   			vPath.push_back(p);
					for(int i = 0; i < (int)adjListEdgeR[p].size(); i++) 
					{
						int reID = adjListEdgeR[p][i].second;
						int eID1 = vEdge[reID].ID1;
						int dTmp = distTmp + vEdge[reID].length;

						if(sE.find(eID1) != sE.end())   
							continue;

						if(reID != e && reID != mineID)
							mArcTmp.insert(make_pair(vEdgeReducedLength[reID], reID)); 
					} 

				    	vPathEdge.push_back(e); 
					distTmp += vEdge[e].length;
						
					p = vSPTParent[p];
					if(vSPTParent[p] == -1) 
					{
						vPath.push_back(p);
						break;
					}
					e = vSPTParentEdge[p];            
				}

				int dist = vDistance[pID] - vSPTDistance[eNodeID2] + vSPTDistance[eNodeID1] + vEdge[mineID].length;    
//				cout << "New Dist:" << dist << endl << endl;	 
				vDistance.push_back(dist); 
				vFather.push_back(pID); 
				qPath.update(vDistance.size()-1, dist); 
				// cout<<"push"<<endl;

				reverse(vPath.begin(), vPath.end());
				reverse(vPathEdge.begin(), vPathEdge.end()); 
				unordered_map<int, int> mE; 
				int i;
				//Pos stop at eNodeID1 as it is boundary of fixed
				for(i = 0; i < (int)vPath.size(); i++)
					mE[vPath[i]] = i; 
				vmPathNodePos.push_back(mE); 
				
				vPath.push_back(eNodeID2);
				vPathEdge.push_back(mineID);
					
				for(int j = vmPathNodePos[pID][eNodeID2]; j+1 < (int)vvPathCandidate[pID].size(); j++)
				{
					int nodeID = vvPathCandidate[pID][j+1]; 
					vPath.push_back(nodeID);
					int edgeID = vvPathCandidateEdge[pID][j];
					vPathEdge.push_back(edgeID);
				}

				vmArc.push_back(mArcTmp); 
				vvPathCandidate.push_back(vPath);
				vvPathCandidateEdge.push_back(vPathEdge);
			}
		}
	} 

	return -1;
}

void Graph::SPT(int root, vector<int>& vSPTDistance, vector<int>& vSPTParent, vector<int>& vSPTParentEdge, vector<vector<int> >& vSPT)   
{
	benchmark::heap<2, int, int> queue(nodeNum); 
	queue.update(root, 0);

	vector<bool> vbVisited(nodeNum, false);
	int topNodeID, neighborNodeID, neighborLength, neighborRoadID; 
	vector<pair<int, int> >::iterator ivp;

	vSPTDistance[root] = 0;
	compareNode cnTop;
	while(!queue.empty())
	{
		int topDistance; 
		queue.extract_min(topNodeID, topDistance); 
		vbVisited[topNodeID] = true; 
		for(int i = 0; i < (int)adjList[topNodeID].size(); i++)
		{
			neighborNodeID = adjList[topNodeID][i].first;
			neighborLength = adjList[topNodeID][i].second;
			neighborRoadID = adjListEdge[topNodeID][i].second;
			int d = vSPTDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vSPTDistance[neighborNodeID] > d)
				{
					vSPTDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d); 
					vSPTParent[neighborNodeID] = topNodeID;
					vSPTParentEdge[neighborNodeID] = neighborRoadID; 
				}
			}
		}
	}
	// cout<<vSPTDistance[1]<<endl;
	// cout<<vSPTDistance[2]<<endl;
	
	//Construct SPT //father nodeid stores some children nodeid. tree root is ID1
	for(int i = 0; i < nodeNum; i++)
		if(vSPTParent[i] != -1)
			vSPT[vSPTParent[i]].push_back(i);

}
