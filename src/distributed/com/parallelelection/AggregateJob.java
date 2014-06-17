package com.parallelelection;

import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

import org.apache.commons.httpclient.URI;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.conf.Configured;
import org.apache.hadoop.filecache.DistributedCache;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.reduce.LongSumReducer;
import org.apache.hadoop.util.Tool;
import org.apache.hadoop.util.ToolRunner;
 
public class AggregateJob extends Configured implements Tool {
 
  @Override
  public int run(String[] args) throws Exception {
    
	  
	  
	Job job = new Job(getConf());
	
	String count = "0";
	String line = "0";
	int countNumber = 0;
	
	BufferedReader reader = new BufferedReader(new FileReader(args[2]));
	
	while ((line = reader.readLine()) != null) {
		   
		job.getConfiguration().set(count, line);
 	    countNumber++;
 	    count = String.valueOf(countNumber); 	
 	
		   }
	
	job.getConfiguration().set("numberLines", count);
	reader.close();	
	
	
    job.setJarByClass(getClass());
    job.setJobName(getClass().getSimpleName());
          
 
    FileInputFormat.addInputPath(job, new Path(args[0]));
    FileOutputFormat.setOutputPath(job, new Path(args[1]));
 
    job.setMapperClass(ElectionMapper.class);
    job.setCombinerClass(LongSumReducer.class);
    job.setReducerClass(LongSumReducer.class);
 
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(LongWritable.class);
    
    
    return job.waitForCompletion(true) ? 0 : 1;
  }
 
  public static void main(String[] args) throws Exception {
      
    		
	int rc = ToolRunner.run(new AggregateJob(), args);
    System.exit(rc);
    
    
  }
}
