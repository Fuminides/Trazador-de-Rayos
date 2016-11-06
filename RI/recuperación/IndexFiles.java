package recuperación;

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

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.es.SpanishAnalyzer;
//import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.Field.Store;
import org.apache.lucene.document.FieldType;
import org.apache.lucene.document.IntField;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig.OpenMode;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.index.Term;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Date;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

/** Index all text files under a directory.
 * <p>
 * This is a command-line application demonstrating simple Lucene indexing.
 * Run it with no command-line arguments for usage information.
 */
public class IndexFiles {
  
  private IndexFiles() {}

  private final static float BOOST_AUTOR = 1.0f;
  private final static float BOOST_FECHA = 0.5f;
  private final static float BOOST_TEMA = 1.5f;
  
  /** Index all text files under a directory. 
 * @throws ParserConfigurationException 
 * @throws SAXException */
  public static void main(String[] args) throws ParserConfigurationException, SAXException {
    String usage = "Por favor, indicame la ruta de la coleccion de documentos a indexar.";
    String error = "Por favor, indicame la ruta del directorio donde generar los indices y la ruta de la coleccion de documentos a indexar.";
    String docsPath= null;
    boolean reescribir = true;
    String indexPath = "index";

    if (args.length == 0) {
      System.err.println(error);
      System.exit(1);
    }else {
    	for(int i=0;i<args.length;i++) {
	      if ("-index".equals(args[i])) {
	        indexPath = args[i+1];
	        i++;
	      } else if ("-docs".equals(args[i])) {
	        docsPath = args[i+1];
	        i++;
	      } 
    	}
    }
    
    if (docsPath == null) {
      System.err.println("Usage: " + usage);
      System.exit(1);
    }

    final File docDir = new File(docsPath);
    if (!docDir.exists() || !docDir.canRead()) {
      System.out.println("El directorio '" +docDir.getAbsolutePath()+ "' no existe o no dispone de permisos de lectura.");
      System.exit(1);
    }
    
    Date start = new Date();
    try {
	      System.out.println("Indexando en el directorio '" + indexPath + "'...");
	
	      Directory dir = FSDirectory.open(new File(indexPath));
	      Analyzer analyzer = new SpanishAnalyzer(Version.LUCENE_44);
	      IndexWriterConfig iwc = new IndexWriterConfig(Version.LUCENE_44, analyzer);
	
	      if (reescribir) {
	        iwc.setOpenMode(OpenMode.CREATE);
	      } else {
	        iwc.setOpenMode(OpenMode.CREATE_OR_APPEND);
	      }
	
	      IndexWriter writer = new IndexWriter(dir, iwc);
      	  indexDocs(writer, docDir);
	
	      writer.close();
	
	      Date end = new Date();
	      System.out.println("Indexado en " + (end.getTime() - start.getTime()) + " milisegundos");
	      
    	
    } catch (IOException e) {
      System.out.println(" caught a " + e.getClass() +
       "\n with message: " + e.getMessage());
    }
  }

  /**
   * Indexes the given file using the given writer, or if a directory is given,
   * recurses over files and directories found under the given directory.
   * 
   * NOTE: This method indexes one document per input file.  This is slow.  For good
   * throughput, put multiple documents into your input file(s).  An example of this is
   * in the benchmark module, which can create "line doc" files, one document per line,
   * using the
   * <a href="../../../../../contrib-benchmark/org/apache/lucene/benchmark/byTask/tasks/WriteLineDocTask.html"
   * >WriteLineDocTask</a>.
   *  
   * @param writer Writer to the index where the given file/dir info will be stored
   * @param file The file to index, or the directory to recurse into to find files to index
   * @throws IOException If there is a low-level I/O error
 * @throws ParserConfigurationException 
 * @throws SAXException 
   */
  private static void indexDocs(IndexWriter writer, File file)
    throws IOException, ParserConfigurationException, SAXException {
    if (file.canRead()) {
      if (file.isDirectory()) {
        String[] files = file.list();
        if (files != null) {
          for (int i = 0; i < files.length; i++) {
            indexDocs(writer, new File(file, files[i]));
          }
        }
      } else {

        FileInputStream fis;
        try {
          fis = new FileInputStream(file);
        } catch (FileNotFoundException fnfe) {
          return;
        }

        try {
        	//A�adido para la creacion de indices
        	DocumentBuilderFactory xmlParserF = DocumentBuilderFactory.newInstance();
        	DocumentBuilder xmlParser = xmlParserF.newDocumentBuilder();
        	org.w3c.dom.Document parseador = xmlParser.parse(file);
          // make a new, empty document
          Document doc = new Document();
 
          Field pathField = new StringField("ruta", file.getPath(), Field.Store.YES);
          doc.add(pathField);

          //A�adimos la creacion de los indices
         
          indexDate(parseador,doc);
          indexAutor(parseador, doc);
          indexTematico(parseador, doc);
         
          if (writer.getConfig().getOpenMode() == OpenMode.CREATE) {
            System.out.println("Añadiendo: " + file);
            writer.addDocument(doc);
          } else {         
            System.out.println("Actualizando: " + file);
            writer.updateDocument(new Term("path", file.getPath()), doc);
          }
          
        } finally {
          fis.close();
        }
      }
    }
  }
    
    private static void indexTematico(org.w3c.dom.Document parseador,Document doc){
        NodeList lista = parseador.getElementsByTagName("dc:title");
        String titulo = "";
        
        //Cogemos el campo title
        for(int i = 0; i<lista.getLength(); i++){
      	  titulo = titulo + " " + lista.item(i).getFirstChild().getNodeValue();
        }
        
        lista = parseador.getElementsByTagName("dc:description");
        String desc = "";
        
        //Cogemos el campo description
        for(int i = 0; i<lista.getLength(); i++){
      	  try{
          	  desc = desc + " " + lista.item(i).getFirstChild().getNodeValue();
          	  } catch (NullPointerException e){
          		  System.out.println("Acceso a puntero nulo");
          	  }
        }
        
        Field tema = new TextField(SearchFiles.TEMATICO, titulo + " " + desc, Store.YES);
        tema.setBoost(BOOST_TEMA);
        doc.add(tema);
    }
    
    private static void indexAutor(org.w3c.dom.Document parseador,Document doc){
  	  	NodeList lista = parseador.getElementsByTagName("dc:identifier");
        String id = "";
        
        for(int i = 0; i<lista.getLength(); i++){
      	  id = id + " " + lista.item(i).getFirstChild().getNodeValue();
        }
        
        lista = parseador.getElementsByTagName("dc:publisher");
        String pub = "";
        
        for(int i = 0; i<lista.getLength(); i++){
      	  pub = pub + " " + lista.item(i).getFirstChild().getNodeValue();
        }
        
       lista = parseador.getElementsByTagName("dc:creator");
   	   String subject = "";
   	   
   	   for(int i = 0; i<lista.getLength(); i++){
   		   try{
   			   subject = subject + " " + lista.item(i).getFirstChild().getNodeValue();
   		   } catch (NullPointerException e){
   			   System.out.println("ERROR: acceso a puntero nulo.");
   		   }
   	   }
        
        Field autor = new StringField(SearchFiles.AUTOR, id + " " + pub + " " + subject, Store.YES);
        autor.setBoost(BOOST_AUTOR);
        doc.add(autor);
    }   
    
   
    
   private static void  indexDate(org.w3c.dom.Document parseador,Document doc){
  	  NodeList lista = parseador.getElementsByTagName("dc:date");
  	  int date = 0;
        
        for(int i = 0; i<lista.getLength(); i++){
      	  try{
  			    date = Integer.parseInt(lista.item(i).getFirstChild().getNodeValue().trim());
  		   } catch (NullPointerException e){
  			   System.out.println("ERROR: acceso a puntero nulo.");
  		   }
      	 
        }
        
        FieldType myIntType = new FieldType(IntField.TYPE_STORED);
        myIntType.setOmitNorms(false);
        Field dateF = new IntField(SearchFiles.FECHA, date, myIntType);
        dateF.setBoost(BOOST_FECHA);
        doc.add(dateF);
    }

}
