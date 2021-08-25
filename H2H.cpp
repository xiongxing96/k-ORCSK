#include "graph.h"

int Graph::LCAQuery(int _p, int _q){
		int p = toRMQ[_p], q = toRMQ[_q];
		
		if (p > q){
			int x = p;
			p = q;
			q = x;
		}
		int len = q - p + 1;
		
		int i = LOGD[len], k = LOG2[len];
		
		q = q - i + 1;
		if (height[RMQIndex[k][p]] < height[RMQIndex[k][q]])
			return RMQIndex[k][p];
		else return RMQIndex[k][q]; 
	}
	
//long long queryCnt;	
//long long aCnt;
int Graph::distanceQuery(int p, int q){
		if (p == q) return 0;
		int x = belong[p], y = belong[q];	
		int lca = LCAQuery(x, y);
		if (lca == x || lca == y){
	//		queryCnt++;
	//		aCnt++;
			if (lca == y){
				int v = y;
				y = x;
				x = v;
				v = p;
				p = q;
				q = v;
			}
			return dis[y][pos[x][posSize[x] - 1]];
		}
		else {
			int res = infinity;
			int *dx = dis[x], *dy = dis[y],*p2 = pos2[lca];
			_mm_prefetch(dx, _MM_HINT_T0);
			_mm_prefetch(dy, _MM_HINT_T0);
			_mm_prefetch(p2, _MM_HINT_T0);
			int ps = pos2Size[lca];
			for (int i = 0; i < ps; i++){
				//queryCnt ++;
				int tmp = dx[p2[i]] + dy[p2[i]];
				if (res > tmp)
					res = tmp;
			}
			return res;
		}
		
	}
	
void Graph::readGraph(const char *filename){
		FILE * file = fopen(filename, "r");
		int n, m;
		if(fscanf(file, "%d %d", &n, &m)){};
		Degree = (int*)malloc(sizeof(int) * (n + 1));
		vector< vector<pair<int, int> > > nb;
		vector<pair<int, int> > v;
		v.clear();
		for (int i = 0; i <= n; i++){
		//	Degree[i] = 0;
			nb.push_back(v);	
		}
	//	cout << n << " " << m << endl;
		for (int i = 0; i < m; i++){
			int x, y, z;
			if(fscanf(file, "%d %d %d", &x, &y, &z)){};
	//		Degree[x]++;
	//		cout << x << " " << y << " " << z << endl;
			nb[x].push_back(make_pair(y, z));
		}
		Neighbor = (int**)malloc(sizeof(int*) * (n + 1));
		Weight = (int**)malloc(sizeof(int*) * (n + 1));
		for (int i = 1; i <= n; i++){
			Degree[i] = nb[i].size();
			Neighbor[i] = (int*)malloc(sizeof(int) * nb[i].size());
			Weight[i] = (int*)malloc(sizeof(int) * nb[i].size());
			for (int j = 0; j < nb[i].size(); j++){
				Neighbor[i][j] = nb[i][j].first;
				Weight[i][j] = nb[i][j].second;
			}
		}
	}
int Graph::shortestPathQuery(int p, int q){
	//	p = p + 1;
	//	q = q + 1;
		int res = distanceQuery(p, q);
	//	cout<<p-1<<" -> ";
		while (p != q){
			int pq = distanceQuery(p, q);
			for (int i = 0; i < Degree[p]; i++){
				int x = Neighbor[p][i];
				int xq = distanceQuery(x, q);
				if (xq + Weight[p][i] == pq){
					p = x;
	//				cout<<x-1<<" -> ";
					break;
				}
			} 
		}
		cout<<endl;
		return res;
	}
/*	
	int test(char *file){
	//	cout << "test: " << file << " BEGIN" << endl;
		FILE *fin = fopen(file, "r");
		int x, y, dis, res = 0;
		vector<int> X, Y, DIS;
		X.clear();
		Y.clear();
		DIS.clear();
		while (fscanf(fin, "%d %d %d", &x, &y, &dis) != EOF){
			if (x <= 0 || y <= 0) break;
			X.push_back(x);
			Y.push_back(y);
			DIS.push_back(dis);
			res++;
		}
		cout << X.size() << endl;

		//
		queryCnt = 0;
	//	aCnt = 0;
		//
		double t= 0;
		int *ANS;
		ANS = (int*)malloc(sizeof(int) * (X.size() + 1));
		double start_time = GetTime();
		for (int i = 0; i < X.size(); i++){
			ANS[i] = distanceQuery(X[i], Y[i]);
		//	if (ANS[i] != DIS[i]){
		//		cout << X[i] << " " << Y[i] << " " << DIS[i] << " " << ANS[i] << endl;
		//		while(1);
		//	}
		}
		double end_time = GetTime();
		for (int i = 0; i < X.size(); i++){
			t += ANS[i];
		}
	//	cout << end_time - start_time << endl;
	//	cout << res << endl;
		cout << "Check Count: " << double(queryCnt) / res  << endl;
	//	cout << "aCnt: " << double(aCnt) / res << endl;
		printf("Distance Query Time : %lf usec\n", (end_time - start_time) * 1e6 / res);
		printf("average distance: %.6lf\n",t / res);
		/*
		start_time = GetTime();
		long long step = 0;
		for (int i = 0; i < X.size(); i++){
			step += shortestPathQuery(X[i], Y[i]);
		}		
		end_time = GetTime();
		cout << step;
		printf("Shortest Path Query Time : %lf usec\n", (end_time - start_time) * 1e6 / res);
		printf("average step: %.6lf\n", double(step) / double(res));
		
		return res;
	}
*/	

//	string TT = "";

	void Graph::scanIntArray(int *a, int n){
		if(fread(a, SIZEOFINT, n, fin_Index)){};
	}
	int* Graph::scanIntVector(int *a){
		int _n;
		if(fread(&_n, SIZEOFINT, 1, fin_Index)){};
		a = (int*)malloc(sizeof(int) * _n);
		scanIntArray(a, _n);
		return a;
	}

	void Graph::readIndex(const char *file){
		//double _time = GetTime();
		cout<<"--------loading H2H index---------"<<endl;
		int tree_height = 0, tree_width = 0, most_sp = 0;
		fin_Index = fopen(file, "rb");
		if(fread(&nodeNum, SIZEOFINT, 1, fin_Index)){};
		int ts;
		if(fread(&ts, SIZEOFINT, 1, fin_Index)){};
		TreeSize = ts;
		height = (int*)malloc(sizeof(int) * (ts + 1));
		for (int i = 0; i < ts; i++){
			if(fread(&height[i], SIZEOFINT, 1, fin_Index)){};
		}
		belong = (int*)malloc(sizeof(int) * (nodeNum + 1));
	  	if(fread(belong, SIZEOFINT, nodeNum + 1, fin_Index)){};
		toRMQ = (int*)malloc(sizeof(int) * (nodeNum + 1));
	  	if(fread(toRMQ, SIZEOFINT, nodeNum + 1, fin_Index)){};
		int ris;
		if(fread(&ris, SIZEOFINT, 1, fin_Index)){};
		if(fread(&ts, SIZEOFINT, 1, fin_Index)){};
		EulerSeq = (int*)malloc(sizeof(int) * (ts + 1));
		RMQIndex = (int**)malloc(sizeof(int*) * (ris + 1));
		for (int i = 0; i < ris; i++){
			RMQIndex[i] = scanIntVector(RMQIndex[i]);
		}
		if(fread(&root, SIZEOFINT, 1, fin_Index)){};
		cout << "root: " << root << endl;
		
		posSize = (int*)malloc(sizeof(int) * (nodeNum + 1));
		pos2Size = (int*)malloc(sizeof(int) * (nodeNum + 1));
		pos = (int**)malloc(sizeof(int*) * (TreeSize));
		pos2 = (int**)malloc(sizeof(int*) * (TreeSize));
		dis = (int**)malloc(sizeof(int*) * (TreeSize));
		chSize = (int*)malloc(sizeof(int) * (TreeSize));
		ch = (int**)malloc(sizeof(int*) * (TreeSize));
			
		for (int i = 0; i < TreeSize; i++){
			if(fread(&chSize[i], SIZEOFINT, 1, fin_Index)){};
			ch[i] = (int*)malloc(sizeof(int) * chSize[i]);
			for (int j = 0; j < chSize[i]; j++){
				int x;
				if(fread(&x, SIZEOFINT, 1, fin_Index)){};
				ch[i][j] = x;
			}
		}
		for (int i = 0; i < TreeSize; i++){
			int x;
			if(fread(&x, SIZEOFINT, 1, fin_Index)){};
			if(fread(&posSize[x], SIZEOFINT, 1, fin_Index)){};
			pos[x] = (int*)malloc(sizeof(int) * (posSize[x] + 1));
			if(fread(pos[x], SIZEOFINT, posSize[x], fin_Index)){};
			if (posSize[x] > tree_width)
				tree_width = posSize[x];
			int _n;
			if(fread(&_n, SIZEOFINT, 1, fin_Index)){};
			dis[x] = (int*)malloc(sizeof(int) * _n);
			if(fread(dis[x], SIZEOFINT, _n, fin_Index)){};
			if (_n > tree_height)
				tree_height = _n;
		}
		printf("dis read finished!\n");
		for (int i = 0; i < TreeSize; i++){
			int x;
			if(fread(&x, SIZEOFINT, 1, fin_Index)){};
			if(fread(&pos2Size[x], SIZEOFINT, 1, fin_Index)){};
			pos2[x] = (int*)malloc(sizeof(int) * (pos2Size[x] + 1));
			if(fread(pos2[x], SIZEOFINT, pos2Size[x], fin_Index)){};
			if (pos2Size[x] > most_sp)
				most_sp = pos2Size[x];
		}
		
		fclose(fin_Index);
		//
		//printf("Load Index Time : %lf sec\n", (GetTime() - _time));
		printf("tree height: %d\n", tree_height);
		printf("tree width: %d\n", tree_width);
		printf("most search space: %d\n", most_sp);
		//
	}
/*	int cnt;
	void getDFSListDFS(int p){
		toDFS[p] = cnt;
		DFSList[cnt++] = p;
		for (int i = 0; i < chSize[p]; i++){
			getDFSListDFS(ch[p][i]);
		}
		BS[p] = (int**)malloc(sizeof(int*) * chSize[p]);
		for (int i = 0; i < chSize[p]; i++){
			BS[p][i] = (int*)malloc(sizeof(int) * chSize[p]);
			for (int j = 0; j < chSize[p]; j++){
				if (posSize[ch[p][i]] < posSize[ch[p][j]])
					BS[p][i][j] = ch[p][i];
				else BS[p][i][j] = ch[p][j];
			}
		}
	}
	void getDFSList(){
		DFSList = (int*) malloc(sizeof(int) * (TreeSize + 1));
		toDFS = (int*) malloc(sizeof(int) * (TreeSize + 1));
		BS = (int***)malloc(sizeof(int**) * (TreeSize + 1));
		cnt = 0;
		getDFSListDFS(root);
	}
*/	
//index for shortest path cost and graph for recover path. 
void Graph::Init_H2H_Index(const char *index, const char* graph){
	readIndex(index);
	readGraph(graph);
	LOG2 = (int*)malloc(sizeof(int) * (nodeNum * 2 + 10));
	LOGD = (int*)malloc(sizeof(int) * (nodeNum * 2 + 10));
	int k = 0, j = 1;
	for (int i = 0; i < nodeNum * 2 + 10; i++){
		if (i > j * 2){
			j *= 2;
			k++;
		}
		LOG2[i] = k;
		LOGD[i] = j;
	}
	cout << "--------load index finished---------!!!" << endl;
}

int Graph::H2HPath(int ID1, int ID2, vector<int>& vPath, vector<int>& vPathEdge)
{
	int p = ID1+1, q = ID2+1;
	int res = distanceQuery(p, q);
	vPath.push_back(p-1);
	int tn = p;
        while (p != q){ 
        	int pq = distanceQuery(p, q); 
                for (int i = 0; i < Degree[p]; i++){
  			int x = Neighbor[p][i];
                        int xq = distanceQuery(x, q); 
                        if (xq + Weight[p][i] == pq){
 	                       p = x;
                               vPath.push_back(p-1);
		       	       vPathEdge.push_back(adjListEdge[tn-1][i].second);
			    // cout<<tn-1<<": "<<p-1<<" = "<<adjListEdge[tn-1][i].second<<endl;
			       tn = x; 
                               break;
                         }
                 }
        }
        return res;
}
