#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <queue>
#include <iostream>
using namespace std;

#define forn(i, n) for(int i = 0; i < int(n); ++i)
#define for1(i, n) for(int i = 1; i <= int(n); ++i)
#define forv(i, v) forn(i, v.size())
#define pb push_back
#define all(v) v.begin(), v.end()
#define mp make_pair

typedef pair<int, int> pii;
typedef vector<int> vi;
typedef long long ll;
typedef long double ld;

void require(bool cond, const string& message = "Runtime error")
{
    if (!cond) {
        cerr << message << endl;
        assert(false);
    }
}

template <typename T>
struct PointT
{
    PointT() : x(T()), y(T()) { }
    PointT(T xx, T yy) : x(xx), y(yy) { }
    T x, y;
};

typedef PointT<ll> Point;
typedef vector<Point> Points;

void readData(Points& points)
{
    int n;
    cin >> n;
    points.resize(n);
    forn(i, n) {
        cin >> points[i].x >> points[i].y;
    }
}

bool lexComp(const Point& a, const Point& b)
{
    return a.x < b.x || a.x == b.x && a.y < b.y;
}

template <typename T>
T vectProd(const PointT<T>& a, const PointT<T>& b, const PointT<T>& c)
{
    return a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
}

bool isClockWise(const Point& a, const Point& b, const Point& c)
{
    return vectProd(a, b, c) < 0;
}

bool isCounterClockWise(const Point& a, const Point& b, const Point& c)
{
    return vectProd(a, b, c) > 0;
}

Points convexHull(Points points)
{
    if (points.size() == 1) return points;
    sort(all(points), &lexComp);
    Point p1 = *points.begin(),  p2 = points.back();
    Points up, down;
    up.push_back(p1);
    down.push_back(p1);

    for1(i, points.size() - 1) {
        const Point& pt = points[i];
        if (i + 1 == points.size() || isClockWise(p1, pt, p2)) {
            while (up.size() >= 2 &&
                   !isClockWise(up[up.size()-2], up.back(), pt)) {
                up.pop_back();
            }
            up.pb(pt);
        }
        if (i + 1 == points.size() || isCounterClockWise(p1, pt, p2)) {
            while (down.size() >= 2 &&
                   !isCounterClockWise(
                            down[down.size()-2], down.back(), pt)) {
                down.pop_back();
            }
            down.pb(pt);
        }
    }

    Points result;
    forv(i, up) result.pb(up[i]);
    reverse(all(down));
    require(!down.empty(), "Down vector is empty");
    down.pop_back();
    forv(i, down) result.pb(down[i]);
    return result;
}

double dist(const Point& a, const Point& b)
{
    return hypot(a.x - b.x, a.y - b.y);
}

double naiveMostDistant(const Points& points)
{
    double result = 0.0;
    forv(j, points) {
        forn(i, j) {
            result = max(result, dist(points[i], points[j]));
        }
    }
    return result;
}

bool checkNoConsequentColinear(const Points& points)
{
    if (points.size() <= 2) {
        return true;
    }
    int n = points.size();
    forv(i, points) {
        const Point& start = points[i];
        const Point& middle = points[(i + 1) % n];
        const Point& end = points[(i + 2) % n];
        if (vectProd(start, middle, end) == 0) {
            return false;
        }
    }
    return true;
}

double mostDistant(const Points& points)
{
    int n = points.size();
    int lap = 0, rap = 1;
    double result = 0.0;
    forn(index, n) {
        int prevIndex = (index + n - 1) % n;
        const Point& prev = points[prevIndex];
        const Point& cur = points[index];
        while (vectProd(prev, cur, points[(lap + 1) % n]) >
               vectProd(prev, cur, points[lap])) ++lap;
        while (vectProd(prev, cur, points[(rap + 1) % n]) >=
               vectProd(prev, cur, points[rap])) ++rap;
        for (int i = lap; i != rap; i = (i + 1) % n) {
            result = max(result, dist(cur, points[i]));
        }
        result = max(result, dist(cur, points[rap]));
    }
    return result;
}

void solve(const Points& points, double& result)
{
    const int MAX_NUM_POINTS_NAIVE = 10;
    if (points.size() <= MAX_NUM_POINTS_NAIVE) {
        result = naiveMostDistant(points);
    }
    else {
        require(checkNoConsequentColinear(points),
                "There are three consequent colinear points");
        result = mostDistant(points);
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    Points points;
    readData(points);
    double result;
    solve(convexHull(points), result);
    cout.precision(10);
    cout << fixed << result << endl;

    return 0;
}