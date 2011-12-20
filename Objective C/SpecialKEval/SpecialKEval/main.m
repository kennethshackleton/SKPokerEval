/*!
 main.m
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

#import <Cocoa/Cocoa.h>
#import "HandEval.h"

int main(int argc, char *argv[]) {
  NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
  HandEval *e = [[HandEval alloc] init];
  int h[4] = {0, 11, 37, 38};
  NSLog(@"%@", [e computePreFlopEquityForSpecificHoleCards:h withNumberOfPlayers:2]);
  [pool drain];
  return NSApplicationMain(argc, (const char **)argv);
}
