#include <cstdio>
#include <cerrno>
#include <algorithm>
#include <iostream>
#include <utility>
#include <ext/pb_ds/assoc_container.hpp>
#include <cstring>
#include <fstream>
#include <cstdint>

using namespace std;
using namespace __gnu_pbds;

template <class K, class V>
using ht = gp_hash_table
<K, V, hash<K>, equal_to<K>,
direct_mask_range_hashing<>,
linear_probe_fn<>,
hash_standard_resize_policy<
hash_exponential_size_policy<>,
hash_load_check_resize_trigger
<>, true>>;

const int N = 30;

int lv[N][N], lv_sz[N];
int n, sub[N], deg[N], adj[N][N], par[N], sub_lv_sz[N][N], sub_sz[N], nsub[N];
ht<uint64_t, null_type> trees;

uint64_t treePat(int [][N], int[], int, int = -1);
uint64_t treePat(int [][N], int[]);
inline uint64_t concat(uint64_t, uint64_t);
pair<int, int> center(int [][N], int []);
bool prunning(int);
void backtrack(int, int, int);

uint64_t treePat(int g[][N], int deg[], int v, int p) {
	uint64_t s[N], repr = 1;
	int len = 0;
	for(int k = 0; k < deg[v]; ++k) {
		int u = g[v][k];
		if(p == u) continue;
		s[len++] = treePat(g, deg, u, v);
	}
	sort(s, s + len);
	for(int k = 0; k < len; ++k) repr = concat(repr, s[k]);
	return concat(repr, 0);
}

uint64_t treePat(int g[][N], int deg[]) {
	auto [c1, c2] = center(g, deg);
	uint64_t S1 = treePat(g, deg, c1), S2 = 0;
	if(c2 != n) S2 = treePat(g, deg, c2);
	return max(S1, S2);
}

uint64_t concat(uint64_t a, uint64_t b) {
	int len = b ? 64 - __builtin_clzll(b) : 1;
	return (a << len) | b;
}

pair<int, int> center(int g[][N], int deg[]) {
	int T = -1, t[N], d[N], q[N], C[] = {n, n}, lo = 0, hi = 0;
	for(int v = 0; v < n; ++v) {
		d[v] = deg[v];
		t[v] = 0;
		if(deg[v] == 1) q[hi++] = v;
	}
	while(lo < hi) {
		int v = q[lo++];
		if(T <= t[v]) {
			if(T < t[v]) C[1] = n;
			C[T == t[v]] = v;
			T = t[v];
		}
		for(int k = 0; k < deg[v]; ++k) {
			int u = g[v][k];
			if(--d[u] == 1) {
				t[u] = t[v] + 1;
				q[hi++]=u;
			}
		}
	}
	return {C[0],C[1]};
}

bool prunning(int h) {
	int max_depth[N];
	for(int i = h; i >= 0; --i) {
		for(int k = lv_sz[i] - 1; k >= 0; --k) {
			int v = lv[i][k], p = par[v];
			max_depth[v] = i;
			if(deg[v] - (p != -1)) {
				int u = adj[v][p != -1];
				max_depth[v] = max_depth[u];
			}
			if(k < lv_sz[i]-1 && par[lv[i][k+1]] == p && max_depth[v]<max_depth[lv[i][k+1]]) 
				return true;
		}
	}
	return false;
}

void backtrack(int h, int cur_p, int v) {
	if(prunning(h)) return;

	if(v == -1) {
		uint64_t pat = treePat(adj, deg);
		trees.insert(pat);
		return;
	}

	int p = lv[h - 1][cur_p];

	par[v] = p;
	adj[p][deg[p]++] = v, adj[v][deg[v]++] = p;
	lv[h][lv_sz[h]++] = v;

	sub[v] = h == 1 ? n - v - 2 : sub[p];

	if(sub_lv_sz[h][sub[v]]++ == 0) ++nsub[h];

	backtrack(h, cur_p, v - 1);

	if(v && nsub[h] >= 2) backtrack(h + 1, 0, v - 1);

	if(--sub_lv_sz[h][sub[v]] == 0) --nsub[h];

	--deg[p], --deg[v], --lv_sz[h];

	if(cur_p + 1 < lv_sz[h - 1]) {
		int a = lv[h-1][cur_p], b = lv[h-1][cur_p+1], k = cur_p;
		while(k < lv_sz[h-1] && par[a] == par[lv[h-1][k]]) ++k;
		if(par[a]!=par[b] || deg[a] > 1) backtrack(h, cur_p + 1, v);
		else if(k < lv_sz[h-1]) backtrack(h, k, v);
	}
}

int main(int argc, char* argv[]) {
	
	int save;
	
	if(argc < 3 || sscanf(argv[1], "%d", &n) <= 0 || sscanf(argv[2], "%d", &save) <= 0) {
		cout << "use: ./trees <nós> <save=0/1>\n";
		return 0;
	} else if(n > 24 || n < 4) {
		cout << "o número de nós deve ser entre 4 e 24\n";
		return 0;
	} else if(save && argc < 4) {
		cout << "use: ./trees <nós> 1 <arquivo>\n";
		return 0;
	}

	lv[0][0] = n - 1, lv_sz[0] = 1;
	par[n - 1] = -1;

	backtrack(1, 0, n - 2);

	if(save) {
		ofstream fs(argv[3], ofstream :: binary);
		vector<uint64_t> t(trees.begin(), trees.end());
		int cnt_trees = trees.size();
		fs.write((const char*)&n, sizeof(int));
		fs.write((const char*)&cnt_trees, sizeof(int));
		fs.write((const char*)t.data(), sizeof(uint64_t) * t.size());
		fs.close();
	}

	cout << "foram encontradas " << trees.size() << " árvores não rotuladas\n";

	return 0;
}
