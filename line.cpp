#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;
struct Point
{
    Point() = default;
    Point(int x, int y)
    {
        this->x = x;
        this->y = y;
    };
    int x;
    int y;
};
//排斥实验
bool IsRectCross(const Point &p1, const Point &p2, const Point &q1, const Point &q2)
{
    bool ret = min(p1.x, p2.x) <= max(q1.x, q2.x) &&
               min(q1.x, q2.x) <= max(p1.x, p2.x) &&
               min(p1.y, p2.y) <= max(q1.y, q2.y) &&
               min(q1.y, q2.y) <= max(p1.y, p2.y);
    return ret;
}
//跨立判断
bool IsLineSegmentCross(const Point &pFirst1, const Point &pFirst2, const Point &pSecond1, const Point &pSecond2)
{
    long line1, line2;
    line1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond1.y) +
            pSecond1.x * (pFirst2.y - pFirst1.y);
    line2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pFirst2.y - pFirst1.y);
    if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
        return false;

    line1 = pSecond1.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst1.y) +
            pFirst1.x * (pSecond2.y - pSecond1.y);
    line2 = pSecond1.x * (pFirst2.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pSecond2.y - pSecond1.y);
    if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
        return false;
    return true;
}

bool GetCrossPoint(const Point &p1, const Point &p2,
                   const Point &q1, const Point &q2,
                   long &x, long &y)
{
    if (IsRectCross(p1, p2, q1, q2))
    {
        if (IsLineSegmentCross(p1, p2, q1, q2))
        {
            //求交点
            long tmpLeft, tmpRight;
            tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
            tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);

            x = (int)((double)tmpRight / (double)tmpLeft);

            tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
            tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
            y = (int)((double)tmpRight / (double)tmpLeft);
            return true;
        }
    }
    return false;
}
//  The function will return true if the point (x,y) is inside the polygon, or
//  false if it is not.
//  Note:
//  If the point is exactly on the edge of the polygon,
//  the function may return either true or false.
bool IsPointInPolygon(std::vector<Point> poly, Point pt)
{
    int i, j;
    bool c = false;
    for (i = 0, j = poly.size() - 1; i < poly.size(); j = i++)
    {
        if ((((poly[i].y <= pt.y) && (pt.y < poly[j].y)) ||
             ((poly[j].y <= pt.y) && (pt.y < poly[i].y))) &&
            (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x))
        {
            c = !c;
        }
    }
    return c;
}
//若点a大于点b,即点a相对点center在点b顺时针方向,返回true,否则返回false
bool PointCmp(const Point &a, const Point &b, const Point &center)
{
    if (a.x >= 0 && b.x < 0)
        return true;
    if (a.x == 0 && b.x == 0)
        return a.y > b.y;
    //向量OA和向量OB的叉积
    int det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
    if (det < 0)
        return true;
    if (det > 0)
        return false;
    //向量OA和向量OB共线，以距离判断大小
    int d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
    int d2 = (b.x - center.x) * (b.x - center.y) + (b.y - center.y) * (b.y - center.y);
    return d1 > d2;
}
void ClockwiseSortPoints(std::vector<Point> &vPoints)
{
    //计算重心
    Point center;
    double x = 0, y = 0;
    for_each(vPoints.begin(), vPoints.end(), [&](const Point &p) {
        x += p.x;
        y += p.y;
    });
    center.x = (int)x / vPoints.size();
    center.y = (int)y / vPoints.size();
    /* sort(vPoints.begin(), vPoints.end(), [&](const Point &a, const Point &b) {
        return PointCmp(a, b, center);
    }); */
    cout<<"center: "<<center.x<<" "<<center.y<<endl;
    //冒泡排序
    for (int i = 0; i < vPoints.size() - 1; i++)
    {
        for (int j = 0; j < vPoints.size() - i - 1; j++)
        {
            if (PointCmp(vPoints[j], vPoints[j + 1], center))
            {
                Point tmp = vPoints[j];
                vPoints[j] = vPoints[j + 1];
                vPoints[j + 1] = tmp;
            }
        }
    }
}
bool PolygonUnion(const vector<Point> &poly1,
                  const vector<Point> &poly2,
                  std::vector<Point> &interPoly)
{
    if (poly1.size() < 3 || poly2.size() < 3)
    {
        return false;
    }

    long x, y;
    //计算多边形交点
    for (int i = 0; i < poly1.size(); i++)
    {
        int poly1_next_idx = (i + 1) % poly1.size();
        for (int j = 0; j < poly2.size(); j++)
        {
            int poly2_next_idx = (j + 1) % poly2.size();
            if (GetCrossPoint(poly1[i], poly1[poly1_next_idx],
                              poly2[j], poly2[poly2_next_idx],
                              x, y))
            {
                interPoly.push_back(Point(x, y));
            }
        }
    }

    //计算多边形外部点
    for (int i = 0; i < poly1.size(); i++)
    {
        if (!IsPointInPolygon(poly2, poly1[i]))
        {
            interPoly.push_back(poly1[i]);
        }
    }
    for (int i = 0; i < poly2.size(); i++)
    {
        if (!IsPointInPolygon(poly1, poly2[i]))
        {
            interPoly.push_back(poly2[i]);
        }
    }

    if (interPoly.size() <= 0)
        return false;

    //点集逻辑排序
    ClockwiseSortPoints(interPoly);
    return true;
}
int main(int argc, char const *argv[])
{
    /* fstream in;
    if(argc>1){

    } */
    vector<Point> polygonA, polygonB, targetPoly;
    int a, b;
    int x, y;
    cout << "请输入两个多边形，格式： \n多边形1顶点数\n多边形1各顶点坐标\n多边形2顶点数\n多边形2各顶点坐标。\n";
    cout << "示例：\n4\n1 1 1 3 3 3 3 1\n4\n2 2 2 4 4 4 4 2\n";
    cout << "输入：\n";
    cin >> a;
    for (int i = 0; i < a; i++)
    {
        cin >> x >> y;
        polygonA.push_back(Point(x, y));
    }
    cin >> b;
    for (int i = 0; i < b; i++)
    {
        cin >> x >> y;
        polygonB.push_back(Point(x, y));
    }
    if (PolygonUnion(polygonA, polygonB, targetPoly))
    {
        cout << "Successed!\n";
        for_each(targetPoly.begin(), targetPoly.end(), [](const Point &p) {
            cout << p.x << " " << p.y << " \n";
        });
    }
    else
    {
        cout << "Error!\n";
    }
    return 0;
}
