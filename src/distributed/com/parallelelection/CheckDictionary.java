package com.parallelelection;


import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.List;

public class CheckDictionary {

	
	
	public static String getSentiment(String word, String line) throws IOException{
		
		String sentimentLocal[] = new String[2];		
		
		
		int countSentiment = 0;
		int aux = 0;
		
		if (line.toLowerCase().contains(word.toLowerCase())){
			
			sentimentLocal = line.split("\t");
			
		}
		else{
			sentimentLocal[0] = "0";
		}
		
		return sentimentLocal[0];
		
	}
	
}
