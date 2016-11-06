package parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.lucene.analysis.es.SpanishAnalyzer;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.Query;
import org.apache.lucene.util.Version;

import trabajo.SearchFiles;

public class Parser {

	int anyoActual = 2016;
	//Hardcodeado por simpleza.
	private String[] diccionarioLenguajes = 
		{ "frances", "español", "ingles", "portugues",
		  "flamenco", "escoces", "gaelico", "aleman",
		   "polaco", "ruso", "hindu", "japones"
		};
	
	private String[] paises = 
		{ "España", "Francia", "Alemania", "Estados Unidos" };
	
	private String[] diccionarioPublishers =
		{ "Universidad de Zaragoza" };
	
	private String frase;
	
	
	public Parser(String f) {
		frase = f;
	}
	
	public String buscarID(){
		String resultado = "";
		String pattern1 = "oai:zaguan.unizar.es:([0-9])+",
				pattern2 = "TESIS-[0-9][0-9][0-9][0-9]-[0-9][0-9][0-9]";
		 Pattern p = Pattern.compile(pattern1);
		 Matcher m = p.matcher(frase);
		 boolean b = m.find();
		 if (!b){
			 p = Pattern.compile(pattern2);
			 m = p.matcher(frase);
			 b = m.find();
			 
			 if (b){
				return m.group(); 
			 }
		 }
		 else{
			 return m.group();
		 }
		 
		
		return resultado;
	}
	
	public String buscarPublisher(){
		String resultado = "";
		for ( String publisher: diccionarioPublishers){
			Pattern p = Pattern.compile(publisher);
			 Matcher m = p.matcher(frase);
			 boolean b = m.find();
			 
			 if (b){
				return m.group(); 
			 }
		 }
			 
		return resultado;
	}
	
	public String buscarLenguaje(){
		String resultado = ""; String[] palabras = frase.split(" ");
		for(String lenguaje: diccionarioLenguajes){
			for ( String palabra: palabras){
				if ( palabra.compareToIgnoreCase(lenguaje) == 0){
					return palabra;
				}
			}
		}
		return resultado;
	}
	
	public String buscarAutor(){
		String resultado = "";
		String[] palabras = frase.split(" ");
		for ( int i = 1; i < palabras.length; i++){
			if ( Character.isUpperCase(palabras[i].charAt(0))){
				resultado += palabras[i] + " ";
			}
		}
		
		resultado = resultado.replace(".", "").trim();
		
		for (String pais: paises){
			if (pais.compareToIgnoreCase(resultado) == 0) { return "";}
		}
	
		return resultado;
	}
	
	public String buscarFecha(){
		String resultado = "",
				pattern = "entre ([0-9])+ y ([0-9])+",
				pattern2 = "año ([0-9])+",
				pattern3 = "ultimos ([0-9])+ años",
				patternNumber = "([0-9])+";
		
		Pattern p = Pattern.compile(pattern);
		 Matcher m = p.matcher(frase);
		 boolean b = m.find();
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(m.group(0));
			 m.find();
			 String anyo1 = m.group(0);
			 String fecha=m.group(0);
			 System.out.println("fecha antes es: "+fecha);
			 m.find();
			 String anyo2 = m.group(0);
			 fecha=fecha+","+m.group(0);
			 System.out.println("fecha despues es: "+fecha);
			 frase = frase.replace(anyo1, "");
			 frase = frase.replace(anyo2, "");
			 /**El problema esta aqui en este return **/
			 return fecha;
			 //return (anyo1 + "," + anyo2);
		 }
		 
		 p = Pattern.compile(pattern2);
		 m = p.matcher(frase);
		 b = m.find();
		 
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(frase);
			 m.find();
			 frase = frase.replace("año " + m.group(0), "");
			 return m.group(0);
		 }
		 
		 p = Pattern.compile(pattern3);
		 m = p.matcher(frase);
		 b = m.find();
		 
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(frase);
			 m.find();
			 frase = frase.replace("ultimos " + m.group(0) + " años", "");
			 return (anyoActual - Integer.parseInt(m.group(0))) + "," + anyoActual;
		 }
		
		return resultado;
	}
	
	public Query execute() throws ParseException{
		String id = buscarID(), publisher = buscarPublisher(),
				buscarAutor = buscarAutor(), buscarFecha = buscarFecha(), fecha = SearchFiles.FECHA +":", 
				tema = SearchFiles.TEMATICO + ":", autor = SearchFiles.AUTOR + ":", resultado = "";
	    QueryParser parser = new QueryParser(Version.LUCENE_44, SearchFiles.TEMATICO, new SpanishAnalyzer(Version.LUCENE_44));

		if ( id != "" ){
			autor += id;
			 frase = frase.replaceAll(id, "");
		}
		if ( publisher != ""){
			if (autor.charAt(autor.length()-1) == ':' ) autor += publisher;
			else autor += "," + publisher;
			 frase = frase.replaceAll(autor, "");
		}
		if ( buscarAutor != ""){
			if (autor.charAt(autor.length()-1) == ':' ) autor += buscarAutor;
			else autor += "," + buscarAutor;
			 frase = frase.toLowerCase().replaceAll(buscarAutor.toLowerCase(), "");
		}
		
		if ( buscarFecha != ""){
			fecha += buscarFecha;
		}
		
		resultado += tema + frase;
		if ( fecha != SearchFiles.FECHA+":" ) resultado += " " + fecha;
		if ( autor != SearchFiles.AUTOR +":" ) resultado += " " + autor;
	    
		return parser.parse(resultado);
	}
	
	/*public static void main(String[] args) throws ParseException{
		Parser pars = new Parser("Un usuario necesita obtener técnicas de inteligencia artificial para implementarlas en un videojuego. Ofrecer proyectos relacionados con la inteligencia artificial en los ultimos 5 años.");
		System.out.println(pars.execute());
	}*/

}
