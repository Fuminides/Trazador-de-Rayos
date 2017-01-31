package semantico;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;

import org.apache.jena.query.Query;
import org.apache.jena.query.QueryExecution;
import org.apache.jena.query.QueryExecutionFactory;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.QuerySolution;
import org.apache.jena.query.ResultSet;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.RDFNode;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.util.FileManager;

import generarModelo.Cargar;
import generarModelo.ConvertirColeccion;



public class SemanticSearcher {
	 public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
	    // ruta del fichero donde se realicen las consultas sparql
		//String sparql = "necesidadesInformacionElegidas.txt";
		
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
	    String fichero =null;
	    Model modelo = ModelFactory.createDefaultModel(),
	    		aux = ModelFactory.createDefaultModel();
	    
	    modelo.read(FileManager.get().open(rdfsPath), null);
	    
	    aux.read(FileManager.get().open(rdfPath), null);
	    modelo.add(aux);
	    
	    procesarSPARQL(infoNeeds,modelo);
	    
	    Evaluation e = new Evaluation(fichero,resultFile);
	    e.evaluar();   
	    
	 }
	 
	 public static void procesarSPARQL(String sparql,Model model) throws FileNotFoundException{
		//Procesamos el fichero sparql
		Scanner leerFichero = new Scanner(new File(sparql));
		String prefijos = "PREFIX  zaguan: <http://www.semanticweb.org/javi-/ontologies/2017/0/Zaguan#>"
				+ " PREFIX  Skos: <http://www.semanticweb.org/javi-/ontologies/2017/0/Skos#> "
				+ " PREFIX skos: <http://www.w3.org/2004/02/skos/core#> "
				+ " PREFIX xsd: <http://www.w3.org/2001/XMLSchema#>";
		while ( leerFichero.hasNextLine() ){
			@SuppressWarnings("unused")
			String info_need = leerFichero.next(),consulta = leerFichero.nextLine();
			//aplica las consultas al grafo y muestra los resultados
			consulta = prefijos +"\n" +consulta;
			Query query = QueryFactory.create(consulta);
			System.out.println(query);
			//Query query = query.a ;
			
			  QueryExecution qexec = QueryExecutionFactory.create(query, model) ;
			  try {
			    ResultSet results = qexec.execSelect() ;
			    while (results.hasNext()){
			      QuerySolution soln = results.nextSolution() ;
			      Resource x = soln.getResource("x");
			      Resource y = soln.getResource("y");
			      RDFNode z = soln.get("z") ;  
			      if (z.isLiteral()) {
						System.out.println(x.getURI()+" - "+ y.getURI()+" - "+ z.toString());
					} else {
						System.out.println(x.getURI()+" - "+ y.getURI()+" - "+ z.asResource().getURI());
					}
			    }
			  } 
			  finally { qexec.close() ; }

			  
		}
		leerFichero.close();
	 }
}
