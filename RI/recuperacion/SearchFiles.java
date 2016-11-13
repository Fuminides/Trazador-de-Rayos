package recuperacion;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.search.BooleanClause;
import org.apache.lucene.search.BooleanClause.Occur;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopDocs;
import org.apache.lucene.search.NumericRangeQuery;
import org.apache.lucene.search.BooleanQuery;
import org.apache.lucene.store.FSDirectory;
import org.w3c.dom.NodeList;

import parser.Parser;

/**
 * Clase: SearchFiles
 * Se encarga de procesar las necesidades de informacion y buscarlas en un indice Lucene
 * ya creado previamente.
 * 
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
public class SearchFiles {

  private SearchFiles() {}

  public final static String AUTOR = "autor";
  public final static String TEMATICO = "tema";
  public final static String FECHA = "fecha";
  
  public static void main(String[] args) throws Exception {
    String usage ="No se han introducido los archivos correctos";
    String error = "Por favor, indicame la ruta del directorio donde generar los indices y la ruta del fichero de las necesidades de informacion y la ruta del fichero de resultados";
    if (args.length > 0 && ("-h".equals(args[0]) || "-help".equals(args[0]))) {
      System.out.println(usage);
      System.exit(0);
    }

    String index = "index";
    String infoNeeds=null;
    String resultFile=null;
    if (args.length == 0) {
        System.err.println(error);
        System.exit(1);
      }else {
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
      }
    
    if (infoNeeds == null || resultFile == null || index == null) {
        System.err.println("Usage: " + usage);
        System.exit(1);
      }
    
    
    IndexReader reader = DirectoryReader.open(FSDirectory.open(new File(index)));
    IndexSearcher searcher = new IndexSearcher(reader);
    
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
    ArrayList<String> result = null;
    
    FileWriter fichero = null;
    PrintWriter pw = null;
    try{
        fichero = new FileWriter(resultFile);
        pw = new PrintWriter(fichero);
	    
	    for(int i=1;i<necesidades.length;i++){
	    	Parser pars = new Parser(necesidades[i]);
	    	q= pars.execute();
	    	
	    	//Se a�ade la consulta booleana para la fecha 
	    	String fecha=pars.getFecha();
	  
	    	BooleanQuery globalQuery = new BooleanQuery();
	    	globalQuery.add(q, Occur.SHOULD);
	    	
	    	if(!fecha.equals("")){
	    		String [] anyos = fecha.split(",");
	    		try{
		    		int anno1 = Integer.parseInt(anyos[0]);
		    		if(anyos.length>1){
		    			int anno2 = Integer.parseInt(anyos[1]);
				  		NumericRangeQuery<Integer> entre = NumericRangeQuery.newIntRange(FECHA, anno1, anno2, true, true);
				  		globalQuery.add(entre, BooleanClause.Occur.SHOULD);
		    		}
		    		else{
		    			NumericRangeQuery<Integer> apartir = NumericRangeQuery.newIntRange(FECHA, anno1, null, true, false);
		    			globalQuery.add(apartir, BooleanClause.Occur.SHOULD);
		    		}
		    		
	    		}
	    		catch(NumberFormatException e){
	    			e.printStackTrace();
	    		}
	    	}
	    	result = doSearch(searcher, q);
	        System.out.println("Total de documentos encontrados consulta " + i + ": " +result.size());
	    	
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
   * Devuelve en un arrayList la lista de ficheros como resultados a la consulta <query> . 
   * Como parametros recibe un buscador con la lista de los indices previamente indexados y 
   * una query , consulta que se desea realizar. 
   * 
   */
  public static ArrayList<String> doSearch(IndexSearcher searcher, Query query) throws IOException {
	  
	TopDocs results = searcher.search(query,Integer.MAX_VALUE);
    ScoreDoc[] hits = results.scoreDocs;
    ArrayList<String> resultadosFinales= new ArrayList<String>();
    
    for (int i = 0; i < hits.length; i++){
        
    	Document doc = searcher.doc(hits[i].doc);        
        String path = doc.get("ruta");
        
        if (path != null) {
        	resultadosFinales.add(path);
        }
        else {
          System.out.println((i+1) + ". " + "No se ha encontrado la ruta de este fichero.");
        }
                  
    }
    return resultadosFinales;
  }

}