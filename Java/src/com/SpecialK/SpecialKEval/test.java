package com.SpecialK.SpecialKEval;

import com.SpecialK.SpecialKEval.SevenEval;

public class test {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		SevenEval eval = new SevenEval();
		System.out.println("" + eval.getRankOf(1, 2, 3, 4, 5, 6, 7));
		System.out.println("" + eval.getRankOf(22, 26, 30, 39, 43, 47, 51));
		System.out.println("" + eval.getRankOf(3, 14, 39, 2, 1, 19, 31));
	}

}
