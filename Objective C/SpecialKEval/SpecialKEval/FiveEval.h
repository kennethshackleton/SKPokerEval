/*!
 FiveEval.h<br />
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

#import <Foundation/Foundation.h>

@interface FiveEval : NSObject {
 @private	
	// Ranks for 5-card evaluation separated into non-flushes and flushes, each with their own respective keys
	unsigned short rankArray[MAX_FIVE_NONFLUSH_KEY_INT+1];
	unsigned short flushRankArray[MAX_SEVEN_FLUSH_KEY_INT+1];

	// Card face values beginning with ACE_ from index 0 and TWO_ from index 48
	unsigned int deckcardsFace[DECK_SIZE];
	unsigned short deckcardsFlush[DECK_SIZE];
	unsigned short deckcardsSuit[DECK_SIZE];
}

/*!
 Returns an instance of the 5-card evaluator; call this rather than initialising
 your own.
 */
+ (id)sharedEvaluator;

/*!
 Call to release the singleton instance.
 */
+ (void)releaseToNilSharedEvaluator;

/*!
 The set of all 5-card hand ranks is ranges from 1 to 7462 inclusive.
 */
- (short)rankOfFive:(const int)CARD1 :(const int)CARD2 :(const int)CARD3 :(const int)CARD4 :(const int)CARD5;

/*!
 The corresponding method in SevenEval.h is superior. This one is only used to initialise the 7-card evaluator as it carries of the overhead of having to
 rank the twenty-one 5-card hands in a 7-card hand.
 */
- (short)rankOfSeven:(const int)CARD1 :(const int)CARD2 :(const int)CARD3 :(const int)CARD4 :(const int)CARD5 :(const int)CARD6 :(const int)CARD7;

@end