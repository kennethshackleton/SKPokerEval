#!/usr/bin/env python
# encoding: utf-8
"""
FiveEval.py

Created by Kenneth J. Shackleton on 14 June 2011.
Copyright (c) 2011 Ringo Limited.
All rights reserved.
"""

import sys
import os

import Constants

class FiveEval :
	
	def __init__(self) :
		
		self.rankArray = [0] * (Constants.MAX_FIVE_NONFLUSH_KEY_INT + 1)
		self.flushRankArray = [0] * (Constants.MAX_FIVE_FLUSH_KEY_INT + 1)
		self.deckcardsFace = [0] * Constants.DECK_SIZE
		self.deckcardsFlush = [0] * Constants.DECK_SIZE
		self.deckcardsSuit = [0] * Constants.DECK_SIZE
		
		face = [Constants.TWO_FIVE, Constants.THREE_FIVE, Constants.FOUR_FIVE,
			 	Constants.FIVE_FIVE, Constants.SIX_FIVE, Constants.SEVEN_FIVE,
			 	Constants.EIGHT_FIVE, Constants.NINE_FIVE, Constants.TEN_FIVE,
			 	Constants.JACK_FIVE, Constants.QUEEN_FIVE, Constants.KING_FIVE,
			 	Constants.ACE_FIVE]
				
		faceFlush = [Constants.TWO_FLUSH, Constants.THREE_FLUSH,
				 	 Constants.FOUR_FLUSH, Constants.FIVE_FLUSH,
					 Constants.SIX_FLUSH, Constants.SEVEN_FLUSH,
					 Constants.EIGHT_FLUSH, Constants.NINE_FLUSH,
					 Constants.TEN_FLUSH, Constants.JACK_FLUSH,
					 Constants.QUEEN_FLUSH, Constants.KING_FLUSH,
					 Constants.ACE_FLUSH]
						
		for n in range(Constants.NUMBER_OF_FACES) :
			
			self.deckcardsSuit[4*n] = Constants.SPADE
			self.deckcardsSuit[4*n + 1] = Constants.HEART
			self.deckcardsSuit[4*n + 2] = Constants.DIAMOND
			self.deckcardsSuit[4*n + 3] = Constants.CLUB

			self.deckcardsFace[4*n] = face[12 - n]
			self.deckcardsFace[4*n + 1] = face[12 - n]
			self.deckcardsFace[4*n + 2] = face[12 - n]
			self.deckcardsFace[4*n + 3] = face[12 - n]
			
			self.deckcardsFlush[4*n] = faceFlush[12 - n]
			self.deckcardsFlush[4*n + 1] = faceFlush[12 - n]
			self.deckcardsFlush[4*n + 2] = faceFlush[12 - n]
			self.deckcardsFlush[4*n + 3] = faceFlush[12 - n]
		
		# n increments as rank.
		n = 0
		
		# High card.
		for i in range(5, Constants.NUMBER_OF_FACES) :
			for j in range(3, i) :
				for k in range(2, j) :
					for l in range(1, k) :
						# No straights
						for m in range(0, l) :
							if not (i - m == 4 or (i == 12 and j == 3 and k == 2 and l == 1 and m == 0)) :
								n += 1
								self.rankArray[face[i] + face[j] + face[k] + face[l] + face[m]] = n
								
		# Pair.
		for i in range(0, Constants.NUMBER_OF_FACES) :
			for j in range(2, Constants.NUMBER_OF_FACES) :
				for k in range(1, j) :
					for l in range(0, k) :
						if i != j and i != k and i != l :
							n += 1
							self.rankArray[(2*face[i]) + face[j] + face[k] + face[l]] = n

		# Two pair.
		for i in range(1, Constants.NUMBER_OF_FACES) :
			for j in range(0, i) :
				for k in range(0, Constants.NUMBER_OF_FACES) :
					# No fullhouse
					if k != i and k != j :
						n += 1
						self.rankArray[(2*face[i]) + (2*face[j]) + face[k]] = n		
		
		# Triple.
		for i in range(0, Constants.NUMBER_OF_FACES) :
			for j in range(1, Constants.NUMBER_OF_FACES) :
				for k in range(0, j) :
					# No quad
					if i != j and i != k :
						n += 1
						self.rankArray[(3*face[i]) + face[j] + face[k]] = n
						
		# Low straight non-flush.
		n += 1
		self.rankArray[face[12] + face[0] + face[1] + face[2] + face[3]] = n

		# Usual straight non-flush.
		for i in range(0, 9) :
			n += 1
			self.rankArray[face[i] + face[i+1] + face[i+2] + face[i+3] + face[i+4]] = n

		# Flush not a straight.
		for i in range(5, Constants.NUMBER_OF_FACES) :
			for j in range(3, i) :
				for k in range(2, j) :
					for l in range(1, k) :
						for m in range(0, l) :
							if not (i - m == 4 or (i == 12 and j == 3 and k == 2 and l == 1 and m == 0)) :
								n += 1
								self.flushRankArray[faceFlush[i] + faceFlush[j] + faceFlush[k] + faceFlush[l] + faceFlush[m]] = n		

		# Full house.
		for i in range(0, Constants.NUMBER_OF_FACES) :
			for j in range(0, Constants.NUMBER_OF_FACES) :
				if i != j :
					n += 1
					self.rankArray[(3*face[i]) + (2*face[j])] = n		

		# Quad.
		for i in range(0, Constants.NUMBER_OF_FACES) :
			for j in range(0, Constants.NUMBER_OF_FACES) :
				if i != j :
					n += 1
					self.rankArray[(4*face[i]) + face[j]] = n

		# Low straight flush.
		n += 1
		self.flushRankArray[faceFlush[0] + faceFlush[1] + faceFlush[2] + faceFlush[3] + faceFlush[12]] = n;
		
		# Usual straight flush.
		for i in range(0, 9) :
			n += 1
			self.flushRankArray[faceFlush[i] + faceFlush[i+1] + faceFlush[i+2] + faceFlush[i+3] + faceFlush[i+4]] = n
				
		return

	def getRankOfFive(self, card_1, card_2, card_3, card_4, card_5) :
		if (self.deckcardsSuit[card_1] == self.deckcardsSuit[card_2] and
		    self.deckcardsSuit[card_1] == self.deckcardsSuit[card_3] and
		    self.deckcardsSuit[card_1] == self.deckcardsSuit[card_4] and
		    self.deckcardsSuit[card_1] == self.deckcardsSuit[card_5]) :
					
			return self.flushRankArray[self.deckcardsFlush[card_1] +
								  	   self.deckcardsFlush[card_2] +
		                          	   self.deckcardsFlush[card_3] +
								  	   self.deckcardsFlush[card_4] +
		                          	   self.deckcardsFlush[card_5]]
		
		else :
						
			return self.rankArray[self.deckcardsFace[card_1] +
							 	  self.deckcardsFace[card_2] +
		                     	  self.deckcardsFace[card_3] +
							 	  self.deckcardsFace[card_4] +
		                     	  self.deckcardsFace[card_5]]
		
		return -1
		
	def getRankOfSeven(self, CARD1, CARD2, CARD3, CARD4, CARD5, CARD6, CARD7) :
		seven_cards = [CARD1, CARD2, CARD3, CARD4, CARD5, CARD6, CARD7]
		five_temp = [0] * 5
		BEST_RANK_SO_FAR = 0
		CURRENT_RANK = 0
		m = 0
		
		for i in range(1, 7) :
			for j in range(0, i) :
				m = 0
				for k in range(0, 7) :
					if k != i and k != j :
						five_temp[m] = seven_cards[k]
						m += 1
						
				CURRENT_RANK = self.getRankOfFive(five_temp[0], five_temp[1], five_temp[2], five_temp[3], five_temp[4])
				if BEST_RANK_SO_FAR < CURRENT_RANK :
					BEST_RANK_SO_FAR = CURRENT_RANK
				
		return BEST_RANK_SO_FAR