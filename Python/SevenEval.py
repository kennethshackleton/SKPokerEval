#!/usr/bin/env python
# encoding: utf-8
"""
SevenEval.py

Created by Kenneth J. Shackleton on 15 June 2011.
Copyright (c) 2011 Ringo Limited.
All rights reserved.
"""

import sys
import os

from FiveEval import *
import Constants

class SevenEval :
	def __init__(self) :
		self.rankArray = [0] * Constants.CIRCUMFERENCE_SEVEN
		self.flushRankArray = [0] * (Constants.MAX_SEVEN_FLUSH_KEY_INT + 1)
		self.deckcardsKey = [0] * Constants.DECK_SIZE
		self.deckcardsFlush = [0] * Constants.DECK_SIZE
		self.deckcardsSuit = [0] * Constants.DECK_SIZE	
		self.flushCheck = [0] * (Constants.MAX_FLUSH_CHECK_SUM + 1)
		
		face = [Constants.ACE, Constants.KING, Constants.QUEEN, Constants.JACK, Constants.TEN,
				Constants.NINE, Constants.EIGHT, Constants.SEVEN, Constants.SIX, Constants.FIVE,
				Constants.FOUR, Constants.THREE, Constants.TWO]

		faceFlush = [Constants.ACE_FLUSH, Constants.KING_FLUSH, Constants.QUEEN_FLUSH,
					 Constants.JACK_FLUSH, Constants.TEN_FLUSH, Constants.NINE_FLUSH,
					 Constants.EIGHT_FLUSH, Constants.SEVEN_FLUSH, Constants.SIX_FLUSH,
					 Constants.FIVE_FLUSH, Constants.FOUR_FLUSH, Constants.THREE_FLUSH,
					 Constants.TWO_FLUSH]

		for n in range(0, Constants.NUMBER_OF_FACES) :
			self.deckcardsKey[4*n] = (face[n] << Constants.NON_FLUSH_BIT_SHIFT) + Constants.SPADE
			self.deckcardsKey[4*n + 1] = (face[n] << Constants.NON_FLUSH_BIT_SHIFT) + Constants.HEART
			self.deckcardsKey[4*n + 2]	= (face[n] << Constants.NON_FLUSH_BIT_SHIFT) + Constants.DIAMOND
			self.deckcardsKey[4*n + 3]	= (face[n] << Constants.NON_FLUSH_BIT_SHIFT) + Constants.CLUB

			self.deckcardsFlush[4*n] = faceFlush[n]
			self.deckcardsFlush[4*n + 1] = faceFlush[n]
			self.deckcardsFlush[4*n + 2] = faceFlush[n]
			self.deckcardsFlush[4*n + 3] = faceFlush[n]

			self.deckcardsSuit[4*n]	= Constants.SPADE
			self.deckcardsSuit[4*n + 1]	= Constants.HEART
			self.deckcardsSuit[4*n + 2]	= Constants.DIAMOND
			self.deckcardsSuit[4*n + 3]	= Constants.CLUB
		
		# Track increments.
		count = 0
		
		fiveCardEvaluator = FiveEval()
		
		# High card.
		for i in range(1, Constants.NUMBER_OF_FACES) :
			for j in range(1, i+1) :
				for k in range(1, j+1) :
					for l in range(0, k+1) :
						for m in range(0, l+1) :
							for n in range(0, m+1) :
								for p in range(0, n+1) :
									if i != m and j != n and k != p :
										count += 1
										key = face[i] + face[j] + face[k] + face[l] + face[m] + face[n] + face[p]
										# The 4*i+0 and 4*m+1 trick prevents flushes.
										rank = fiveCardEvaluator.getRankOfSeven(4*i, 4*j, 4*k, 4*l, 4*m+1, 4*n+1, 4*p+1)
										self.rankArray[key % Constants.CIRCUMFERENCE_SEVEN] = rank

		# Flush ranks.
		# All 7 same suit.
		for i in range(6, Constants.NUMBER_OF_FACES) :
			for j in range(5, i) :
				for k in range(4, j) :
					for l in range(3, k) :
						for m in range(2, l) :
							for n in range(1, m) :
								for p in range(0, n) :
									count += 1
									key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m] + faceFlush[n] + faceFlush[p]
									rank = fiveCardEvaluator.getRankOfSeven(4*i, 4*j, 4*k, 4*l, 4*m, 4*n, 4*p)
									self.flushRankArray[key] = rank
									
		# Only 6 same suit.
		for i in range(5, Constants.NUMBER_OF_FACES) :
			for j in range(4, i) :
				for k in range(3, j) :
					for l in range(2, k) :
						for m in range(1, l) :
							for n in range(0, m) :
								count += 1
								key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m] + faceFlush[n]
								
								# The Two of clubs is the card at index 51; the other six
								# cards all have the spade suit.
								rank = fiveCardEvaluator.getRankOfSeven(4*i, 4*j, 4*k, 4*l, 4*m, 4*n, 51)
								self.flushRankArray[key] = rank
				
		# Only 5 same suit.
		for i in range(4, Constants.NUMBER_OF_FACES) :
			for j in range(3, i) :
				for k in range(2, j) :
					for l in range(1, k) :
						for m in range(0, l) :
							count += 1
							key = faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m]
							rank = fiveCardEvaluator.getRankOfFive(4*i, 4*j, 4*k, 4*l, 4*m);
							self.flushRankArray[key] = rank
			
		##
		# Initialise flush checks.
		SUIT_COUNT = 0
		FLUSH_SUIT_INDEX = -1
		card_S_MATCHED_SO_FAR = 0
		SUIT_KEY = Constants.SPADE
		suits = [Constants.SPADE, Constants.HEART, Constants.DIAMOND, Constants.CLUB]

		# Initialise all entries of flushCheck[] to UNVERIFIED, as yet unchecked.	
		self.flushCheck = [Constants.UNVERIFIED] * (Constants.MAX_FLUSH_CHECK_SUM + 1)

		# 7-card flush.
		for card_1 in range(0, Constants.NUMBER_OF_SUITS) :
			for card_2 in range(0, card_1 + 1) :
				for card_3 in range(0, card_2 + 1) :
					for card_4 in range(0, card_3 + 1) :
						for card_5 in range(0, card_4 + 1) :
							for card_6 in range(0, card_5 + 1) :
								for card_7 in range(0, card_6 + 1) :
									SUIT_COUNT = 0
									FLUSH_SUIT_INDEX = -1
									CARDS_MATCHED_SO_FAR = 0
									SUIT_KEY = suits[card_1] + suits[card_2] + suits[card_3] + \
											   suits[card_4] + suits[card_5] + suits[card_6] + \
											   suits[card_7]
									
									if self.flushCheck[SUIT_KEY] == Constants.UNVERIFIED :
										while CARDS_MATCHED_SO_FAR < 3 and FLUSH_SUIT_INDEX < 4 :
											FLUSH_SUIT_INDEX += 1
											SUIT_COUNT = (suits[card_1] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_2] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_3] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_4] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_5] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_6] == suits[FLUSH_SUIT_INDEX]) + \
														 (suits[card_7] == suits[FLUSH_SUIT_INDEX])
											CARDS_MATCHED_SO_FAR += SUIT_COUNT

									# A count of 5 or more means we have a flush. We place
									# the value of the flush suit here.
									if SUIT_COUNT > 4 :
										self.flushCheck[SUIT_KEY] = suits[FLUSH_SUIT_INDEX]
									
									# Otherwise this is a non-flush hand.
									else :
										self.flushCheck[SUIT_KEY] = Constants.NOT_A_FLUSH
		
		return
	
	def getRankOfSeven(self, card_1, card_2, card_3, card_4, card_5, card_6, card_7) :
		# Create a 7-card hand key by adding up each of the card keys.
		KEY = self.deckcardsKey[card_1] + \
			  self.deckcardsKey[card_2] + \
			  self.deckcardsKey[card_3] + \
			  self.deckcardsKey[card_4] + \
		  	  self.deckcardsKey[card_5] + \
			  self.deckcardsKey[card_6] + \
			  self.deckcardsKey[card_7]

		# Tear off the flush check strip.
		FLUSH_CHECK_KEY = ( KEY & Constants.SUIT_BIT_MASK )
		FLUSH_SUIT = self.flushCheck[FLUSH_CHECK_KEY]
		
		if FLUSH_SUIT == Constants.NOT_A_FLUSH :
			# Tear off the non-flush key strip, and look up the rank.
			KEY = ( KEY >> Constants.NON_FLUSH_BIT_SHIFT )

			# Take key modulo the circumference. A dichotomy is faster than using
			# the usual modulus operation. This is fine for us because the circumference
			# is more than half the largest face key we come across.
			rank = self.rankArray[KEY] if KEY < Constants.CIRCUMFERENCE_SEVEN else self.rankArray[KEY - Constants.CIRCUMFERENCE_SEVEN]

			return rank
		
		else :
			print "flush"
			# Generate a flush key, and look up the rank.
			FLUSH_KEY = (self.deckcardsFlush[card_1] if self.deckcardsSuit[card_1] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_2] if self.deckcardsSuit[card_2] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_3] if self.deckcardsSuit[card_3] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_4] if self.deckcardsSuit[card_4] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_5] if self.deckcardsSuit[card_5] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_6] if self.deckcardsSuit[card_6] == FLUSH_SUIT else 0) + \
						(self.deckcardsFlush[card_7] if self.deckcardsSuit[card_7] == FLUSH_SUIT else 0)
			print FLUSH_KEY
			return self.flushRankArray[FLUSH_KEY]
		
		return -1