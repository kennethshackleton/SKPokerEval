//
//  KeyTest.cpp
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

#include <iostream>

using namespace std;

//_SEVEN tag suppressed
#define TWO 0
#define THREE 1
#define FOUR 5
#define FIVE 22
#define SIX 98
#define SEVEN 453
#define EIGHT 2031
#define NINE 8698
#define TEN 22854
#define JACK 83661
#define QUEEN 262349
#define KING 636345
#define ACE 1479181
//end of _SEVEN tag suppressed

bool sums[8000000];

int main (int argc, char * const argv[]) {
    // insert code here...
	typedef enum ranks {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	
	ranks rankArray[1]={11];
	
	int face[13]={ACE, KING, QUEEN, JACK, TEN, NINE,
		EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO};
	int i, j, k, l, m, n, p, SUM;
	
	for(n=0; n<8000000; n++){sums[n]=0;}
	
	for(i=1; i<13; i++){
		for(j=1; j<=i; j++){
			for(k=1; k<=j; k++){
				for(l=0; l<=k; l++){
					for(m=0; m<=l; m++){
						for(n=0; n<=m; n++){
							for(p=0; p<=n; p++){
								if(i!=m && j!=n && k!=p){
									SUM=face[i]+face[j]+face[k]+face[l]+face[m]+face[n]+face[p];
									if(sums[SUM]==1){cout << "Error" << endl; return 0;}
									else{sums[SUM]=1; cout << SUM << endl;}
								}
							}
						}
					}
				}
			}
		}
	}
	
	cout << "No collision";
	
    return 0;
}
