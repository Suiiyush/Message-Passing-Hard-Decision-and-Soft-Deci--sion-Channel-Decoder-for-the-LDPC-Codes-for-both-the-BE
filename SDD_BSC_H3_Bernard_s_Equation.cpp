#include<bits/stdc++.h>
using namespace std;

// Soft Decision Decoding For BSC Channel (for 9 x 12 H - matrix) using Bernard's Equation


// Degree of CN is 4
struct CN
{
	float v[4][2]={{-1,-1},{-1,-1},{-1,-1},{-1,-1}};     // In first column I'll store which number of VN is connected (from 1 to 12) and in second column the values of that VN in message passing from VN to CN
}arr_CN[9];


// Degree of VN is 3
struct VN
{
	float c[3][2]={{-1,-1},{-1,-1},{-1,-1}};  // In first column I'll store which number of CN is connected (from 1 to 9) and in second column the values of that CN in message passing from CN to VN     
	float value;                              // Store the conditional probability of transmitted signal after adding the noise that the value of corresponding VN is 1
	float decision;                           // After each iteration it makes decision (by taking likelihood ratios) and that conditional probability it sends to CNs
}arr_VN[12];

int main()
{
	int N=12,U=9,i,j,c1,c2,c3,v1,v2,v3,v4,flag,count,terminate,Nsim=10000,Ksim,l;
	int Ncorr[101]={0},Nerr[101]={0};
	
	float p[101],r,k,alpha,bita,VN_is_1,VN_is_0,VN_decision_1,VN_decision_0,arr[3];      
	
	p[0]=0;                                                                          // Crossover Probability
	
	for(i=1;i<=100;i++)
	{
		p[i]=p[i-1]+0.01;
	}
	
	int H[U][N]= {{1,0,0,0,0,1,0,1,0,1,0,0},                 // 9 x 12 H - matrix
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
	for(l=0;l<=100;l++)                                   // Outer loop for crossover probability
	{
		
	for(Ksim=1;Ksim<=Nsim;Ksim++)                         // Loop for Monte - Carlo simulations
	{
	
	int tr[N]={0};                                        // Transmitted Signal
	int noise[N]={0};                                     // Noise ( or received signal contains bits)
	float rx[N]={0};                                      // Received Signal ( contains conditional probabilities of VNi is 1 given received bit
	
	// Loading VNs with conditional probabilities that VN is 1 given received bit 
	
	for(i=0;i<N;i++)
	{
		r=((float) rand() / (RAND_MAX + 1));              // Generation of a random number between 0 to 1
		
		if(r>(1-p[l]))                                    // If r > 1-p then, the bit will be flipped 
		noise[i]=1;
		else
		noise[i]=0;
		
		if(noise[i]==1)
		rx[i]=1-p[l];
		else
		rx[i]=p[l];
		
		
		arr_VN[i].value=rx[i];
		
		for(j=0;j<3;j++)
		{
			alpha = 1/(rx[i]*rx[i]*rx[i] + (1-rx[i])*(1-rx[i])*(1-rx[i]));
			
			arr_VN[i].c[j][1]=alpha*rx[i]*rx[i]*rx[i];
		}
	}
	
	
	// Tanner Graph decoding
	
	terminate=0;
	
	// terminate is used for 100 iterations breaking condition
	
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
		
		
		// CN j sends VN i the conditional probability that VN i is 1 by Bernard's Equation and by using other VNs conditional probabilities	
			
	    for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v1-1].c[j][0]==i+1)
	    	{   
	    	    arr_VN[v1-1].c[j][1] = (0.5-0.5*(1-2*arr_CN[i].v[1][1])*(1-2*arr_CN[i].v[2][1])*(1-2*arr_CN[i].v[3][1]));
	    	    
	    		break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v2-1].c[j][0]==i+1)
	    	{   
	    	    arr_VN[v2-1].c[j][1] = (0.5-0.5*(1-2*arr_CN[i].v[0][1])*(1-2*arr_CN[i].v[2][1])*(1-2*arr_CN[i].v[3][1]));
	    		
				break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v3-1].c[j][0]==i+1)
	    	{
	    	    arr_VN[v3-1].c[j][1] = (0.5-0.5*(1-2*arr_CN[i].v[0][1])*(1-2*arr_CN[i].v[1][1])*(1-2*arr_CN[i].v[3][1]));
	    	    
	    		break;
			}
		}
		
		for(j=0;j<3;j++)
	    {
	    	if(arr_VN[v4-1].c[j][0]==i+1)
	    	{
	    	    arr_VN[v4-1].c[j][1] = (0.5-0.5*(1-2*arr_CN[i].v[0][1])*(1-2*arr_CN[i].v[1][1])*(1-2*arr_CN[i].v[2][1]));
	    	    
	    		break;
			}
		}
	}
	
	// Computing the conditional probability that VN i is 1 for CN j
	
	flag=count=0;
	
	// flag and count variables are used for second breaking condition
	// If all VNs value after decision making became 1 or 0 then, after that iteration their values will remain same, so this is the breaking condition
	
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
    	
    	
    	// Final decision after t th iteration that VN i is 1 or 0
    	
    	arr_VN[i].decision=(arr_VN[i].value/(1-arr_VN[i].value))*(arr[0]/(1-arr[0]))*(arr[1]/(1-arr[1]))*(arr[2]/(1-arr[2]));
    	
    	if(arr_VN[i].decision>=1)
    	{
    		arr_VN[i].decision=1;
    		++count;
		}
    	else
    	{
    	    arr_VN[i].decision=0;
    	    ++flag;
    	}
	}
	
	if(flag==12 || count==12)
	break;
	
	    ++terminate;
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
	
	for(i=0;i<=100;i++)
	//cout<<"p = "<<p[i]<<" "<<"Ncorr = "<<Ncorr[i]<<" Nerr = "<<Nerr[i]<<" Ncorr/Nsim = "<<(Ncorr[i]*1.0)/Nsim<<endl<<endl;
	cout<<(Ncorr[i]*1.0)/Nsim<<" ";
	
}