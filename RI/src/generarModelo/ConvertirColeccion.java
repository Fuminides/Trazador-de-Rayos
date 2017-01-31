package generarModelo;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.HashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.tartarus.snowball.ext.spanishStemmer;
import org.apache.jena.ontology.OntModel;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.ModelFactory;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.riot.Lang;
import org.apache.jena.riot.RDFDataMgr;
import org.apache.jena.vocabulary.RDF;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

public class ConvertirColeccion {

	String NS;
	HashMap<String, Boolean> autores, publishers;
	spanishStemmer analizador = new spanishStemmer();
	Model coleccionOnt;
	public static String namespace = "http://www.semanticweb.org/javi-/ontologies/2017/0/Coleccion";

	
	public ConvertirColeccion() {
		autores = new HashMap<>(); publishers = new HashMap<>();
	}
	
	public static void main(String[] args) throws Exception{
		String coleccion = "C:/Users/javi-/Downloads/recordsdc/prueba", destino = "C:/Users/javi-/Downloads/recordsdc/result/Coleccion.rdf",
				skos = "C:/Users/javi-/Documents/Skos.owl";
		new ConvertirColeccion().convertirColeccion(skos, coleccion, destino);
	}
	
	public void convertirColeccion(String skos, String rutaColeccion, String destino) throws Exception{
		Cargar loader = new Cargar(skos);
    	NS = loader.NS;
		File coleccion = new File(rutaColeccion);
		coleccionOnt = ModelFactory.createDefaultModel();
		
		coleccionOnt.setNsPrefix("", namespace + "#");
    	coleccionOnt.setNsPrefix("zaguan", NS);
		coleccionOnt.setNsPrefix("terms", loader.SNS );
		coleccionOnt.setNsPrefix("base", namespace );
		
		if ( !coleccion.isDirectory() ){
			System.err.println("No se ha encontrado la coleccion!");
			System.exit(-1);
		}
				
		for(File documento: coleccion.listFiles()){
			anyadirModelo(loader, documento);
		}
		//lo guardamos en un fichero rdf
		try {
			coleccionOnt.add(loader.skos);
			RDFDataMgr.write(new FileOutputStream(new File(destino)), coleccionOnt, Lang.RDFXML);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}	    
	}

	/**
	 * Anyade al fichero RDF el documento de Zaguan.
	 * 
	 * @param loader
	 * @param documento
	 * @throws Exception
	 */
	private void anyadirModelo(Cargar loader, File documento) throws Exception {
		
		OntModel model = loader.getOntModel();
		HashMap<String, ArrayList<Resource>> claves = loader.indiceInvertido;
		
		DocumentBuilderFactory xmlParserF = DocumentBuilderFactory.newInstance();
    	DocumentBuilder xmlParser = xmlParserF.newDocumentBuilder();
    	org.w3c.dom.Document parseador = xmlParser.parse(documento);
    	
    	String identificador = getIdentificador(parseador);
    	Resource doc = coleccionOnt.createResource(namespace + "#" + identificador);
    	
    	parseTipo(parseador, doc, model);
    	
    	parseAutor(parseador, doc, model);
    	parsePublicadoPor(parseador, doc, model);
    	parseContent(parseador, doc, model, claves);
    	
    	parseAnyo(parseador, doc, model);
    	parseTitulo(parseador, doc, model);
    	parseResumen(parseador, doc, model);
	}
	
	/**
	 * Anyade el titulo de un documento al modelo.
	 * 
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parseTitulo(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:title");
		String id = null;
		
		 for(int i = 0; i<lista.getLength(); i++){
	      	  id = lista.item(i).getFirstChild().getNodeValue();
		 }
		 if ( model.getProperty(NS + "titulo")==null){
			 model.createProperty(NS + "titulo");
		 }
     	 coleccionOnt.add(doc, model.getProperty(NS + "titulo"), id);
	}
	
	/**
	 * Anyade el titulo de un documento al modelo.
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parseResumen(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:description");
		String id = null;
		
		 for(int i = 0; i<lista.getLength(); i++){
	      	  id = lista.item(i).getFirstChild().getNodeValue();
		 }
		 if ( model.getProperty(NS + "Summary")==null){
			 model.createProperty(NS + "Summary");
		 }
		 coleccionOnt.add(doc, model.getProperty(NS + "Summary"), id);
	}

	/**
	 * Anyade las categorias al documento en el modelo.
	 * 
	 * @param parseador
	 * @param doc
	 * @param model
	 * @param claves
	 */
	private void parseContent(Document parseador, Resource doc, OntModel model, HashMap<String, ArrayList<Resource>> claves) {
		NodeList lista = parseador.getElementsByTagName("dc:description");
		String id = null;
		 for(int i = 0; i<lista.getLength(); i++){
	      	  id = lista.item(i).getFirstChild().getNodeValue();
		 }
		 if(model.getProperty(NS + "tematica")==null){
			 model.createProperty(NS + "tematica");
		 }
		 String[] palabras = id.split(" ");
		 for (String palabra:palabras){
			 palabra = palabra.trim().toLowerCase();
			 analizador.setCurrent(palabra);
			 analizador.stem();
			 palabra = analizador.getCurrent();
			 if ( claves.containsKey(palabra.trim()) ){
				 ArrayList<Resource> categorias = claves.get(palabra.trim());
				 for(Resource categoria: categorias){
					 coleccionOnt.add(doc, model.getProperty(NS + "tematica"), categoria);
				 }
			 }
		 }
		 
		 lista = parseador.getElementsByTagName("dc:title");
		 id = null;
		
		 for(int i = 0; i<lista.getLength(); i++){
	      	  id = lista.item(i).getFirstChild().getNodeValue();
		 }
		 
		 palabras = id.split(" ");
		 for (String palabra:palabras){
			 palabra = palabra.trim().toLowerCase();
			 analizador.setCurrent(palabra);
			 analizador.stem();
			 palabra = analizador.getCurrent();
			 if ( claves.containsKey(palabra.trim()) ){
				 ArrayList<Resource> categorias = claves.get(palabra.trim());
				 for(Resource categoria: categorias){
					 coleccionOnt.add(doc, model.getProperty(NS + "tematica"), categoria);
				 }
			 }
		 }
	}

	/**
	 * Anyade el anyo del documento al modelo.
	 * 
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parseAnyo(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:date");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	  if (model.getProperty(NS + "anyo")==null){
      		  model.createProperty(NS + "anyo");
      	  }
      	  coleccionOnt.add(doc, model.getProperty(NS + "anyo"), id);
        }
	}

	/**
	 * Anyade el autor al documento en el modelo.
	 *   
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parseAutor(Document parseador, Resource doc, Model model) {
		NodeList lista = parseador.getElementsByTagName("dc:creator");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	  if ( !autores.containsKey(id)){
      		  if ( model.getResource(NS + "Autor")==null){
      			  model.createResource(NS + "Autor");
      			  model.createProperty(NS + "escritoPor");
      		  }
      		  autores.put(id, true);
      		  Resource autor = model.createResource(namespace + "#"+ id);
      		  Resource autorC = model.getResource(NS + "Autor");
      		  coleccionOnt.add(autor, RDF.type, autorC);
      	  }
      	  coleccionOnt.add(doc, model.getProperty(NS + "escritoPor"), model.getResource(namespace +"#"+ id));
        }
	}
	
	/**
	 * Anyade al modelo la entidad que ha publicado el documento.
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parsePublicadoPor(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:publisher");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	  if ( !publishers.containsKey(id)){
      		  if ( model.getResource(NS + "Publicador") == null){
      			  model.createResource(NS + "Publicador");
      			  model.createProperty(NS + "publicadoPor");
      		  }
      		  publishers.put(id, true);
      		  Resource pb = model.createResource(namespace +"#"+ id);
      		  Resource pbC = model.getResource(NS + "Publicador");
      		  coleccionOnt.add(pb, RDF.type, pbC);
      	  }
      	  coleccionOnt.add(doc, model.getProperty(NS + "publicadoPor"), model.getResource(namespace +"#"+ id));
        }
	}

	/**
	 *  Crea el documento en el model segun el tipo de documento que sea: TFG, TFM o Tesis.
	 * @param parseador
	 * @param doc
	 * @param model
	 */
	private void parseTipo(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:identifier");
		boolean guarda = true;
	        
        for(int i = 0; i<lista.getLength(); i++){
        	if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TFG")|
        			lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("PFC")){
        		if (model.getResource(NS + "TFG") == null){
        			model.createResource(NS + "TFG");
        		}
        		Resource TFG = model.getResource(NS + "TFG");
        		coleccionOnt.add(doc,RDF.type, TFG);
        		guarda = false;
        	}
        	else if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TFM")){
        		if (model.getResource(NS + "TFM") == null){
        			model.createResource(NS + "TFG");
        		}
        		Resource TFM = model.getResource(NS + "TFM");
        		coleccionOnt.add(doc,RDF.type, TFM);
        		guarda = false;
        	}
        	else if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TESIS")){
        		if (model.getResource(NS + "Tesis") == null){
        			model.createResource(NS + "TFG");
        		}
        		Resource Tesis = model.getResource(NS + "Tesis");
        		coleccionOnt.add(doc,RDF.type, Tesis);
        		guarda = false;
        	}
        }
        if (model.getResource(NS + "TFG") == null){
			model.createResource(NS + "TFG");
		}
        if ( guarda ) coleccionOnt.add(doc,RDF.type, model.getResource(NS + "TFG"));
	}

	/**
	 * Devuelve el numero del documento Zaguan.
	 * 
	 * @param parseador
	 * @return
	 */
	private static String getIdentificador(Document parseador) {
		NodeList lista = parseador.getElementsByTagName("dc:identifier");
		String id = "";
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  id = id + " " + lista.item(i).getFirstChild().getNodeValue();
      	  id = id.replaceAll("\\D+","");
      	  if ( id.length() > 0 ) return id;
        }
        
   		return id;
	}
	
	
	
	

}
