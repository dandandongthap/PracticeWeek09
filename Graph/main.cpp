#include <iostream>
#include "graph.h"

void test_graph();

int main()
{
	test_graph();

	system("pause");
	return 0;
}

void test_graph()
{
	using std::cout;
	using std::endl;

	Graph graph("graph.txt");

	cout << "Adjacency list (weight, vertex):\n";
	graph.display();
	cout << "Is undirected graph: " << graph.is_undirected_graph() << endl;
	cout << "Number of edges: " << graph.count_edges() << endl;
	print(graph.count_in_out_degrees());
	cout << "DFS (2): " << graph.dfs(2) << endl;
	cout << "BFS (2): " << graph.bfs(2) << endl;
	cout << "Has cycle: " << graph.has_cycle() << endl;
	cout << "Is strongly connected: " << graph.is_connected() << endl;
	cout << "Number of WCC: " << graph.count_wcc() << endl;
	cout << "Shortest path (Dijkstra):\n";
	print(graph.dijkstra(2));
	cout << "Shortest path (Floyd-Warshall):\n";
	print(graph.floyd_warshall(2));
	cout << "Shortest path (Bellman-Ford):\n";
	print(graph.bellman_ford(2));
	cout << "Weight of MST (Prim): " << graph.prim() << endl;
	cout << "Weight of MST (Kruskal): " << graph.kruskal() << endl;
}
