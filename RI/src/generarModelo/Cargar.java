package generarModelo;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;

import org.apache.jena.ontology.OntModel;
import org.apache.jena.ontology.OntModelSpec;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.Property;
import org.apache.jena.rdf.model.ResIterator;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.shared.JenaException;
import org.apache.jena.util.FileManager;


public class Cargar {

	OntModel model;
	
	String NS, keyword = "keyword";
	HashMap<String, ArrayList<String>> indiceInvertido = new HashMap<>();
	
	public OntModel getOntModel() {
		return model;
	}
	
	public Cargar(String ontoFile)
	{   
	    OntModel ontoModel = ModelFactory.createOntologyModel(OntModelSpec.OWL_MEM, null);
	    try 
	    {
	        InputStream in = FileManager.get().open(ontoFile);
	        try 
	        {
	            ontoModel.read(in, null);
	        } 
	        catch (Exception e) 
	        {
	            e.printStackTrace();
	        }
	    } 
	    catch (JenaException je) 
	    {
	        System.err.println("ERROR" + je.getMessage());
	        je.printStackTrace();
	        System.exit(0);
	    }
	    model = ontoModel;
	    
	    NS = model.getNsPrefixURI("");
	    
 		Property prop = model
 				.getProperty(NS + keyword);
 		ResIterator ri = model.listSubjectsWithProperty(prop);
 		while (ri.hasNext()) {
 			Resource r = ri.next();
 			String categoria = r.getURI().split("#")[1],
 					keywords = r.getProperty(prop).getString();
 			
 			for ( String clave:keywords.split(",")){
 				clave = clave.trim();
 				if ( clave.length() > 0){
 					if ( indiceInvertido.containsKey(clave)){
 						indiceInvertido.get(clave).add(categoria.trim());
 					}
 					else{
 						indiceInvertido.put(clave, new ArrayList<>());
 						indiceInvertido.get(clave).add(categoria.trim());
 					}
 				}
 			}
 		}
 		
 		/*
 		for(String palabra:indiceInvertido.keySet()){
 			System.out.print(palabra +": ");
 			for(String cat:indiceInvertido.get(palabra)){
 				System.out.print(cat + " ");
 			}
 			System.out.println("");
 		}
 		*/
	    
	}
	
	public static void main(String[] args) {
		Cargar modelo = new Cargar(args[0]);
		
	}

}
