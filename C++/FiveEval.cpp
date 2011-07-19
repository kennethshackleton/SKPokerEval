/*
 *  FiveEval.cpp
 *  SpecialKEvalCPP
 *
 *  Created 26 April 2011.
 *  Copyright 2011 Kenneth J. Shackleton.
 *  Ringo Limited.
 *  All rights reserved.
 */

#include "FiveEval.h"

#pragma mark -
#pragma mark Constructor
#pragma mark -

FiveEval::FiveEval()
{
		//Enter face values into arrays
	int face[13] = { TWO_FIVE, THREE_FIVE, FOUR_FIVE, FIVE_FIVE, SIX_FIVE,
									 SEVEN_FIVE, EIGHT_FIVE, NINE_FIVE, TEN_FIVE, JACK_FIVE,
									 QUEEN_FIVE, KING_FIVE, ACE_FIVE };
	
	int faceFlush[13] = { TWO_FLUSH, THREE_FLUSH, FOUR_FLUSH, FIVE_FLUSH,
												SIX_FLUSH, SEVEN_FLUSH, EIGHT_FLUSH, NINE_FLUSH,
												TEN_FLUSH, JACK_FLUSH, QUEEN_FLUSH, KING_FLUSH,
												ACE_FLUSH };
	
	for (int n = 0; n < 13; n++)
	{
		deckcardsSuit[4*n]		 = SPADE;
		deckcardsSuit[4*n + 1] = HEART;
		deckcardsSuit[4*n + 2] = DIAMOND;
		deckcardsSuit[4*n + 3] = CLUB;
		
		deckcardsFace[4*n] = face[12 - n];
		deckcardsFace[4*n + 1] = face[12 - n];
		deckcardsFace[4*n + 2] = face[12 - n];
		deckcardsFace[4*n + 3] = face[12 - n];
		
		deckcardsFlush[4*n] = faceFlush[12 - n];
		deckcardsFlush[4*n + 1] = faceFlush[12 - n];
		deckcardsFlush[4*n + 2] = faceFlush[12 - n];
		deckcardsFlush[4*n + 3] = faceFlush[12 - n];
	}
	
		// n increments as rank
	int n = 1;
	
		// Zero out.
	for (int i = 0; i < MAX_FIVE_NONFLUSH_KEY_INT + 1; i++)
	{
    rankArray[i] = 0;
	}
	
		// High card
	for (int i = 5; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 3; j < i; j++)
		{
			for (int k = 2; k < j; k++)
			{
				for (int l = 1; l < k; l++)
				{
						// No straights
					for (int m = 0;
               m < l &&
               !(i-m == 4 || (i == 12 && j == 3 && k == 2 && l == 1 && m == 0));
               m++)
					{
						rankArray[face[i] + face[j] + face[k] + face[l] + face[m]] = n;
            n++;
					}
				}
			}
		}
	}
	
		// Pair
	for (int i = 0; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 2; j < NUMBER_OF_FACES; j++)
		{
			for (int k = 1; k < j; k++)
			{
				for (int l = 0; l < k; l++)
				{
					if (i != j && i != k && i != l)
					{
						rankArray[(2*face[i]) + face[j] + face[k] + face[l]] = n;
						n++;
					}
				}
			}
		}
	}
	
		// Two pair
	for (int i = 1; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 0; j < i; j++)
		{
			for (int k = 0; k < NUMBER_OF_FACES; k++)
			{
					// No fullhouse
				if (k!=i && k!=j)
				{
					rankArray[(2*face[i]) + (2*face[j]) + face[k]] = n;
					n++;
				}
			}
		}
	}
	
		// Triple
	for (int i = 0; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 1; j < NUMBER_OF_FACES; j++)
		{
			for (int k = 0; k < j; k++)
			{
					// No quad
				if (i != j && i != k)
				{		
					rankArray[(3*face[i]) + face[j] + face[k]] = n;
					n++;
				}
			}
		}
	}
	
		// Low straight nonflush
	rankArray[face[12] + face[0] + face[1] + face[2] + face[3]] = n;
  n++;
	
		// Usual straight nonflush
	for (int i = 0; i < 9; i++)
	{
		rankArray[face[i] + face[i+1] + face[i+2] + face[i+3] + face[i+4]] = n;
    n++;
	}
	
		// Flush not a straight
	for (int i = 5; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 3; j < i; j++)
		{
			for (int k = 2; k < j; k++)
			{
				for (int l = 1; l <k ; l++)
				{
					for (int m = 0; m < l; m++)
					{
						if (!(i-m == 4 ||
                  (i == 12 && j == 3 && k == 2 && l == 1 && m == 0)))
						{
							flushRankArray[faceFlush[i] + faceFlush[j] + faceFlush[k] +
                             faceFlush[l] + faceFlush[m]] = n;
							n++;
						}
					}
				}
			}
		}
	}
	
		// Full house
	for (int i = 0; i < NUMBER_OF_FACES; i++)
		for (int j = 0; j < NUMBER_OF_FACES; j++)
		{
			if (i !=j )
			{
				rankArray[(3*face[i]) + (2*face[j])] = n;
				n++;
			}
		}
	
		// Quad
	for (int i = 0; i < NUMBER_OF_FACES; i++)
	{									
		for (int j = 0; j < NUMBER_OF_FACES; j++)
		{
			if (i != j)
			{
				rankArray[(4*face[i]) + face[j]] = n;
				n++;
			}
		}
	}
	
		// Low straight flush
	flushRankArray[faceFlush[0] + faceFlush[1] + faceFlush[2] + faceFlush[3] +
                 faceFlush[12]] = n;
	n++;
	
		// Usual straight flush
	for (int i = 0; i < 9; i++)
	{
		flushRankArray[faceFlush[i] + faceFlush[i+1] + faceFlush[i+2] +
                   faceFlush[i+3] + faceFlush[i+4]] = n;
		n++;
	}
}

#pragma mark -
#pragma mark Destructor
#pragma mark -

FiveEval::~FiveEval() {}

#pragma mark -
#pragma mark Ranker
#pragma mark -

short FiveEval::getRankOfFive (const int CARD1,
															 const int CARD2,
															 const int CARD3,
															 const int CARD4, 
															 const int CARD5)
{	
	if ((deckcardsSuit[CARD1] == deckcardsSuit[CARD2]) &&
	    (deckcardsSuit[CARD1] == deckcardsSuit[CARD3]) &&
	    (deckcardsSuit[CARD1] == deckcardsSuit[CARD4]) &&
	    (deckcardsSuit[CARD1] == deckcardsSuit[CARD5]))
	{
		return flushRankArray[deckcardsFlush[CARD1] +
													deckcardsFlush[CARD2] +
                          deckcardsFlush[CARD3] +
													deckcardsFlush[CARD4] +
                          deckcardsFlush[CARD5]];
	}
	else
	{
		return rankArray[deckcardsFace[CARD1] + deckcardsFace[CARD2] +
                     deckcardsFace[CARD3] + deckcardsFace[CARD4] +
                     deckcardsFace[CARD5]];
	}

	return -1;
}

short FiveEval::getRankOfSeven (const int CARD1,
																const int CARD2,
																const int CARD3,
																const int CARD4,
																const int CARD5,
																const int CARD6,
																const int CARD7)
{
	int seven_cards[7] = {CARD1, CARD2, CARD3, CARD4, CARD5, CARD6, CARD7};
	int five_temp[5];
	
	short BEST_RANK_SO_FAR = 0, CURRENT_RANK = 0;
	int m = 0;
  
	for(int i = 1; i < 7; i++)
	{
		for(int j = 0; j < i; j++)
		{
      m = 0;
			for(int k = 0; k < 7; k++)
			{
				if (k != i && k !=j)
				{
					five_temp[m] = seven_cards[k];
					m++;
				}
			}
			CURRENT_RANK = getRankOfFive(five_temp[0],
																	 five_temp[1],
																	 five_temp[2],
																	 five_temp[3],
																	 five_temp[4]);
			if (BEST_RANK_SO_FAR < CURRENT_RANK)
			{
        BEST_RANK_SO_FAR = CURRENT_RANK;
			}
		}
	}
	return BEST_RANK_SO_FAR;
}