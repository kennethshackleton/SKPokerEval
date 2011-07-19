/*!
 SevenEval.h<br />
 SpecialKEval
 <p>
 Copyright 2010 Kenneth J. Shackleton<br />
 codingfeedback@gmail.com<br />
 http://specialk-coding.blogspot.com/
 <p>
 ***********************************************************************
 <i>An evolution of this evaluator has been released under Apple's EULA and
 is behind the app
 <a href="http://itunes.apple.com/us/app/poker-ace/id392530020?mt=8&ls=1">Poker Ace</a>
 available through iTunes Store.</i>
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
 You should have received a copy of the GNU
 <a href="http://www.gnu.org/licenses/">General Public License</a> along
 with this program in a file in the toplevel directory called "GPLv3".
 <p>
 @author Kenneth J. Shackleton
 */

#ifndef SEVENEVAL_H
#define SEVENEVAL_H

#import <Foundation/Foundation.h>

@interface SevenEval : NSObject
{	
 @private
	//Ranks for 7-card evaluation separated
	//into non-flushes and flushes, each with
	//their own respective keys
	unsigned short rankArray[CIRCUMFERENCE_SEVEN];
	unsigned short flushRankArray[MAX_SEVEN_FLUSH_KEY_INT + 1];
  
	//Card face values beginning with ACE_ from
	//index 0 and TWO_ from index 48.
	unsigned int deckcardsKey[DECK_SIZE];
	unsigned short deckcardsFlush[DECK_SIZE];
	unsigned short deckcardsSuit[DECK_SIZE];	
  
	//Array comprising of the flush suits.
	short flushCheck[MAX_FLUSH_CHECK_SUM + 1];
}

/*!
 Singleton instance accessor; always use this rather than creating your own
 instance.
 */
+ (SevenEval*) theEvaluator;
/*!
 Call to release the singleton instance.
 */
+ (void) releaseTheEvaluator;

/*!
 Call this method to assign a rank to a 7-card hand. Each card is an integer
 from 0 (Ace of Spades), 1 (Ace of Hearts) to 51 (Two of Clubs). For performance
 reasons this method does nothing to verify that the input is a valid hand, the
 responsibility for this rests entirely with the caller. The ranks are all
 positive, and the highest rank is 7462. The set of all 7-card ranks is not an
 interval from 1: the worst 7-card poker hand has a rank of 49. Still, the
 higher the rank the stronger the hand, and two hands of equal rank always draw.
 Valid hands, i.e. 7-tuples of pairwise distinct integers from 0 to 51
 inclusive, don't need to be sorted.
 <p>
 It's helpful to think of the ranks as living on an interval which has been
 mapped onto a circle whose circumference is a constant, like thread round a
 cotton wheel. The circumference is chosen so that no two ranks overlap. This
 trick cuts the RAM cost by 40%, to 9MB, with negligible cost in performance.
 <p>
 The method now works as follows. First, it looks up a pre-calculated 32-bit
 integer card key for each of the seven parameters and adds these all together
 to generate a <i>hand key</i>. It then breaks up the hand key in two pieces,
 one a 23-bit integer (the <i>face key</i>) and the other a 9-bit integer (the
 <i>flush check key</i>), using bitshifts and masks. We look up the value in
 the flushCheck array placed (flush check key)th. This value is either a suit
 value, the suit of the flush in the event the suit is a flush of some kind,
 or a common non-suit value otherwise.
 <p>
 Usually, about 97% of the time, the hand does not contain a flush. If the hand
 is not a flush we replace the face key with its value modulo the circumference
 constant. We look up the entry in the rankArray whose index is this new face
 key. The value we find is exactly the rank, in the event the hand is not a
 flush. Otherwise, if the hand is a flush, we form a new <i>flush key</i> for
 the hand (the sum of five, six or seven non-zero integers). We look up the
 value positioned (flush key)th in the flushRankArray. This is the rank of the
 hand, in the event it's a flush.
 <p>
 Typically this method will return the rank of a valid 7-card hand with no more
 than 6 additions.
 <p>
 When applying this to equity calculations we recommend taking the method as a
 block of code and developing it into a custom method. This saves method calls
 which, while not costly individually, accumulate to appreciably hamper
 performance. Specific applications also often have their own combinatorics
 which you can exploit.
 */
- (short) getRankOfSeven:(const int) CARD1 :(const int) CARD2
                        :(const int) CARD3 :(const int) CARD4
                        :(const int) CARD5 :(const int) CARD6
                        :(const int) CARD7;

@end

#endif