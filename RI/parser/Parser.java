
package parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.lucene.analysis.es.SpanishAnalyzer;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.Query;
import org.apache.lucene.util.Version;

import recuperacion.SearchFiles;

/**
 * Clase: Parser
 * Se encarga de parsear las consultas para formar una query valida para Lucene con los indices existentes.
 * 
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
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
	private String anyos = "";
	
	public Parser(String f) {
		frase = f;
	}
	
	public String getFecha(){ return anyos; }
	
	/**
	 * Devuelve el id buscado en la consulta el siguiente patrón:
	 *  "Tesis-<id>-<id>" o "oai:zaguan.unizar.es:<id>" 
	 *  Si no lo hay, devuelve la cadena vacía.
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
	 * Devuelve el publisher buscado en la consulta realizada por el usuario.  
	 * Si no lo hay, devuelve la cadena vacía. 
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
	 * Devuelve el lenguaje buscado en la consulta realizada por el usuario.
	 * Si no lo hay, deveulve la cadena vacía.
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
	 * Devuelve el autor buscado en la consulta realizada por el usuario. 
	 * Si no lo hay devuelve la cadena vacía.
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
	 * Devuelve la fecha buscada en la consulta realizada por el usuario. 
	 * Si no la hay, devuelve la cadena vacía. 
	 */
	public String buscarFecha(){
		String resultado = "",
				pattern = "entre ([0-9])+ y ([0-9])+",
				pattern2 = "a�o ([0-9])+",
				pattern3 = "ultimos ([0-9])+ a�os",
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
			 frase = frase.replace("a�o " + anyos, "");
			 return anyos;
		 }
		 
		 p = Pattern.compile(pattern3);
		 m = p.matcher(frase);
		 b = m.find();
		 
		 if ( b ){
			 p = Pattern.compile(patternNumber);
			 m = p.matcher(frase);
			 m.find();
			 frase = frase.replace("ultimos " + m.group(0) + " a�os", "");
			 anyos = (anyoActual - Integer.parseInt(m.group(0))) + "," + anyoActual;
			 return anyos;
		 }
		 
		anyos = resultado;
		return resultado;
	}
	/**
	 * Devuelve la query que se ha formado despues de procesar la consulta hecha en lenguaje natural 
	 * por el usuario y de separar sus terminos en los distintos campos 
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
		
		return parser.parse(resultado);
	}

}
