package juicio;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.HashMap;
import java.util.Scanner;

public class Evaluation {

	HashMap<String,HashMap<String, Integer>> necesidades;
	int numeroDocumentos;
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

	}
	
	/**
	 * @throws FileNotFoundException 
	 * 
	 */
	public void LeerNecesidades(String juiciosValor) throws FileNotFoundException{
		necesidades = new HashMap<String,HashMap<String, Integer>>();
		int cuenta = 0;
		String previousInfo = "";
		Scanner leerFichero = new Scanner(new File(juiciosValor));

		leerFichero.nextLine();
		while ( leerFichero.hasNextLine() ){
			@SuppressWarnings("unused")
			String info_need = leerFichero.next(), url = leerFichero.next(),
				documento = leerFichero.next();
				int relevancia = leerFichero.nextInt();
			if ( info_need.equals(previousInfo)  ){
				necesidades.get(cuenta).put(documento, relevancia);
			}
			else{
				cuenta++;
				previousInfo = info_need;
				necesidades.put(info_need, new HashMap<String, Integer>());
				necesidades.get(cuenta).put(documento, relevancia);
			}
		}
		
		leerFichero.close();
	}
	
	public HashMap<String, Necesidad> juicios(String resultados) throws FileNotFoundException{
		Scanner resultadosLeer = new Scanner(new File(resultados));
		HashMap<String, Necesidad> preguntas = new HashMap<String, Necesidad>();
		
		while ( resultadosLeer.hasNextLine() ){
			String id = resultadosLeer.next(),
				documento = resultadosLeer.next();
			
			if ( !preguntas.containsKey(id) ){
				preguntas.put(id, new Necesidad(numeroDocumentos));
			}
			Necesidad necesidad = preguntas.get(id);
			
			if ( necesidades.get(id).containsKey(documento)){
				
				if ( necesidades.get(id).get(documento) == 1 ){
					necesidad.setTruePositive(necesidad.getTruePositive()+1);
				}
				else if ( necesidades.get(id).get(documento) == 0){
					necesidad.setFalsePositive(necesidad.getFalsePositive()+1);
				}
				else{
					System.out.println("Valoracion " + necesidades.get(id).get(documento) + " no permitida.");
				}
				
				necesidades.get(id).remove(documento);
			}
			else{
				necesidad.setFalsePositive(necesidad.getFalsePositive()+1);
			}
			
			resultadosLeer.close();
				
		}
		
		for (String key : necesidades.keySet()){
			
			for(String documento : necesidades.get(key).keySet()){
				if ( necesidades.get(key).get(documento) == 1){
					preguntas.get(key).setFalseNegative(preguntas.get(key).getFalseNegative()+1);
				}	
			}
		}
		
		return preguntas;
	}
	

}
