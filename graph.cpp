#include "graph.h"

int Graph::readBeijingMapDirected(string filename)
{
	ifstream inGraph(filename.c_str());
	if(!inGraph)
	{
		cout << "Cannot open Beijing Map " << filename << endl;
		return -1;
	}

	int nodeNum, i;
	inGraph >> nodeNum >> minX >> maxX >> minY >> maxY;
	cout << "Beijing Node number: " << nodeNum << endl;
	this->nodeNum = nodeNum;

	vector<Edge> vEdges; 
	vCoor.reserve(nodeNum);
	

	double x, y;
	int nodeID, type, j, k;
	int ID2, length;
	for(i = 0; i < nodeNum; i++)
	{
		inGraph >> nodeID >> type >> x >> y >> j >> j; 
		vCoor.push_back(make_pair(x, y));

		for(k = 0; k < j; k++)
		{
			inGraph >> ID2 >> length; 
			struct Edge e, eR;
			e.ID1 = nodeID;
			e.ID2 = ID2;
			e.length = length;
			vEdges.push_back(e);

			eR.ID1 = ID2;
			eR.ID2 = nodeID;
			eR.length = length;
			vEdgeR.push_back(eR);
		}

		inGraph >> j;
		for(k = 0; k < j; k++)
			inGraph >> ID2 >> length;

		inGraph >> j;
		for(k = 0; k < j; k++)
			inGraph >> ID2 >> length;
	}

	cout << "Finish Reading " << filename << endl;
	inGraph.close();

	adjList.resize(nodeNum);
	adjListR.resize(nodeNum);
	adjListEdge.resize(nodeNum);
	adjListEdgeR.resize(nodeNum);
	int edgeCount = 0;
	for(auto &edge: vEdges)
	{
		int ID1 = edge.ID1;
		int ID2 = edge.ID2;
		int length = edge.length;

		bool b = false;
		for(auto ivp = adjList[ID1].begin(); ivp != adjList[ID1].end(); ivp++)
		{
			if((*ivp).first == ID2) 
			{
				b = true;
				break;
			}
		}

		if(!b) 
		{
			adjList[ID1].push_back(make_pair(ID2, length)); 
			adjListR[ID2].push_back(make_pair(ID1, length));

			edge.edgeID = edgeCount; 
			cout << ID2 << "\t" << ID1 << "\t" << edgeCount << endl;
			adjListEdge[ID1].push_back(make_pair(ID2, edgeCount)); 
			adjListEdgeR[ID2].push_back(make_pair(ID1, edgeCount));
			vEdge.push_back(edge);
			edgeCount++;
		}
	}

	cout << "Beijing Road number: " << edgeCount << endl;

	return nodeNum;
}

int Graph::readUSMap(string filename)
{ 
	ifstream inGraph(filename);
	if(!inGraph)
		cout << "Cannot open Map " << filename << endl; 
	cout << "Reading " << filename << endl;

	string line;
	do
	{
		getline(inGraph,line);
		if(line[0]=='p')
		{ 
			vector<string> vs;
			boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
			nodeNum = stoi(vs[2]); //edgenum=stoi(vs[3]);
			cout << "Nodenum " << nodeNum<<endl;
			edgeNum = 0;
		}
	}while(line[0]=='c'|| line[0]=='p');

	int ID1, ID2, length;
	adjList.resize(nodeNum);
	adjListR.resize(nodeNum);
	adjListEdge.resize(nodeNum);
	adjListEdgeR.resize(nodeNum); 
	int edgeCount = 0; 
	string a;
	while(!inGraph.eof())
	{
/*		vector<string> vs;
		boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
		ID1 = stoi(vs[1]) - 1; 
		ID2 = stoi(vs[2]) - 1; 
		length = stoi(vs[3]);
*/		
		inGraph >> a >> ID1 >> ID2 >> length;
		ID1 -= 1;
		ID2 -= 1;
		
		struct Edge e; 
		e.ID1 = ID1;
		e.ID2 = ID2;
		e.length = length;
		e.edgeID = edgeCount; 
		
		bool bExisit = false;
		for(int i = 0; i < (int)adjList[ID1].size(); i++) 
		{
			if(adjList[ID1][i].first == ID2)
			{
				bExisit = true;
				break;
			}
		}

		//cout << ID1 << "\t" << ID2 << "\t" << length << endl;
		if(!bExisit)
		{
			vEdge.push_back(e);
			adjList[ID1].push_back(make_pair(ID2, length));
			adjListR[ID2].push_back(make_pair(ID1, length));
			adjListEdge[ID1].push_back(make_pair(ID2, edgeCount)); 
			adjListEdgeR[ID2].push_back(make_pair(ID1, edgeCount));
			edgeCount++;
		}
		//getline(inGraph,line);
	}

	vbISO.assign(nodeNum, false); 
	inGraph.close();
	
	string coorFile = filename.substr(0, filename.size()-2)+"co"; 
	ifstream inCoor(coorFile.c_str());
	if(!inCoor)
		cout << "Cannot open Coordinate " << coorFile << endl; 
	cout << "Reading " << coorFile << endl;

	do
	{
		getline(inCoor,line);
	}while(line[0]=='c'|| line[0]=='p');

	double lon, lat;   
	// vCoor.resize(nodeNum);
	vCoor.push_back(make_pair(0,0));
	while(!inCoor.eof())
	{
		vector<string> vs;
		boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
		ID1 = stod(vs[1]) - 1; 
		lon = stod(vs[2].substr(0,3) + "." + vs[2].substr(3));  
		lat = stod(vs[3].substr(0,2) + "." + vs[3].substr(2));  
		vCoor.push_back(make_pair(lon, lat));
		getline(inCoor,line);
	}

//	mCoor["NY"] = make_pair(84000, 69000);
	cout << "--------Graph loading finished-----!!!" << endl;
	return nodeNum;
}

int Graph::readUSMapCost(string filename)
{ 
	ifstream inGraph(filename);
	if(!inGraph)
		cout << "Cannot open Map " << filename << endl; 
	cout << "Reading " << filename << endl;

	string line; 
//	int eNum;
	inGraph >> nodeNum;  
//	inGraph >> nodeNum >> eNum; 

	int ID1, ID2, length, cost;
	adjList.resize(nodeNum);
	adjListR.resize(nodeNum);
	adjListEdge.resize(nodeNum);
	adjListEdgeR.resize(nodeNum); 
	adjListCost.resize(nodeNum);
	adjListCostR.resize(nodeNum);
	int edgeCount = 0;
//	cost = 0;
	while(!inGraph.eof())
	{
		inGraph >> ID1 >> ID2 >> length >> cost;
//		inGraph >> ID1 >> ID2 >> length; 
		ID1 -= 1; 
		ID2 -= 1;  

		struct Edge e; 
		e.ID1 = ID1;
		e.ID2 = ID2;
		e.length = length;
		e.edgeID = edgeCount;  
		e.cost = cost;

		bool bExisit = false;
		for(int i = 0; i < (int)adjListCost[ID1].size(); i++) 
		{
			if(adjListCost[ID1][i].first == ID2)
			{
				bExisit = true;
				if(cost < adjListCost[ID1][i].second)
				{
//					cout << "Cost update:" << adjListCost[ID1][i].second << "\t" << cost << endl; 
					adjListCost[ID1][i].second = cost; 
					for(int j = 0; j < (int)adjListCostR[ID2].size(); j++)
					{
						if(adjListCostR[ID2][j].first == ID1)
						{
							adjListCostR[ID2][j].second = cost;    
							break;
						}
					}

					int eID = adjListEdge[ID1][i].second;
					vEdge[eID].cost = cost;
				}
				break;
			}
		}

		if(!bExisit)
		{
			vEdge.push_back(e);
			adjList[ID1].push_back(make_pair(ID2, length));
			adjListR[ID2].push_back(make_pair(ID1, length));
			adjListEdge[ID1].push_back(make_pair(ID2, edgeCount)); 
			adjListEdgeR[ID2].push_back(make_pair(ID1, edgeCount));
			adjListCost[ID1].push_back(make_pair(ID2, cost));
			adjListCostR[ID2].push_back(make_pair(ID1, cost)); 
			edgeCount++;
		}
	//	getline(inGraph,line);
	}

/*	for(int i = 0; i < (int)adjListCost.size(); i++)
	{
		unordered_map<int, int> us;
		for(auto& c: adjListCost[i])
		{
			if(us.find(c.first) != us.end())
			{
				cout << "Repeated! " << i << "\t" << c.first << "\t" << us[c.first] << "\t" << c.second << endl;
			}
			else
				us[c.first] = c.second;
		}
	}
*/
	vbISO.assign(nodeNum, false); 
	inGraph.close();
/*	
	vector<int> vCorrect;
	ifstream ic("./c"); 
	int icc;
	while(ic >> icc)
		vCorrect.push_back(icc); 
	int c= 0;
	int d = 0;
	int oldv = -1; 
	for(auto&v : vCorrect)
	{
		cout << v << "\t";
		bool b = false;
		if(oldv > -1)
		{
			for(int j= 0 ; j < (int)adjList[oldv].size(); j++)
			{
				if(adjList[oldv][j].first == v)
				{
					d += adjList[oldv][j].second;
					c += adjListCost[oldv][j].second;
					b = true;
					break;
				}
			}
			if(!b)
			{
				cout << endl << "Disconnected!" << endl;
			}
		}
		oldv = v;
		cout << endl << d << "\t" << c << "\t" << endl;
	}
	*/

	return nodeNum;
}

int Graph::readExampleMap(string filename)
{ 
	ifstream inGraph(filename);
	if(!inGraph)
		cout << "Cannot open Map " << filename << endl; 
	cout << "Reading " << filename << endl;

	string line;
	do
	{
		getline(inGraph,line);
		if(line[0]=='p')
		{ 
			vector<string> vs;
			boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
			nodeNum = stoi(vs[2]); //edgenum=stoi(vs[3]);
			cout << "Nodenum " << nodeNum<<endl;
			edgeNum = 0;
		}
	}while(line[0]=='c'|| line[0]=='p');

	int ID1, ID2, length;
	adjList.resize(nodeNum);
	adjListR.resize(nodeNum);
	adjListEdge.resize(nodeNum);
	adjListEdgeR.resize(nodeNum); 
	int edgeCount = 0;
	while(!inGraph.eof())
	{
		vector<string> vs;
		boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
		ID1 = stoi(vs[1]); 
		ID2 = stoi(vs[2]); 
		length = stoi(vs[3]);
		
		struct Edge e; 
		e.ID1 = ID1;
		e.ID2 = ID2;
		e.length = length;
		e.edgeID = edgeCount; 
		vEdge.push_back(e);

		cout << ID1 << "\t" << ID2 << "\t" << length << endl;
		adjList[ID1].push_back(make_pair(ID2, length));
		adjListR[ID2].push_back(make_pair(ID1, length));
		adjListEdge[ID1].push_back(make_pair(ID2, edgeCount)); 
		adjListEdgeR[ID2].push_back(make_pair(ID1, edgeCount));
		edgeCount++;
		getline(inGraph,line);
	}

	vbISO.assign(nodeNum, false); 
	inGraph.close();

//	mCoor["NY"] = make_pair(84000, 69000);

	return nodeNum;
}
	

void Graph::set_nodeKEYS_NodesBit(string filename){
	//set RSP
	for(int i = 0;i < KIND; i++){
		vector< int >tmp;
		RSP.push_back(tmp);
	}

	//set nodeKEYS
	
	fstream fp(filename);
        int nid;
        string ss;
        long x,y;
        bitset<KIND>t;
        NodesBit.push_back(t);
        unordered_map<int,int>ma;
        KEYS.push_back(ma);
        while(fp >> nid >> x >> y >> ss){
        ///	cout<<nid<<" "<<ss<<endl;
                char str[100]; //according to the keywordstr.length (20*4)
                strcpy(str,ss.c_str());
                const char * split = ",";
                char * p;
                p = strtok (str,split);
                unordered_map<int,int>mp;
                mp.clear();
                bitset<KIND> bit;
                bit.reset();
                while(p!=NULL) {
                                if(strcmp(p,"-1") ==0)
                                        break;
                                mp.insert(make_pair(atoi(p),1));
                                RSP[atoi(p)].push_back(nid);
                                bit.set(atoi(p));
                                p = strtok(NULL,split);
                }
                KEYS.push_back(mp);
                NodesBit.push_back(bit);

        }
		cout<<"--------KEYS map and Query init completed-----!!!"<<endl;
        fp.close();
}

int Graph::Clen(int S, int T, bitset<KIND>& query, vector<int> &result){
	// cout<<"-------Clen----------"<<endl;
	set< int > minCandtmp;
	vector< int >minCand;
	for(int i = 0;i < QueryWord.size();i++){
		int MIN_LEN = INT_MAX;
	        int PRE = 40;//the number of node nearest S and T for one keyword
		int min_pos;
		// if(QueryBit.test(QueryWord[i])){
		// 	cout<<QueryWord[i]<<endl;
			int pos = QueryWord[i];
			vector<pair<int,double>>pri_node;
			pri_node.clear();
			for(int k = 0;k < RSP[pos].size();k++){
				int _len = distanceQuery(S+1, RSP[pos][k]+1) + distanceQuery(RSP[pos][k]+1, T+1);
				pri_node.push_back(make_pair(RSP[pos][k], _len));
			}
			sort(pri_node.begin(),pri_node.end(),[](const pair<int, double>& p,const pair<int, double>& q){return p.second <= q.second;});
			if(PRE > RSP[pos].size()){PRE = RSP[pos].size();}
			for(int j = 0;j < PRE ;j++){
				// int len = Dijkstra(S, pri_node[j].first) + Dijkstra(pri_node[j].first, T);
				int len = AStar(S, pri_node[j].first) + AStar(pri_node[j].first, T);
				// int len = distanceQuery(S+1, pri_node[j].first+1) + distanceQuery(pri_node[j].first+1, T+1);
				if(MIN_LEN > len){
					MIN_LEN = len;
					min_pos = pri_node[j].first;
				}
			}	
			minCandtmp.insert(min_pos);
	}
//	cout<<minCandtmp.size()<<endl;
	for(set<int>::iterator it = minCandtmp.begin(); it!=minCandtmp.end(); it++){
//		cout<<*it<<endl;
		minCand.push_back(*it);
	}
//	cout<<endl;
//	cout<<"cand.size() = "<<minCand.size()<<endl;
	for(vector<int>::iterator it = minCand.begin(); it!=minCand.end(); it++){
//		cout<<*it<<endl;
	}
//	cout<<endl;
	int dist = 0;
	int index;
	int Start = S;
	result.push_back(S);
//	cout<<Start<<" -> ";
	while(minCand.size() > 0){
		int min = INT_MAX;
		for(int i=0;i<minCand.size();i++){
			// cout<<"mincand["<<i<<"]="<<minCand[i]<<endl;
			// int len = Dijkstra(Start, minCand[i]);
			int len = AStar(Start, minCand[i]);
			// int len = distanceQuery(Start + 1, minCand[i] + 1);
			if(min > len){
				min = len;
				index = minCand[i];
			}
		}
		result.push_back(index);
		dist += min;
//		cout<<min<<endl;
//		cout<<index<<"("<<minCand.size()<<") -> ";
		for(vector<int>::iterator it = minCand.begin(); it!=minCand.end();){
			if(*it == index){
				it = minCand.erase(it);
				break;
			}else{it++;}
		}
		Start = index; 
	}
//	cout<<T<<endl;;
	dist += AStar(Start, T);
	// dist += Dijkstra(Start, T);
	// dist += distanceQuery(Start + 1, T + 1);
	result.push_back(T);
	return dist;
}

int Graph::readUSCost(string filename)
{
	ifstream inGraph(filename);
	if(!inGraph)
		cout << "Cannot open Map " << filename << endl; 
	cout << "Reading " << filename << endl;

	string line;
	do
	{
		getline(inGraph,line);
	}while(line[0]=='c'||line[0]=='p');

	int ID1, ID2, cost; 
	adjListCost.resize(nodeNum);
	adjListCostR.resize(nodeNum);
	int edgeCount = 0;
	while(!inGraph.eof())
	{
		vector<string> vs;
		boost::split(vs,line,boost::is_any_of(" "),boost::token_compress_on);
		ID1 = stoi(vs[1]) - 1;  
		ID2 = stoi(vs[2]) - 1; 
		cost = stoi(vs[3]);
		
		adjListCost[ID1].push_back(make_pair(ID2, cost));
		adjListCostR[ID2].push_back(make_pair(ID1, cost)); 
		vEdge[edgeCount].cost = cost;
		edgeCount++;
		getline(inGraph,line);
	}
	
	cout << vEdge.size() << endl;
	return nodeNum;
}

int Graph::DijkstraPath(int ID1, int ID2, vector<int>& vPath, vector<int>& vPathEdge)
{
	benchmark::heap<2, int, int> queue(adjList.size());
	queue.update(ID1, 0);

	vector<int> vDistance(adjList.size(), INF);
	vector<int> vPrevious(adjList.size(), -1);
	vector<int> vPreviousEdge(adjList.size(), -1);
	vector<bool> vbVisited(adjList.size(), false);
	int topNodeID, neighborNodeID, neighborLength, neighborRoadID;

	vDistance[ID1] = 0;

	while(!queue.empty())
	{
		int topDistance; 
		queue.extract_min(topNodeID, topDistance); 
		vbVisited[topNodeID] = true;
		if(topNodeID == ID2)
			break;

		for(int i = 0; i < (int)adjList[topNodeID].size(); i++)
		{
			neighborNodeID = adjList[topNodeID][i].first;
			neighborLength = adjList[topNodeID][i].second; 
			neighborRoadID = adjListEdge[topNodeID][i].second;
			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d);
					vPrevious[neighborNodeID] = topNodeID;
					vPreviousEdge[neighborNodeID] = neighborRoadID;
				}
			}
		}
	}

	vPath.clear();
	vPathEdge.clear();
	vPath.push_back(ID2);
	int p = vPrevious[ID2];
	int e = vPreviousEdge[ID2];
	while(p != -1)
	{
		vPath.push_back(p);
		vPathEdge.push_back(e);
		e = vPreviousEdge[p];
		p = vPrevious[p];
	}

//	if(vPathEdge.size() > 0)
//		vPathEdge.erase(vPathEdge.end()-1);

	reverse(vPath.begin(), vPath.end());
	reverse(vPathEdge.begin(), vPathEdge.end());

	return vDistance[ID2];
}

int Graph::DijkstraPath2(int ID1, int ID2, unordered_set<int>& sRemovedNode, vector<int>& vPath, vector<int>& vPathEdge)
{
	benchmark::heap<2, int, int> queue(adjList.size());
	queue.update(ID1, 0);

	vector<int> vDistance(adjList.size(), INF);
	vector<int> vPrevious(adjList.size(), -1);
	vector<int> vPreviousEdge(adjList.size(), -1);
	vector<bool> vbVisited(adjList.size(), false);
	int topNodeID, neighborNodeID, neighborLength, neighborRoadID;

	vDistance[ID1] = 0;

	while(!queue.empty())
	{
		int topDistance; 
		queue.extract_min(topNodeID, topDistance); 
		vbVisited[topNodeID] = true;
		if(topNodeID == ID2)
			break;

		for(int i = 0; i < (int)adjList[topNodeID].size(); i++)
		{
			neighborNodeID = adjList[topNodeID][i].first; 
			if(sRemovedNode.find(neighborNodeID) != sRemovedNode.end())
				continue;
			neighborLength = adjList[topNodeID][i].second; 
			neighborRoadID = adjListEdge[topNodeID][i].second;
			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d);
					vPrevious[neighborNodeID] = topNodeID;
					vPreviousEdge[neighborNodeID] = neighborRoadID;
				}
			}
		}
	}

	vPath.clear();
	vPathEdge.clear();
	vPath.push_back(ID2);
	int p = vPrevious[ID2];
	int e = vPreviousEdge[ID2];
	while(p != -1)
	{
		vPath.push_back(p);
		vPathEdge.push_back(e);
		e = vPreviousEdge[p];
		p = vPrevious[p];
	}

//	if(vPathEdge.size() > 0)
//		vPathEdge.erase(vPathEdge.end()-1);

	reverse(vPath.begin(), vPath.end());
	reverse(vPathEdge.begin(), vPathEdge.end());

	return vDistance[ID2];
}

int Graph::Dijkstra(int ID1, int ID2)
{
	benchmark::heap<2, int, int> queue(adjList.size());
	queue.update(ID1, 0);

	vector<int> vDistance(nodeNum, INF);
	vector<bool> vbVisited(nodeNum, false);
	int topNodeID, neighborNodeID, neighborLength;
	vector<pair<int, int> >::iterator ivp;

	vDistance[ID1] = 0;

	compareNode cnTop;
	while(!queue.empty())
	{
		int topDistance; 
		queue.extract_min(topNodeID, topDistance); 
		vbVisited[topNodeID] = true; 
//		cout << topNodeID << "\t" << vDistance[topNodeID] << endl;
		if(topNodeID == ID2)
			break;

		for(ivp = adjList[topNodeID].begin(); ivp != adjList[topNodeID].end(); ivp++)
		{
			neighborNodeID = (*ivp).first;
			neighborLength = (*ivp).second; 
			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d);
				}
			}
		}
	}

	return vDistance[ID2];
}

int Graph::AStar(int ID1, int ID2)
{
	benchmark::heap<2, int, int> queue(adjList.size());
	vector<int> vDistance(adjList.size(), INF);
	vector<bool> vbVisited(adjList.size(), false);
	int topNodeID, neighborNodeID, neighborLength;
	vector<pair<int, int> >::iterator ivp;

	queue.update(ID1, EuclideanDistance(ID1, ID2)); 
	vDistance[ID1] = 0;

	compareNode cnTop;
	while(!queue.empty())
	{
		int topDistance;
		queue.extract_min(topNodeID, topDistance);
		vbVisited[topNodeID] = true;

		if(topNodeID == ID2)
			break;

		for(ivp = adjList[topNodeID].begin(); ivp != adjList[topNodeID].end(); ivp++)

		{
			neighborNodeID = (*ivp).first;
			neighborLength = (*ivp).second; 
			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d+EuclideanDistance(neighborNodeID, ID2));
				}
			}
		}
	}

	return vDistance[ID2];
}

int Graph::AStarPath(int ID1, int ID2, vector<int>& vPath, vector<int>& vPathEdge, string& city)
{
	benchmark::heap<2, int, int> queue(adjList.size());
	vector<int> vDistance(adjList.size(), INF);
	vector<int> vPrevious(adjList.size(), -1);
	vector<int> vPreviousEdge(adjList.size(), -1);
	vector<bool> vbVisited(adjList.size(), false);
	int topNodeID, neighborNodeID, neighborLength, neighborRoadID;

	int latU, lonU;
	if(city == "US")
	{
		lonU = 84000;
		latU = 69000;
	}
	else
	{
		lonU = 83907;
		latU = 111319; 
	}

	queue.update(ID1, EuclideanDistance(ID1, ID2)); 
	vDistance[ID1] = 0;

	compareNode cnTop;
	while(!queue.empty())
	{
		int topDistance;
		queue.extract_min(topNodeID, topDistance);
		vbVisited[topNodeID] = true;

		if(topNodeID == ID2)
			break;

		for(int i = 0; i < (int)adjList[topNodeID].size(); i++)
		{
			neighborNodeID = adjList[topNodeID][i].first;
			neighborLength = adjList[topNodeID][i].second; 
			neighborRoadID = adjListEdge[topNodeID][i].second;
			int d = vDistance[topNodeID] + neighborLength;
			if(!vbVisited[neighborNodeID])
			{
				if(vDistance[neighborNodeID] > d)
				{
					vDistance[neighborNodeID] = d;
					queue.update(neighborNodeID, d+EuclideanDistance(neighborNodeID, ID2));
					vPrevious[neighborNodeID] = topNodeID;
					vPreviousEdge[neighborNodeID] = neighborRoadID;
				}
			}
		}
	}
	
	vPath.clear();
	vPathEdge.clear();
	vPath.push_back(ID2);
	int p = vPrevious[ID2];
	int e = vPreviousEdge[ID2];
	while(p != -1)
	{
		vPath.push_back(p);
		vPathEdge.push_back(e);
		e = vPreviousEdge[p];
		p = vPrevious[p];
	}

	reverse(vPath.begin(), vPath.end());
	reverse(vPathEdge.begin(), vPathEdge.end());

	return vDistance[ID2];
}

inline int Graph::EuclideanDistance(int ID1, int ID2)
{
	int lat=(int)(abs(vCoor[ID1].first - vCoor[ID2].first)*111319);
	int lon=(int)(abs(vCoor[ID1].second - vCoor[ID2].second)*83907);
	int min, max;
	min = (lat > lon) ? lon : lat;
	max = (lat > lon) ? lat : lon;
	int approx = max*1007 + min*441;
	if(max < (min << 4))
		approx -= max * 40;
	return (approx + 512) >> 10;
}

inline int Graph::EuclideanDistanceAdaptive(int ID1, int ID2, int latU, int lonU)
{
	int lat=(int)(abs(vCoor[ID1].first - vCoor[ID2].first)*latU);
	int lon=(int)(abs(vCoor[ID1].second - vCoor[ID2].second)*lonU);
	int min, max;
	min = (lat > lon) ? lon : lat;
	max = (lat > lon) ? lat : lon;
	int approx = max*1007 + min*441;
	if(max < (min << 4))
		approx -= max * 40;
	return (approx + 512) >> 10;
}

int Graph::ISONodes()
{
	srand (time(NULL));
	vbISOF.assign((int)adjList.size(), false);	//forward
	vbISOB.assign((int)adjList.size(), false);	//backward
	vbISOU.assign( (int)adjList.size(), false);	//F & B
	vbISO.assign((int)adjList.size(), false);	//F | B

	ifstream ifISOF("./beijingISOF");
	ifstream ifISOB("./beijingISOB");
	ifstream ifISOU("./beijingISOU");
	ifstream ifISO("./beijingISO");

	if(!ifISOF || !ifISOB || !ifISOU || !ifISO)
	{
		//ISO files do not exist
		//Create new ones
		srand(time(NULL));
		cout << "Identifying ISO Nodes" << endl;
		for(int i = 0; i < 10; i++)
		{
			int nodeID = rand() % adjList.size();
			cout <<nodeID <<endl;
			vector<bool> vbVisitedF;
			vector<bool> vbVisitedB;
			int vnF = BFS(nodeID, true, vbVisitedF);
			int vnB = BFS(nodeID, false, vbVisitedB);
			cout <<vnF <<"\t" << vnB <<endl;

			if(vnF < 1000 || vnB < 1000)
				continue;

			for(int j = 0; j < (int)adjList.size(); j++)
			{
				if(!vbVisitedF[j])
					vbISOF[j] = true;

				if(!vbVisitedB[j])
					vbISOB[j] = true;

				if(!vbVisitedF[j] || !vbVisitedB[j])
					vbISOU[j] = true;

				if(!vbVisitedF[j] && !vbVisitedB[j])
					vbISO[j] = true;
			}
		}

		ofstream ofF("./beijingISOF");
		ofstream ofB("./beijingISOB");
		ofstream ofU("./beijingISOU");
		ofstream of("./beijingISO");

		for(int i = 0; i < (int)adjList.size(); i++)
		{
			if(vbISOF[i])
				ofF << i << endl;

			if(vbISOB[i])
				ofB << i << endl;

			if(vbISOU[i])
				ofU << i << endl;

			if(vbISO[i])
				of << i << endl;
		}

		ofF.close();
		ofB.close();
		ofU.close();
		of.close();

		return 0;
	}
	else
	{
		int nodeID;
		cout << "Loading ISO Nodes" << endl;
		while(ifISOF >> nodeID)
			vbISOF[nodeID] = true;

		while(ifISOB >> nodeID)
			vbISOB[nodeID] = true;

		while(ifISOU >> nodeID)
			vbISOU[nodeID] = true;

		while(ifISO >> nodeID)
			vbISO[nodeID] = true;

		return 0;
	}
}

int Graph::BFS(int nodeID, bool bF, vector<bool>& vbVisited)
{
	vbVisited.assign(adjList.size()+1, false);
	queue<int> Q;
	Q.push(nodeID);
	vbVisited[nodeID] = true;
	int count = 0;
	while(!Q.empty())
	{
		int topNodeID = Q.front();
		Q.pop();
		count++;
		if(bF)
		{
			for(auto& it : adjList[topNodeID])
				if(!vbVisited[it.first])
				{
					vbVisited[it.first] = true;
					Q.push(it.first);
				}
		}
		else
		{
			for(auto& it : adjListR[topNodeID])
				if(!vbVisited[it.first])
				{
					vbVisited[it.first] = true;
					Q.push(it.first);
				}
		}
	}

	return count;
}
	
int Graph::readUSCoor(string filename) 
{
	vCoor.reserve(nodeNum);
	ifstream inFile(filename.c_str()); 
	if(!inFile)
	{
		cout << "Cannot open " << filename << endl;
		return -1;
	}

	string s;
	int nodeID;
	double lon, lat;
	while(inFile >> s)
	{
		if(s == "v") 
		{
			inFile >> nodeID >> lon >> lat;
			lon = -lon / 1000000;
			lat = lat / 1000000;
			vCoor.push_back(make_pair(lon, lat));
		}
	}

	return 0;
}

