package com.parallelelection;

import java.io.IOException;

import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Mapper;

public class ElectionMapper extends Mapper<LongWritable, Text, Text, LongWritable> {
	  private Text wordDilma = new Text();
	  private Text wordAecio = new Text();
	  private Text wordCampos = new Text();
	  private LongWritable count = new LongWritable();
	 
	  @Override
	  protected void map(LongWritable key, Text value, Context context)
	      throws IOException, InterruptedException {
	    // value is tab separated values: word, year, occurrences, #books, #pages
	    // we project out (word, occurrences) so we can sum over all years
	    String[] split = value.toString().split(" ");
	    String countSentiment = "0";
	    String auxSentiment = "0";
	    int totalSentiment = 0;
	    boolean containsDilma = false;
	    boolean containsAecio = false;
	    boolean containsCampos = false;
	    CheckDictionary checkSentiment = new CheckDictionary();
	    String line;
	    
	
	    String numberLines = context.getConfiguration().get("numberLines");
	    int totalNumberLines = Integer.parseInt(numberLines);
	    
	    if(value.toString().toLowerCase().contains("dilma")){
	    	wordDilma.set("dilma");
	    	containsDilma = true;
	    }
	    
	    if(value.toString().toLowerCase().contains("aecio")){
	    	wordAecio.set("aecio");
	    	containsAecio = true;
	    }
	    
	    if(value.toString().toLowerCase().contains("campos")){
	    	wordCampos.set("campos");
	    	containsCampos = true;
	    }
	    
	    if(containsDilma || containsCampos || containsAecio){
	    	
	    	for (int i = 0; i < split.length; i++){
		    	
		    	for(int j = 0; j < totalNumberLines; j++){
		    		
		    		
		    		line =  context.getConfiguration().get(String.valueOf(j));
		    		
		    		auxSentiment = checkSentiment.getSentiment(split[i], line); 
		    		
		    				    		
		    		totalSentiment = Integer.parseInt(auxSentiment);
		    		
		    		if(totalSentiment != 0){
		    			
		    			try {
		    		        count.set(Long.parseLong(auxSentiment));
		    		        if(containsDilma){
		    		        	 context.write(wordDilma, count);
		    		        }
		    		        if(containsAecio){
		    		        	 context.write(wordAecio, count);
		    		        }
		    		        if(containsCampos){
		    		        	 context.write(wordCampos, count);
		    		        }        
		    		       
		    		      } catch (NumberFormatException e) {
		    		        // cannot parse - ignore
		    		      }	    	
		    			
		    			break;
		    		}
		    		
		    		
		    	}  		  	  	
		    	
		    	
		    	
		    }  	
	    	    	
	    	
	    	
	    }   
	    	    
	  }
	}