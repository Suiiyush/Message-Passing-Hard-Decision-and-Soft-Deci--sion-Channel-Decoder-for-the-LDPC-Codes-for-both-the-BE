#include<bits/stdc++.h>
using namespace std;

// Soft Decision Decoding For BEC Channel (for 3000 x 5000 H - matrix)


// Degree of CN is 5
// CN container
struct CN
{
	float v[5][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};     // In first column I'll store which number of VN is connected (from 1 to 5000) and in second column the values of that VN in message passing from VN to CN
}arr_CN[3000];


// Degree of VN is 3
// VN container
struct VN
{
	float c[3][2]={{-1,-1},{-1,-1},{-1,-1}};  // In first column I'll store which number of CN is connected (from 1 to 3000) and in second column the values of that CN in message passing from CN to VN     
	float value;                              // Store the conditional probability of transmitted signal after adding the noise that the value of corresponding VN is 1
	float decision;                           // At last it makes decision for it's final value
}arr_VN[5000];

int main()
{
	int N=5000,U=3000,i,j,count,c1,c2,c3,v1,v2,v3,v4,v5,flag,stop,terminate,Nsim=1000,Ksim,l;
	int Ncorr[101]={0},Nerr[101]={0};
	
	float p[101],r,k,alpha,bita,VN_is_1,VN_is_0,VN_decision_1,VN_decision_0,arr[3];      // Crossover Probability
	
	p[0]=0;
	
	for(i=1;i<101;i++)
	{
		p[i]=p[i-1]+0.01;
	}

	int **H = new int *[U];        // Read H matrix from .txt file

    for (int i = 0; i < U; i++)
        H[i] = new int[N];


    ifstream fin;
    fin.open("Hmatrix2.txt");


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
	for(l=0;l<101;l++)                                    // Outer loop for crossover probability
	{
		
	for(Ksim=1;Ksim<=Nsim;Ksim++)                         // Loop for Monte - Carlo simulations
	{
	
	int tr[N]={0};                                        // Transmitted Signal
	int noise[N]={0};                                     // Noise ( or received signal contains bits)

	
	// Loading VNs with conditional probabilities that VN is 1 given received bit 
	
	for(i=0;i<N;i++)
	{
		r=((float) rand() / (RAND_MAX + 1));              // Generation of a random number between 0 to 1
		
		if(r>p[l])
		noise[i]=tr[i];
		else
		noise[i]=-1;                                      // If r <= p then, the received bit will be flipped
		
		// Conditional Probability that VN i is 1 given received bit
		
		if(noise[i]==0)                                  // If received bit is 0 then it is 0 
		{
			arr_VN[i].value=0;
			
			for(j=0;j<3;j++)
			arr_VN[i].c[j][1]=0;
		}
		else                                             // If received bit is erasure then it is 0.5
		{
			arr_VN[i].value=0.5;
			
			for(j=0;j<3;j++)
			arr_VN[i].c[j][1]=0.5;
		}
	}
	
	
	// Tanner Graph decoding
	
	terminate=0;
	flag=1;
	stop=0;
	
	// terminate is used for 100 iterations breaking codition
	
	while(terminate<100 && flag!=0 && stop!=U)
	{
	
	// VN sends massege to CN (first iteration and next iterations)
	
	for(i=0;i<N;i++)
	{
		c1=arr_VN[i].c[0][0];
		c2=arr_VN[i].c[1][0];
		c3=arr_VN[i].c[2][0];
		
		for(j=0;j<5;j++)
		{
			if(arr_CN[c1-1].v[j][0]==i+1)
		    arr_CN[c1-1].v[j][1]=arr_VN[i].c[0][1];
		}
		for(j=0;j<5;j++)
		{
			if(arr_CN[c2-1].v[j][0]==i+1)
		    arr_CN[c2-1].v[j][1]=arr_VN[i].c[1][1];
		}
		for(j=0;j<5;j++)
		{
			if(arr_CN[c3-1].v[j][0]==i+1)
		    arr_CN[c3-1].v[j][1]=arr_VN[i].c[2][1];
		}
	}
	
	
	// CN sends VN
	
	flag=stop=0;
	
	for(i=0;i<U;i++)
	{
		v1=arr_CN[i].v[0][0];
		v2=arr_CN[i].v[1][0];
		v3=arr_CN[i].v[2][0];
		v4=arr_CN[i].v[3][0];
		v5=arr_CN[i].v[4][0];
		
		count=0;
		
		for(j=0;j<5;j++)
		{
			if(arr_CN[i].v[j][1]==0.5)            // Count the number of erasures (or p==0.5) out of 5
			++count;
		}
		
		if(count==1)                              // If there is only one VN has value erasure then it's value will change but if there are two or more erasures then their values will not change and so we can skip that case
		{
		if(arr_CN[i].v[0][1]==0.5)
		{	
	    for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v1-1].c[j][0]==i+1)
	    	{
	    	    VN_is_1=(arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    VN_is_0=((1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    bita=1/(VN_is_1 + VN_is_0);
	    	    
	    	    arr_VN[v1-1].c[j][1]=bita*VN_is_1;
	    	    
	    		break;
			}
		}
	    }
	    
	    if(arr_CN[i].v[1][1]==0.5)
	    {
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v2-1].c[j][0]==i+1)
	    	{
	    		VN_is_1=(arr_CN[i].v[0][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    VN_is_0=((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    bita=1/(VN_is_1 + VN_is_0);
	    	    
	    	    arr_VN[v2-1].c[j][1]=bita*VN_is_1;
	    	    
	    		break;
			}
		}
	    }
		
		if(arr_CN[i].v[2][1]==0.5)
		{
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v3-1].c[j][0]==i+1)
	    	{
	    	    VN_is_1=(arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    VN_is_0=((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[3][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[3][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[3][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[3][1]*arr_CN[i].v[4][1]);
	    	    
	    	    bita=1/(VN_is_1 + VN_is_0);
	    	    
	    	    arr_VN[v3-1].c[j][1]=bita*VN_is_1;
	    	    
	    		break;
			}
		}
	    }
		
		if(arr_CN[i].v[3][1]==0.5)
		{
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v4-1].c[j][0]==i+1)
	    	{
	    		VN_is_1=(arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[4][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[4][1]) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[4][1]);
	    	    
	    	    VN_is_0=((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[4][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[4][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[4][1]);
	    	    
	    	    bita=1/(VN_is_1 + VN_is_0);
	    	    
	    	    arr_VN[v4-1].c[j][1]=bita*VN_is_1;
	    	    
	    		break;
			}
		}
	    }
	    
	    if(arr_CN[i].v[4][1]==0.5)
		{
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v5-1].c[j][0]==i+1)
	    	{
	    		VN_is_1=(arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]);
	    	    
	    	    VN_is_0=((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*(1-arr_CN[i].v[3][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])) + (arr_CN[i].v[0][1]*(1-arr_CN[i].v[1][1])*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]) + ((1-arr_CN[i].v[0][1])*(1-arr_CN[i].v[1][1])*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*(1-arr_CN[i].v[2][1])*arr_CN[i].v[3][1]) + ((1-arr_CN[i].v[0][1])*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*(1-arr_CN[i].v[3][1])) + (arr_CN[i].v[0][1]*arr_CN[i].v[1][1]*arr_CN[i].v[2][1]*arr_CN[i].v[3][1]);
	    	    
	    	    bita=1/(VN_is_1 + VN_is_0);
	    	    
	    	    arr_VN[v5-1].c[j][1]=bita*VN_is_1;
	    	    
	    		break;
			}
		}
	    }
	    
	    flag=1;
	    
	    // Here, if any VN out of all which are erasures don't change it's value after t th iteration then, flag will be 0 and this is the second condition for breaking the Tanner Graph loop
	    }
	    else if(count==0)
		++stop;
		
		// If out of 5 VNs connected with a single CN there is not any erasure then, value of stop will increase and if stop's value reach to U (means equals to number of CNs), it means that in all VNs there is not any erasure and this is the third breaking condition
	}
	
	// Computing the conditional probability that VN i is 1
	
	for(i=0;i<N;i++)
	{
		for(j=0;j<3;j++)
		arr[j]=arr_VN[i].c[j][1];
		
		for(j=0;j<3;j++)
		{
		    VN_decision_1=arr_VN[i].value*arr[(j+1)%3]*arr[(j+2)%3];
		    
		    VN_decision_0=(1-arr_VN[i].value)*(1-arr[(j+1)%3])*(1-arr[(j+2)%3]);
		    
		    alpha=1/(VN_decision_1 + VN_decision_0);
		    
		    arr_VN[i].c[j][1]=alpha*VN_decision_1;
		}
	}
	
	    ++terminate;
    }
    
    // Computing likelihood ratios or determine VN i is 1 or 0
    
    flag=1;
    
    for(i=0;i<N;i++)
    {
    	count=k=0;
    	
    	arr_VN[i].decision=(arr_VN[i].value/(1-arr_VN[i].value))*(arr_VN[i].c[0][1]/(1-arr_VN[i].c[0][1]))*(arr_VN[i].c[1][1]/(1-arr_VN[i].c[1][1]))*(arr_VN[i].c[2][1]/(1-arr_VN[i].c[2][1]));
    	
    	if(arr_VN[i].decision>=1)
    	arr_VN[i].decision=1;
    	else
    	arr_VN[i].decision=0;
    	
		
		// Determine after decoding signal has still any error or not
		
		if(arr_VN[i].decision!=tr[i])
    	{
    		flag=0;
    		break;
		}
	}
	
	// Increase Nerr if there is any error otherwise that is successfull decoding
	
	if(flag==1)
	++Ncorr[l];
	else
	++Nerr[l];
    
    }
    }
	
	for(i=0;i<101;i++)
	//cout<<"p = "<<p[i]<<" "<<"Ncorr = "<<Ncorr[i]<<" Nerr = "<<Nerr[i]<<" Ncorr/Nsim = "<<(Ncorr[i]*1.0)/Nsim<<endl<<endl;
	cout<<(Ncorr[i]*1.0)/Nsim<<" ";
	
}