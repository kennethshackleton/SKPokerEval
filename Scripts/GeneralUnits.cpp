//
//  GeneralUnits.cpp
//  SpecialKEvalv1
//	4/27/2010
//
//	Copyright 2010 Kenneth J. Shackleton
//	k314j159s@gmail.com
//	http://specialk-coding.blogspot.com/
//
//	This program gives you software freedom; you can copy, convey,
//	propagate, redistribute and/or modify this program under the terms of
//	the GNU General Public License (GPL) as published by the Free Software
//	Foundation (FSF), either version 3 of the License, or (at your option)
//	any later version of the GPL published by the FSF.
//
//	This program is distributed in the hope that it will be useful, but
//	WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//	General Public License for more details.
//
//	You should have received a copy of the GNU General Public License along
//	with this program in a file in the toplevel directory called "GPLv3".
//	If not, see http://www.gnu.org/licenses/.
//

//Script for generating the first 13 non-negative integers with the property
//that the sum of any seven with each appearing at most four times is always
//unique among all such sums. The output is:
//{0, 1, 5, 22, 98, 453, 2031, 8698, 22854, 83661, 262349, 636345, 1479181};

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
	int i, j, k, l, m, n, r, t, s=0;

	int prime[13]={0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int sums[1000000];
	bool ACCEPT=1;
	
	int K=3;
	
	while(K<=12){
	
		s=prime[K-1]+1;
		
		do{
			n=0; ACCEPT=1; prime[K]=s;
			
			for(i=0; i<=K; i++){
				for(j=i; j<=K; j++){
					for(k=j; k<=K; k++){
						for(l=k; l<=K; l++){
							for(m=l; m<=K; m++){
								for(r=m; r<=K; r++){
									for(t=r; t<=K; t++){
										if(i!=m && j!=r && k!=t){
											sums[n]=prime[i]+prime[j]+prime[k]+prime[l]+prime[m]+prime[r]+prime[t];
											n++;}
									}
								}
							}
						}
					}
				}
			}
			
			i=0;
			do{j=i+1;
				do{if(sums[i]==sums[j]){ACCEPT=0;} j++;}
					while(ACCEPT==1 && j<n);
				i++;}
			while(ACCEPT==1 && i<n-1);
		
			if(ACCEPT==1){cout << s << endl;}
			else{s++;}
		
		}while(ACCEPT==0);
		K++;
	}
}