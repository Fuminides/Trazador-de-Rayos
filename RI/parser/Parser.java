package parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.lucene.analysis.es.SpanishAnalyzer;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.Query;
import org.apache.lucene.util.Version;

import recuperaci√≥n.SearchFiles;


public class Parser {

	int anyoActual = 2016;
	
	//Hardcodeado por simpleza.
	private String[] diccionarioLenguajes = 
		{ "frances", "espa√±ol", "ingles", "portugues",
		  "flamenco", "escoces", "gaelico", "aleman",
		   "polaco", "ruso", "hindu", "japones"
		};
	
	private String[] paises = 
		{ "Espa√±a", "Francia", "Alemania", "Estados Unidos" };
	
	private String[] diccionarioPublishers =
		{ "Universidad de Zaragoza" };
	
	private String frase;
	private String anyos = "";
	
	public Parser(String f) {
		frase = f;
	}
	
	public String getFecha(){ return anyos; }
	
	/**
	 * Devuelve el id buscado en la consulta que siga el siguiente esquema Tesis-<id>-<id> o bien "oai:zaguan.unizar.es:<id>î 
	 * si no lo hay devuelve ""
	 */
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
	/**
	 * Devuelve el publisher buscado en la consulta realizada por el usuario  si no lo hay devuelve ""
	 */
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
	/**
	 * Devuelve el lenguaje buscado en la consulta realizada por el usuario si no lo hay deveulve ""
	 */
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
	/**
	 * Devuelve el autor buscado en la consulta realizada por el usuario si no lo hay devuelve ""
	 */
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
	/**
	 * Devuelve la fecha buscada en la consulta realizada por el usuario si no la hay devuelve "" 
	 */
	public String buscarFecha(){
		String resultado = "",
				pattern = "entre ([0-9])+ y ([0-9])+",
				pattern2 = "aÒo ([0-9])+",
				pattern3 = "ultimos ([0-9])+ aÒos",
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
			 m.find();
			 String anyo2 = m.group(0);
			 fecha=fecha+","+m.group(0);
			 frase = frase.replace(anyo1 + " y " + anyo2, "");
			 anyos = fecha;
			 return fecha;
		 }
		 
		 p = Pattern.compile(pattern2);
		 m = p.matcher(frase);
		 b = m.find();
		 
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(frase);
			 m.find();
			 anyos = m.group(0);
			 frase = frase.replace("aÒo " + anyos, "");
			 return anyos;
		 }
		 
		 p = Pattern.compile(pattern3);
		 m = p.matcher(frase);
		 b = m.find();
		 
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(frase);
			 m.find();
			 frase = frase.replace("ultimos " + m.group(0) + " aÒos", "");
			 anyos = (anyoActual - Integer.parseInt(m.group(0))) + "," + anyoActual;
			 return anyos;
		 }
		 
		anyos = resultado;
		return resultado;
	}
	/**
	 * Devuelve la query que se ha formado despues de procesar la consulta hecha en lenguaje natural 
	 * por el usuario y separar sus tÈrminos en los distintos campos 
	 */
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
	    System.out.println(parser.parse(resultado));
		return parser.parse(resultado);
	}

}
