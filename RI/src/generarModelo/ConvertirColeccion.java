package generarModelo;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.util.ArrayList;
import java.util.HashMap;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.jena.ontology.Individual;
import org.apache.jena.ontology.OntModel;
import org.apache.jena.rdf.model.Model;
import org.apache.jena.rdf.model.Property;
import org.apache.jena.rdf.model.ResIterator;
import org.apache.jena.rdf.model.Resource;
import org.apache.jena.vocabulary.RDF;
import org.apache.jena.vocabulary.RDFS;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;

public class ConvertirColeccion {

	String NS;
	HashMap<String, Boolean> autores, publishers;
	
	public ConvertirColeccion() {
		autores = new HashMap<>(); publishers = new HashMap<>();
	}
	
	public static void main(String[] args) throws Exception{
		String zaguan = "C:/Users/javi-/Documents/Zaguan.owl", coleccion = "C:/Users/javi-/Downloads/recordsdc/prueba", destino = "C:/Users/javi-/Downloads/recordsdc/result/Coleccion.owl";
		new ConvertirColeccion().convertirColeccion(zaguan, coleccion, destino);
	}
	
	public void convertirColeccion(String rutaOntologia, String rutaColeccion, String destino) throws Exception{
		Cargar loader = new Cargar(rutaOntologia);
		OntModel model = loader.getOntModel(); 	
		File coleccion = new File(rutaColeccion);
		
		if ( !coleccion.isDirectory() ){
			System.err.println("No se ha encontrado la coleccion!");
			System.exit(-1);
		}
		
		for(File documento: coleccion.listFiles()){
			anyadirModelo(loader, documento);
		}
		//lo guardamos en un fichero rdf
		try {
			model.write(new FileOutputStream(new File(destino)));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}	    
	}

	private void anyadirModelo(Cargar loader, File documento) throws Exception {
		OntModel model = loader.getOntModel();
		HashMap<String, ArrayList<String>> claves = loader.indiceInvertido;
		
		DocumentBuilderFactory xmlParserF = DocumentBuilderFactory.newInstance();
    	DocumentBuilder xmlParser = xmlParserF.newDocumentBuilder();
    	org.w3c.dom.Document parseador = xmlParser.parse(documento);
    	
    	String identificador = getIdentificador(parseador);
    	NS = loader.NS;
    	
    	iniciarKeywords(model, claves);
    	
    	Resource doc = model.createResource(NS + identificador);
    	
    	parseTipo(parseador, doc, model);
    	
    	parseAutor(parseador, doc, model);
    	parsePublicadoPor(parseador, doc, model);
    	
    	parseAnyo(parseador, doc, model);
    	parseContent(parseador, doc, model, claves);

	}
	
	private void parseContent(Document parseador, Resource doc, OntModel model, HashMap<String, ArrayList<String>> claves) {
		NodeList lista = parseador.getElementsByTagName("dc:description");
		String id = null;
		
		 for(int i = 0; i<lista.getLength(); i++){
	      	  id = lista.item(i).getFirstChild().getNodeValue();
		 }
		 
		 String[] palabras = id.split(" ");
		 for (String palabra:palabras){
			 if ( claves.containsKey(palabra.trim()) ){
				 ArrayList<String> categorias = claves.get(palabra.trim());
				 for(String categoria: categorias){
			      	 model.add(doc, model.getProperty(NS + "tematica"), categoria);
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
			 if ( claves.containsKey(palabra.trim()) ){
				 ArrayList<String> categorias = claves.get(palabra.trim());
				 for(String categoria: categorias){
					 System.out.println("Cool!");
			      	 model.add(doc, model.getProperty(NS + "tematica"), categoria);
				 }
			 }
		 }
	}

	private void iniciarKeywords(OntModel model, HashMap<String, ArrayList<String>> claves) {
		Property prop = model
				.getProperty(NS + "keyword");
		ResIterator ri = model.listSubjectsWithProperty(prop);
		while (ri.hasNext()) {
			Resource r = ri.next();
			String keywords = r.getProperty(prop).getString();
			//System.out.println("URI: " + keywords);
			
			String[] keys = keywords.split(",");
			for(String termino:keys){
				termino = termino.trim();
				if ( !claves.containsKey(termino)){
					claves.put(termino ,new ArrayList<String>());
				}
				claves.get(termino).add(r.getLocalName());
				//System.out.println(r.getLocalName());
			}
		}
		for (String l:claves.keySet()) System.out.println(l);
	}

	private void parseAnyo(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:date");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	 
      	  model.add(doc, model.getProperty(NS + "anyo"), id);
        }
	}

	private void parseAutor(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:creator");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	  if ( !autores.containsKey(id)){
      		  autores.put(id, true);
      		  Resource autor = model.createResource(NS + id);
      		  Resource autorC = model.getResource(NS + "Autor");
      		  model.add(autor, RDF.type, autorC);
      	  }
      	  model.add(doc, model.getProperty(NS + "escritoPor"), model.createResource(NS + id));
        }
	}
	
	private void parsePublicadoPor(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:publisher");
	        
        for(int i = 0; i<lista.getLength(); i++){
      	  String id = lista.item(i).getFirstChild().getNodeValue();
      	  if ( !publishers.containsKey(id)){
      		  publishers.put(id, true);
      		  Resource pb = model.createResource(NS + id);
      		  Resource pbC = model.getResource(NS + "Publicador");
      		  model.add(pb, RDF.type, pbC);
      	  }
      	  model.add(doc, model.getProperty(NS + "publicadoPor"), model.createResource(NS + id));
        }
	}

	private void parseTipo(Document parseador, Resource doc, OntModel model) {
		NodeList lista = parseador.getElementsByTagName("dc:title");
		boolean guarda = true;
	        
        for(int i = 0; i<lista.getLength(); i++){
        	if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TFG")|
        			lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("PFC")){
        		Resource TFG = model.getResource(NS + "TFG");
        		model.add(doc,RDF.type, TFG);
        		guarda = false;
        	}
        	else if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TFM")){
        		Resource TFM = model.getResource(NS + "TFM");
        		model.add(doc,RDF.type, TFM);
        		guarda = false;
        	}
        	else if (lista.item(i).getFirstChild().getNodeValue().toUpperCase().contains("TESIS")){
        		Resource Tesis = model.getResource(NS + "Tesis");
        		model.add(doc,RDF.type, Tesis);
        		guarda = false;
        	}
        }
        
        if ( guarda ) model.add(doc,RDF.type, model.getResource(NS + "TFG"));
	}

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
