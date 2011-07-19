package com.SpecialK.SpecialKEval;

//  SevenEval.h
//  SpecialKEvalv1
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

import static com.SpecialK.SpecialKEval.Constants.*;
import com.SpecialK.SpecialKEval.FiveEval;

public class SevenEval {
	
	public SevenEval() {
		
		System.out.println("Initialising Deck");
		initialiseDeck();
		System.out.println("Initialising Ranking");
		initialiseRanking();
		System.out.println("Initialising FlushCheck");
		generateFlushCheck();
		System.out.println("Initialising complete");
		
	}
	
	//Ranks for 5-card evaluation separated
	//into non-flushes and flushes, each with
	//their own respective keys
	private int[] rankArray;
	private int[] flushRankArray;
	
	//Card face values beginning with ACE_ from
	//index 0 and ending with TWO_ from index 48
	private long[] deckcardsKey;
	private int[] deckcardsFlush;
	private int[] deckcardsSuit;
	
	private short[] flushCheckArray;
	
	private void initialiseDeck() {
		
		deckcardsKey = new long[DECK_SIZE];
		deckcardsFlush = new int[DECK_SIZE];
		deckcardsSuit = new int[DECK_SIZE];
    
		//Enter face values into arrays to later build up the
		//respective keys. The values of ACE and ACE_FLUSH etc.
		//are different.
		int[] face = { ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO };
		int[] faceflush = {	ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH, TEN_FLUSH, NINE_FLUSH,
      EIGHT_FLUSH, SEVEN_FLUSH, SIX_FLUSH, FIVE_FLUSH, FOUR_FLUSH, THREE_FLUSH, TWO_FLUSH };
		int n=0;
		for (n=0; n<13; n++) {					
			deckcardsKey[4*n]		= (face[n] << NON_FLUSH_BIT_SHIFT) + SPADE;
			deckcardsKey[4*n+1]		= (face[n] << NON_FLUSH_BIT_SHIFT) + HEART;
			deckcardsKey[4*n+2]		= (face[n] << NON_FLUSH_BIT_SHIFT) + DIAMOND;
			deckcardsKey[4*n+3]		= (face[n] << NON_FLUSH_BIT_SHIFT) + CLUB;
      
			deckcardsFlush[4*n]		= faceflush[n];
			deckcardsFlush[4*n+1]	= faceflush[n];
			deckcardsFlush[4*n+2]	= faceflush[n];
			deckcardsFlush[4*n+3]	= faceflush[n];
			
			deckcardsSuit[4*n]		= SPADE;
			deckcardsSuit[4*n+1]	= HEART;
			deckcardsSuit[4*n+2]	= DIAMOND;
			deckcardsSuit[4*n+3]	= CLUB;		
		}
	}
  
	public void initialiseRanking() {
		
		FiveEval fiveEval = new FiveEval();
		
		rankArray = new int[MAX_NONFLUSH_KEY_INT+1];
		flushRankArray = new int[MAX_KEY_INT+1];
		
		int[] face={		ACE, KING, QUEEN, JACK,
							TEN, NINE, EIGHT, SEVEN,
							SIX, FIVE, FOUR, THREE,
							TWO};
		int[] faceFlush={	ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH,
							TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH,
							SIX_FLUSH, FIVE_FLUSH, FOUR_FLUSH, THREE_FLUSH,
							TWO_FLUSH};
		int i, j, k, l, m, n, p, count=0;
		
		//Clean all ranks and flushranks
		for(i=0; i<MAX_NONFLUSH_KEY_INT+1; i++){rankArray[i]=0;}
		for(i=0; i<MAX_FLUSH_KEY_INT+1; i++){flushRankArray[i]=0;}
		
		//Non-flush ranks
		for(i=1; i<13; i++){for(j=1; j<=i; j++){for(k=1; k<=j; k++){for(l=0; l<=k; l++){
			for(m=0; m<=l; m++){for(n=0; n<=m; n++){for(p=0; p<=n; p++){
				
				if(i!=m && j!=n && k!=p){
					
					int key=face[i]+face[j]+face[k]+face[l]+face[m]+face[n]+face[p];
					
					//The 4*i+0 and 4*m+1 trick prevents flushes
					int rank=fiveEval.getBestRankOf(4*i, 4*j, 4*k, 4*l, 4*m+1, 4*n+1, 4*p+1);
					rankArray[key]=rank; count++;}}}}}}}}
		
		//Flush ranks
		//All 7 same suit:
		for(i=6; i<13; i++){for(j=5; j<i; j++){for(k=4; k<j; k++){for(l=3; l<k; l++){
			for(m=2; m<l; m++){for(n=1; n<m; n++){for(p=0; p<n; p++){
				
				int key=faceFlush[i]+faceFlush[j]+faceFlush[k]+faceFlush[l]+faceFlush[m]+
																		faceFlush[n]+faceFlush[p];				
				int rank=fiveEval.getBestRankOf(4*i, 4*j, 4*k, 4*l, 4*m, 4*n, 4*p);

				flushRankArray[key]=rank; count++;}}}}}}}
		
		//Only 6 same suit:
		for(i=5; i<13; i++){for(j=4; j<i; j++){for(k=3; k<j; k++){for(l=2; l<k; l++){
			for(m=1; m<l; m++){for(n=0; n<m; n++){
				
				int key=faceFlush[i]+faceFlush[j]+faceFlush[k]+faceFlush[l]+faceFlush[m]+faceFlush[n];
				
				//The Two of clubs is the card at index 51, the
				//other cards are all spades
				int rank=fiveEval.getBestRankOf(4*i, 4*j, 4*k, 4*l, 4*m, 4*n, 51);
				
				flushRankArray[key]=rank; count++;}}}}}}
		
		//Only 5 same suit:
		for(i=4; i<13; i++){for(j=3; j<i; j++){for(k=2; k<j; k++){for(l=1; l<k; l++){
			for(m=0; m<l; m++){
				
				int key=faceFlush[i]+faceFlush[j]+faceFlush[k]+faceFlush[l]+faceFlush[m];
				
				int rank=fiveEval.getRankOf(4*i, 4*j, 4*k, 4*l, 4*m);

				flushRankArray[key]=rank; count++;}}}}}		
	}

	private void generateFlushCheck() {
    
		flushCheckArray = new short[MAX_FLUSH_CHECK_SUM + 1];
		
		int card_1, card_2, card_3, card_4, card_5, card_6, card_7;
		
		//Begin with spades and run no further than clubs
		int SUIT_KEY=SPADE;
		
		int[] suits = {SPADE, HEART, DIAMOND, CLUB};
		
		//Initialise all entries of flushCheck[] to UNVERIFIED, as yet unchecked.
		//memset(&flushCheck[0], UNVERIFIED, sizeof(int)*(MAX_FLUSH_CHECK_SUM+1));
		
		for ( int i = 0 ; i < MAX_FLUSH_CHECK_SUM+1 ; i++ ) { flushCheckArray[i]=UNVERIFIED; }
		
		//7-card
		for(card_1=0; card_1<NUMBER_OF_SUITS; card_1++){
			for(card_2=0; card_2<=card_1; card_2++){
				for(card_3=0; card_3<=card_2; card_3++){
					for(card_4=0; card_4<=card_3; card_4++){
						for(card_5=0; card_5<=card_4; card_5++){
							for(card_6=0; card_6<=card_5; card_6++){
								for(card_7=0; card_7<=card_6; card_7++){
									
									int SUIT_COUNT=0, FLUSH_SUIT_INDEX=-1, CARDS_MATCHED_SO_FAR=0;
									
									SUIT_KEY = suits[card_1] + suits[card_2] + suits[card_3] + suits[card_4] +
											   suits[card_5] + suits[card_6] + suits[card_7];
                  
									if ( flushCheckArray[SUIT_KEY] == UNVERIFIED ){
										
										do{
											FLUSH_SUIT_INDEX++;
											SUIT_COUNT=	(suits[card_1] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_2] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_3] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_4] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_5] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_6] == suits[FLUSH_SUIT_INDEX] ? 1 : 0) +
														(suits[card_7] == suits[FLUSH_SUIT_INDEX] ? 1 : 0);
											CARDS_MATCHED_SO_FAR += SUIT_COUNT;
										} while(CARDS_MATCHED_SO_FAR < 3 && FLUSH_SUIT_INDEX < 4);
										
										//7-card flush check means flush
										if(SUIT_COUNT>4){flushCheckArray[SUIT_KEY] = (short)suits[FLUSH_SUIT_INDEX];}
										else{flushCheckArray[SUIT_KEY] = (short)NOT_A_FLUSH;}
									}else{;}
									
								}
							}
						}
					}
				}
			}
		}
	}
	
	final int getRankOf (int CARD1, int CARD2, int CARD3, int CARD4, int CARD5, int CARD6, int CARD7) {
		
		long KEY = 	deckcardsKey[CARD1] +
					deckcardsKey[CARD2] +
					deckcardsKey[CARD3] +
					deckcardsKey[CARD4] +
					deckcardsKey[CARD5] +
					deckcardsKey[CARD6] +
					deckcardsKey[CARD7];
						
    int FLUSH_CHECK_KEY = (int)( KEY & SUIT_BIT_MASK );
    int FLUSH_SUIT = flushCheckArray[FLUSH_CHECK_KEY];
    
		if (FLUSH_SUIT == NOT_A_FLUSH) {
			
			KEY = KEY >> NON_FLUSH_BIT_SHIFT;
			
			return rankArray[(int)KEY];
		
		}
		
		else {
			
			KEY = (deckcardsSuit[CARD1] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD1] +
				  (deckcardsSuit[CARD2] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD2] +
				  (deckcardsSuit[CARD3] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD3] +
				  (deckcardsSuit[CARD4] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD4] +
				  (deckcardsSuit[CARD5] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD5] +
            	  (deckcardsSuit[CARD6] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD6] +
            	  (deckcardsSuit[CARD7] == FLUSH_SUIT ? 1 : 0) * deckcardsFlush[CARD7];
						
			return flushRankArray[(int)KEY];
		}	
	}
}
