package com.SpecialK.SpecialKEval;

public interface Constants {
	public final int DECK_SIZE=52;

	public final int NUMBER_OF_SUITS=4;
	public final int NUMBER_OF_FACES=13;

	public final int SPADE=0;
	public final int HEART=1;
	public final int DIAMOND=8;
	public final int CLUB=57;

	public final int TWO_FIVE=0;
	public final int THREE_FIVE=1;
	public final int FOUR_FIVE=5;
	public final int FIVE_FIVE=22;
	public final int SIX_FIVE=94;
	public final int SEVEN_FIVE=312;
	public final int EIGHT_FIVE=992;
	public final int NINE_FIVE=2422;
	public final int TEN_FIVE=5624;
	public final int JACK_FIVE=12522;
	public final int QUEEN_FIVE=19998;
	public final int KING_FIVE=43258;
	public final int ACE_FIVE=79415;

	public final int TWO_FLUSH=1;
	public final int THREE_FLUSH=2;
	public final int FOUR_FLUSH=4;
	public final int FIVE_FLUSH=8;
	public final int SIX_FLUSH=16;
	public final int SEVEN_FLUSH=32;
	public final int EIGHT_FLUSH=64;
	public final int NINE_FLUSH=EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+THREE_FLUSH+TWO_FLUSH+1;
	public final int TEN_FLUSH=NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+THREE_FLUSH+1;
	public final int JACK_FLUSH=TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+FOUR_FLUSH+1;
	public final int QUEEN_FLUSH=JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+FIVE_FLUSH+1;
	public final int KING_FLUSH=QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+SIX_FLUSH+1;
	public final int ACE_FLUSH=KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH+SEVEN_FLUSH+1;

	public final int MAX_FIVE_NONFLUSH_KEY_INT=4*ACE_FIVE+KING_FIVE;
	public final int MAX_SEVEN_FLUSH_KEY_INT=ACE_FLUSH+KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH;

	//_SEVEN tag suppressed
	public final int TWO=0;
	public final int THREE=1;
	public final int FOUR=5;
	public final int FIVE=22;
	public final int SIX=98;
	public final int SEVEN=453;
	public final int EIGHT=2031;
	public final int NINE=8698;
	public final int TEN=22854;
	public final int JACK=83661;
	public final int QUEEN=262349;
	public final int KING=636345;
	public final int ACE=1479181;
	//end of _SEVEN tag suppressed

	public final int MAX_NONFLUSH_KEY_INT=4*ACE+3*KING;
	public final int MAX_FLUSH_KEY_INT=ACE_FLUSH+KING_FLUSH+QUEEN_FLUSH+JACK_FLUSH+TEN_FLUSH+NINE_FLUSH+EIGHT_FLUSH;
	public final int MAX_KEY_INT=MAX_NONFLUSH_KEY_INT+MAX_FLUSH_KEY_INT;

	public final int MAX_FLUSH_CHECK_SUM=7*CLUB;

	public final short L_WON=-1;
	public final short R_WON=1;
	public final short DRAW=0;

	public final int CIRCUMFERENCE_FIVE=187853;
	public final int CIRCUMFERENCE_SEVEN=4565145;
	
	//Bit masks
	public final int SUIT_BIT_MASK=511;
	public final int NON_FLUSH_BIT_SHIFT=9;

	/////////
	//The following are used with NSAssert for
	//debugging, ignored by release mode
	public final short RANK_OF_A_WORST_HAND=0;
	public final short RANK_OF_A_BEST_HAND=7462;
	public final short RANK_OF_WORST_FLUSH=5864;
	public final short RANK_OF_BEST_FLUSH=7140;
	public final short RANK_OF_WORST_STRAIGHT=5854;
	public final short RANK_OF_BEST_STRAIGHT=5863;
	public final short RANK_OF_WORST_STRAIGHT_FLUSH=7453;
	public final short RANK_OF_BEST_STRAIGHT_FLUSH=RANK_OF_A_BEST_HAND;

	public final int KEY_COUNT=53924;
	public final int NON_FLUSH_KEY_COUNT=49205;
	public final int FLUSH_KEY_COUNT=4719;
	
	//Used in flush checking
	public final int UNVERIFIED=-2;
	public final int NOT_A_FLUSH=-1;
	/////////
	
	public final int MAX_NUMBER_OF_UNDEALT_CARDS=DECK_SIZE-4;
	public final int MAX_NUMBER_OF_PLAYERS=8;
	
	public final int CAKE=840;
}
