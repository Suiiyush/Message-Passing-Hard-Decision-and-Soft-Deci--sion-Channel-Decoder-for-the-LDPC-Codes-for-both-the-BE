#include<bits/stdc++.h>
using namespace std;


// BEC Hard Decision Decoding Scheme for 3792 x 5056 H - matrix

// Degree of CN is 4
// CN container
struct CN
{
	int v[4][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};       // In first column I'll store which number of VN is connected (from 1 to 5056) and in second column the values of that VN in message passing from VN to CN 
}arr_CN[3792];


// Degree of VN is 3
// VN container
struct VN
{
	int c[3][2]={{-1,-1},{-1,-1},{-1,-1}};              // In first column I'll store which number of CN is connected (from 1 to 3792) and in second column the values of that CN in message passing from CN to VN
	int value;                                          // Store the value of the transmitted signal after adding noise in it
}arr_VN[5056];

int main()
{
	int N=5056,U=3792,i,j,k,count,c1,c2,c3,Nsim=1000,Ksim,stop;             // N is number of columns and U is number of rows of H - matrix
	int Ncorr[101]={0},Nerr[101]={0},convergence[100]={0};
	
	float p=0.7,r;      // Crossover Probability
	
	
	int **H = new int *[U];                                       // Read H matrix from .txt file

    for (int i = 0; i < U; i++)
        H[i] = new int[N];


    ifstream fin;
    fin.open("Hmatrix.txt");


    if (!fin)
    {
        cout << "Cannot open the file" << endl;
        exit(0);
    }

    int inRow = 0, inCol = 0;


    char data;
    while (!fin.eof()) // Here I want to fill the H matrix with values given in text file,
                       //keeping in mind the size of each row and column of H
    {
        fin >> data;

        if (data != ',')
        {
            if (inCol == N)
            {

                inCol = 0;
                inRow++;
            }

            H[inRow][inCol] = data - 48;
            inCol++;

            if(inRow == U-1 && inCol == N )
                break;


        }
    }
    fin.close();
	              
	              
	// Connection of CNs with VNs
	              
	for(i=0;i<U;i++)
	{
		count=0;
		for(j=0;j<N;j++)
		{
			if(H[i][j]==1)
			{
				arr_CN[i].v[count][0]=j+1;
				++count;
			}
		}
	}
	
	// Connection of VNs with CNs
	
	for(j=0;j<N;j++)
	{
		count=0;
		arr_VN[i].value=-1;
		for(i=0;i<U;i++)
		{
			if(H[i][j]==1)
			{
				arr_VN[j].c[count][0]=i+1;
				++count;
			}
		}
	}
	
	// Tanner Graph Decoding With Monte Carlo Simulations
	
	srand (time(NULL));
		
	for(Ksim=1;Ksim<=Nsim;Ksim++)                         // Loop for Monte - Carlo simulations
	{
	
	int tr[N]={0};                                        // Transmitted Signal
	int noise[N]={0};                                        // Received Signal
	
	// Loading VNs with values 0 or erasure (-1)
	
	
	for(i=0;i<N;i++)
	{
		r=((float) rand() / (RAND_MAX + 1));                // Generating a random number between 0 to 1
		
		if(r>p)
		noise[i]=tr[i];
		else
		noise[i]=-1;                                        // If generated number is less than or equal to crossover probability p then, it is erasure
		
		arr_VN[i].value=noise[i];
		
		if(arr_VN[i].value==-1)
		++convergence[0];
	}
	
	
	// Tanner Graph decoding
	
	stop=0;
	
	// stop is used for 100 iterations condition
	
	while(stop<100)
	{
	
	// VN sends massege to CN (first iteration and next iterations)
	
	for(i=0;i<N;i++)
	{
		k=arr_VN[i].value;
		c1=arr_VN[i].c[0][0];
		c2=arr_VN[i].c[1][0];
		c3=arr_VN[i].c[2][0];
		
		for(j=0;j<4;j++)
		{
			if(arr_CN[c1-1].v[j][0]==i+1)
		    arr_CN[c1-1].v[j][1]=k;
		}
		for(j=0;j<4;j++)
		{
			if(arr_CN[c2-1].v[j][0]==i+1)
		    arr_CN[c2-1].v[j][1]=k;
		}
		for(j=0;j<4;j++)
		{
			if(arr_CN[c3-1].v[j][0]==i+1)
		    arr_CN[c3-1].v[j][1]=k;
		}
	}
	
	
	// CN sends VN
	
	
	for(i=0;i<U;i++)
	{
		count=0;
		
		for(j=0;j<4;j++)
		{
			if(arr_CN[i].v[j][1]==-1)               // Count the number of erasures out of 4
			++count;
		}
		
		if(count==1)                                // If there is only one VN has value erasure then it's value will change but if there are two or more erasures then their values will not change and so we can skip that case
		{
			if(arr_CN[i].v[0][1]==-1)
			{
				k=arr_CN[i].v[0][0];
				arr_VN[k-1].value=(arr_CN[i].v[1][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				if(arr_VN[k-1].c[0][0]==i+1)
				arr_VN[k-1].c[0][1]=(arr_CN[i].v[1][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[1][0]==i+1)
				arr_VN[k-1].c[1][1]=(arr_CN[i].v[1][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[2][0]==i+1)
				arr_VN[k-1].c[2][1]=(arr_CN[i].v[1][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
			}
			else if(arr_CN[i].v[1][1]==-1)
			{
				k=arr_CN[i].v[1][0];
				arr_VN[k-1].value=(arr_CN[i].v[0][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				if(arr_VN[k-1].c[0][0]==i+1)
				arr_VN[k-1].c[0][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[1][0]==i+1)
				arr_VN[k-1].c[1][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[2][0]==i+1)
				arr_VN[k-1].c[2][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[2][1]+arr_CN[i].v[3][1])%2;
			}
			else if(arr_CN[i].v[2][1]==-1)
			{
				k=arr_CN[i].v[2][0];
				arr_VN[k-1].value=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[3][1])%2;
				
				if(arr_VN[k-1].c[0][0]==i+1)
				arr_VN[k-1].c[0][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[1][0]==i+1)
				arr_VN[k-1].c[1][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[3][1])%2;
				
				else if(arr_VN[k-1].c[2][0]==i+1)
				arr_VN[k-1].c[2][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[3][1])%2;
			}
			else
			{
				k=arr_CN[i].v[3][0];
				arr_VN[k-1].value=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[2][1])%2;
				
				if(arr_VN[k-1].c[0][0]==i+1)
				arr_VN[k-1].c[0][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[2][1])%2;
				
				else if(arr_VN[k-1].c[1][0]==i+1)
				arr_VN[k-1].c[1][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[2][1])%2;
				
				else if(arr_VN[k-1].c[2][0]==i+1)
				arr_VN[k-1].c[2][1]=(arr_CN[i].v[0][1]+arr_CN[i].v[1][1]+arr_CN[i].v[2][1])%2;
			}
		}
	}
	
	
	for(i=0;i<N;i++)
	if(arr_VN[i].value==-1)
	++convergence[stop+1];
	
	  ++stop;
    }
    }
	
	for(i=0;i<100;i++)
	cout<<convergence[i]*(1.0)/Nsim<<" ";
	
}