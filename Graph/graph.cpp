#include "graph.h"
#include <iostream>
#include <algorithm>
#include <queue>
#include <tuple>

class DSU {
	std::vector<int> parent;
public:
	DSU(int n) { parent.assign(n, -1); }

	int find(int v) { return parent.at(v) < 0 ? v : parent.at(v) = find(parent.at(v)); }
	void unite(int x, int y) { if (find(x) != find(y)) parent.at(find(x)) = find(y); }
};

Graph::Graph() : vertices(0) { }

Graph::Graph(const std::string& filename) : vertices(0) {
	std::fstream file;
	int weight = 0;

	file.open(filename, std::ios::in);

	if (!file) {
		std::cerr << "Cannot open file!\n";
		return;
	}

	file >> vertices;

	adj_list.resize(vertices);
	for (int i = 0; i < vertices; i++) {
		for (int j = 0; j < vertices; j++) {
			file >> weight;

			if (weight) {
				adj_list.at(i).push_back(std::make_pair(weight, j));
			}
		}
	}
	file.close();
}

Graph::Graph(int vertices) : vertices(vertices) {
	adj_list.resize(vertices);
}

Graph::~Graph() {
	clear();
}

void Graph::display() const {
	int i = 0;

	for (const auto& vertex : adj_list) {
		std::cout << i++ << " -> ";
		for (const auto& edge : vertex) {
			std::cout << edge << "  ";
		}
		std::cout << std::endl;
	}
}

bool Graph::is_undirected_graph() const {
	for (auto i = adj_list.cbegin(); i != adj_list.cend(); i++) {
		for (const auto& j : *i) {
			auto second = i - adj_list.cbegin();
			auto edge = std::make_pair(j.first, static_cast<int>(second));
			auto it = std::find(adj_list.at(j.second).begin(), adj_list.at(j.second).end(), edge);

			if (it == adj_list.at(j.second).end()) return false;
		}
	}
	return true;
}

int Graph::count_edges() const {
	int edges = 0;
	for (const auto& vertex : adj_list) {
		edges += static_cast<int>(vertex.size());
	}
	return edges;
}

std::vector<std::pair<int, int>> Graph::count_in_out_degrees() const {
	std::vector<std::pair<int, int>> in_out_degrees(vertices, std::make_pair(0, 0));

	//In-degree
	for (const auto& i : adj_list) {
		for (const auto& j : i) {
			in_out_degrees.at(j.second).first++;
		}
	}
	//Out-degree
	for (int i = 0; i < vertices; i++) {
		in_out_degrees.at(i).second = static_cast<int>(adj_list.at(i).size());
	}
	return in_out_degrees;
}

std::vector<int> Graph::dfs(int start_vertex) const {
	if (start_vertex >= vertices)
		return std::vector<int>();

	std::vector<int> path;
	std::vector<bool> visited(vertices, false);

	dfs(start_vertex, visited, path);
	return path;
}

std::vector<int> Graph::bfs(int start_vertex) const {
	if (start_vertex >= vertices)
		return std::vector<int>();

	std::vector<int> path;
	std::queue<int> queue;
	std::vector<bool> visited(vertices, false);

	visited.at(start_vertex) = true;
	queue.push(start_vertex);
	while (!queue.empty()) {
		int vertex = queue.front();
		path.push_back(vertex);
		queue.pop();

		for (const auto& i : adj_list.at(vertex)) {
			if (!visited.at(i.second)) {
				visited.at(i.second) = true;
				queue.push(i.second);
			}
		}
	}
	return path;
}

bool Graph::has_cycle() const {
	std::vector<bool> visited(vertices, false);
	std::vector<bool> stack(vertices, false);

	for (int i = 0; i < vertices; i++) {
		if (has_cycle(i, visited, stack))
			return true;
	}
	return false;
}

bool Graph::is_connected() const {
	for (int i = 0; i < vertices; i++) {
		if (dfs(i).size() < vertices)
			return false;
	}
	return true;
}

int Graph::count_wcc() const {
	int count = 0;
	std::vector<int> path;
	std::vector<bool> visited(vertices, false);
	Graph undirected_graph(vertices);

	for (int i = 0; i < vertices; i++) {
		for (const auto& j : adj_list.at(i)) {
			undirected_graph.add_edge(i, j.second);
		}
	}
	for (int i = 0; i < vertices; i++) {
		path.clear();
		if (!visited.at(i)) {
			count++;
			undirected_graph.dfs(i, visited, path);
		}
	}
	return count;
}

std::vector<int> Graph::dijkstra(int start_vertex) const {
	using pii = std::pair<int, int>;
	std::vector<int> distance(vertices, INT_MAX);
	std::priority_queue<pii, std::vector<pii>, std::greater<pii>> queue;

	queue.push(std::make_pair(0, start_vertex));
	distance.at(start_vertex) = 0;

	while (!queue.empty()) {
		int vertex = queue.top().second;
		queue.pop();

		for (const auto& i : adj_list.at(vertex)) {
			if (distance.at(vertex) == INT_MAX)
				continue;
			if (distance.at(vertex) + i.first < distance.at(i.second)) {
				distance.at(i.second) = distance.at(vertex) + i.first;
				queue.push(std::make_pair(distance.at(i.second), i.second));
			}
		}
	}
	return distance;
}

std::vector<int> Graph::floyd_warshall(int start_vertex) const {
	std::vector<std::vector<int>> distance(vertices, std::vector<int>(vertices, INT_MAX));

	for (int i = 0; i < vertices; i++) {
		for (const auto& j : adj_list.at(i)) {
			distance.at(i).at(j.second) = j.first;
		}
		distance.at(i).at(i) = 0;
	}

	for (int k = 0; k < vertices; k++) {
		for (int i = 0; i < vertices; i++) {
			for (int j = 0; j < vertices; j++) {
				if (distance.at(i).at(k) == INT_MAX || distance.at(k).at(j) == INT_MAX)
					continue;
				distance.at(i).at(j) = std::min(distance.at(i).at(j), distance.at(i).at(k) + distance.at(k).at(j));

			}
		}
	}
	return distance.at(start_vertex);

}

std::vector<int> Graph::bellman_ford(int start_vertex) const {
	std::vector<int> distance(vertices, INT_MAX);

	distance.at(start_vertex) = 0;
	for (int k = 0; k + 1 < vertices; k++) {
		for (int i = 0; i < vertices; i++) {
			for (const auto& j : adj_list.at(i)) {
				if (distance.at(i) == INT_MAX)
					continue;
				distance.at(j.second) = std::min(distance.at(j.second), distance.at(i) + j.first);
			}
		}
	}

	for (int i = 0; i < vertices; i++) {
		for (const auto& j : adj_list.at(i)) {
			if (distance.at(i) == INT_MAX)
				continue;
			if (distance.at(i) + j.first < distance.at(j.second)) {
				return std::vector<int>(vertices, -1);
			}
		}
	}
	return distance;
}

int Graph::prim() const {
	if (!is_undirected_graph())
		return 0;

	using pii = std::pair<int, int>;
	int cost = 0;
	std::priority_queue<pii, std::vector<pii>, std::greater<pii>> queue;
	std::vector<bool> visited(vertices, false);

	queue.push(std::make_pair(0, 0));

	while (!queue.empty()) {
		auto best = queue.top();
		queue.pop();

		if (visited.at(best.second))
			continue;
		cost += best.first;
		visited.at(best.second) = true;

		for (const auto& i : adj_list.at(best.second)) {
			if (!visited.at(i.second)) {
				queue.push(i);
			}
		}

	}
	return cost;
}

int Graph::kruskal() const {
	if (!is_undirected_graph())
		return 0;

	int cost = 0;
	DSU dsu(vertices);
	std::vector<std::tuple<int, int, int>> edge_list;	//<weight,<v1,v2>>

	for (int i = 0; i < vertices; i++) {
		for (const auto& j : adj_list.at(i)) {
			edge_list.push_back(std::make_tuple(j.first, i, j.second));
		}
	}
	std::sort(edge_list.begin(), edge_list.end());

	for (const auto& i : edge_list) {
		if (dsu.find(std::get<1>(i)) != dsu.find(std::get<2>(i))) {
			dsu.unite(std::get<1>(i), std::get<2>(i));
			cost += std::get<0>(i);
		}
	}
	return cost;
}

void Graph::clear() {
	for (auto& vertex : adj_list) {
		vertex.clear();
	}
	adj_list.clear();
	vertices = 0;
}

void Graph::dfs(int start_vertex, std::vector<bool>& visited, std::vector<int>& path) const {
	visited.at(start_vertex) = true;
	path.push_back(start_vertex);

	for (const auto& i : adj_list.at(start_vertex)) {
		if (!visited.at(i.second)) {
			dfs(i.second, visited, path);
		}
	}
}

bool Graph::has_cycle(int start_vertex, std::vector<bool>& visited, std::vector<bool>& stack) const {
	stack.at(start_vertex) = true;
	visited.at(start_vertex) = true;

	for (const auto& i : adj_list.at(start_vertex)) {
		if (!visited.at(i.second) && has_cycle(i.second, visited, stack))
			return true;
		if (stack.at(i.second))
			return true;
	}
	stack.at(start_vertex) = false;
	return false;
}

void Graph::add_edge(int vertex0, int vertex1) {
	adj_list.at(vertex0).push_back(std::make_pair(1, vertex1));
	adj_list.at(vertex1).push_back(std::make_pair(1, vertex0));
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& pair) {
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<int>& vector)
{
	for (const auto& i : vector) {
		std::cout << i << "  ";
	}
	return os;
}

Graph create_graph(const std::string& filename) {
	return Graph(filename);
}

void print(const std::vector<std::pair<int, int>>& vector) {
	int i = 0;

	std::cout << "Vertex  (In, Out):\n";
	for (const auto& itr : vector) {
		std::cout << "  " << i++ << "     " << itr << std::endl;
	}
}

void print(const std::vector<int>& vector) {
	int i = 0;

	std::cout << "Vertex  Distance from source:\n";
	for (const auto& itr : vector) {
		std::cout << "  " << i++ << "     " << itr << std::endl;
	}
}
