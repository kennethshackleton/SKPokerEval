/*!
 HandEval.m
 SpecialKEval
 <p>
 Copyright 2010 Kenneth J. Shackleton
 codingfeedback@gmail.com
 http://specialk-coding.blogspot.com/
 <p>
 ***********************************************************************
 An evolution of this evaluator has been released under Apple's EULA and
 is behind the app "Poker Ace" available through iTunes Store. For more
 details visit http://itunes.apple.com/us/app/poker-ace/id392530020?mt=8
 ***********************************************************************
 <p>
 This program gives you software freedom; you can copy, convey,
 propagate, redistribute and/or modify this program under the terms of
 the GNU General Public License (GPL) as published by the Free Software
 Foundation (FSF), either version 3 of the License, or (at your option)
 any later version of the GPL published by the FSF.
 <p>
 This program is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.
 <p>
 You should have received a copy of the GNU General Public License along
 with this program in a file in the toplevel directory called "GPLv3".
 If not, see http://www.gnu.org/licenses/.
 <p>
 @author Kenneth J. Shackleton
 */

// Key values are 32-bit integers, the first 9 bits are given to the suit and all of the remaining 23 bits are given to the face value. We add card
// keys together to generate a hand key, check the first 9 bits to see if it's a flush. If it's not a flush we bit shift >> 9 the hand key and look-up
// the non-flush rank at the corresponding index. We really do need all of the 32 bits on offer.

#import "HandEval.h"
#import "FiveEval.h"

@interface HandEval()

- (void)generateDeckKeys;
- (void)generateRanks;
- (void)generateFlushCheck;

@end

@implementation HandEval

#pragma mark -
#pragma mark Memory

- (id)init
{
  if (self = [super init]) {
		[self generateDeckKeys];
		[self generateRanks];
    [self generateFlushCheck];
	}
	return self;
}

#pragma mark -
#pragma mark Generation

- (void)generateDeckKeys
{	
	// Enter face values into arrays to later build up the respective keys. The values of ACE and ACE_FLUSH etc. are different.
	int face[13] = {ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO};
	
	int faceflush[13] = {ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH, TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH, SIX_FLUSH, FIVE_FLUSH, FOUR_FLUSH,
    THREE_FLUSH, TWO_FLUSH};
	
	for (int n = 0; n < 13; n++) {
		deckcardsKey[4*n]     = (face[n] << NON_FLUSH_BIT_SHIFT) + SPADE;
		deckcardsKey[4*n + 1]	= (face[n] << NON_FLUSH_BIT_SHIFT) + HEART;
		deckcardsKey[4*n + 2]	= (face[n] << NON_FLUSH_BIT_SHIFT) + DIAMOND;
		deckcardsKey[4*n + 3]	= (face[n] << NON_FLUSH_BIT_SHIFT) + CLUB;
		
		deckcardsFlush[4*n]     = faceflush[n];
		deckcardsFlush[4*n + 1] = faceflush[n];
		deckcardsFlush[4*n + 2] = faceflush[n];
		deckcardsFlush[4*n + 3] = faceflush[n];
    
		deckcardsSuit[4*n]      = SPADE;
		deckcardsSuit[4*n + 1]	= HEART;
		deckcardsSuit[4*n + 2]	= DIAMOND;
		deckcardsSuit[4*n + 3]	= CLUB;		
	}
}

- (void)generateRanks
{
	// Zero out all ranks and flushranks.
	for (int i = 0; i < MAX_SEVEN_FLUSH_KEY_INT + 1; i++) {
    flushRankArray[i] = 0;
  }
	
	// Generate seven-ranks from five-ranks.
	FiveEval *fiveCardEvaluator = [FiveEval sharedEvaluator];
	
	// Atoms to build up integer keys.
	int face[13] = {ACE, KING, QUEEN, JACK, TEN, NINE, EIGHT, SEVEN, SIX, FIVE, FOUR, THREE, TWO};
	int faceFlush[13] = {ACE_FLUSH, KING_FLUSH, QUEEN_FLUSH, JACK_FLUSH, TEN_FLUSH, NINE_FLUSH, EIGHT_FLUSH, SEVEN_FLUSH, SIX_FLUSH, FIVE_FLUSH, FOUR_FLUSH,
    THREE_FLUSH, TWO_FLUSH};
  
	// Non-flush ranks
	for (int i = 1; i < 13; i++) {
    for (int j = 1; j <= i; j++) {
      for (int k = 1; k <= j; k++) {
        for (int l = 0; l <= k; l++) {
          for (int m = 0; m <= l; m++) {
            for (int n = 0; n <= m; n++) {
              for (int p = 0; p <= n; p++) {
                if (i != m && j != n && k != p) {
                  unsigned int key = face[i] + face[j] + face[k] + face[l] + face[m] + face[n] + face[p];
                  short rank = [fiveCardEvaluator rankOfSeven:4*i :4*j :4*k :4*l :4*m+1 :4*n+1 :4*p+1]; // The 4*i+0 and 4*m+1 trick prevents flushes.
                  rankArray[key < CIRCUMFERENCE_SEVEN ? key : key - CIRCUMFERENCE_SEVEN] = rank;
                }
              }
            }
          }
        }
      }
    }
  }
  
  //**** Flush ranks
	//** All 7 same suit:
	// We generate the ranks assuming the flush is a spade flush.
	// The suit of the flush does not matter.
	for (int i = 6; i < 13; i++) {
		for (int j = 5; j <i ; j++) {
			for (int k = 4; k < j; k++) {
				for (int l = 3; l < k; l++) {
					for (int m = 2; m < l; m++) {
						for (int n = 1; n < m; n++) {
							for (int p = 0; p < n; p++) {
								int key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m] + faceFlush[n] + faceFlush[p];				
								short rank = [fiveCardEvaluator rankOfSeven:4*i :4*j :4*k :4*l :4*m :4*n :4*p];
								flushRankArray[key] = rank;
							}
						}
					}
				}
			}
		}
	}
  
	// Only 6 same suit.
	for (int i = 5; i < 13; i++) {
    for (int j = 4; j < i; j++) {
      for (int k = 3; k < j; k++) {
        for (int l = 2; l < k; l++) {
          for (int m = 1; m < l; m++) {
            for (int n = 0; n < m; n++) {
              int key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m] + faceFlush[n];
              short rank = [fiveCardEvaluator rankOfSeven:4*i :4*j :4*k :4*l :4*m :4*n :51]; // The Two of Clubs is the card at index 51; must not be a spade.
              flushRankArray[key] = rank;
            }
          }
        }
      }
    }
  }
  
	// Only 5 same suit.
	for (int i = 4; i < 13; i++) {
    for (int j = 3; j < i; j++) {
      for (int k = 2; k < j; k++) {
        for (int l = 1; l < k; l++) {
          for (int m = 0; m < l; m++) {
            int key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m];
            short rank = [fiveCardEvaluator rankOfFive:4*i :4*j :4*k :4*l :4*m];
            flushRankArray[key] = rank;
          }
        }
      }
    }
  }
  
	[FiveEval releaseToNilSharedEvaluator];	
}

- (void)generateFlushCheck
{
	short SUIT_COUNT = 0, FLUSH_SUIT_INDEX = -1, CARDS_MATCHED_SO_FAR = 0;
	short SUIT_KEY = SPADE;
	short suits[4] = {SPADE, HEART, DIAMOND, CLUB};
	
	// Initialise all entries of flushCheck[] to UNVERIFIED, as yet unchecked.
	//memset(&flushCheck[0], UNVERIFIED, sizeof(int)*(MAX_FLUSH_CHECK_SUM+1));
	
	for (int i = 0 ; i < MAX_FLUSH_CHECK_SUM + 1 ; i++) {
    flushCheck[i] = UNVERIFIED;
  }
	
	// 7-card
	for (int card_1 = 0; card_1 < NUMBER_OF_SUITS; card_1++) {
		for (int card_2 = 0; card_2 <= card_1; card_2++) {
			for (int card_3 = 0; card_3 <= card_2; card_3++) {
				for (int card_4 = 0; card_4 <= card_3; card_4++) {
					for (int card_5 = 0; card_5 <= card_4; card_5++) {
						for (int card_6 = 0; card_6 <= card_5; card_6++) {
							for (int card_7 = 0; card_7 <= card_6; card_7++) {
								SUIT_COUNT = 0, FLUSH_SUIT_INDEX =-1, CARDS_MATCHED_SO_FAR = 0;
								SUIT_KEY = suits[card_1] + suits[card_2] + suits[card_3] + suits[card_4] + suits[card_5] + suits[card_6] + suits[card_7];
								if (flushCheck[SUIT_KEY] == UNVERIFIED) {
									do {
                    FLUSH_SUIT_INDEX++;
                    SUIT_COUNT = (suits[card_1] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_2] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_3] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_4] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_5] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_6] == suits[FLUSH_SUIT_INDEX]) +
                    (suits[card_7] == suits[FLUSH_SUIT_INDEX]);
                    CARDS_MATCHED_SO_FAR += SUIT_COUNT;
									} while(CARDS_MATCHED_SO_FAR < 3 && FLUSH_SUIT_INDEX < 4);
									
									// 7-card flush check means flush
									if (SUIT_COUNT > 4) {
                    flushCheck[SUIT_KEY] = suits[FLUSH_SUIT_INDEX];
                  } else {
                    flushCheck[SUIT_KEY] = NOT_A_FLUSH;
                  }
								}
							}
						}
					}
				}
			}
		}
	}
}

#pragma mark -
#pragma mark Rank

- (short)rankOfSeven:(const int)card_1 :(const int)card_2 :(const int)card_3 :(const int)card_4 :(const int)card_5 :(const int)card_6 :(const int)card_7 {
	unsigned int KEY = deckcardsKey[card_1] + deckcardsKey[card_2] + deckcardsKey[card_3] + deckcardsKey[card_4] + deckcardsKey[card_5] + deckcardsKey[card_6] +
                     deckcardsKey[card_7];
	
	int FLUSH_CHECK_KEY = KEY & SUIT_BIT_MASK;
	int FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
  
	if (FLUSH_SUIT == NOT_A_FLUSH) {
  	KEY >>= NON_FLUSH_BIT_SHIFT;
		
		return KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN];
	} else {
		int FLUSH_KEY = (deckcardsSuit[card_1] == FLUSH_SUIT ? deckcardsFlush[card_1] : 0) +
                    (deckcardsSuit[card_2] == FLUSH_SUIT ? deckcardsFlush[card_2] : 0) +
                    (deckcardsSuit[card_3] == FLUSH_SUIT ? deckcardsFlush[card_3] : 0) +
                    (deckcardsSuit[card_4] == FLUSH_SUIT ? deckcardsFlush[card_4] : 0) +
                    (deckcardsSuit[card_5] == FLUSH_SUIT ? deckcardsFlush[card_5] : 0) +
                    (deckcardsSuit[card_6] == FLUSH_SUIT ? deckcardsFlush[card_6] : 0) +
                    (deckcardsSuit[card_7] == FLUSH_SUIT ? deckcardsFlush[card_7] : 0);
		
		return flushRankArray[FLUSH_KEY];
	}
	return 0;	
}

#pragma mark -
#pragma mark Evaluation

- (NSString *)computePreFlopEquityForSpecificHoleCards:(int *)hole_cards withNumberOfPlayers:(int)number_of_players {
	// Count and list the undealt cards, ie. the remaining deck.
	int undealtCards[DECK_SIZE];
	int number_of_undealt_cards = 0;
	
	for (int i = 0; i < DECK_SIZE; i++) {
		bool bool_add_card = YES;
		for (int j = 0; j < 2*number_of_players; j++) {
			bool_add_card = (hole_cards[j] == i ? NO : bool_add_card);
    }
		if (bool_add_card) {
      undealtCards[number_of_undealt_cards] = i;
      number_of_undealt_cards++;
    }
  }
	////////////
	
	// Cumulative equities
	unsigned long equity[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    equity[i] = 0;
  }
	////////////
	
	// Record player_rank
	int player_rank[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    player_rank[i] = 0;
  }
	////////////
	
	// Possible shares in equity
	int equity_share[MAX_NUMBER_OF_PLAYERS + 1];
	// The 0th entry is redundant
	equity_share[0] = -1;
	for (int i = 1; i < MAX_NUMBER_OF_PLAYERS + 1; i++) {
    equity_share[i] = CAKE/i;
  }
	///////////
	
	// Player table key and suit stems
	unsigned int player_key[MAX_NUMBER_OF_PLAYERS];
	int player_flush_stem[MAX_NUMBER_OF_PLAYERS][CLUB+1];
	int player_suit_stem[MAX_NUMBER_OF_PLAYERS];
	
	for (int p = 0; p < number_of_players; p++) {
		int P = 2*p;
		
		int card_1 = hole_cards[P];
		int card_2 = hole_cards[P + 1];
		
		player_key[p] = deckcardsKey[card_1] + deckcardsKey[card_2];
		
		player_flush_stem[p][SPADE] = (deckcardsSuit[card_1] == SPADE ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == SPADE ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][HEART]	=	(deckcardsSuit[card_1] == HEART ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == HEART ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][DIAMOND]	=	(deckcardsSuit[card_1] == DIAMOND ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == DIAMOND ?
                                     deckcardsFlush[card_2] : 0);
		player_flush_stem[p][CLUB] = (deckcardsSuit[card_1] == CLUB ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == CLUB ? deckcardsFlush[card_2] : 0);

		player_suit_stem[p] = deckcardsSuit[hole_cards[P]] + deckcardsSuit[hole_cards[P + 1]];
  }
	////////
	
  int player_defect = number_of_players-1;
	int number_of_players_sharing_the_pot = 1;
	int current_best_rank = 0;
	int next_rank = 0;
	
	// Begin calculating equities by dealing out all possible combinations of five table cards, adding up the incremental equities to find the total integral
  // equities.
	
	int i = 4;
	do {
		unsigned int table_card_1 = undealtCards[i];
		int j = 3;
		do {
			unsigned int table_card_2 = undealtCards[j];
			unsigned int table_key_cumulative_to_2 = deckcardsKey[table_card_1] + deckcardsKey[table_card_2];
			int k = 2;
			do {
				unsigned int table_card_3 = undealtCards[k];
				unsigned int table_key_cumulative_to_3 = table_key_cumulative_to_2 + deckcardsKey[table_card_3];
				int l = 1;
				do {
					unsigned int table_card_4 = undealtCards[l];
					unsigned int table_key_cumulative_to_4 = table_key_cumulative_to_3 + deckcardsKey[table_card_4];
					int m = 0;
					do {
						unsigned int table_card_5 = undealtCards[m];
						unsigned int table_key = table_key_cumulative_to_4 + deckcardsKey[table_card_5];
												
						// Reset table_flush_key.
						int table_flush_key = UNVERIFIED;
						// Reset number of interested players.
						number_of_players_sharing_the_pot = 1;
						////////
						
						unsigned int KEY = table_key + player_key[0];
						
						int FLUSH_CHECK_KEY = (KEY & SUIT_BIT_MASK);
						int FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
						
						if (FLUSH_SUIT == NOT_A_FLUSH) {
							KEY >>= NON_FLUSH_BIT_SHIFT;
							current_best_rank = KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN];
							player_rank[0] = current_best_rank;
						} else {
							(table_flush_key == UNVERIFIED ?
               table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                 (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                 (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                 (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                 (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO);
							
							int player_flush_key = table_flush_key + player_flush_stem[0][FLUSH_SUIT];
							
							current_best_rank = flushRankArray[player_flush_key];
							player_rank[0] = current_best_rank;
						}
						
						int n = 1;
						do {
							KEY = table_key + player_key[n];
							
							FLUSH_CHECK_KEY = ( KEY & SUIT_BIT_MASK );
							FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
							
							if (FLUSH_SUIT == NOT_A_FLUSH) {
								KEY >>= NON_FLUSH_BIT_SHIFT;
                next_rank = KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN];
								player_rank[n] = next_rank;
							} else {
								(table_flush_key == UNVERIFIED ?
                 table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                   (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                   (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                   (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                   (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO);
								
								int player_flush_key = table_flush_key + player_flush_stem[n][FLUSH_SUIT];
								next_rank = flushRankArray[player_flush_key];
								player_rank[n] = next_rank;
							}
							
							//// Compare the player_rank
							//
							//Case: new outright strongest player
							if (current_best_rank < next_rank) {
								// Update current best rank
								current_best_rank = next_rank;
								// Reset number of players interested
								number_of_players_sharing_the_pot = 1;
							} else if (current_best_rank == next_rank) { // Case: pot shared with another player
								// Increment by 1 the number of interest players
								number_of_players_sharing_the_pot++;
							}
							//Last Case: new player loses
							//There is nothing we need to do.
							///////
							n++;
						} while (n < number_of_players);
							
						
						// Calculate incremental equity
						int incremental_equity = equity_share[number_of_players_sharing_the_pot];
						
						// Add to the cumulative equities
						int p = 0;
						do {
							if (player_rank[p] == current_best_rank) {
                equity[p] += incremental_equity;
              }
							p++;
						} while (p < number_of_players);
						m++;
          } while (m < l); l++;
				} while (l < k); k++;
			} while (k < j); j++;
		} while (j < i); i++;
	} while (i < number_of_undealt_cards);
	
	double equity_percentage[] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  int Equity[] = EQUITY;
	double equity_sum = 0.0;
  for (int i = 0; i < player_defect; i++) {
		equity_percentage[i] = 100.0f * (equity[i] + 0.0) / Equity[number_of_players-2];
    equity_sum += equity_percentage[i];
	}
  equity_percentage[player_defect] = 100.0 - equity_sum;
  
  return [NSString stringWithFormat:@"%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f", equity_percentage[0], equity_percentage[1], equity_percentage[2],
          equity_percentage[3], equity_percentage[4], equity_percentage[5], equity_percentage[6], equity_percentage[7]];
}

- (NSString *)computeFlopEquityForSpecificHoleCards:(int *)hole_cards withTableCards:(int *)table_cards andNumberOfPlayers:(int)number_of_players {
	// Count and list the undealt cards, ie. the remaining deck.
	int undealtCards[DECK_SIZE];
	int number_of_undealt_cards = 0;
	
	for (int i = 0; i < DECK_SIZE; i++) {
		bool bool_add_card = YES;
		
		for (int j = 0; j < 2 * number_of_players; j++) {
			if (hole_cards[j] == i) {
        bool_add_card = NO;
      }
    }
		
		for (int j = 0; j < MAX_NUMBER_OF_TABLE_CARDS; j++) {
			if (table_cards[j] == i) {
        bool_add_card = NO;
      }
    }
		
		if (bool_add_card) {
      undealtCards[number_of_undealt_cards] = i;
      number_of_undealt_cards++;
    }
	}
	////////////
	
	// Cumulative equities
	unsigned long equity[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    equity[i] = 0;
  }
	////////////
	
	// Record player_rank
	int player_rank[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    player_rank[i] = 0;
  }
	////////////
	
	// Possible shares in equity
	int equity_share[MAX_NUMBER_OF_PLAYERS + 1];
	// The 0th entry is redundant
	equity_share[0] = -1;
	for (int i = 1; i < MAX_NUMBER_OF_PLAYERS + 1; i++) {
    equity_share[i] = CAKE/i;
  }
	///////////
	
	// Player table key and suit stems
	unsigned int player_key[MAX_NUMBER_OF_PLAYERS];
	// unsigned int player_key_stem[MAX_NUMBER_OF_PLAYERS];
	int player_flush_stem[MAX_NUMBER_OF_PLAYERS][CLUB+1];
	int player_suit_stem[MAX_NUMBER_OF_PLAYERS];
	
	for (int p = 0; p < number_of_players; p++) {
		int card_1 = hole_cards[2*p];
		int card_2 = hole_cards[2*p + 1];
		
		player_key[p] = deckcardsKey[card_1] + deckcardsKey[card_2];
		
		player_flush_stem[p][SPADE] = (deckcardsSuit[card_1] == SPADE ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == SPADE ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][HEART] = (deckcardsSuit[card_1] == HEART ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == HEART ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][DIAMOND]	=	(deckcardsSuit[card_1] == DIAMOND ? deckcardsFlush[card_1] : 0) +
                                    (deckcardsSuit[card_2] == DIAMOND ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][CLUB] = (deckcardsSuit[card_1] == CLUB ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == CLUB ? deckcardsFlush[card_2] : 0);
		
		player_suit_stem[p] = deckcardsSuit[hole_cards[2*p]] + deckcardsSuit[hole_cards[2*p + 1]];
	}
	////////
	
	int table_card_1 = table_cards[0];
	int table_card_2 = table_cards[1];
	int table_card_3 = table_cards[2];
	
	unsigned int flop_table_key	= deckcardsKey[table_card_1] + deckcardsKey[table_card_2]+ deckcardsKey[table_card_3];
		
	int number_of_players_sharing_the_pot = 1;
	int current_best_rank;
	int next_rank;
	
	// Begin calculating equities by dealing out all possible combinations of five table cards, adding up the incremental equities to find the total integral
  // equities.
	
	int i=1;
	do {
		unsigned int table_card_4 = undealtCards[i];
		unsigned int table_key_cumulative_to_4 = flop_table_key + deckcardsKey[table_card_4];
		
		int j=0;
		do {
			unsigned int table_card_5 = undealtCards[j];
			unsigned int table_key = table_key_cumulative_to_4 + deckcardsKey[table_card_5];
			
			// Reset table_flush_key
			int table_flush_key = UNVERIFIED;
			//Reset number of interested players
			number_of_players_sharing_the_pot = 1;
			////////
			
			unsigned int KEY = table_key + player_key[0];
			
			int FLUSH_CHECK_KEY = (KEY & SUIT_BIT_MASK);
			int FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
			
			if (FLUSH_SUIT == NOT_A_FLUSH) {
				KEY >>= NON_FLUSH_BIT_SHIFT;
				
				current_best_rank = (KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN]);
				player_rank[0] = current_best_rank;
			} else {
				(table_flush_key == UNVERIFIED ? table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                                           (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                                           (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                                           (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                                           (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO );
				
				int player_flush_key = table_flush_key + player_flush_stem[0][FLUSH_SUIT];
				
				current_best_rank = flushRankArray[player_flush_key];
				player_rank[0] = current_best_rank;
			}
			
			int n=1;
			do {
				KEY = table_key + player_key[n];
				
				FLUSH_CHECK_KEY = (KEY & SUIT_BIT_MASK);
				FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
				
				if (FLUSH_SUIT == NOT_A_FLUSH) {
					KEY >>= NON_FLUSH_BIT_SHIFT;
					next_rank = (KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN]);
					player_rank[n] = next_rank;
				} else {
					//** This could be a good place to optimize **
					(table_flush_key == UNVERIFIED ? table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                                             (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                                             (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                                             (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                                             (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO);
					
					int player_flush_key = table_flush_key + player_flush_stem[n][FLUSH_SUIT];
					
					next_rank = flushRankArray[player_flush_key];
					player_rank[n] = next_rank;
				}
				
				////Compare the player_rank
				//
				//Case: new outright strongest player
				if (current_best_rank < next_rank) {
					//Update current best rank
					current_best_rank = next_rank;
					//Reset number of players interested
					number_of_players_sharing_the_pot = 1;
				} else if (current_best_rank == next_rank) { //Case: pot shared with another player
					//Increment by 1 the number of interest players
					number_of_players_sharing_the_pot++;
				}
				//Last Case: new player loses
				//There is nothing we need to do.
				///////
				n++;
				
			} while (n < number_of_players);
			
			//Calculate incremental equity
			int incremental_equity = equity_share[number_of_players_sharing_the_pot];
			
			//Add to the cumulative equities
			int p=0;
			do {
				if (player_rank[p] == current_best_rank) {
          equity[p] += incremental_equity;
        }
				p++;
				
			} while (p < number_of_players);
			j++;
		} while (j<i);
    i++;
	} while (i < number_of_undealt_cards);
	
	unsigned long total_equity = 0;
	
	// Add up all the allocated equity to find the total equity.
	for (int i = 0; i < number_of_players; i++) {
    total_equity += equity[i];
  }
	
	double equity_percentage[MAX_NUMBER_OF_PLAYERS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    equity_percentage[i] = 100 * (equity[i] + 0.0) / total_equity;
  }
	
	return [NSString stringWithFormat:@"%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f", equity_percentage[0], equity_percentage[1], equity_percentage[2],
          equity_percentage[3], equity_percentage[4], equity_percentage[5], equity_percentage[6], equity_percentage[7]];	
}

- (NSString *)computeTurnEquityForSpecificHoleCards:(int *)hole_cards withTableCards:(int *)table_cards andNumberOfPlayers:(int)number_of_players {
	//Count and list the undealt cards, ie. the remaining deck
	int undealtCards[DECK_SIZE];
	int number_of_undealt_cards = 0;
	
	for(int i = 0; i < DECK_SIZE; i++) {
		bool bool_add_card = YES;
		
		for (int j = 0; j < 2 * number_of_players; j++) {
			if (hole_cards[j] == i) {
        bool_add_card = NO;
      }
    }
		
		for (int j = 0; j < MAX_NUMBER_OF_TABLE_CARDS; j++) {
			if (table_cards[j] == i) {
        bool_add_card = NO;
      }
    }
		
		if (bool_add_card == YES) {
      undealtCards[number_of_undealt_cards] = i;
      number_of_undealt_cards++;
    }
	}
	////////////
	
	// Cumulative equities
	unsigned long equity[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    equity[i] = 0;
  }
	////////////
	
	// Record player_rank
	int player_rank[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    player_rank[i] = 0;
  }
	////////////
	
	// Possible shares in equity
	int equity_share[MAX_NUMBER_OF_PLAYERS + 1];
	equity_share[0] = 0;
	for(int i = 1; i < MAX_NUMBER_OF_PLAYERS + 1; i++) {
    equity_share[i] = CAKE/i;
  }
	///////////
	
	//**Player table key and suit stems
	unsigned int player_key[MAX_NUMBER_OF_PLAYERS];
	int player_flush_stem[MAX_NUMBER_OF_PLAYERS][CLUB+1];
	int player_suit_stem[MAX_NUMBER_OF_PLAYERS];
	
	for(int p = 0; p < number_of_players; p++) {
		int card_1 = hole_cards[2*p];
		int card_2 = hole_cards[2*p + 1];
		
		player_key[p] = deckcardsKey[card_1] + deckcardsKey[card_2];
		
		player_flush_stem[p][SPADE]	=	(deckcardsSuit[card_1] == SPADE ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == SPADE ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][HEART] = (deckcardsSuit[card_1] == HEART ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == HEART ? deckcardsFlush[card_2] : 0);
		player_flush_stem[p][DIAMOND] =	(deckcardsSuit[card_1] == DIAMOND ? deckcardsFlush[card_1] : 0) +
                                    (deckcardsSuit[card_2] == DIAMOND ? deckcardsFlush[card_2] : 0);
    player_flush_stem[p][CLUB] = (deckcardsSuit[card_1] == CLUB ? deckcardsFlush[card_1] : 0) + (deckcardsSuit[card_2] == CLUB ? deckcardsFlush[card_2] : 0);
		
		player_suit_stem[p] = deckcardsSuit[hole_cards[2*p]] + deckcardsSuit[hole_cards[2*p + 1]];
  }
	////////
	
	int table_card_1 = table_cards[0];
	int table_card_2 = table_cards[1];
	int table_card_3 = table_cards[2];
	int table_card_4 = table_cards[3];
	
	unsigned int turn_table_key_stem = deckcardsKey[table_card_1] + deckcardsKey[table_card_2] + deckcardsKey[table_card_3] + deckcardsKey[table_card_4];
	
	int number_of_players_sharing_the_pot = 1;
	int current_best_rank = 0, next_rank = 0;
	
	//Begin calculating equities by dealing out all possible
	//combinations of five table cards, adding up the incremental
	//equities to find the total integral equities.
			
		int i = 0;
		do {
			unsigned int table_card_5 = undealtCards[i];
			unsigned int table_key = turn_table_key_stem + deckcardsKey[table_card_5];
			
			// Reset table_flush_key
			int table_flush_key = UNVERIFIED;
			// Reset number of interested players
			number_of_players_sharing_the_pot = 1;
			////////
			
			unsigned int KEY = table_key + player_key[0];
			
			int FLUSH_CHECK_KEY = (KEY & SUIT_BIT_MASK);
			
			int FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
			
			if (FLUSH_SUIT == NOT_A_FLUSH) {
				KEY >>= NON_FLUSH_BIT_SHIFT;
				current_best_rank = KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN];
				player_rank[0] = current_best_rank;
				
			} else {
				(table_flush_key == UNVERIFIED ? table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                                           (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                                           (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                                           (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                                           (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO);
				int player_flush_key = table_flush_key + player_flush_stem[0][FLUSH_SUIT];
				current_best_rank = flushRankArray[player_flush_key];
				player_rank[0] = current_best_rank;
			}
			
			int n = 1;
			do {	
				KEY = table_key + player_key[n];
				
				FLUSH_CHECK_KEY = (KEY & SUIT_BIT_MASK);
				FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
				
				if (FLUSH_SUIT == NOT_A_FLUSH) {
					KEY >>= NON_FLUSH_BIT_SHIFT;
					next_rank = KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN];
					player_rank[n] = next_rank;
				} else {
					//** This could be a good place to optimize **
					(table_flush_key == UNVERIFIED ? table_flush_key = (deckcardsSuit[table_card_1] == FLUSH_SUIT ? deckcardsFlush[table_card_1] : 0) +
                                                             (deckcardsSuit[table_card_2] == FLUSH_SUIT ? deckcardsFlush[table_card_2] : 0) +
                                                             (deckcardsSuit[table_card_3] == FLUSH_SUIT ? deckcardsFlush[table_card_3] : 0) +
                                                             (deckcardsSuit[table_card_4] == FLUSH_SUIT ? deckcardsFlush[table_card_4] : 0) +
                                                             (deckcardsSuit[table_card_5] == FLUSH_SUIT ? deckcardsFlush[table_card_5] : 0) : NO);
					int player_flush_key = table_flush_key + player_flush_stem[n][FLUSH_SUIT];
					next_rank = flushRankArray[player_flush_key];
					player_rank[n] = next_rank;
				}
				
				////Compare the player_rank
				//
				//Case: new outright strongest player
				if (current_best_rank < next_rank) {
					// Update current best rank
					current_best_rank = next_rank;
					// Reset number of players interested
					number_of_players_sharing_the_pot = 1;
				} else if (current_best_rank == next_rank) { // Case: pot shared with another player
					// Increment by 1 the number of interest players
					number_of_players_sharing_the_pot++;
				}
				//Last Case: new player loses
				//There is nothing we need to do.
				///////
				n++;
				
			} while (n < number_of_players);
			
			// Calculate incremental equity
			int incremental_equity = equity_share[number_of_players_sharing_the_pot];
			
			// Add to the cumulative equities
			int p = 0;
			do {
				if (player_rank[p] == current_best_rank) {
          equity[p] += incremental_equity;
        }
				p++;
			} while (p < number_of_players);
			i++;
	} while (i < number_of_undealt_cards);
	
	unsigned long total_equity = 0;
	
	// Add up all the allocated equity to find the total equity.
	for (int i = 0; i < number_of_players; i++) {
    total_equity += equity[i];
  }
	
	double equity_percentage[MAX_NUMBER_OF_PLAYERS] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
		equity_percentage[i] = 100*(equity[i] + 0.0) / total_equity;
  }
	
	return [NSString stringWithFormat:@"%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f", equity_percentage[0], equity_percentage[1], equity_percentage[2],
          equity_percentage[3], equity_percentage[4], equity_percentage[5], equity_percentage[6], equity_percentage[7]];	
}

- (NSString *)computeRiverEquityForSpecificHoleCards:(int *)hole_cards withTableCards:(int *)table_cards andNumberOfPlayers:(int)number_of_players {
	// Cumulative equities
	unsigned long equity[MAX_NUMBER_OF_PLAYERS];
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    equity[i] = 0;
  }
	////////////
	
	// Record player_rank
	int player_rank[MAX_NUMBER_OF_PLAYERS];
	for(int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++) {
    player_rank[i] = 0;
  }
	////////////
	
	int number_of_players_sharing_the_pot = 1;
	int current_best_rank, next_rank;
	
	// Begin calculating equities by dealing out all possible combinations of five table cards, adding up the incremental equities to find the total integral
  // equities.
	
	// Reset number of interested players
	number_of_players_sharing_the_pot = 1;
		
	int card_1 = hole_cards[0];
	int card_2 = hole_cards[1];
	
	current_best_rank = [self rankOfSeven:card_1 :card_2 :table_cards[0] :table_cards[1] :table_cards[2] :table_cards[3] :table_cards[4]];
	
  player_rank[0] = current_best_rank;
	for(int i = 1; i < number_of_players; i++) {
		card_1 = hole_cards[2*i];
		card_2 = hole_cards[2*i + 1];
			
		next_rank = [self rankOfSeven:card_1 :card_2 :table_cards[0] :table_cards[1] :table_cards[2] :table_cards[3] :table_cards[4]];
		
		player_rank[i] = next_rank;
				
		//**Compare the player_rank
		//
		//Case: new outright strongest player
		if (current_best_rank < next_rank)
    {
			//Update current best rank
			current_best_rank = next_rank;
			//Rest number of players interested
			number_of_players_sharing_the_pot = 1;
    }
		//
		////
		//Case: pot shared with another player
		else if (current_best_rank == next_rank)
    {
			//Increment by 1 the number of interest players
			number_of_players_sharing_the_pot++;
    }
		///////
	}
	
	unsigned long pot_share = CAKE/number_of_players_sharing_the_pot;

	// Add to the cumulative equities
	int p = 0;
	do
  {
		if (player_rank[p] == current_best_rank)
    {
      equity[p] = pot_share;
    }
		p++;
	} while (p < number_of_players);

	unsigned long total_equity = 0;
	
	// Add up all the allocated equity to find the total equity.
	for (int i = 0; i < number_of_players; i++)
  {
    total_equity += equity[i];
  }
	
	double equity_percentage[MAX_NUMBER_OF_PLAYERS] =
                  {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	
	for (int i = 0; i < MAX_NUMBER_OF_PLAYERS; i++)
  {
		equity_percentage[i] = 100*(equity[i] + 0.0) / total_equity;
  }
	
	NSString *string = [NSString stringWithFormat:
                      @"%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f_%.16f",
                      equity_percentage[0],
                      equity_percentage[1],
                      equity_percentage[2],
                      equity_percentage[3],
                      equity_percentage[4],
                      equity_percentage[5],
                      equity_percentage[6],
                      equity_percentage[7]];
	
	return string;
}

  // Crude timing method used to test prospective optimizations.
  // When using this, be sure to uncomment the "hands_array" in
  // the interface/header file and the BIG_NUMBER too.
/*
- (void)timeRankMethod
{
    //short hands_array[28000000];
	int rank = 0;
	
    //Test code
	
	for ( int n = 0 ; n < BIG_NUMBER / 7; n++ ) {
		int temp_array[7];
		temp_array[0] = rand()%52;
		for ( int m = 1 ; m < 7 ; m++ ) {
			bool seen = YES;
			while ( seen ) {
				seen = NO;
				temp_array[m] = rand()%52;
				for ( int k = 0 ; k < m && seen == NO ; k++ ) { seen = ( temp_array[m] == temp_array[k] ? YES : NO ); }
			}
		}
		
      //NSLog(@"%i_%i_%i_%i_%i_%i_%i", temp_array[0], temp_array[1], temp_array[2], temp_array[3], temp_array[4], temp_array[5], temp_array[6]);
		
		int N = 7*n;
		hands_array[N]		= temp_array[0];
		hands_array[N+1]	= temp_array[1];
		hands_array[N+2]	= temp_array[2];
		hands_array[N+3]	= temp_array[3];
		hands_array[N+4]	= temp_array[4];
		hands_array[N+5]	= temp_array[5];
		hands_array[N+6]	= temp_array[6];
		
	}
	
	NSLog(@"Generated random numbers");
	NSLog(@"Testing evaluator...");
  
	clock_t start = clock();
	
    // The incremental additions (N++ and N+=7) here aren't
    // really the work of the evaluator and should be subtracted
    // from the total time to get a fairer estimate of the work.
	for ( int N = 0 ; N < BIG_NUMBER ; N += 7 ) {
		    
		int card_1 = hands_array[N++];
		int card_2 = hands_array[N++];
		int card_3 = hands_array[N++];
		int card_4 = hands_array[N++];
		int card_5 = hands_array[N++];
		int card_6 = hands_array[N++];
		int card_7 = hands_array[N++];
		
		unsigned int KEY=	deckcardsKey[card_1]+
    deckcardsKey[card_2]+
    deckcardsKey[card_3]+
    deckcardsKey[card_4]+
    deckcardsKey[card_5]+
    deckcardsKey[card_6]+
    deckcardsKey[card_7];
		
		int FLUSH_CHECK_KEY = ( KEY & SUIT_BIT_MASK );
		
		int FLUSH_SUIT = flushCheck[FLUSH_CHECK_KEY];
		
		if(FLUSH_SUIT == NOT_A_FLUSH){
			
			KEY = ( KEY >> NON_FLUSH_BIT_SHIFT );
						
			rank = ( KEY < CIRCUMFERENCE_SEVEN ? rankArray[KEY] : rankArray[KEY - CIRCUMFERENCE_SEVEN] );
      
		}
		
		else{
			
        //** This could be a good place to optimize **
			int FLUSH_KEY=	(deckcardsSuit[card_1] == FLUSH_SUIT ? deckcardsFlush[card_1] : 0)+
      (deckcardsSuit[card_2] == FLUSH_SUIT ? deckcardsFlush[card_2] : 0)+
      (deckcardsSuit[card_3] == FLUSH_SUIT ? deckcardsFlush[card_3] : 0)+
      (deckcardsSuit[card_4] == FLUSH_SUIT ? deckcardsFlush[card_4] : 0)+
      (deckcardsSuit[card_5] == FLUSH_SUIT ? deckcardsFlush[card_5] : 0)+
      (deckcardsSuit[card_6] == FLUSH_SUIT ? deckcardsFlush[card_6] : 0)+
      (deckcardsSuit[card_7] == FLUSH_SUIT ? deckcardsFlush[card_7] : 0);
			
			rank = flushRankArray[FLUSH_KEY];
      
		}
    
	}
	
	clock_t finish = clock();
	
    // Time how long it takes to ennumerate the hands.
  clock_t start_adjust = clock();
  for ( int N = 0 ; N < BIG_NUMBER ; N += 7 ) {
    N++;
    N++;
    N++;
    N++;
    N++;
    N++;
    N++;
  }
  clock_t finish_adjust = clock();
  
	NSLog(@"last rank was %i", rank);
	NSLog(@"Evaluator testing finished");
	NSLog(@"time taken: %i clocks", finish-start-(finish_adjust - start_adjust));
	
	float rate = BIG_NUMBER / 7 *(CLOCKS_PER_SEC+0.0)/(finish-start-(finish_adjust - start_adjust)+0.0);
	NSLog(@"evaluations per second: %f", rate);
}
 */

@end
