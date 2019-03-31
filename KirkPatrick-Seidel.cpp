#include <bits/stdc++.h>
using namespace std;

struct Point
{
    long long int x, y;
};
vector<Point> S;

bool samePoint(Point a, Point b)
{
    return (a.x == b.x) && (a.y == b.y);
}

void printS()
{
    for (auto i = S.begin(); i != S.end(); i++)
    {
        cout << (*i).x << ", " << (*i).y << endl;
    }
}

void printVector(vector<Point> v)
{
    for (auto i = v.begin(); i != v.end(); i++)
    {
        cout << (*i).x << ", " << (*i).y << endl;
    }
}

double calcSlope(pair<Point, Point> edge)
{
    double slope = (edge.first.y - edge.second.y) / double((edge.first.x - edge.second.x));
    return slope;
}

bool double_equals(double a, double b, double epsilon = 0.001)
{
    return abs(a - b) < epsilon;
}

pair<Point, Point> upperBridge(vector<Point> T, double median)
{

    vector<Point> candidates;
    //if there are only two points, then they constitute the bridge
    if (T.size() == 2)
    {
        pair<Point, Point> ans;
        ans.first = T[0];
        ans.second = T[1];
        if (ans.first.x > ans.second.x)
        {
            Point buff = ans.first;
            ans.first = ans.second;
            ans.second = buff;
        }

        return ans;
    }

    vector<pair<Point, Point>> pairs;

    // Random indices
    // Generate position vector
    vector<int> position(T.size());
    iota(position.begin(), position.end(), 0);
    // // Random Generator
    // random_device random_dev;
    // mt19937 generator(random_dev());
    // shuffle(position.begin(), position.end(), generator);
    random_shuffle(position.begin(), position.end());

    while (position.size() > 1)
    {
        Point buff_a, buff_b;
        buff_a = T[position.back()];
        position.pop_back();
        buff_b = T[position.back()];
        position.pop_back();
        pairs.push_back(make_pair(buff_b, buff_a));
    }
    // Add the unpaired point to candidates
    if (position.size() == 1)
    {
        candidates.push_back(T[position.back()]);
    }

    vector<double> slopes;
    vector<pair<Point, Point>> shortlist;
    for (auto iter = pairs.begin(); iter != pairs.end(); iter++)
    {
        //if selected edge is a vertical line, then push the highest point into candidates
        if ((*iter).first.x == (*iter).second.x)
        {
            if ((*iter).first.y > (*iter).second.y)
                candidates.push_back((*iter).first);
            else
                candidates.push_back((*iter).second);
        }
        else
        {
            //calculate slopes and push back such pair of points (for which slopes are being calculated)
            pair<Point, Point> temp_edge = *iter;
            double k = calcSlope(temp_edge);
            shortlist.push_back(temp_edge);
            slopes.push_back(k);
        }
    }

    if (slopes.size() == 0)
        cout << "no slopes" << endl;

    double slopes_median;
    vector<double> slopes_copy = slopes;
    nth_element(slopes_copy.begin(), slopes_copy.begin() + slopes_copy.size() / 2, slopes_copy.end());
    double slopes_med_right = slopes_copy[slopes_copy.size() / 2];
    if (slopes_copy.size() % 2 == 1)
    {
        slopes_median = slopes_med_right;
    }
    else
    {
        nth_element(slopes_copy.begin(), slopes_copy.begin() + (slopes_copy.size() / 2) - 1, slopes_copy.end());
        double slopes_med_left = slopes_copy[(slopes_copy.size() / 2) - 1];
        slopes_median = (slopes_med_left + slopes_med_right) / 2;
    }

    //split edges in shortlist into small, equal, large based on their resp slope values
    vector<pair<Point, Point>> small, equal, large;
    for (int b = 0; b < shortlist.size(); b++)
    {
        if (slopes[b] < slopes_median)
            small.push_back(shortlist[b]);
        else if (slopes[b] > slopes_median)
            large.push_back(shortlist[b]);
        else
            equal.push_back(shortlist[b]);
    }

    //find a supporting line of S with slope = slopes_median, and the points that lie on this line
    vector<Point> max;
    Point p_k = T[0], p_m = T[0];
    double maximum = p_k.y - (slopes_median * p_k.x);
    max.push_back(p_k);
    for (auto iter = T.begin(); iter != T.end(); iter++)
    {
        Point test = *iter;
        double intercept = test.y - (slopes_median * test.x);
        if (!samePoint(test, p_k) && !samePoint(test, p_m) && double_equals(intercept, maximum))
        {
            max.push_back(test);
            if (test.x < p_k.x)
            {
                p_k = test;
            }
            else if (test.x > p_m.x)
            {
                p_m = test;
            }
        }
        else if (intercept > maximum)
        {
            maximum = intercept;
            max.clear();
            max.push_back(test);
            p_k = test;
            p_m = test;
        }
    }

    //if h contains the bridge
    if (p_k.x <= median && p_m.x > median)
    {
        return make_pair(p_k, p_m);
    }
    //h contains only points of S to the left of or on L
    if (p_m.x <= median)
    {
        //insert all the second points from pairs in large and equal into candidates
        for (auto iter = large.begin(); iter != large.end(); iter++)
            candidates.push_back((*iter).second);
        for (auto iter = equal.begin(); iter != equal.end(); iter++)
            candidates.push_back((*iter).second);
        //insert both points from pairs in small into candidates
        for (auto iter = small.begin(); iter != small.end(); iter++)
        {
            candidates.push_back((*iter).first);
            candidates.push_back((*iter).second);
        }
    }
    //h contains only points of S to the right of L
    if (p_k.x > median)
    {
        //insert all the second points from pairs in small and equal into candidates
        for (auto iter = small.begin(); iter != small.end(); iter++)
            candidates.push_back((*iter).first);
        for (auto iter = equal.begin(); iter != equal.end(); iter++)
            candidates.push_back((*iter).first);
        //insert both points from pairs in large into candidates
        for (auto iter = large.begin(); iter != large.end(); iter++)
        {
            candidates.push_back((*iter).first);
            candidates.push_back((*iter).second);
        }
    }

    return upperBridge(candidates, median);
}

pair<Point, Point> lowerBridge(vector<Point> T, double median)
{

    vector<Point> candidates;
    //if there are only two points, then they constitute the bridge
    if (T.size() == 2)
    {
        pair<Point, Point> ans;
        ans.first = T[0];
        ans.second = T[1];
        if (ans.first.x > ans.second.x)
        {
            Point buff = ans.first;
            ans.first = ans.second;
            ans.second = buff;
        }

        return ans;
    }

    vector<pair<Point, Point>> pairs;

    // Random indices
    // Generate position vector
    vector<int> position(T.size());
    iota(position.begin(), position.end(), 0);
    // Random Generator
    // random_device random_dev;
    // mt19937 generator(random_dev());
    // shuffle(position.begin(), position.end(), generator);

    // vector<int> position(T.size());
    // iota(position.begin(), position.end(), 0);

    while (position.size() > 1)
    {
        Point buff_a, buff_b;
        buff_a = T[position.back()];
        position.pop_back();
        buff_b = T[position.back()];
        position.pop_back();
        pairs.push_back(make_pair(buff_b, buff_a));
    }
    // Add the unpaired point to candidates
    if (position.size() == 1)
    {
        candidates.push_back(T[position.back()]);
    }

    vector<double> slopes;
    vector<pair<Point, Point>> shortlist;
    for (auto iter = pairs.begin(); iter != pairs.end(); iter++)
    {
        //if selected edge is a vertical line, then push the lowest point into candidates
        if ((*iter).first.x == (*iter).second.x)
        {
            if ((*iter).first.y < (*iter).second.y)
                candidates.push_back((*iter).first);
            else
                candidates.push_back((*iter).second);
        }
        else
        {
            //calculate slopes and shortlist such pair of points (for which slopes are being calculated)
            pair<Point, Point> temp_edge = *iter;
            double k = calcSlope(temp_edge);
            shortlist.push_back(temp_edge);
            slopes.push_back(k);
        }
    }

    if (slopes.size() == 0)
        cout << "no slopes" << endl;

    double slopes_median;
    vector<double> slopes_copy = slopes;
    nth_element(slopes_copy.begin(), slopes_copy.begin() + slopes_copy.size() / 2, slopes_copy.end());
    double slopes_med_right = slopes_copy[slopes_copy.size() / 2];
    if (slopes_copy.size() % 2 == 1)
    {
        slopes_median = slopes_med_right;
    }
    else
    {
        nth_element(slopes_copy.begin(), slopes_copy.begin() + (slopes_copy.size() / 2) - 1, slopes_copy.end());
        double slopes_med_left = slopes_copy[(slopes_copy.size() / 2) - 1];
        slopes_median = (slopes_med_left + slopes_med_right) / 2;
    }

    //split edges in shortlist into small, equal, large based on their resp slope values
    vector<pair<Point, Point>> small, equal, large;
    for (int b = 0; b < shortlist.size(); b++)
    {
        if (slopes[b] < slopes_median)
            small.push_back(shortlist[b]);
        else if (slopes[b] > slopes_median)
            large.push_back(shortlist[b]);
        else
            equal.push_back(shortlist[b]);
    }

    //find a supporting line of S with slope = slopes_median, and the points that lie on this line
    vector<Point> min;
    Point p_k = T[0], p_m = T[0];
    double minimum = p_k.y - (slopes_median * p_k.x);
    min.push_back(p_k);
    for (auto iter = T.begin(); iter != T.end(); iter++)
    {
        Point test = *iter;
        double intercept = test.y - (slopes_median * test.x);
        if (!samePoint(test, p_k) && !samePoint(test, p_m) && double_equals(intercept, minimum))
        {
            min.push_back(test);
            if (test.x < p_k.x)
            {
                p_k = test;
            }
            else if (test.x > p_m.x)
            {
                p_m = test;
            }
        }
        else if (intercept < minimum)
        {
            minimum = intercept;
            min.clear();
            min.push_back(test);
            p_k = test;
            p_m = test;
        }
    }

    //if h contains the bridge
    if (p_k.x <= median && p_m.x > median)
    {
        return make_pair(p_k, p_m);
    }
    //h contains only points of S to the left of or on L
    if (p_m.x <= median)
    {
        //insert all the second points from pairs in small and equal into candidates
        for (auto iter = small.begin(); iter != small.end(); iter++)
            candidates.push_back((*iter).second);
        for (auto iter = equal.begin(); iter != equal.end(); iter++)
            candidates.push_back((*iter).second);
        //insert both points from pairs in large into candidates
        for (auto iter = large.begin(); iter != large.end(); iter++)
        {
            candidates.push_back((*iter).first);
            candidates.push_back((*iter).second);
        }
    }
    //h contains only points of S to the right of L
    if (p_k.x > median)
    {
        //insert all the second points from pairs in large and equal into candidates
        for (auto iter = large.begin(); iter != large.end(); iter++)
            candidates.push_back((*iter).first);
        for (auto iter = equal.begin(); iter != equal.end(); iter++)
            candidates.push_back((*iter).first);
        //insert both points from pairs in small into candidates
        for (auto iter = small.begin(); iter != small.end(); iter++)
        {
            candidates.push_back((*iter).first);
            candidates.push_back((*iter).second);
        }
    }

    return lowerBridge(candidates, median);
}

int orientation(Point a, Point b, Point c)
{
    long long int ornt = ((b.y - a.y) * (c.x - b.x)) - ((b.x - a.x) * (c.y - b.y));
    if (ornt == 0)
        return 0;

    ornt = (ornt > 0) ? 1 : 2;
    //0 --> collinear
    //1 --> ornt > 0, clockwise
    //2 --> ornt < 0, counter-clockwise
    return ornt;
}

vector<Point> upperHull(Point p_min, Point p_max, vector<Point> T)
{
    vector<Point> upper;
    //if there is only one point remaining, return it
    if (samePoint(p_min, p_max))
    {
        upper.push_back(p_min);
        return upper;
    }

    //find x_median
    double x_median;
    vector<long long int> temp;
    for (auto i = T.begin(); i != T.end(); i++)
        temp.push_back((*i).x);

    nth_element(temp.begin(), temp.begin() + temp.size() / 2, temp.end());
    int temp_med_right = temp[temp.size() / 2];
    if (temp.size() % 2 == 1)
    {
        x_median = temp_med_right;
    }
    else
    {
        nth_element(temp.begin(), temp.begin() + (temp.size() / 2) - 1, temp.end());
        int temp_med_left = temp[(temp.size() / 2) - 1];
        x_median = (temp_med_left + temp_med_right) / 2;
    }

    //split into T_left and T_right
    vector<Point> T_left, T_right;

    pair<Point, Point> u_bridge = upperBridge(T, x_median);
    Point p_l = u_bridge.first;
    Point p_r = u_bridge.second;

    // cout << "(" << p_l.x << ", " << p_l.y << ")"
    //      << " : "
    //      << "(" << p_r.x << ", " << p_r.y << ")" << endl;

    //T_left: all the points to the left of p_min and p_l
    T_left.push_back(p_l);
    if (p_l.x != p_min.x)
        T_left.push_back(p_min);

    for (auto i = T.begin(); i != T.end(); i++)
    {
        Point subject = (*i);
        if (orientation(p_min, p_l, subject) == 2)
            T_left.push_back(subject);
    }

    //T_right: all the points to the right of p_max and p_r
    T_right.push_back(p_r);
    if (p_r.x != p_max.x)
        T_right.push_back(p_max);

    for (auto i = T.begin(); i != T.end(); i++)
    {
        Point subject = (*i);
        if (orientation(p_max, p_r, subject) == 1)
            T_right.push_back(subject);
    }

    vector<Point> uh_left = upperHull(p_min, p_l, T_left);
    vector<Point> uh_right = upperHull(p_r, p_max, T_right);

    //concatenate vectors uh_left and uh_right and return
    uh_left.insert(uh_left.end(), uh_right.begin(), uh_right.end());
    return uh_left;
}

vector<Point> lowerHull(Point p_min, Point p_max, vector<Point> T)
{
    vector<Point> lower;
    //if there is only one point remaining, return it
    if (samePoint(p_min, p_max))
    {
        lower.push_back(p_min);
        return lower;
    }

    //find x_median
    double x_median;
    vector<long long int> temp;
    for (auto i = T.begin(); i != T.end(); i++)
        temp.push_back((*i).x);

    nth_element(temp.begin(), temp.begin() + temp.size() / 2, temp.end());
    int temp_med_right = temp[temp.size() / 2];
    if (temp.size() % 2 == 1)
    {
        x_median = temp_med_right;
    }
    else
    {
        nth_element(temp.begin(), temp.begin() + (temp.size() / 2) - 1, temp.end());
        int temp_med_left = temp[(temp.size() / 2) - 1];
        x_median = (temp_med_left + temp_med_right) / 2;
    }

    //split into T_left and T_right
    vector<Point> T_left, T_right;

    pair<Point, Point> l_bridge = lowerBridge(T, x_median);
    Point p_l = l_bridge.first;
    Point p_r = l_bridge.second;

    // cout << "(" << p_l.x << ", " << p_l.y << ")"
    //      << " : "
    //      << "(" << p_r.x << ", " << p_r.y << ")" << endl;

    //T_left: all the points to the left of p_min and p_l
    T_left.push_back(p_l);
    if (p_l.x != p_min.x)
        T_left.push_back(p_min);

    for (auto i = T.begin(); i != T.end(); i++)
    {
        Point subject = (*i);
        if (orientation(p_min, p_l, subject) == 1)
            T_left.push_back(subject);
    }

    //T_right: all the points to the right of p_max and p_r
    T_right.push_back(p_r);
    if (p_r.x != p_max.x)
        T_right.push_back(p_max);

    for (auto i = T.begin(); i != T.end(); i++)
    {
        Point subject = (*i);
        if (orientation(p_max, p_r, subject) == 2)
            T_right.push_back(subject);
    }

    vector<Point> lh_left = lowerHull(p_min, p_l, T_left);
    vector<Point> lh_right = lowerHull(p_r, p_max, T_right);

    //concatenate vectors uh_left and uh_right and return
    lh_left.insert(lh_left.end(), lh_right.begin(), lh_right.end());
    return lh_left;
}

void kirkPatrickSeidel(int n)
{
    //pmin and pmax to divide into upper and lower halves
    long long int x_min = LLONG_MAX, x_max = LLONG_MIN;
    Point p_lmin, p_lmax;
    Point p_umin, p_umax;
    for (auto i = begin(S); i != end(S); i++)
    {
        Point holder = *i;
        if (holder.x < x_min)
        {
            x_min = holder.x;
            p_lmin = holder;
            p_umin = holder;
        }
        //if there are two/more points with same x_min, then assign separate p_umin, p_lmin
        else if (holder.x == x_min)
        {
            if (holder.y < p_lmin.y)
                p_lmin = holder;
            else if (holder.y > p_umin.y)
                p_umin = holder;
        }
        if (holder.x > x_max)
        {
            x_max = holder.x;
            p_lmax = holder;
            p_umax = holder;
        }
        //if there are two/more points with same x_max, then assign separate p_umax, p_lmax
        else if (holder.x == x_max)
        {
            if (holder.y < p_lmax.y)
                p_lmax = holder;
            else if (holder.y > p_umax.y)
                p_umax = holder;
        }
    }

    vector<Point> T_upper, T_lower;
    T_lower.push_back(p_lmin);
    T_lower.push_back(p_lmax);
    T_upper.push_back(p_umin);
    T_upper.push_back(p_umax);
    for (int i = 0; i < n; i++)
    {
        //T_upper :=  {p_umin, p_umax} U {p € S|x(p_umin) < x(p) < x(p_umax)}
        if (orientation(p_umin, p_umax, S[i]) == 2)
            T_upper.push_back(S[i]);
        //T_lower :=  {p_lmin, p_lmax} U {p € S|x(p_lmin) < x(p) < x(p_lmax)}
        else if (orientation(p_lmin, p_lmax, S[i]) == 1)
            T_lower.push_back(S[i]);
    }

    vector<Point> upper = upperHull(p_umin, p_umax, T_upper);
    vector<Point> lower = lowerHull(p_lmin, p_lmax, T_lower);
    // concatenate upper and lower
    upper.insert(upper.end(), lower.begin(), lower.end());
    // printVector(upper);

    ofstream out_file;
    out_file.open("output.txt");

    for (auto iter = upper.begin(); iter != upper.end(); iter++)
    {
        out_file << (*iter).x << ", " << (*iter).y << endl;
    }

    out_file.close();
}

void saveTokenize(string str_line)
{
    Point newPoint;

    char line[1024];
    strcpy(line, str_line.c_str());

    char *token = strtok(line, " ");
    char *x = token;
    token = strtok(NULL, " ");
    char *y = token;

    string str_x(x);
    string str_y(y);

    newPoint.x = stoi(str_x);
    newPoint.y = stoi(str_y);

    S.push_back(newPoint);
}

int main()
{
    string line;
    ifstream inp_file("input1.txt");

    if (inp_file.is_open())
    {
        while (getline(inp_file, line))
        {
            saveTokenize(line);
        }
        inp_file.close();
    }
    else
    {
        cout << "err: Cannot open input file" << endl;
    }

    int n = S.size();
    kirkPatrickSeidel(n);

    return 0;
}