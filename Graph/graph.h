#pragma once

#include <vector>
#include <utility>
#include <string>
#include <fstream>

class Graph {
public:
	Graph();
	Graph(const std::string& filename);
	Graph(int vertices);
	~Graph();

	void display() const;
	bool is_undirected_graph() const;
	int count_edges() const;
	std::vector<std::pair<int, int>> count_in_out_degrees() const;
	std::vector<int> dfs(int start_vertex) const;
	std::vector<int> bfs(int start_vertex) const;
	bool has_cycle() const;
	bool is_connected() const;	//strongly connected
	int count_wcc() const;		//weakly connected components
	std::vector<int> dijkstra(int start_vertex) const;
	std::vector<int> floyd_warshall(int start_vertex) const;
	std::vector<int> bellman_ford(int start_vertex) const;
	int prim() const;		//minimum spanning tree (undirected graph)
	int kruskal() const;	//as prim
	void clear();

private:
	int vertices;	//number of vertices;
	std::vector<std::vector<std::pair<int, int>>> adj_list;	//adjacency list

	void dfs(int start_vertex, std::vector<bool>& visited, std::vector<int>& path) const;
	bool has_cycle(int start_vertex, std::vector<bool>& visited, std::vector<bool>& stack) const;
	void add_edge(int, int);
};

std::ostream& operator<<(std::ostream&, const std::pair<int, int>&);

std::ostream& operator<<(std::ostream&, const std::vector<int>&);

Graph create_graph(const std::string& filename);

void print(const std::vector<std::pair<int, int>>&);

void print(const std::vector<int>&);