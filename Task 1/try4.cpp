
//#include <bits/stdc++.h>
#include <iostream>
#include<time.h>
#include<vector>

using namespace std;

const int sizen = 300;

long long costMatrixA[sizen][sizen];
long long costMatrixB[sizen][sizen];

long long productMat[sizen][sizen];
long long rec[sizen][sizen];
long long recB[sizen][sizen];//these 2 matrices keeps record and reduces no of recursive calls

//Simple recursion  which returns the minimum cost of going from i,j to n,n
long long FindMinCostA(int i, int j, int n)
{
    //going out of bounds
    if (i >= n)
        return 0;
    //going out of bounds
    if (j >= n)
        return 0;
    //reaching the last cell
    if (i == n - 1 && j == n - 1)
        return costMatrixA[i][j];
    //going down or right
    if(rec[i][j]!=-1)
    {
        return rec[i][j];
    }
    rec[i][j]=costMatrixA[i][j]+min(FindMinCostA(i+1,j,n),FindMinCostA(i,j+1,n));
    return rec[i][j];
}
//Simple recursion which returns the maximum cost of going from i,j to n,n
long long FindMaxCostB(int i, int j, int n)
{
    //going out of bounds
    if (i >= n)
        return 0;
    //going out of bounds
    if (j >= n)
        return 0;
    //reaching the last cell
    if (i == n - 1 && j == n - 1)
        return costMatrixB[i][j];
    //going down or right
    if(recB[i][j]!=-1)
    {
        return recB[i][j];
    }
    recB[i][j] = costMatrixB[i][j] + max(FindMaxCostB(i + 1, j, n), FindMaxCostB(i, j + 1, n));
    return recB[i][j];
}

int main()
{
    int i, j, k;
    srand(time(0));
    // initialisation
    for (i = 0; i < sizen; i++)
    {
        for (j = 0; j < sizen; j++)
        {
            costMatrixA[i][j] = 1 + rand() % 10;
            costMatrixB[i][j] = 1 + rand() % 10;
            productMat[i][j] = 0;
            rec[i][j]=-1;
            recB[i][j]=-1;
        }
    }
    //creating productMat as explained in the beginning
    long long recBT[sizen][sizen];
    for(i=0;i<sizen;i++)
    {
        for(j=0;j<sizen;j++)
        {
            recBT[j][i]=FindMaxCostB(i,j,sizen);
            rec[i][j]=FindMinCostA(i,j,sizen);
        }
    }// it is the transpose of the matrix of actual maxcost matrix


    for (i = 0; i < sizen; i++)
    {
        for (j = 0; j < sizen; j++)
        {
            for (k = 0; k < sizen; k++)
                productMat[i][j] += rec[i][k] * recBT[j][k];
        }
    }
    //filter of size 4 x n
    long long filterArray[4][sizen];
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < sizen; j++)
            filterArray[i][j] = rand() % 2;
    }
    // matrix of dimension (sizen/c) x 1 where c = 4
    vector<long long> finalMat(sizen / 4,0);
    // applying the filter
    // for (i = 0; i < sizen - 4; i += 4)
    // {
    //     long long sum = 0;
    //     // dot product of 4xn portion of productMat
    //     for (j = 0; j < sizen; j++)
    //     {
    //         for (int filterRow = 0; filterRow < 4; filterRow++)
    //         {
    //             sum += productMat[i + filterRow][j];
    //         }
    //     }
    //     finalMat[i / 4] = sum;
    // }

    for(i=0;i<sizen;i++)
    {
    
        for(j=0;j<sizen;j++)
        {
            finalMat[i/4]+=productMat[i][j]*filterArray[i%4][j];
        }
    }
    cout<<"complete";

    return 0;
}