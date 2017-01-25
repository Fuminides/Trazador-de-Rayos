package semantico;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Scanner;

import org.apache.jena.query.Query;
import org.apache.jena.query.QueryExecution;
import org.apache.jena.query.QueryExecutionFactory;
import org.apache.jena.query.QueryFactory;
import org.apache.jena.query.QuerySolution;
import org.apache.jena.query.ResultSet;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.RDFNode;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.util.FileManager;



public class SemanticSearcher {
	 public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
	    // ruta del fichero donde se realicen las consultas sparql
		String sparql = "necesidadesInformacionElegidas.txt";
		
	    String rdfPatth = null;
	    //Es el entregado en XML (el ultimo)
	    String rdfsPath=null;
	    String infoNeeds=null;
	    String resultFile = null;
	    if (args.length <8) {
	        System.err.println(usage);
	        System.exit(1);
	      }else {
		    for(int i = 0;i < args.length;i++) {
		        if ("-rdf".equals(args[i])) {
		        	rdfPatth = args[i+1];
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
		  // no se exactamente que fichero es
	    String fichero =null;
	    procesarSPARQL(sparql,rdfPatth);
	    
	    Evaluation e = new Evaluation(fichero,resultFile);
	    e.evaluar();   
	    
	 }
	 
	 public static void procesarSPARQL(String sparql,String rdfPatth) throws FileNotFoundException{
		//Procesamos el fichero sparql
		Scanner leerFichero = new Scanner(new File(sparql));
		Model model = FileManager.get().loadModel(rdfPatth);
		while ( leerFichero.hasNextLine() ){
			String info_need = leerFichero.next(),consulta = leerFichero.next();
			//aplica las consultas al grafo y muestra los resultados
			Query query = QueryFactory.create(consulta) ;
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
