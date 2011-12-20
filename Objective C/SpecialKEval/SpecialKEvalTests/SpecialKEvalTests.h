/*!
 SpecialKEvalTests.h<br />
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

#import <SenTestingKit/SenTestingKit.h>
#import "HandEval.h"
#import "SevenEval.h"
#import "FiveEval.h"

@interface SpecialKEvalTests : SenTestCase {
 @private
  HandEval *hEval_;
  SevenEval *sEval_;
  FiveEval *fEval_;
}
@end
