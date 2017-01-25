package semantico;

public class SemanticGenerator {
	public static void main(String[] args) throws Exception {	
		String usage ="No se han introducido los archivos correctos";
	    
		
	    String rdfPatth = null;
	    //modelo terminologico en SKOS
	    String skosPath=null;
	    String docsPath=null;
	    if (args.length <6) {
	        System.err.println(usage);
	        System.exit(1);
	      }
	    else {
		    for(int i = 0;i < args.length;i++) {
		        if ("-rdf".equals(args[i])) {
		        	rdfPatth = args[i+1];
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
	    
	    
	    
	    
	    
	    
	} 

}
