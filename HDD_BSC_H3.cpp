#include<bits/stdc++.h>
using namespace std;

// BSC channel


// Degree of CN is 4
// CN container
struct CN
{
	int v[4][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};         // In first column I'll store which number of VN is connected (from 1 to 12) and in second column the values of that VN in message passing from VN to CN 
}arr_CN[9];


// Degree of VN is 3
//VN container
struct VN
{
	int c[3][2]={{-1,-1},{-1,-1},{-1,-1}};                // In first column I'll store which number of CN is connected (from 1 to 9) and in second column the values of that CN in message passing from CN to VN     
	int value;                                            // Store the value of the transmitted signal after adding noise in it
	int decision;                                         // After tanner graph decoding it makes final decision that VN i is 1 or 0
}arr_VN[12];

int main()
{
	int N=12,U=9,i,j,k,count,c1,c2,c3,v1,v2,v3,v4,flag=1,terminate=1,Nsim=10000,Ksim,l,stop;       // N is number of columns and U is number of rows of H - matrix
	int Ncorr[81]={0},Nerr[81]={0},arr[3];
	
	float p[81],r;      // Crossover Probability
	
	p[0]=0;
	
	for(i=1;i<81;i++)
	{
		p[i]=p[i-1]+0.01;
	}
	
	int H[U][N]= {{1,0,0,0,0,1,0,1,0,1,0,0},                   // 9 x 12 H - matrix
	              {1,0,0,1,1,0,0,0,0,0,1,0},
                  {0,1,0,0,1,0,1,0,1,0,0,0},
	              {0,0,1,0,0,1,0,0,0,0,1,1},
	              {0,0,1,0,0,0,1,1,0,0,0,1},
	              {0,1,0,0,1,0,0,0,1,0,1,0},
	              {1,0,0,1,0,0,1,0,0,1,0,0},
	              {0,1,0,0,0,1,0,1,0,1,0,0},
	              {0,0,1,1,0,0,0,0,1,0,0,1}};
	              
	              
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
	for(l=0;l<81;l++)                                     // Outer for loop for crossover probability p 
	{
		
	for(Ksim=1;Ksim<=Nsim;Ksim++)                         // Loop for Monte - Carlo simulations
	{
	
	int tr[N]={0};                                        // Transmitted Signal
	int noise[N]={0};                                     // Noise
	int rx[N]={0};                                        // Received Signal
	
	// Loading VNs with values 0 or 1
	
	for(i=0;i<N;i++)
	{
		r=((float) rand() / (RAND_MAX + 1));              // Generation of a random number between 0 to 1
		
		if(r>(1-p[l]))
		noise[i]=1;
		else
		noise[i]=0;
		
		if(noise[i]==tr[i])                               // Same as XOR operation (if r > 1-p then, bit will be flipped)
		rx[i]=0;
		else
		rx[i]=1;
		
		arr_VN[i].value=rx[i];
		
		for(j=0;j<3;j++)
		arr_VN[i].c[j][1]=rx[i];
	}
	
	
	// Tanner Graph decoding 
	
	flag=terminate=0;
	
	// terminate variable is used for 100 iterations breaking condition
	
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
	
	stop=0;                     // This is for checking even parity of CNs, if all CNs have even parity then that will also be the breaking condition
	
	for(i=0;i<U;i++)
	{
		
		v1=arr_CN[i].v[0][0];
		v2=arr_CN[i].v[1][0];
		v3=arr_CN[i].v[2][0];
		v4=arr_CN[i].v[3][0];
			
			
		if((arr_CN[i].v[0][1] + arr_CN[i].v[1][1] + arr_CN[i].v[2][1] + arr_CN[i].v[3][1])%2==0)    // Every CN is checking for even parity
		++stop;
		
		
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
	
	
	if(stop==9)    // Because all CNs have even parity and therefore no meaning to decode again
	break;
	
	// Decision Time For VN i to CN j  (in this it is checking values other than CN j)
	
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
    
    // Determine after decoding signal has stil any error or not
    
    flag=1;
    
    for(i=0;i<N;i++)
    {
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
	
	for(i=0;i<81;i++)
	//cout<<"p = "<<p[i]<<" "<<"Ncorr = "<<Ncorr[i]<<" Nerr = "<<Nerr[i]<<" Ncorr/Nsim = "<<(Ncorr[i]*1.0)/Nsim<<endl<<endl;
	cout<<(Ncorr[i]*1.0)/Nsim<<" ";
	
}