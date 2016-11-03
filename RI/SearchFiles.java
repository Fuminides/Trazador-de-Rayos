package trabajo;

/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Date;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.es.SpanishAnalyzer;
//import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.BooleanClause;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.NumericRangeQuery;
import org.apache.lucene.search.BooleanQuery;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;


public class SearchFiles {

  private SearchFiles() {}

  public final static String AUTOR = "autor";
  public final static String TEMATICO = "tema";
  public final static String FECHA = "fecha";
  
  public final static String TITULO = "titulo";
  public final static String ID = "identificador";
  public final static String DESC = "descripcion";
  public final static String PUBLISHER = "publisher";
  public final static String LANG = "lenguaje";
  public final static String CREATOR = "autor";
  public final static String DATE = "fecha";
  
  public static void main(String[] args) throws Exception {
    String usage ="No se han introducido los archivos correctos";
    if (args.length > 0 && ("-h".equals(args[0]) || "-help".equals(args[0]))) {
      System.out.println(usage);
      System.exit(0);
    }

    String indexAutor = "autor", indexTheme = "tematico", indexDate = "fecha";
    String field = "contents";
    String index = "index";
    String infoNeeds=null;
    String resultFile=null;
    String queries = null;
    String queryString = null;
    
    for(int i = 0;i < args.length;i++) {
        if ("-index".equals(args[i])) {
          index = args[i+1];
          i++;
        } 
        else if("-infoNeeds".equals(args[i])){
      	  infoNeeds=args[i+1];
      	  i++;
        }
        else if("-output".equals(args[i])){
      	  resultFile=args[i+1];
      	  i++;
        }
    }
    if (infoNeeds == null || resultFile == null || index == null) {
        System.err.println("Usage: " + usage);
        System.exit(1);
      }
    
    
    IndexReader reader = DirectoryReader.open(FSDirectory.open(new File(index)));
    IndexSearcher searcher = new IndexSearcher(reader);
    Analyzer analyzer = new SpanishAnalyzer(Version.LUCENE_44);

    BufferedReader in = null;
    if (queries != null) {
      in = new BufferedReader(new InputStreamReader(new FileInputStream(queries), "UTF-8"));
    } else {
      in = new BufferedReader(new InputStreamReader(System.in, "UTF-8"));
    }
    QueryParser parser = new QueryParser(Version.LUCENE_44, field, analyzer);
    while (true) {
      if (queries == null && queryString == null) {                        // prompt the user
        System.out.println("Enter query: ");
      }

      String line = queryString != null ? queryString : in.readLine();

      if (line == null || line.length() == -1) {
        break;
      }

      line = line.trim();
      if (line.length() == 0) {
        break;
      }
      
      Query query = null;
      //System.out.println("Searching for: " + line);
      ArrayList<Integer> resultSpatial = new ArrayList<Integer>();
      ArrayList<Integer> resultNoSpatial = new ArrayList<Integer>();
      while(!line.isEmpty()){
	      if ( line.contains("spatial")) { 
	    	  	    	
	    	//Dividimos todo el string en las diferentes consultas y guardamos cada una en una posicion del array
	    	String dividir[]  = line.split(" ");
		    int i;
		    //Buscamos cual de ellas es la consulta del spatial
	    	for (i=0;i<dividir.length;i++){
	    		if(dividir[i].contains("spatial")){
	    			line=dividir[i];
	    			break;
	    		}
	    	}
	  	    String[] coordenadas = line.substring(line.indexOf(":")+1).split(",");
		    
	  		double west = Double.parseDouble(coordenadas[0]),
	  		east = Double.parseDouble(coordenadas[1]),
	  		south = Double.parseDouble(coordenadas[2]),
	  		north = Double.parseDouble(coordenadas[3]);
	  		BooleanQuery spatialQuery = new BooleanQuery();
	  		NumericRangeQuery<Double> wRQ = NumericRangeQuery.newDoubleRange("west", null, east, true, true);
	  		NumericRangeQuery<Double> eRQ = NumericRangeQuery.newDoubleRange("east", west, null, true, true);
	  		NumericRangeQuery<Double> sRQ = NumericRangeQuery.newDoubleRange("south", null, north, true, true);
	  		NumericRangeQuery<Double> nRQ = NumericRangeQuery.newDoubleRange("north", south, null, true, true);
	  		
	  		spatialQuery.add(wRQ, BooleanClause.Occur.MUST);
	  		spatialQuery.add(eRQ, BooleanClause.Occur.MUST);
	  		spatialQuery.add(sRQ, BooleanClause.Occur.MUST);
	  		spatialQuery.add(nRQ, BooleanClause.Occur.MUST);
	  		
	  		
  		    doPagingSearchString(in, searcher, spatialQuery, queries == null && queryString == null,resultSpatial);
  		    
	  		//El line sera toda la query menos la parte del spatial
	  		line="";
	  		for (int j=0;j<dividir.length;j++){
	  			if(j!=i){
	  				line=line+dividir[j]+" ";
	  			}
	  		}
	      }
	      else if(line.contains("temporal")){
	    	  // Para las consultas temporal 
	    	  String[] t = line.substring(line.indexOf(":")+1).split(";");
	          System.out.println("Tama�o:"+t.length);
	          if(t.length>1){
	        	  String dividir[]  = line.split(" ");
		  		    int i;
		  		    //Buscamos cual de ellas es la consulta del temporal
		  	    	for (i=0;i<dividir.length;i++){
		  	    		if(dividir[i].contains("temporal")){
		  	    			break;
		  	    		}
		  	    	}
	     
	        	  String b=t[0].split("=")[1];
	        	  String e=t[1].split("=")[1];
	        	  int begin=Integer.parseInt(b.split("-")[0]+b.split("-")[1]+b.split("-")[2]);
	        	  int end=Integer.parseInt(e.split("-")[0]+e.split("-")[1]+e.split("-")[2]);
	        	  
	        	  System.out.println("Temporal:"+begin);
	        	  System.out.println("Temporal:"+end);
	        	  
	        	  BooleanQuery temporalQuery = new BooleanQuery();
	        	  
	  	  		  NumericRangeQuery<Integer> bTQ = NumericRangeQuery.newIntRange("begin", begin, null, true, false);//("begin",begin, null,end,null,true);
	  	  		  NumericRangeQuery<Integer> eTQ = NumericRangeQuery.newIntRange("end",begin,end, null, true, true);
	  	  		
	  	  		  temporalQuery.add(bTQ, BooleanClause.Occur.MUST);
	  	  	      temporalQuery.add(eTQ, BooleanClause.Occur.MUST);
	  	  	      
		  		  doPagingSearchString(in, searcher, temporalQuery, queries == null && queryString == null,resultNoSpatial);
				
		  		  //Quitamos la parte del temporal de la consulta 
			  		line="";
			  		for (int j=0;j<dividir.length;j++){
			  			if(j!=i){
			  				line=line+dividir[j]+" ";
			  			}
			  		}
		          }
	    	  
	      }
	      else{
	    	 String consultas[]=line.split(" ");
	    	  for(int i=0;i<consultas.length;i++){
	    		  ArrayList<Integer> resultados = new ArrayList<Integer>();
	    		  query = parser.parse(consultas[i]);
	    	      System.out.println("Searching for: " + query);

	    		  if (repeat > 0) {                           // repeat & time as benchmark
	    		        Date start = new Date();
	    		        for (int j = 0; j < repeat; j++) {
	    		        	searcher.search(query, 100);
	    		        }
	    		        Date end = new Date();
	    		        System.out.println("Time: "+(end.getTime()-start.getTime())+"ms");
	    		   }
	    		  doPagingSearchString(in, searcher, query, queries == null && queryString == null,resultados);
	    		  if (queryString != null) {
	    		        break;
	    		  }
	    		  if(resultNoSpatial.isEmpty()){
	    			  resultNoSpatial=(ArrayList<Integer>) resultados.clone();
	    		  }
	    		  else{
	    			  for(int j=0;j<resultNoSpatial.size();j++){
	    				  if(!resultados.contains(resultNoSpatial.get(j))){
	    					  resultNoSpatial.remove(j);
	    				  }
	    			  }
	    		  }
	    	  }
	    	  line="";
	      }    
      }

      //*********ESTA PARTE SE MODIFICARA EN BASE A LAS CONSULTAS DEL TRABAJO  
      //Concateno los resultados obtenidos 
      int total=0;
      ArrayList<Integer> resultadosFinales = new ArrayList<Integer>();
      if(!resultSpatial.isEmpty()){
    	  total+=resultSpatial.size();
    	  for(int i=0;i<resultSpatial.size();i++){
    		  resultadosFinales.add(resultSpatial.get(i));
    	  }
      }
      if(!resultNoSpatial.isEmpty()){
    	  total+=resultNoSpatial.size();
    	  for(int i=0;i<resultNoSpatial.size();i++){
    		  resultadosFinales.add(resultNoSpatial.get(i));
    	  } 
      }
      //Escribimos los resultados de la consulta obtenidos, en el fichero resultFile
      FileWriter fichero = null;
      PrintWriter pw = null;
      try{
          fichero = new FileWriter(resultFile);
          pw = new PrintWriter(fichero);
          pw.println("Total de documentos encontrados: "+total);
          for (int i = 0; i < resultadosFinales.size(); i++)
              pw.println(resultadosFinales.get(i));

      } 
      catch (Exception e) {
          e.printStackTrace();
      }
      fichero.close();
    }
    reader.close();
  }

  
  /**
   * This demonstrates a typical paging search scenario, where the search engine presents 
   * pages of size n to the user. The user can then go to the next page if interested in
   * the next hits.
   * 
   * When the query is executed for the first time, then only enough results are collected
   * to fill 5 result pages. If the user wants to page beyond this limit, then the query
   * is executed another time and all hits are collected.
   * 
   */
  //DUPLICADO para que este metodo devuelva los resultados en un array en vez de por pantalla
  public static void doPagingSearchString(BufferedReader in, IndexSearcher searcher, Query query, 
                                       boolean interactive,ArrayList<Integer> resultados) throws IOException {
	  
    //********************* no estoy muy segura de que hay que poner en el segundo parametro del searcher.search****//
	TopDocs results = searcher.search(query, 0);
    ScoreDoc[] hits = results.scoreDocs;
    
    while (true) {
      for (int i = 0; i < results.totalHits; i++){
        
    	Document doc = searcher.doc(hits[i].doc);
    	long tiempo = Long.parseLong(doc.getField("modified").stringValue());
    	Date fecha = new Date(tiempo);
    	//System.out.println("modified: " + fecha);
        
        String path = doc.get("path");
        if (path != null) {
        	//Solo muestra el identificador del documento en vez del path
        	String cadena[]=path.split("/");
        	int number=Integer.parseInt(cadena[cadena.length-1].split("-")[0]);
        	resultados.add(number);
        	
        } else {
          System.out.println((i+1) + ". " + "No path for this document");
        }
                  
      }
      if (!interactive ||  results.totalHits== 0) {
        break;
      }
      boolean quit = false;
      while (true) {
    	  System.out.print("Press(q)uit");
    	  String line = in.readLine();
    	  if (line.length() == 0 || line.charAt(0)=='q') {
    		  quit = true;
    		  break;
    	  }
      }
      if (quit) break;
     }
  }
  /**
   * This demonstrates a typical paging search scenario, where the search engine presents 
   * pages of size n to the user. The user can then go to the next page if interested in
   * the next hits.
   * 
   * When the query is executed for the first time, then only enough results are collected
   * to fill 5 result pages. If the user wants to page beyond this limit, then the query
   * is executed another time and all hits are collected.
   * 
   */
  public static void doPagingSearch(BufferedReader in, IndexSearcher searcher, Query query, 
                                     int hitsPerPage, boolean raw, boolean interactive) throws IOException {
    // Collect enough docs to show 5 pages
    TopDocs results = searcher.search(query, 5 * hitsPerPage);
    ScoreDoc[] hits = results.scoreDocs;
    
    int numTotalHits = results.totalHits;
    System.out.println(numTotalHits + " total matching documents");

    int start = 0;
    int end = Math.min(numTotalHits, hitsPerPage);
        
    while (true) {
      if (end > hits.length) {
        System.out.println("Only results 1 - " + hits.length +" of " + numTotalHits + " total matching documents collected.");
        System.out.println("Collect more (y/n) ?");
        String line = in.readLine();
        if (line.length() == 0 || line.charAt(0) == 'n') {
          break;
        }

        hits = searcher.search(query, numTotalHits).scoreDocs;
      }
      
      end = Math.min(hits.length, start + hitsPerPage);
      
      for (int i = start; i < end; i++) {
        if (raw) {                              // output raw format
          System.out.println("doc="+hits[i].doc+" score="+hits[i].score);
          continue;
        }
        else{
        	Document doc = searcher.doc(hits[i].doc);
        	long tiempo = Long.parseLong(doc.getField("modified").stringValue());
        	Date fecha = new Date(tiempo);
        	System.out.println("modified: " + fecha);
        }

        Document doc = searcher.doc(hits[i].doc);
     
        String path = doc.get("path");
        if (path != null) {
        	System.out.println((i+1) + ". " + path);
        } else {
          System.out.println((i+1) + ". " + "No path for this document");
        }
                  
      }

      if (!interactive || end == 0) {
        break;
      }

      if (numTotalHits >= end) {
        boolean quit = false;
        while (true) {
          System.out.print("Press ");
          if (start - hitsPerPage >= 0) {
            System.out.print("(p)revious page, ");  
          }
          if (start + hitsPerPage < numTotalHits) {
            System.out.print("(n)ext page, ");
          }
          System.out.println("(q)uit or enter number to jump to a page.");
          
          String line = in.readLine();
          if (line.length() == 0 || line.charAt(0)=='q') {
            quit = true;
            break;
          }
          if (line.charAt(0) == 'p') {
            start = Math.max(0, start - hitsPerPage);
            break;
          } else if (line.charAt(0) == 'n') {
            if (start + hitsPerPage < numTotalHits) {
              start+=hitsPerPage;
            }
            break;
          } else {
            int page = Integer.parseInt(line);
            if ((page - 1) * hitsPerPage < numTotalHits) {
              start = (page - 1) * hitsPerPage;
              break;
            } else {
              System.out.println("No such page");
            }
          }
        }
        if (quit) break;
        end = Math.min(numTotalHits, start + hitsPerPage);
      }
    }
  }
}
