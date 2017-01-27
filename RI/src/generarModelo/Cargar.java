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
import org.tartarus.snowball.ext.spanishStemmer;


public class Cargar {

	OntModel model;
	
	String NS, SNS, keyword = "keyword";
	HashMap<String, ArrayList<Resource>> indiceInvertido = new HashMap<>();
	
	public OntModel getOntModel() {
		return model;
	}
	
	/**
	 * Carga una coleccion en memoria en RDF.
	 * 
	 * @param ontoFile
	 * @param dummy solo para diferenciar constructores con sobrecarga.
	 */
	public Cargar(String ontoFile, boolean dummy)
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
	}
	
	/**
	 * Carga una ontologia, para la posterior utilizacion en una coleccion.
	 * Inicializa las palabras clave.
	 * 
	 * @param ontoFile
	 */
	public Cargar(String ontoFile, String skosFile)
	{   
	    OntModel ontoModel = ModelFactory.createOntologyModel(OntModelSpec.OWL_MEM, null);
	    OntModel ontoModelAux = ModelFactory.createOntologyModel(OntModelSpec.OWL_MEM, null);

	    try 
	    {
	        InputStream in = FileManager.get().open(ontoFile), 
	        		skosInput = FileManager.get().open(skosFile);
	        try 
	        {
	            ontoModel.read(in, null);
	    	    NS = ontoModel.getNsPrefixURI("");
	    	    skosInput = FileManager.get().open(skosFile);
	            ontoModelAux.read(skosInput, null);
	         
	            ontoModel.add(ontoModelAux);
	            
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
	    
	    SNS = ontoModelAux.getNsPrefixURI("");
	    
 		Property prop = model
 				.getProperty(SNS + keyword);
 		ResIterator ri = model.listSubjectsWithProperty(prop);
 		spanishStemmer stemmer = new spanishStemmer();
 		while (ri.hasNext()) {
 			Resource r = ri.next();
 			String keywords = r.getProperty(prop).getString();
 			for ( String clave:keywords.split(",")){
 				clave = clave.trim().toLowerCase();
 				stemmer.setCurrent(clave);
 				stemmer.stem();
 				clave = stemmer.getCurrent();
 				if ( clave.length() > 0){
 					if ( indiceInvertido.containsKey(clave)){
 						indiceInvertido.get(clave).add(r);
 					}
 					else{
 						indiceInvertido.put(clave, new ArrayList<>());
 						indiceInvertido.get(clave).add(r);
 					}
 				}
 			}
 		}
	    
	}

}
