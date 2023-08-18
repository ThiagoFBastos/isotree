#include <cstdio>
#include <cerrno>
#include <algorithm>
#include <iostream>
#include <utility>
#include <ext/pb_ds/assoc_container.hpp>
#include <cstring>
#include <fstream>
#include <cstdint>
#include <pthread.h>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <queue>

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

struct Data {
	int lv[N][N];
	int lv_sz[N];
	int sub[N];
	int deg[N];
	int adj[N][N];
	int par[N];
	int sub_lv_sz[N][N];
	int nsub[N];
	ht<uint64_t, null_type> trees;

	Data() {
		memset(deg, 0, sizeof deg);
		memset(nsub, 0, sizeof nsub);
		memset(lv_sz, 0, sizeof lv_sz);
		for(int i = 0; i < N; ++i)
			memset(sub_lv_sz[i], 0, sizeof sub_lv_sz[i]);
	}

    void join(const Data& other) {
        memcpy(&lv[0][0], &other.lv[0][0], sizeof lv);
        memcpy(lv_sz, other.lv_sz, sizeof lv_sz);
        memcpy(sub, other.sub, sizeof sub);
        memcpy(deg, other.deg, sizeof deg);
        memcpy(&adj[0][0], &other.adj[0][0], sizeof adj);
        memcpy(par, other.par, sizeof par);
        memcpy(&sub_lv_sz[0][0], &other.sub_lv_sz[0][0], sizeof sub_lv_sz);
        memcpy(nsub, other.nsub, sizeof nsub);
    }
};

int n;

uint64_t treePat(int [][N], int[], int, int = -1);
uint64_t treePat(int [][N], int[]);
inline uint64_t concat(uint64_t, uint64_t);
pair<int, int> center(int [][N], int []);
bool prunning(int, int);
void backtrack(int, int, int, int);

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
	return {C[0], C[1]};
}

bool prunning(Data& td, int h) {
	int max_depth[N];
	for(int i = h; i; --i) {
		for(int k = td.lv_sz[i] - 1; k >= 0; --k) {
			int v = td.lv[i][k], p = td.par[v];
			max_depth[v] = i;
			if(td.deg[v] - (p != -1)) max_depth[v] = max_depth[td.adj[v][p != -1]];
			if(k < td.lv_sz[i] - 1 && td.par[td.lv[i][k + 1]] == p && max_depth[v] < max_depth[td.lv[i][k + 1]])
				return true;
		}
	}
	return false;
}

void backtrack(Data& td, int h, int cur_p, int v) {
	if(prunning(td, h)) 
        return;

	else if(v == -1) {
		uint64_t pat = treePat(td.adj, td.deg);
		td.trees.insert(pat);
		return;
	}

	int p = td.lv[h - 1][cur_p];

	td.par[v] = p;
	td.adj[p][td.deg[p]++] = v;
	td.adj[v][td.deg[v]++] = p;
	td.lv[h][td.lv_sz[h]++] = v;

	td.sub[v] = h == 1 ? n - v - 2 : td.sub[p];

	if(td.sub_lv_sz[h][td.sub[v]]++ == 0) ++td.nsub[h];

	backtrack(td, h, cur_p, v - 1);

	if(v && td.nsub[h] >= 2) backtrack(td, h + 1, 0, v - 1);

	if(--td.sub_lv_sz[h][td.sub[v]] == 0) --td.nsub[h];

	--td.deg[p];
	--td.deg[v];
	--td.lv_sz[h];

	if(cur_p + 1 < td.lv_sz[h - 1]) {
		int a = td.lv[h-1][cur_p], b = td.lv[h - 1][cur_p + 1], k = cur_p;
		while(k < td.lv_sz[h - 1] && td.par[a] == td.par[td.lv[h - 1][k]]) ++k;
		if(td.par[a] != td.par[b] || td.deg[a] > 1) backtrack(td, h, cur_p + 1, v);
		else if(k < td.lv_sz[h - 1]) backtrack(td, h, k, v);
	}
}

queue<int> Q;
vector<int> status, hp, cur_pp, vp, id;
vector<pthread_t> thread_id;
vector<Data> td;
vector<pthread_mutex_t> mutex;
pthread_mutex_t mutex_q;

void* work(void* args) {
    int tid = *(int*)args;

   // printf("thread %d rodando(status = %d)\n", tid, status[tid]);

    while(true) {
        pthread_mutex_lock(&mutex[tid]);
        if(status[tid] == -1) { // nothing
            pthread_mutex_unlock(&mutex[tid]);
            continue;
        } else if(status[tid] == 0) { // run
            backtrack(td[tid], hp[tid], cur_pp[tid], vp[tid]);
            status[tid] = -1;
            pthread_mutex_unlock(&mutex[tid]);
            pthread_mutex_lock(&mutex_q);
            Q.push(tid);
            pthread_mutex_unlock(&mutex_q);
        } else { // exit
            pthread_mutex_unlock(&mutex[tid]);
            break;
        }
    }

    return nullptr;
}

void backtrack(Data& d, int h, int cur_p, int v, int lim) {
    if(prunning(d, h))
        return;
    else if(v == -1) {
        uint64_t pat = treePat(d.adj, d.deg);
		d.trees.insert(pat);
        return;
    } else if(lim == h) {
        while(true) {
            if(!pthread_mutex_trylock(&mutex_q)) {
                if(Q.empty()) pthread_mutex_unlock(&mutex_q);
                else {
                    int tid = Q.front();
                    Q.pop();
                    pthread_mutex_unlock(&mutex_q);
                    pthread_mutex_lock(&mutex[tid]);
                    status[tid] = 0;
                    td[tid].join(d);
                    hp[tid] = h, cur_pp[tid] = cur_p, vp[tid] = v;
                    pthread_mutex_unlock(&mutex[tid]);
                    break;
                }
            }
        }
        return;
    }

    int p = d.lv[h - 1][cur_p];

	d.par[v] = p;
	d.adj[p][d.deg[p]++] = v;
	d.adj[v][d.deg[v]++] = p;
	d.lv[h][d.lv_sz[h]++] = v;

	d.sub[v] = h == 1 ? n - v - 2 : d.sub[p];

	if(d.sub_lv_sz[h][d.sub[v]]++ == 0) ++d.nsub[h];

	backtrack(d, h, cur_p, v - 1, lim);

	if(v && d.nsub[h] >= 2) backtrack(d, h + 1, 0, v - 1, lim);

	if(--d.sub_lv_sz[h][d.sub[v]] == 0) --d.nsub[h];

	--d.deg[p];
	--d.deg[v];
	--d.lv_sz[h];

	if(cur_p + 1 < d.lv_sz[h - 1]) {
		int a = d.lv[h-1][cur_p], b = d.lv[h - 1][cur_p + 1], k = cur_p;
		while(k < d.lv_sz[h - 1] && d.par[a] == d.par[d.lv[h - 1][k]]) ++k;
		if(d.par[a] != d.par[b] || d.deg[a] > 1) backtrack(d, h, cur_p + 1, v, lim);
		else if(k < d.lv_sz[h - 1]) backtrack(d, h, k, v, lim);
	}
}

int main(int argc, char* argv[]) {

	int num_threads, lim;

	if(argc < 4 || sscanf(argv[1], "%d", &n) <= 0 || sscanf(argv[2], "%d", &num_threads) <= 0 || sscanf(argv[3], "%d", &lim) <= 0) {
		cout << "use: ./trees <nós> <save=0/1>\n";
		return 0;
	} else if(n > 24 || n < 4) {
		cout << "o número de nós deve ser entre 4 e 24\n";
		return 0;
	}

    pthread_mutex_init(&mutex_q, nullptr);

    status.assign(num_threads, -1);
    td.resize(num_threads);
    hp.resize(num_threads);
    cur_pp.resize(num_threads);
    vp.resize(num_threads);
    thread_id.resize(num_threads);
    id.resize(num_threads);
    mutex.resize(num_threads);

    for(int i = 0; i < num_threads; ++i) Q.push(i);

    for(int i = 0; i < num_threads; ++i) {
        id[i] = i;
        pthread_mutex_init(&mutex[i], nullptr);
        pthread_create(&thread_id[i], nullptr, work, &id[i]);
    }

    Data all;

    all.lv[0][0] = n - 1;
    all.lv_sz[0] = 1;
    all.par[n - 1] = -1;
        
    backtrack(all, 1, 0, n - 2, lim);

    vector<uint64_t> all_trees(all.trees.begin(), all.trees.end());

    for(int i = 0; i < num_threads; ++i) {
        while(true) {
            if(!pthread_mutex_trylock(&mutex[i])) {
                if(status[i] == -1) {
                    status[i] = 1;
                    pthread_mutex_unlock(&mutex[i]);
                    break;
                }
                pthread_mutex_unlock(&mutex[i]);
            }
        }
    }

    for(int i = 0; i < num_threads; ++i) pthread_join(thread_id[i], nullptr);

    for(int i = 0; i < num_threads; ++i) {
        auto& S = td[i].trees;
        all_trees.insert(all_trees.end(), S.begin(), S.end());
    }

    sort(all_trees.begin(), all_trees.end());
    all_trees.resize(unique(all_trees.begin(), all_trees.end()) - all_trees.begin());

    cout << "número de árvores = " << all_trees.size() << '\n';

	return 0;
}
