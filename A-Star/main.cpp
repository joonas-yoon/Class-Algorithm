#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
using namespace std;

typedef long long lld;

#define INF 987654321

typedef pair<int, int> ii;
int dy[] = { -1,0,1,-1,1,-1,0,1 };
int dx[] = { -1,-1,-1,0,0,1,1,1 };

struct point {
	int x, y;
	bool operator == (const point& p) const {
		return x == p.x && y == p.y;
	}
};

class node {
public:
	node() {}
	node(point& _p, double _f) : p(_p), f(_f) {}
	bool operator < (const node& n) const {
		return f > n.f;
	}
public:
	point p;
	double f;
};

double eucliean_dist(const point& a, const point& b) {
	double x = a.x - b.x;
	double y = a.y - b.y;
	return sqrt(x*x + y*y);
}

int W, H;
bool grid[1001][1001];
double dist[1001][1001];
point history[1001][1001];

inline bool isOutOfBound(int y, int x) {
	return y < 1 || x < 1 || y > H || x > W;
}

void reconstruct(point& p) {
	if (!(history[p.y][p.x] == p)) {
		reconstruct(history[p.y][p.x]);
	}
	printf("(%d, %d) ", p.y, p.x);
}

int main(){
	int TCase = 0;
	while (~scanf("%d %d", &H, &W)) {
		printf("Test case #%d:\n", ++TCase);

		for (int i = 1; i <= H; ++i) {
			for (int j = 1; j <= W; ++j) scanf("%d ", &grid[i][j]);
			for (int j = 0; j <= W; ++j) fill(dist[i], dist[i] + W + 1, INF);
		}

		point startPoint, endPoint;
		scanf("%d %d %d %d", &startPoint.y, &startPoint.x, &endPoint.y, &endPoint.x);

		priority_queue<node> q;
		q.push(node(startPoint, 0));
		dist[startPoint.y][startPoint.x] = 0;
		history[startPoint.y][startPoint.x] = startPoint;

		double weight[8];
		for (int i = 0; i < 8; ++i) {
			if (dx[i] != 0 && dy[i] != 0) weight[i] = sqrt(2.0);
			else weight[i] = 1;
		}

		while (!q.empty()) {
			node cur = q.top();
			q.pop();

			if (cur.p == endPoint) break;

			double g = dist[cur.p.y][cur.p.x];

			for (int d = 0; d < 8; ++d) {
				int ny = cur.p.y + dy[d];
				int nx = cur.p.x + dx[d];
				if (isOutOfBound(ny, nx) || grid[ny][nx]) continue;
				point next = { nx, ny };
				double h = eucliean_dist(next, endPoint);
				if (dist[ny][nx] <= g + weight[d]) continue;
				dist[ny][nx] = g + weight[d];
				history[ny][nx] = cur.p;
				q.push(node(next, g + h));
			}
		}
		double answer = dist[endPoint.y][endPoint.x];
		if (answer < INF) {
			printf("Path: "), reconstruct(endPoint);
			printf("\nLength is : %.4lf\n", answer);
		}
		else {
			puts("Impossible");
		}
		puts("");
	}
	return 0;
}