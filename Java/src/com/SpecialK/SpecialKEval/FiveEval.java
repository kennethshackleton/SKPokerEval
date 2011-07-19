//
//  SpecialKEval
//
//	Copyright 2010 Kenneth J. Shackleton
//	codingfeedback@gmail.com
//	http://specialk-coding.blogspot.com/
//
//	***********************************************************************
//	An evolution of this evaluator has been released under Apple's EULA and
//	is behind the app "Poker Ace" available through iTunes Store.
//	***********************************************************************
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

package com.SpecialK.SpecialKEval;

import static com.SpecialK.SpecialKEval.Constants.*;

public class FiveEval {
	
	public FiveEval() {
		
		initialiseDeck();
		initialiseRanking();
	}
	
	//Ranks for 5-card evaluation separated
	//into non-flushes and flushes, each with
	//their own respective keys
	private int[] rankArray;
	private int[] flushRankArray;
	
	//Card face values beginning with ACE_ from
	//index 0 and ending with TWO_ from index 48
	private int[] deckcardsFace;
	private int[] deckcardsFlush;
	private int[] deckcardsSuit;
	
	public void initialiseDeck(){
				
		deckcardsFace = new int[52];
		deckcardsFlush = new int[52];
		deckcardsSuit = new int[52];
		
		int[] face={		ACE_FIVE, KING_FIVE, QUEEN_FIVE, JACK_FIVE,
							TEN_FIVE, NINE_FIVE, EIGHT_FIVE, SEVEN_FIVE,
							SIX_FIVE, FIVE_FIVE, FOUR_FIVE, THREE_FIVE,
							TWO_FIVE};
		int[] faceflush={	ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH,
							TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH,
							SIX_FLUSH, FIVE_FLUSH, FOUR_FLUSH, THREE_FLUSH,
							TWO_FLUSH};
		
		int n;
		
		for(n=0; n<13; n++){
			
			deckcardsSuit[4*n]=SPADE;
			deckcardsSuit[4*n+1]=HEART;
			deckcardsSuit[4*n+2]=DIAMOND;
			deckcardsSuit[4*n+3]=CLUB;
			
			deckcardsFace[4*n]=face[n];
			deckcardsFace[4*n+1]=face[n];
			deckcardsFace[4*n+2]=face[n];
			deckcardsFace[4*n+3]=face[n];
			
			deckcardsFlush[4*n]=faceflush[n];
			deckcardsFlush[4*n+1]=faceflush[n];
			deckcardsFlush[4*n+2]=faceflush[n];
			deckcardsFlush[4*n+3]=faceflush[n];
			
		}
	
	}

	public void initialiseRanking(){
				
		rankArray = new int[MAX_FIVE_NONFLUSH_KEY_INT+1];
		flushRankArray = new int[MAX_FLUSH_KEY_INT+1];
		
		int[] face={		TWO_FIVE, THREE_FIVE, FOUR_FIVE, FIVE_FIVE,
							SIX_FIVE, SEVEN_FIVE, EIGHT_FIVE, NINE_FIVE,
							TEN_FIVE, JACK_FIVE, QUEEN_FIVE, KING_FIVE,
							ACE_FIVE};
				
		int[] faceflush={	TWO_FLUSH, THREE_FLUSH, FOUR_FLUSH, FIVE_FLUSH,
							SIX_FLUSH, SEVEN_FLUSH, EIGHT_FLUSH, NINE_FLUSH,
							TEN_FLUSH, JACK_FLUSH, QUEEN_FLUSH, KING_FLUSH,
							ACE_FLUSH};
		
	//Generates both the non-flush and the flush
	//rank, with their respective keys
	int i, j, k, l, m;
	int n=1;		//n increments as current rank
	
	for(i=0; i<MAX_FIVE_NONFLUSH_KEY_INT+1; i++){rankArray[i]=0;}
	for(i=0; i<MAX_FLUSH_KEY_INT+1; i++){flushRankArray[i]=0;}

	//high card
	for(i=5; i<=12; i++){									
		for(j=3; j<=i-1; j++){
			for(k=2; k<=j-1; k++){
				for(l=1; l<=k-1; l++){
					//no straights
					for(m=0; m<=l-1 && !(i-m==4 || (i==12 && j==3 && k==2 && l==1 && m==0)); m++){
						rankArray[face[i]+face[j]+face[k]+face[l]+face[m]]=n; n++;}}}}}
		
	//pair
	for(i=0; i<=12; i++){									
		for(j=2; j<=12; j++){
			for(k=1; k<=j-1; k++){
				for(l=0; l<=k-1; l++){
					if(i!=j && i!=k && i!=l){
						rankArray[(2*face[i])+face[j]+face[k]+face[l]]=n;
						n++;}}}}}
		
	//2pair
	for(i=1; i<=12; i++){									
		for(j=0; j<=i-1; j++){
			for(k=0; k<=12; k++){
				//no fullhouse
				if(k!=i && k!=j){
					rankArray[(2*face[i])+(2*face[j])+face[k]]=n;
					n++;}}}}
		
	//triple
	for(i=0; i<=12; i++){									
		for(j=1; j<=12; j++){
			for(k=0; k<=j-1; k++){
				//no quad
				if(i!=j && i!=k){		
					rankArray[(3*face[i])+face[j]+face[k]]=n;
					n++;}}}}
		
	//low straight nonflush
	rankArray[face[12]+face[0]+face[1]+face[2]+face[3]]=n; n++;
	
	//usual straight nonflush
	for(i=0; i<=8; i++){
		rankArray[face[i]+face[i+1]+face[i+2]+face[i+3]+face[i+4]]=n; n++;}
		
	//flush not a straight
	for(i=5; i<=12; i++){									
		for(j=3; j<=i-1; j++){
			for(k=2; k<=j-1; k++){
				for(l=1; l<=k-1; l++){
					for(m=0; m<=l-1; m++){
						if(!(i-m==4 || (i==12 && j==3 && k==2 && l==1 && m==0))){
							flushRankArray[faceflush[i]+faceflush[j]+faceflush[k]+
											faceflush[l]+faceflush[m]]=n;
							n++;}}}}}}
		
	//full house
	for(i=0; i<=12; i++)
		for(j=0; j<=12; j++){
			if(i!=j){
				rankArray[(3*face[i])+(2*face[j])]=n;
				n++;}}
		
	//quad
	for(i=0; i<=12; i++){									
		for(j=0; j<=12; j++){
			if(i!=j){
				rankArray[(4*face[i])+face[j]]=n;
				n++;}}}
		
	//low straight flush
	flushRankArray[faceflush[0]+faceflush[1]+faceflush[2]+faceflush[3]+faceflush[12]]=n;
	n++;
	
	//usual straight flush
	for(i=0; i<=8; i++){
		flushRankArray[faceflush[i]+faceflush[i+1]+faceflush[i+2]+
						faceflush[i+3]+faceflush[i+4]]=n;
		n++;
		}	
	}
	public int getRankOf(int CARD1, int CARD2, int CARD3, int CARD4, int CARD5){
	
		if( (deckcardsSuit[CARD1] == deckcardsSuit[CARD2]) &&
			(deckcardsSuit[CARD1] == deckcardsSuit[CARD3]) &&
			(deckcardsSuit[CARD1] == deckcardsSuit[CARD4]) &&
			(deckcardsSuit[CARD1] == deckcardsSuit[CARD5])){
					
			return flushRankArray[deckcardsFlush[CARD1]+deckcardsFlush[CARD2]+
								  deckcardsFlush[CARD3]+deckcardsFlush[CARD4]+
								  deckcardsFlush[CARD5]];
			}
				
		else{
			return rankArray[deckcardsFace[CARD1]+deckcardsFace[CARD2]+
			                 deckcardsFace[CARD3]+deckcardsFace[CARD4]+
							 deckcardsFace[CARD5]];
			}
	}
	
	public int getBestRankOf(int CARD1, int CARD2, int CARD3, int CARD4, int CARD5, int CARD6, int CARD7){
		int[] seven_cards={CARD1, CARD2, CARD3, CARD4, CARD5, CARD6, CARD7};
		int[] five_temp={CARD1, CARD2, CARD3, CARD4, CARD5};
		
		int BEST_RANK_SO_FAR=0, CURRENT_RANK=0;
		int i, j, k, m;
		
		for(i=1; i<7; i++){
			for(j=0; j<i; j++){m=0;
				for(k=0; k<7; k++){
					if(k!=i && k!=j){
						five_temp[m]=seven_cards[k];
						m++;
					}
				}
				
				CURRENT_RANK=getRankOf(five_temp[0], five_temp[1], five_temp[2],
											five_temp[3], five_temp[4]);
				
				if(BEST_RANK_SO_FAR<CURRENT_RANK){BEST_RANK_SO_FAR=CURRENT_RANK;}
			}
		}
		
		return BEST_RANK_SO_FAR;
	}
}
