/*
 *  FiveEval.h
 *  SpecialKEvalCPP
 *
 *  Created 26 April 2011.
 *  Copyright 2011 Kenneth J. Shackleton.
 *  Ringo Limited.
 *  All rights reserved.
 *
 */

#ifndef FIVEEVAL_H
#define FIVEEVAL_H

#include "Constants.h"

class FiveEval
{
 private:
		/*!
		 Rank look-up tables.
		 */
	unsigned short rankArray[MAX_FIVE_NONFLUSH_KEY_INT + 1];
	unsigned short flushRankArray[MAX_FIVE_FLUSH_KEY_INT + 1];
  
		/*!
		 Card face values beginning with ACE_ from index 0 and TWO_ from index 48.
		 */
	unsigned int deckcardsFace[DECK_SIZE];
	unsigned short deckcardsFlush[DECK_SIZE];
	unsigned short deckcardsSuit[DECK_SIZE];
 
 public:
		/*!
		 Constructor.
		 */
  FiveEval();
  
		/*!
		 Destructor.
		 */
  ~FiveEval();
  
		/*!
		 Returns an instance of the 5-card evaluator; call this rather than
		 initialising your own.
		 */
  FiveEval * theEvaluator();
  
		/*!
		 Call to release the singleton instance.
		 */
  void releaseTheEvaluator();
  
		/*!
		 The set of all 5-card hand ranks is an interval from 1 to 7462 inclusive.
		 */
  short getRankOfFive(const int CARD1, const int CARD2, const int CARD3,
                      const int CARD4, const int CARD5);
  
		/*!
		 The corresponding method in SevenEval.h is superior. This one is only used
		 to initialise the 7-card evaluator as it carries of the overhead of having
		 to rank the twenty-one 5-card hands in a 7-card hand.
		 */
  short getRankOfSeven(const int CARD1, const int CARD2, const int CARD3,
                       const int CARD4, const int CARD5, const int CARD6,
                       const int CARD7);
};

#endif
