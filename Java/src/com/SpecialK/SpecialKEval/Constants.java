package com.SpecialK.SpecialKEval;

public class Constants
{
	public static final int BIG_NUMBER=100000000*7;
	
	public static final int DECK_SIZE=52;

	public static final int NUMBER_OF_SUITS=4;
	public static final int NUMBER_OF_FACES=13;

	public static final int SPADE=0;
	public static final int HEART=1;
	public static final int DIAMOND=8;
	public static final int CLUB=57;

	public static final int TWO_FIVE=0;
	public static final int THREE_FIVE=1;
	public static final int FOUR_FIVE=5;
	public static final int FIVE_FIVE=22;
	public static final int SIX_FIVE=94;
	public static final int SEVEN_FIVE=312;
	public static final int EIGHT_FIVE=992;
	public static final int NINE_FIVE=2422;
	public static final int TEN_FIVE=5624;
	public static final int JACK_FIVE=12522;
	public static final int QUEEN_FIVE=19998;
	public static final int KING_FIVE=43258;
	public static final int ACE_FIVE=79415;

	public static final int TWO_FLUSH=1;
	public static final int THREE_FLUSH=2;
	public static final int FOUR_FLUSH=4;
	public static final int FIVE_FLUSH=8;
	public static final int SIX_FLUSH=16;
	public static final int SEVEN_FLUSH=32;
	public static final int EIGHT_FLUSH=64;
	public static final int NINE_FLUSH=EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+THREE_FLUSH+TWO_FLUSH+1;
	public static final int TEN_FLUSH=NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+THREE_FLUSH+1;
	public static final int JACK_FLUSH=TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+1;
	public static final int QUEEN_FLUSH=JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+1;
	public static final int KING_FLUSH=QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+1;
	public static final int ACE_FLUSH=KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+1;

	public static final int MAX_FIVE_NONFLUSH_KEY_INT=4*ACE_FIVE+KING_FIVE;
	public static final int MAX_SEVEN_FLUSH_KEY_INT=ACE_FLUSH+KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH;

	//_SEVEN tag suppressed
	public static final int TWO=0;
	public static final int THREE=1;
	public static final int FOUR=5;
	public static final int FIVE=22;
	public static final int SIX=98;
	public static final int SEVEN=453;
	public static final int EIGHT=2031;
	public static final int NINE=8698;
	public static final int TEN=22854;
	public static final int JACK=83661;
	public static final int QUEEN=262349;
	public static final int KING=636345;
	public static final int ACE=1479181;
	//end of _SEVEN tag suppressed

	public static final int MAX_NONFLUSH_KEY_INT=4*ACE+3*KING;
	public static final int MAX_FLUSH_KEY_INT=ACE_FLUSH+KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH;
	public static final int MAX_KEY_INT=MAX_NONFLUSH_KEY_INT+MAX_FLUSH_KEY_INT;

	public static final int MAX_FLUSH_CHECK_SUM=7*CLUB;

	public static final short L_WON=-1;
	public static final short R_WON=1;
	public static final short DRAW=0;

	public static final int CIRCUMFERENCE_FIVE=187853;
	public static final int CIRCUMFERENCE_SEVEN=4565145;
	
	//Bit masks
	public static final int SUIT_BIT_MASK=511;
	public static final int NON_FLUSH_BIT_SHIFT=9;

	/////////
	//The following are used with NSAssert for
	//debugging, ignored by release mode
	public static final short RANK_OF_A_WORST_HAND=0;
	public static final short RANK_OF_WORST_STRAIGHT=5854;
	public static final short RANK_OF_BEST_STRAIGHT=5863;
	public static final short RANK_OF_WORST_FLUSH=5864;
	public static final short RANK_OF_BEST_FLUSH=7140;
	public static final short RANK_OF_WORST_STRAIGHT_FLUSH=7453;
	public static final short RANK_OF_BEST_STRAIGHT_FLUSH=7462;
	public static final short RANK_OF_A_BEST_HAND=RANK_OF_BEST_STRAIGHT_FLUSH;

	public static final int KEY_COUNT=53924;
	public static final int NON_FLUSH_KEY_COUNT=49205;
	public static final int FLUSH_KEY_COUNT=4719;
	
	//Used in flush checking
	public static final int UNVERIFIED=-2;
	public static final int NOT_A_FLUSH=-1;
	public static final int NO_FLUSH_POSSIBLE=-1;
	/////////
	
	//Limits
	public static final int MAX_NUMBER_OF_UNDEALT_CARDS=DECK_SIZE-4;
	public static final int MAX_NUMBER_OF_PLAYERS=8;
	
	//Equity
	public static final int CAKE=840;
	public static final int EQUITY_TWO=1438335360;
	public static final int EQUITY_THREE=1151433360;
	public static final int EQUITY_FOUR=912246720;
	public static final int EQUITY_FIVE=714561120;
	public static final int EQUITY_SIX=552726720;
	public static final int EQUITY_SEVEN=421631280;
	public static final int EQUITY_EIGHT=316673280;
}
