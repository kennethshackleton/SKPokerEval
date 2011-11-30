/*!
 HandEval.h
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

#ifndef HANDEVAL_H
#define HANDEVAL_H

#include "Constants.h"

  // Uncomment when testing. Make this number is
  // very large, to allow the evaluator to get up
  // to speed, and divisible by 7. Also close any
  // other applications that are open.
  #define BIG_NUMBER 300000000*7

class HandEval {
 private:
  
    // Uncomment when timing.
    char hands_array[BIG_NUMBER];
  
    /*!
     Ranks for 7-card evaluation separated
     into non-flushes and flushes, each with
     their own respective keys
     */
	unsigned short rankArray[CIRCUMFERENCE_SEVEN];
	unsigned short flushRankArray[MAX_SEVEN_FLUSH_KEY_INT+1];
  
    /*!
     Card face values beginning with ACE_ from
     index 0 and TWO_ from index 48.
     */
	unsigned int deckcardsKey[DECK_SIZE];
	unsigned short deckcardsFlush[DECK_SIZE];
	unsigned short deckcardsSuit[DECK_SIZE];	
  
    /*!
     Array comprising of the flush suits.
     */
	short flushCheck[MAX_FLUSH_CHECK_SUM + 1];
  
    /*!
     Generators, called during construction.
     */
  void generateDeckKeys();
  void generateRanks();
  void generateFlushCheck();
  
    /*!
     Ranking method.
     */
  short getRankOfSeven(const int card_1,
                       const int card_2,
                       const int card_3,
                       const int card_4,
                       const int card_5,
                       const int card_6,
                       const int card_7);
  
public:
  /*!
   Constructor.
   */
  HandEval();
  
  /*!
   Destructor.
   */
  ~HandEval();
  
  /*!
   Equity evalaution methods.
   */
  const char * computePreFlopEquityForSpecificHoleCards (int * holeCards,
                                                         int number_of_players);
  
  const char * computeFlopEquityForSpecificCards (int * holeCards,
                                                  int * tableCards,
                                                  int number_of_players);
  
  const char * computeTurnEquityForSpecificCards (int * hole_cards,
                                                  int * table_cards,
                                                  int number_of_players);
  
  const char * computeRiverEquityForSpecificCards (int * hole_cards,
                                                   int * table_cards,
                                                   int number_of_players);
  
  // Uncomment when timing the performance.
  void timeRankMethod();
};

#endif
