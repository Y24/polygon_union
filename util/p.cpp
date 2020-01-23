#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
using namespace std;
struct Point
{
    Point() = default;
    Point(double x, double y)
    {
        this->x = x;
        this->y = y;
    };
    double x;
    double y;
    ostream &operator<<(ostream &out) const
    {
        out << x << " " << y << endl;
        return out;
    }
    bool operator==(const Point &p) const
    {
        return x == p.x && y == p.y;
    }
};
istream &operator>>(istream &in, Point &p)
{
    in >> p.x >> p.y;
    return in;
}
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
    double line1, line2;
    line1 = pFirst1.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond1.y) +
            pSecond1.x * (pFirst2.y - pFirst1.y);
    line2 = pFirst1.x * (pSecond2.y - pFirst2.y) +
            pFirst2.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pFirst2.y - pFirst1.y);
    if (line1 == line2 && line1 != 0 || line1 != line2 && (line1 != 0 || line2 != 0) && line1 * line2 >= 0)
        return false;

    line1 = pSecond1.x * (pFirst1.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst1.y) +
            pFirst1.x * (pSecond2.y - pSecond1.y);
    line2 = pSecond1.x * (pFirst2.y - pSecond2.y) +
            pSecond2.x * (pSecond1.y - pFirst2.y) +
            pFirst2.x * (pSecond2.y - pSecond1.y);
    if (line1 == line2 && line1 != 0 || line1 != line2 && (line1 != 0 || line2 != 0) && line1 * line2 >= 0)
        return false;
    return true;
}

bool GetCrossPoint(const Point &p1, const Point &p2,
                   const Point &q1, const Point &q2,
                   double &x, double &y)
{
    if (p1 == q1)
    {
        x = p1.x;
        y = p1.y;
        return true;
    }
    if (p1 == q2)
    {
        x = p1.x;
        y = p1.y;
        return true;
    }
    if (p2 == q1)
    {
        x = p2.x;
        y = p2.y;
        return true;
    }
    if (p2 == q2)
    {
        x = p2.x;
        y = p2.y;
        return true;
    }
    if (IsRectCross(p1, p2, q1, q2))
    {
        if (IsLineSegmentCross(p1, p2, q1, q2))
        {
            //求交点
            double tmpLeft, tmpRight;
            tmpLeft = (q2.x - q1.x) * (p1.y - p2.y) - (p2.x - p1.x) * (q1.y - q2.y);
            tmpRight = (p1.y - q1.y) * (p2.x - p1.x) * (q2.x - q1.x) + q1.x * (q2.y - q1.y) * (p2.x - p1.x) - p1.x * (p2.y - p1.y) * (q2.x - q1.x);
            if (tmpLeft == 0)
                return false;
            x = tmpRight / tmpLeft;
            tmpLeft = (p1.x - p2.x) * (q2.y - q1.y) - (p2.y - p1.y) * (q1.x - q2.x);
            tmpRight = p2.y * (p1.x - p2.x) * (q2.y - q1.y) + (q2.x - p2.x) * (q2.y - q1.y) * (p1.y - p2.y) - q2.y * (q1.x - q2.x) * (p2.y - p1.y);
            if (tmpLeft == 0)
                return false;
            y = tmpRight / tmpLeft;
            return true;
        }
    }
    return false;
}

int main()
{
    Point p1, p2, q1, q2;
    double x = 0, y = 0;
    while (true)
    {
        cin >> p1 >> p2 >> q1 >> q2;
        if (GetCrossPoint(p1, p2, q1, q2, x, y))
        {
            cout << "cross true\n";
        }
        else
        {
            cout << "cross false\n";
        }
    }
    return 0;
}
