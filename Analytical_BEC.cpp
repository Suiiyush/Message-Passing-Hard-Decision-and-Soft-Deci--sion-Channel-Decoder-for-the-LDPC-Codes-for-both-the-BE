#include<bits/stdc++.h>
using namespace std;


main()
{
	int i,j,dc=5,dv=3;
	float e0[8]={0.1,0.2,0.3,0.4,0.5,0.51,0.52,0.54},et,arr[8][51];
	
	
	for(i=0;i<8;i++)
	{
		et=e0[i];
		
	    for(j=0;j<51;j++)
	    {
		    arr[i][j]=et;
		
		    et = e0[i]*pow(1-pow(1-et,dc-1),dv-1);
		
		    cout<<arr[i][j]<<" ";
	    }
	    cout<<endl;
    }
}