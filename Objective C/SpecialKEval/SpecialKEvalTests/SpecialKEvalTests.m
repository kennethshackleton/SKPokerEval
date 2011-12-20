/*!
 SpecialKEvalTests.m<br />
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

#import "SpecialKEvalTests.h"
#import "SevenEval.h"
#import "FiveEval.h"

@implementation SpecialKEvalTests

- (void)setUp {
  [super setUp];
  hEval_ = [[HandEval alloc] init];
  sEval_ = [SevenEval sharedEvaluator];
  fEval_ = [FiveEval sharedEvaluator];
}

- (void)tearDown {
  hEval_ = nil;
  sEval_ = nil;
  fEval_ = nil;
  [super tearDown];
}

/*
- (void)testHandEvaluator {
  char s1[BUFSIZ], s2[BUFSIZ], s3[BUFSIZ];
  char line[BUFSIZ];
  FILE *file = fopen([[[NSBundle mainBundle] pathForResource:@"HandEvalUnitTests" ofType:@"txt"] UTF8String], "r");
  if (file != NULL) {
    while (fgets(line, MAX(0, BUFSIZ-1), file) != NULL) {
      if (strlen(line) > 2 && line[0] != '/' && line[1] != '/') { // Check that the line is long enough and is not a comment.
        sscanf(line, "%s\t%s\t%s", &s1[0], &s2[0], &s3[0]);
        NSString *S2 = [NSString stringWithCString:s1 encoding:NSUTF8StringEncoding], *nS2 = nil;
        @try {
          nS2 = [S2 stringByReplacingAllHiraganaWithKatakana];
        } @catch (NSException *e) {
          NSLog(@"%@", e.description);
        }
        STAssertTrue(nS2 != nil, @"Image string from %@ is nil", S2);
        STAssertTrue(strlen([nS2 UTF8String]), @"Image has of %@ no length", S2);
        STAssertTrue(strcmp([nS2 UTF8String], s2) == 0, @"%@ not mapped to %s", s1, s2);
      }
    }
  }
  fclose(file);
}
 */

- (void)testSevenEvalRankingAgainstFiveEvalSevenCardRanking {
	for (int i = 6; i < 52; i++) {
    for (int j = 5; j < i; j++) {
      NSLog(@"%i, %i", i, j);
			for (int k = 4; k < j; k++) {
				for (int l = 3; l < k; l++) {
					for (int m = 2; m < l; m++) {
						for(int n = 1; n < m; n++) {
							for (int p = 0; p < n; p++) {
								int rank5 = [fEval_ rankOfSeven:i :j :k :l :m :n :p];
								int rank7 = [sEval_ rankOfSeven:i :j :k :l :m :n :p];
								if (rank5 != rank7) {
                  STAssertTrue(rank5 == rank7, @"%i %i %i %i %i %i %i mismatched", i, j, k, l, m, n, p);
                }
							}
						}
					}
				}
			}
		}
	}
}

@end
