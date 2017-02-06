package semantico;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;

import org.apache.jena.query.Query;
import org.apache.jena.query.QueryExecution;
import org.apache.jena.query.QueryExecutionFactory;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.QuerySolution;
import org.apache.jena.query.ResultSet;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.util.FileManager;

public class SemanticSearcher {
	
	 public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
		
	    String rdfPath = null;
	    String rdfsPath=null;
	    String infoNeeds=null;
	    String resultFile = null;
	   
	    if (args.length <8) {
	        System.err.println(usage);
	        System.exit(1);
	      }else {
		    for(int i = 0;i < args.length;i++) {
		        if ("-rdf".equals(args[i])) {
		        	rdfPath = args[i+1];
		          i++;
		        } 
		        else if("-rdfs".equals(args[i])){
		        	rdfsPath=args[i+1];
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
	    Model modelo = ModelFactory.createDefaultModel(),
	    		aux = ModelFactory.createDefaultModel();
	    
	    modelo.read(FileManager.get().open(rdfsPath), null);
	    
	    aux.read(FileManager.get().open(rdfPath), null);
	    modelo.add(aux);
	    procesarSPARQL(infoNeeds,modelo,resultFile);
	    
	    Evaluation e = new Evaluation("zaguanRels.txt",resultFile);
	    e.evaluar();   
	    
	 }
	 
	 /**
	  * Procesa el fichero de consultas, y escribe los resultados y la evaluacion.
	  * 
	  * @param sparql Fichero con las consultas.
	  * @param model Modelo rdf a consultar.
	  * @param resultFile Resultados de las consultas.
	  * @throws IOException Si no puede escribir en los ficheros.
	  */
	 public static void procesarSPARQL(String sparql,Model model,String resultFile) throws IOException{
		//Procesamos el fichero sparql
		Scanner leerFichero = new Scanner(new File(sparql));
		String prefijos = "PREFIX  zaguan: <http://www.semanticweb.org/javi-/ontologies/2017/0/Zaguan#>"
				+ " PREFIX  Skos: <http://www.semanticweb.org/javi-/ontologies/2017/0/Skos#> "
				+ " PREFIX skos: <http://www.w3.org/2004/02/skos/core#> "
				+ " PREFIX xsd: <http://www.w3.org/2001/XMLSchema#> "
				+ " PREFIX rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> ";
		
		//Abrimos el fichero en el que vamos a escribir los resultados		
		FileWriter fichero = null;
	    PrintWriter pw = null;
	    try {
			fichero = new FileWriter(new File(resultFile));
			pw = new PrintWriter(fichero);
		 
			while ( leerFichero.hasNextLine() ){
				String info_need = leerFichero.next(),consulta = leerFichero.nextLine();
				//aplica las consultas al grafo y muestra los resultados
				consulta = prefijos +"\n" +consulta;
				Query query = QueryFactory.create(consulta);
				
				  QueryExecution qexec = QueryExecutionFactory.create(query, model) ;
				  try {
				    ResultSet results = qexec.execSelect() ;
				    while (results.hasNext()){
				      QuerySolution soln = results.nextSolution() ;
				      Resource x = soln.getResource("x");				     
				      String [] doc = x.toString().split("#");
				      int id = Integer.parseInt(doc[1]);
				      String ruta=info_need+'	'+"oai_zaguan.unizar.es_"+id+".xml";
				      pw.println(ruta);
				      
				    }
				    
				  } 
				  finally { qexec.close() ; }
	
				  
			}
			leerFichero.close();
		 } 
	    catch (IOException e) {
			e.printStackTrace();
		}
	    pw.close();
	    fichero.close();
		
	 }
}
