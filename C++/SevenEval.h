/*
 *  SevenEval.h
 *  SpecialKEvalCPP
 *
 *  Created 26 April 2011.
 *  Copyright 2011 Kenneth J. Shackleton.
 *  All rights reserved.
 */

#ifndef SEVENEVAL_H
#define SEVENEVAL_H

#include "Constants.h"

class SevenEval {
 private:
	
	unsigned short rankArray[CIRCUMFERENCE_SEVEN];
	unsigned short flushRankArray[MAX_SEVEN_FLUSH_KEY_INT+1];
	unsigned long deckcardsKey[DECK_SIZE];
	unsigned short deckcardsFlush[DECK_SIZE];
	unsigned short deckcardsSuit[DECK_SIZE];	
	short flushCheck[MAX_FLUSH_CHECK_SUM+1];
  
 public:
	/*!
	 Constructor.
	 */
  SevenEval();
  
	/*!
	 Destructor.
	 */
  ~SevenEval();
  
  short getRankOfSeven(const int CARD1, const int CARD2, const int CARD3, const int CARD4, const int CARD5, const int CARD6, const int CARD7);
  void validateAgainstFiveEval();
};

#endif
