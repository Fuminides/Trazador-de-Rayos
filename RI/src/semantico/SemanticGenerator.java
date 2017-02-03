package semantico;

import generarModelo.ConvertirColeccion;
/**
 * Clase que crea el fichero RDF con los documentos de la coleccion.
 * 
 * @author Javier Fumanal Idocin, Silvia Uson Fortanet
 *
 */
public class SemanticGenerator {
	public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
		
	    String rdfPath = null;
	    String skosPath=null;
	    String docsPath=null;
	    
	    if (args.length <6) {
	        System.err.println(usage);
	        System.exit(1);
	      }
	    else {
		    for(int i = 0;i < args.length;i++) {
		        if ("-rdf".equals(args[i])) {
		        	rdfPath = args[i+1];
		        	i++;
		        }
		        else if("-skos".equals(args[i])){
		        	skosPath=args[i+1];
			      	i++;
			    }
		        else if("-docs".equals(args[i])){
		        	docsPath=args[i+1];
		      	  i++;
		        }
		    }
	    }
	    
	    new ConvertirColeccion().convertirColeccion(skosPath, docsPath, rdfPath);
	    
	} 

}
