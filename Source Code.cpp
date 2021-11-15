#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string>
#define PI 2*acos(0.0)

using namespace cv;
using namespace std;

int getMin(vector<int> arr)
{
    int N = arr.size();
    int minInd = 0;
    for (int i = 1; i < N; i++)
        if (arr[i] < arr[minInd])
            minInd = i;
    return minInd;
}

int getMax(vector<int> arr)
{
    int N = arr.size();
    int maxInd = 0;
    for (int i = 1; i < N; i++)
        if (arr[i] > arr[maxInd])
            maxInd = i;
    return maxInd;
}

int minOf2(int x, int y)
{
    return (x < y) ? x : y;
}

void minCashFlowRec(vector<int> amount, vector<vector<int>> &solution)
{
    int mxCredit = getMax(amount), mxDebit = getMin(amount);

    if (amount[mxCredit] == 0 && amount[mxDebit] == 0)
        return;

    int min = minOf2(-amount[mxDebit], amount[mxCredit]);
    amount[mxCredit] -= min;
    amount[mxDebit] += min;
    solution[mxDebit][mxCredit] = min;

    minCashFlowRec(amount, solution);
}

int lineof(int y1, int y0, int x1, int x0, int x)
{
    int ans = (((y1 - y0) / (x1 - x0)) * (x - x0)) + y0;
    return ans;
}

int S1(double x0, double y0, double x, double y, double r)
{
    double s = ((x - x0) * (x - x0)) + ((y - y0) * (y - y0)) - (r * r);
    if (s < 0)
    {
        return 1;
    }
    return 0;
}

void PlotInput(vector<vector<int>>& graph)
{
    int N = graph.size();
    vector<vector<double>> coordinates;
    auto BackGroundColour = Scalar(255, 255, 255);
    Mat _img(1000, 1300, CV_8UC3, BackGroundColour);
    int boundaryRadius = 250;
    int shiftX = 650, shiftY = 350;
    double TWO_PI = (double)2 * PI;
    int d = (((double)20 / TWO_PI)*atan(N)) + 5;
    int radius = ((TWO_PI * boundaryRadius) / N) - d;
    radius /= 4;
    int i = 1;
    double fontSize = ((double)2.9 * radius) / 76, theta = 0.0;
    for (int k = 0;k < N; k++)
    {
        int x = (double)boundaryRadius*cos(theta);
        int y = (double)boundaryRadius*sin(theta);
        x += shiftX;
        y += shiftY;
        vector<double> v;
        v.push_back((double)x);
        v.push_back((double)y);
        v.push_back(theta);
        coordinates.push_back(v);
        circle(_img, Point(x, y),radius , Scalar(0, 0, 0));
        string p = to_string(i);
        i++;
        putText(_img, p, Point(x - (radius/1.5), y + radius/4), FONT_HERSHEY_SIMPLEX, fontSize, Scalar(0, 69, 255), 1);
        theta += TWO_PI / N;
    }
    double x0, y0, x1, y1, x2, y2, x3, y3, D;
    double angle;
    vector<int> arr(N, 0);
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (graph[i][j] != 0)
            {
                x0 = coordinates[i][0];
                y0 = coordinates[i][1];

                x1 = coordinates[j][0];
                y1 = coordinates[j][1];
                D = sqrt(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)));
                x2 = (((D- radius)*(x0)) + (radius*x1))/D;
                y2 = (((D - radius) * (y0))+(radius * y1)) / D;

                x3 = (((D - radius) * (x1)) + (radius * x0)) / D;
                y3 = (((D - radius) * (y1)) + (radius * y0)) / D;
                
                angle = atan((y1 - y0) / (x1 - x0));

                double X0, Y0, X1, Y1;
                double signX = 1, signY = 1;
                double R = (double)sqrt(((x0 - x3) * (x0 - x3)) + ((y3 - y0) * (y3 - y0)));
                bool flag = false;
                for (int i = 0; i < 2; i++)
                {
                    signX = 1 - signX;
                    if (signX == 0)
                    {
                        X0 = x3 + (radius / 3) * (cos(angle + ((double)30 * PI) / 180));
                        X1 = x3 + (radius / 3) * cos(angle - ((double)30 * PI) / 180);
                        Y0 = y3 + (radius / 3) * (sin(angle + ((double)30 * PI) / 180));
                        Y1 = y3 + (radius / 3) * sin(angle - ((double)30 * PI) / 180);
                    }
                    else
                    {
                        X0 = x3 - (radius / 3) * (cos(angle + ((double)30 * PI) / 180));
                        X1 = x3 - (radius / 3) * cos(angle - ((double)30 * PI) / 180);
                        Y0 = y3 - (radius / 3) * (sin(angle + ((double)30 * PI) / 180));
                        Y1 = y3 - (radius / 3) * sin(angle - ((double)30 * PI) / 180);
                    }
                    if (S1(x0, y0, X0, Y0, R) && S1(x0, y0, X1, Y1, R))
                    {
                        break;
                    }
                }

                line(_img, Point(x2, y2), Point(x3, y3), Scalar(200, 0, 10), 2);
                line(_img, Point(x3, y3), Point(X0, Y0), Scalar(200, 0, 10), 2);
                line(_img, Point(x3, y3), Point(X1, Y1), Scalar(200, 0, 10), 2);

                string payment = "Pay ";
                payment += to_string(graph[i][j]);
                payment += " to ";
                payment += to_string(j + 1);
                arr[i] = arr[i] + (radius / 2.5);
                putText(_img, payment, Point(x0 - (radius / 2),y0 + radius + arr[i]), FONT_HERSHEY_SIMPLEX, fontSize/4, Scalar(0, 0, 0), 1.8);
            }
        }
    }
    imshow("Image", _img);
    waitKey(0);
}

void minCashFlow(vector<vector<int>> graph, vector<vector<int>>& solution)
{
    int N = graph.size();
    vector<int> amount(N, 0);

    for (int p = 0; p < N; p++)
        for (int i = 0; i < N; i++)
            amount[p] += (graph[i][p] - graph[p][i]);

    minCashFlowRec(amount, solution);
}

int main()
{
    int N1;
    cout << "Enter number of persons: ";
    cin >> N1;
    cout << "List of Persons: \n";
    for (int i = 0; i < N1; i++)
    {
        cout << "Person: " << i + 1 << "\n";
    }
    int m;
    cout << "\n" << "Enter number of transactions: ";
    cin >> m;

    vector<vector<int>> graph(N1, vector<int>(N1, 0));
    cout << "Enter transactions like \"x y m\" where Person (x) pays Person (y) an amount of \"m\"\n";
    int x, y, z;
    for (int i = 0; i < m; i++)
    {
        cout << "Transaction number " << i + 1 << ": ";
        cin >> x >> y >> z;
        graph[x - 1][y - 1] = z;
    }
    cout << "\n";
    PlotInput(graph);
    vector<vector<int>> solution(N1, vector<int>(N1, 0));
    minCashFlow(graph, solution);
    PlotInput(solution);
    return 0;
}