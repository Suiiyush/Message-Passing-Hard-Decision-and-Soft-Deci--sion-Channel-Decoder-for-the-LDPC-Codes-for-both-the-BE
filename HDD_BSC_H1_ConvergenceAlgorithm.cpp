#include<bits/stdc++.h>
using namespace std;

// BSC channel


// Degree of CN is 4
// CN container
struct CN
{
	int v[4][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};         // In first column I'll store which number of VN is connected (from 1 to 5056) and in second column the values of that VN in message passing from VN to CN 
}arr_CN[3792];


// Degree of VN is 3
// VN container
struct VN
{
	int c[3][2]={{-1,-1},{-1,-1},{-1,-1}};                // In first column I'll store which number of CN is connected (from 1 to 3792) and in second column the values of that CN in message passing from CN to VN     
	int value;                                            // Store the value of the transmitted signal after adding noise in it
	int decision;                                         // After tanner graph decoding it makes final decision that VN i is 1 or 0
}arr_VN[5056];

int main()
{
	int N=5056,U=3792,i,j,k,count,c1,c2,c3,v1,v2,v3,v4,terminate=1,Nsim=1000,Ksim;         // N is number of columns and U is number of rows of H - matrix
	int arr[3],convergence[100]={0};
	
	float p=0.18,r;      // Crossover Probability
	
	
	int **H = new int *[U];        // Read H matrix from .txt file

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
	int noise[N]={0};                                     // Noise
	int rx[N]={0};                                        // Received Signal
	
	// Loading VNs with values 0 or 1
	
	for(i=0;i<N;i++)
	{
		r=((float) rand() / (RAND_MAX + 1));              // Generation of a random number between 0 to 1
		
		if(r>(1-p))
		noise[i]=1;
		else
		noise[i]=0;
		
		if(noise[i]==tr[i])                               // Same as XOR operation (if r > 1-p then, bit will be flipped)
		rx[i]=0;
		else
		{
		    rx[i]=1;
		    ++convergence[0];
		}
		
		arr_VN[i].value=rx[i];
		
		for(j=0;j<3;j++)
		arr_VN[i].c[j][1]=rx[i];
	}
	
	
	// Tanner Graph decoding 
	
	terminate=0;
	
	// terminate variable is used for 100 iterations breaking conditions
	
	while(terminate<100)              
	{
	
	// VN sends massege to CN (first iteration and next iterations)
	
	for(i=0;i<N;i++)
	{
		c1=arr_VN[i].c[0][0];
		c2=arr_VN[i].c[1][0];
		c3=arr_VN[i].c[2][0];
		
		for(j=0;j<4;j++)
		{
			if(arr_CN[c1-1].v[j][0]==i+1)
		    arr_CN[c1-1].v[j][1]=arr_VN[i].c[0][1];
		}
		for(j=0;j<4;j++)
		{
			if(arr_CN[c2-1].v[j][0]==i+1)
		    arr_CN[c2-1].v[j][1]=arr_VN[i].c[1][1];
		}
		for(j=0;j<4;j++)
		{
			if(arr_CN[c3-1].v[j][0]==i+1)
		    arr_CN[c3-1].v[j][1]=arr_VN[i].c[2][1];
		}
	}
	
	
	// CN sends VN

	
	for(i=0;i<U;i++)
	{
		
		v1=arr_CN[i].v[0][0];
		v2=arr_CN[i].v[1][0];
		v3=arr_CN[i].v[2][0];
		v4=arr_CN[i].v[3][0];
		
		
		// CN sends message to each VN connected to it - the modulo two sum of all other VNs
			
	    for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v1-1].c[j][0]==i+1)
	    	{
	    		arr_VN[v1-1].c[j][1]=(arr_CN[i].v[1][1] + arr_CN[i].v[2][1] + arr_CN[i].v[3][1])%2;
	    		break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v2-1].c[j][0]==i+1)
	    	{
	    		arr_VN[v2-1].c[j][1]=(arr_CN[i].v[0][1] + arr_CN[i].v[2][1] + arr_CN[i].v[3][1])%2;
	    		break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v3-1].c[j][0]==i+1)
	    	{
	    		arr_VN[v3-1].c[j][1]=(arr_CN[i].v[0][1] + arr_CN[i].v[1][1] + arr_CN[i].v[3][1])%2;
	    		break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v4-1].c[j][0]==i+1)
	    	{
	    		arr_VN[v4-1].c[j][1]=(arr_CN[i].v[0][1] + arr_CN[i].v[1][1] + arr_CN[i].v[2][1])%2;
	    		break;
			}
		}
	}
	
	
	// Decision Time For VN i to CN j (in this it is checking values other than CN j)
	
	if(terminate!=99)
	{
	for(i=0;i<N;i++)
	{
		for(j=0;j<3;j++)
		arr[j]=arr_VN[i].c[j][1];
		
		for(j=0;j<3;j++)
		{
		    count=k=0;
		
			if(arr[(j+1)%3]==0)
			++count;
			else
			++k;
			
			if(arr[(j+2)%3]==0)
			++count;
			else
			++k;
			
			if(arr_VN[i].value==0)
			++count;
			else
			++k;
			
			if(count>k)
			arr_VN[i].c[j][1]=0;
			else
			arr_VN[i].c[j][1]=1;
		}
		
		count=0;
		for(j=0;j<3;j++)
		if(arr[j]==1)
		++count;
		
		if(arr_VN[i].value==1)
		++count;
		
		if(count>=3)
		++convergence[terminate+1];
	}
    }
	
	    ++terminate;
    }
    
    // Final decision for VN i that it is 1 or 0
    
    for(i=0;i<N;i++)
    {
		count=k=0;
		
		for(j=0;j<3;j++)
		{	
			if(arr_VN[i].c[j][1]==0)
			++count;
			else
			++k;
		}
		
		if(arr_VN[i].value==0)
		++count;
		else
		++k;
		
		if(count>=k)
		arr_VN[i].decision=0;
		else
		arr_VN[i].decision=1;
	}
    }
	
	for(i=0;i<100;i++)
	cout<<convergence[i]*(1.0)/Nsim<<" ";
	
}