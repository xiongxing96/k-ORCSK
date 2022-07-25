1.How to execute the query example?

In construction operation.
In mian.cpp, we set this for default(you can change it for other maps)
	string keysFile = "./map/data_NY/sub/NY_sub_keys_20_200.node";	//keywords file of NY
	string graphFile = "./map/USA-road-d.NY.gr";			//graph file, subnet of NY 
	const char *indexFile ="./map/data_NY/h2h/NY_sub.index";	//H2H index file of NY
	const char *edgeFile ="./map/data_NY/h2h/NY_sub.edge";		//edge file(graph file same)for loading index
	Graph g = Graph(graphFile);					//create Graph
	g.set_nodeKEYS_NodesBit(keysFile);				//set vertexes'Bit
	g.Init_H2H_Index(indexFile, edgeFile);

In query operation.
step :  execute ./KSP testData/NY.query.Q1.txt  or ./a.sh
	The screen will print result of query of query file. (such as           NY.query.Q1/     NY.query.Q2     )

NY.query.Q1 is a query file. It gives three integer and a string (ID1, ID2, k, querykey) for example(15926 18227 5 25,85,180,12) in each line.  Both distance and its'POI set route  query is enquired between ID1 and ID2, and return top-k shortest results satisfied query keywords set.



2.DAPrune algorithm is divided into three modules

Algorithm body: DAPrune.cpp
Detailed steps:
step1: Handle query keywords (vector<int>\string\bitset) and construct SPT(binary file encoding)</int>

step2: Initialize the minimum priority queue and push the shortest path (calculate the sum of its three segments as Rank value: prefix path length + deviation path length + 		heuristic maxLB value)

step3: Enter the while loop and determine whether the queue is empty or k results are satisfied.

step4: Candidate path P of extract_min each time, record the last deviation position pos of the current path.

step5: Traverse all points behind pos position of the candidate path P as the subsequent deviation point set Ci, and continuously search for 1NN POI(call function FindNN), along 		with the pruning process of continuous deviation

step6: After 1NN POI is obtained each time, a new candidate path Pnew is formed, and the heuristic maxLB value is calculated. The sum of the three values is obtained and pushed into 		the priority queue and the path is maintained
	Note: A path link reverse pruning was performed on the first two sections of the three values to further shorten TMPDIST.
	True maintained candidate path tmPath(prefix path + deviation path +SPT shortest path)

step7: Maintain each value: 	Candidate path: tmPath
				Deviation point of candidate route: PNN
				Candidate path deviation point position: tmpos
				The candidate path has met the keyword set: tmpBit
				Candidate path prefix Path length + Offset path length: tmpdist
				Candidate path heuristic maxLB value: tmpLB
				Update the priority queue and return the top-k routes

Function call:
(1) FindNN
Using RSP inverted index, using the current deviation to find 1NN POI(remaining query keywords)
Find2NN and Find1NN can both be returned.

(2) CompLB
(Max {min{source-keyword-destination}}=> Calculated min can be used to prune uncalculated keywords in advance)



