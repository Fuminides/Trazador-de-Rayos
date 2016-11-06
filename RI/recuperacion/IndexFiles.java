package recuperacion;

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

/**
 * Clase: IndexFiles.
 * Indexa todos los archivos dados en un indice valido para la libreria Lucene.
 * 
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
public class IndexFiles {
  
  private IndexFiles() {}

  private final static float BOOST_AUTOR = 2.0f;
  private final static float BOOST_FECHA = 0.5f;
  private final static float BOOST_TEMA = 1.4f;
  
  /** Indexa todos los ficheros en el 
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
   * Indexa el fichero dado en el indice especificado. Si se especifica un directorio, se reocorre
   * recursivamente y se indexan todos los ficheros contenidos en el mismo.
   * 
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
        	DocumentBuilderFactory xmlParserF = DocumentBuilderFactory.newInstance();
        	DocumentBuilder xmlParser = xmlParserF.newDocumentBuilder();
        	org.w3c.dom.Document parseador = xmlParser.parse(file);

        	Document doc = new Document();
 
          Field pathField = new StringField("ruta", file.getPath(), Field.Store.YES);
          doc.add(pathField);

          //A�adimos la creacion de los indices
         
          indexDate(parseador,doc);
          indexAutor(parseador, doc);
          indexTematico(parseador, doc);
         
          if (writer.getConfig().getOpenMode() == OpenMode.CREATE) {
            writer.addDocument(doc);
          } else {         
            writer.updateDocument(new Term("path", file.getPath()), doc);
          }
          
        } finally {
          fis.close();
        }
      }
    }
  }
    
  /**
   * Indexa los campos del fichero que corresponden con el indice tematico.
   * @param parseador
   * @param doc
   */
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
    
    /**
     * Indexa los campos del documentos que estan asociados al indice de identificacion.
     * @param parseador
     * @param doc
     */
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
        
	   FieldType myStringType = new FieldType(StringField.TYPE_STORED);
	   myStringType.setOmitNorms(false);
	   Field autorF = new Field(SearchFiles.AUTOR, id + " " + pub + " " + subject, myStringType);
	   autorF.setBoost(BOOST_AUTOR);
	   doc.add(autorF);
    }   
    
   
   /**
    * Indexa los campos del fichero asociados al indice fecha.
    * @param parseador
    * @param doc
    */
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
        
        //Hacemos esto para permitir sorportar pesos en un indice de enteros.
        FieldType myIntType = new FieldType(IntField.TYPE_STORED);
        myIntType.setOmitNorms(false);
        Field dateF = new IntField(SearchFiles.FECHA, date, myIntType);
        dateF.setBoost(BOOST_FECHA);
        doc.add(dateF);
    }

}
