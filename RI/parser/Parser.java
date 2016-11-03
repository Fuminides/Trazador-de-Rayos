package parser;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parser {

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
		String resultado = "";
		//TODO
		return resultado;
	}
	
	public static void main(String[] args){
		Parser parser = new Parser("Me interesa encontrar textos sobre información geológica de España.");
		System.out.println(parser.buscarAutor());
	}

}
