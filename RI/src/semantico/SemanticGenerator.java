package semantico;

import java.io.File;

import generarModelo.ConvertirColeccion;

public class SemanticGenerator {
	public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
	    String rdfsPath = "./Zaguan.owl"; //Valor por defecto
		
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
		        if ("-rdfs".equals(args[i])) {
		        	rdfsPath = args[i+1];
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
	    
	    if ( !new File(rdfsPath).exists()){
	    	System.err.println("Parece que has movido o eliminado el fichero con el esquema .owl");
	    	System.exit(-1);
	    }
	    
	    new ConvertirColeccion().convertirColeccion(skosPath, docsPath, rdfPath);
	    
	} 

}
