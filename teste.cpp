#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <stack>
#include <algorithm>
#include <set>
#include <cstring>
#include <cerrno>
#include <cassert>
#include <utility>
#include <queue>
#include <string>

using adjList = std :: vector<std :: vector<int>>;

adjList getAdjList(uint64_t, int);
inline uint64_t concat(uint64_t, uint64_t);
uint64_t treePat(const adjList&, int, int);
uint64_t treePat(const adjList&);
std :: pair<int, int> center(const adjList&);

adjList getAdjList(uint64_t t, int n) {
	adjList adj(n);
	std :: stack<int> st;
	
	int len = 2 * n, v = 0;

	assert(t > 0);
	assert(64 - __builtin_clzll(t) == len);

	for(int i = len - 1; i >= 0; --i) {
		if((t >> i) & 1) {
			if(!st.empty()) {
				int p = st.top();
				adj[p].push_back(v);
				adj[v].push_back(p);
			}
			st.push(v++);
		} else
			st.pop();
	}

	return adj;
}

std :: pair<int, int> center(const adjList& adj) {
	int n = adj.size(), max_t = 0;
	std :: queue<int> q;
	std :: vector<int> t(n), deg(n);

	for(int i = 0; i < n; ++i) {
		deg[i] = adj[i].size();
		if(deg[i] <= 1) {
			t[i] = 0;
			q.push(i);
		}
	}

	while(!q.empty()) {
		int u = q.front(); q.pop();
		max_t = std :: max(max_t, t[u]);
		for(int v : adj[u]) {
			if(--deg[v] != 1) continue;		
			t[v] = 1 + t[u];
			q.push(v);
		}
	}

	std :: vector<int> c;

	for(int i = 0; i < n; ++i) if(t[i] == max_t) c.push_back(i);

	if((int)c.size() == 2) return {c[0], c[1]};

	return {c[0], -1};	
}

uint64_t concat(uint64_t a, uint64_t b) {
	int len = b ? 64 - __builtin_clzll(b) : 1;
	return (a << len) | b;
}

uint64_t treePat(const adjList& adj, int u, int p) {
	std :: vector<uint64_t> sub;
	for(int v : adj[u]) {
		if(v == p) continue;
		sub.push_back(treePat(adj, v, u));
	}
	std :: sort(sub.begin(), sub.end());
	uint64_t code = 1;
	for(uint64_t z : sub) code = concat(code, z);
	return concat(code, 0);
}

uint64_t treePat(const adjList& adj) {
	auto [a, b] = center(adj);
	if(b != -1) return std :: max(treePat(adj, a, -1), treePat(adj, b, -1));
	return treePat(adj, a, -1);
}

int main(int argc, char* argv[]) {
	int save;

	if(argc < 3 || sscanf(argv[2], "%d", &save) <= 0) {
		std :: cout << "use: ./teste <arquivo> <print=0/1>\n";
		return 0;
	} else if(save && argc < 4) {
		std :: cout << "use: ./teste <arquivo> 1 <saida>\n";
		return 0;
	}

	std :: ifstream fs(argv[1], std :: ifstream :: binary);

	if(!fs.is_open()) {
		std :: cout << argv[1] << ": " << strerror(errno) << '\n';
		return 0;
	}

	int n, file_trees = 0;
	std :: set<uint64_t> treeCodes;
	std :: ofstream print;

	fs.read((char*)&n, sizeof(int));
	fs.read((char*)&file_trees, sizeof(int));

	assert(file_trees > 0);

	uint64_t *buffer {new uint64_t[file_trees]};
	
	fs.read((char*)buffer, sizeof(uint64_t) * file_trees);
	
	if(save) {
		print.open(argv[3], std :: ofstream :: binary);
		if(!print.is_open()) {
			std :: cout << argv[3] << ": " << strerror(errno) << '\n';
			return 0;
		}
	}

	for(int i = 0; i < file_trees; ++i) {
		uint64_t t = buffer[i];

		auto adj = getAdjList(t, n);

		uint64_t T = treePat(adj, 0, -1);

		assert(t == T);

		treeCodes.insert(T);

		if(save) {
			for(int v = 0; v < n; ++v) {
				std :: string A(n, '0');	
				for(int u : adj[v]) A[u] = '1';
				print << A << '\n';
			}
			print << '\n';
		}
	}
	
	assert((int)treeCodes.size() == file_trees);

	std :: cout << "o arquivo foi completamente processado.\n";
	std :: cout << "não existem árvores duplicadas nem mal formatadas\n";
	std :: cout << "para ver se o número de árvores está correto: https://oeis.org/A000055/list\n";

	fs.close();

	if(save) print.close();
	
	delete[] buffer;

	return 0;
}
