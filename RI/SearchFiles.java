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
import java.util.Collections;
import java.util.Date;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.es.SpanishAnalyzer;
//import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
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
import org.w3c.dom.NodeList;


public class SearchFiles {

  private SearchFiles() {}

  public final static String AUTOR = "autor";
  public final static String TEMATICO = "tema";
  public final static String FECHA = "fecha";
  
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
    
    //Parseamos el xml para quedarnos con las 5 consultas y sus identificadores
	File file= new File(infoNeeds);
	DocumentBuilderFactory xmlParserF = DocumentBuilderFactory.newInstance();
	DocumentBuilder xmlParser = xmlParserF.newDocumentBuilder();
	org.w3c.dom.Document parseador = xmlParser.parse(file);
	Document doc = new Document();

	Field pathField = new StringField("ruta", file.getPath(), Field.Store.YES);
	doc.add(pathField);
  
	NodeList lista = parseador.getElementsByTagName("text");
	NodeList identificadores=parseador.getElementsByTagName("identifier");
    String consulta = "";
    String iden="";
	//Cogemos el campo texto e id de cada una de las consultas
    for(int i = 0; i<lista.getLength(); i++){
  	  consulta = consulta + "-" + lista.item(i).getFirstChild().getNodeValue();
    }
    for(int i = 0; i<identificadores.getLength(); i++){
  	  iden = iden + "=" + identificadores.item(i).getFirstChild().getNodeValue();
    }
    
    String [] necesidades=consulta.split("-");
    String [] id= iden.split("=");
    for (int i=1;i<necesidades.length;i++){
  	  System.out.println("La consulta "+id[i]+" es:" +necesidades[i]);
    }
    
    
    Query q=null;
    BufferedReader in = null;
    ArrayList<String> result = new ArrayList<String>();
    
    FileWriter fichero = null;
    PrintWriter pw = null;
    try{
        fichero = new FileWriter(resultFile);
        pw = new PrintWriter(fichero);
	    
	    for(int i=0;i<necesidades.length;i++){
	    	in = new BufferedReader(new InputStreamReader(new FileInputStream(necesidades[i]), "UTF-8"));
	    	Parser pars = new Parser(necesidades[i]);
	    	q= pars.execute();
	    	result = doPagingSearchString(in, searcher, q, necesidades[i] == null && queryString == null,result);
	    	
	    	//ordenamos los resultados de forma creciente como exige en el guion
	    	Collections.sort(result);
	    	
	    	//Escribimos los resultados de la consulta obtenidos, en el fichero resultFile
	    	
	    	//Comento estas dos lineas xq pone que no escribamos ningun tipo de cabecera para separar resultados de consultas
	        //pw.println("Consulta: "+necesidades[i]);
            //pw.println("Total de documentos encontrados: "+result.size());
	    	
            for (int j = 0; j < result.size(); j++){
            	pw.println(id[i]+'	'+result.get(j));
            }     
	    }
	    fichero.close();
	    reader.close();
    }
    catch (Exception e) {
    	e.printStackTrace();
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
  
  public static ArrayList<String> doPagingSearchString(BufferedReader in, IndexSearcher searcher, Query query, 
                                       boolean interactive,ArrayList<String> resultados) throws IOException {
	  
    /********** REVISAR que poner en el segundo parametro del searcher.search****/
	TopDocs results = searcher.search(query,100);
    ScoreDoc[] hits = results.scoreDocs;
    ArrayList<String> resultadosFinales= new ArrayList<String>();
    
    for (int i = 0; i < results.totalHits; i++){
        
    	Document doc = searcher.doc(hits[i].doc);        
        String path = doc.get("path");
        
        if (path != null) {
        	
        	/***HABRIA QUE VER COMO DEVUELVE EL FICHERO PARA PARSEARLO DE UNA FORMA Y OTRA PARA QUE SOLO GUARDE EL ID DEL DOC***/
        	String cadena[]=path.split("/");
        	//int number=Integer.parseInt(cadena[cadena.length-1].split("-")[0]);
        	String id=cadena[cadena.length-1];
        	if(resultados.size() + (results.totalHits - i) <  results.totalHits){
        		resultados.add(id);
        	}
        	else{
        		if(resultados.contains(id)){
        			resultadosFinales.add(id);
        		}
        	}
        	
        }
        else {
          System.out.println((i+1) + ". " + "No path for this document");
        }
                  
    }
    return resultadosFinales;
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
