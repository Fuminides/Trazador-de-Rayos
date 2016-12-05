package juicio;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.text.DecimalFormat;

public class Evaluation {

	static HashMap<String,HashMap<String, Integer>> necesidades;
	static int numeroDocumentos = 30000; //Que tamaño tiene la coleccion de documentos?
	static HashMap<String,ArrayList<Integer>> posicionesRel;
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		String usage ="No se han introducido los archivos correctos";
	    String qrelsName = null;
	    String resultsFileName=null;
	    String outputFileName=null;
	    
	    /*if (args.length == 0) {
	        System.err.println(usage);
	        System.exit(1);
	      }else {
		    for(int i = 0;i < args.length;i++) {
		        if ("-qrels".equals(args[i])) {
		        	qrelsName = args[i+1];
		          i++;
		        } 
		        else if("-results".equals(args[i])){
		        	resultsFileName=args[i+1];
		      	  i++;
		        }
		        else if("-output".equals(args[i])){
		        	outputFileName=args[i+1];
		      	  i++;
		        }
		    }
	      }*/
	    
	    	//Para hacer pruebas y no tener que introducir los parametros como argumento:
	      qrelsName = "zaguanRels.txt";
	      resultsFileName = "equipo08.txt";
	      outputFileName="traditionalEvaluationResults08.txt";
	      
	    try {
	    	
	    	
			LeerNecesidades(qrelsName);
			HashMap<String, Necesidad> preguntas = juicios(resultsFileName);
			
			//Para comprobar lo que ha colocado en cada posicion de la hashmap de preguntas 
			/*System.out.println(preguntas.size());
			System.out.println("necesidad: "+preguntas.get("01-1").getId()+" fp: "+preguntas.get("01-1").getFalsePositive()+" fn: "+preguntas.get("01-1").getFalseNegative()+" tp: "+preguntas.get("01-1").getTruePositive()+" tn: "+preguntas.get("01-1").getTrueNegative());
			System.out.println("necesidad: "+preguntas.get("04-3").getId()+" fp: "+preguntas.get("04-3").getFalsePositive()+" fn: "+preguntas.get("04-3").getFalseNegative()+" tp: "+preguntas.get("04-3").getTruePositive()+" tn: "+preguntas.get("04-3").getTrueNegative());
			System.out.println("necesidad: "+preguntas.get("05-1").getId()+" fp: "+preguntas.get("05-1").getFalsePositive()+" fn: "+preguntas.get("05-1").getFalseNegative()+" tp: "+preguntas.get("05-1").getTruePositive()+" tn: "+preguntas.get("05-1").getTrueNegative());
			System.out.println("necesidad: "+preguntas.get("09-4").getId()+" fp: "+preguntas.get("09-4").getFalsePositive()+" fn: "+preguntas.get("09-4").getFalseNegative()+" tp: "+preguntas.get("09-4").getTruePositive()+" tn: "+preguntas.get("09-4").getTrueNegative());
			System.out.println("necesidad: "+preguntas.get("11-5").getId()+" fp: "+preguntas.get("11-5").getFalsePositive()+" fn: "+preguntas.get("11-5").getFalseNegative()+" tp: "+preguntas.get("11-5").getTruePositive()+" tn: "+preguntas.get("11-5").getTrueNegative());
			*/			
			escribirEvaluacion(outputFileName,preguntas);
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

	}
	
	
	public static void escribirEvaluacion(String outputFileName,HashMap<String, Necesidad> preguntas){
		FileWriter fichero = null;
	    PrintWriter pw = null;
	    try{
	        fichero = new FileWriter(outputFileName);
	        pw = new PrintWriter(fichero);
	        
	        DecimalFormat decimales = new DecimalFormat("0.000");
	        
	        double map =0.0;
	        double prec10total = 0.0;
	        double tp= 0.0,fp=0.0,fn=0.0;
	        
	        String [] v = new String [5];
	        v[0] ="01-1";
	        v[1]="04-3";
	        v[2]="05-1";
	        v[3]="09-4";
	        v[4]="11-5";
	        
	        for(int i=0;i<preguntas.size();i++){
	        	pw.println("INFORMATION_NEED	"+preguntas.get(v[i]).getId());
	        	
	        	tp += preguntas.get(v[i]).getTruePositive();
	        	fp += preguntas.get(v[i]).getFalsePositive();
	        	fn += preguntas.get(v[i]).getFalseNegative();
	        	
	        	//PRECISION
	        	double precision =preguntas.get(v[i]).getTruePositive()/(preguntas.get(v[i]).getTruePositive() +preguntas.get(v[i]).getFalsePositive()) ;
	        	pw.println("precision	"+decimales.format(precision));
	        	
	        	// RECALL
	        	double recall =preguntas.get(v[i]).getTruePositive()/(preguntas.get(v[i]).getTruePositive() +preguntas.get(v[i]).getFalseNegative()) ;
	        	pw.println("recall	"+decimales.format(recall));
	        	
	        	// F1
	        	double f1=2*precision*recall / (precision + recall);
	        	pw.println("F1	"+decimales.format(f1));
	        	
	        	
	        	// PREC@10 -> Sum de los 10 primeros( 1 si rel 0 si no) entre 10
	        	int sum=0;
	        	for(int x=0;x<posicionesRel.get(v[i]).size();x++){
	        		if(posicionesRel.get(v[i]).get(x) <= 10){
	        			sum+=1;
	        		}
	        		else{
	        			break;
	        		}
	        	}
	        	double prec10 =sum/10.0;
	        	pw.println("prec@10	"+decimales.format(prec10));
	        	prec10total += prec10;
	      
	        	// AVERAGE_PRECISION -> Es la media de la precision de recall precision
	        	double average_precision =0.0; 
	        	
	        	double p,r;	        	
	        	HashMap<Integer,Objeto> h = new HashMap<Integer,Objeto>();
	        	for(int x=0;x<posicionesRel.get(v[i]).size();x++){
        			p =(x+1)/(double) posicionesRel.get(v[i]).get(x);
	        		average_precision +=p;
	        		r=(x+1)/(double) posicionesRel.get(v[i]).size();
	        		h.put(x,new Objeto(p,r));
	        			
	        	}
	        	average_precision = average_precision /(double) posicionesRel.get(v[i]).size();
	        	pw.println("average_precision	"+decimales.format(average_precision));
	        	map += average_precision;
	        	
	        	// RECALL_PRECISION -> Es el recall y la precision en cada relevante encontrado
	        	pw.println("recall_precision"); 
	        	for(int x=0;x<h.size();x++){
					pw.println(decimales.format(h.get(x).getR())+"	"+decimales.format(h.get(x).getP()));
	        	}
	        	
	        	// INTERPOLATED_RECALL_PRECISION -> Es interpolar lo de arriba 
	        	/**FALTA ESTA**/
	        	double interpolated_recall_precision = 1 ; 
	        	pw.println("interpolated_recall_precision");
	        	pw.println(interpolated_recall_precision);
	        	
	        }
	        pw.println("TOTAL");
	        
	        //PRECISION
	        double ptotal = tp/(tp+fp);
	        pw.println("precision	"+decimales.format(ptotal));
	        
	        //RECALL
	        double rtotal = tp/(tp+fn);
	        pw.println("recall	"+decimales.format(rtotal));
	        
	        //f1
	        double f1total = 2*ptotal*rtotal / (ptotal + rtotal);
	        pw.println("F1	"+decimales.format(f1total));
	        
	        //PREC@10
	        prec10total = prec10total / (double) preguntas.size();
	        pw.println("prec@10	"+decimales.format(prec10total));
	        
	        //MAP
	        map = map /(double) preguntas.size();
	        pw.println("MAP	"+decimales.format(map));
	        
	        //INTERPOLATED_RECALL_PRECISION
	        /**FALTA ESTA**/
	        double interpolated_recall_precision_final = 0.0 ; 
	        pw.println("interpolated_recall_precision");        	
        	pw.println(interpolated_recall_precision_final);
	        
	        fichero.close();
	    }
	    catch(Exception e) {
	    	e.printStackTrace();
	    }
	}
	
	
	
	
	
	
	
	/**
	 * @throws FileNotFoundException 
	 * 
	 */
	public static void LeerNecesidades(String juiciosValor) throws FileNotFoundException{
		necesidades = new HashMap<String,HashMap<String, Integer>>();
		posicionesRel = new HashMap<String,ArrayList<Integer>>();
		String previousInfo = "";
		Scanner leerFichero = new Scanner(new File(juiciosValor));
		int cuenta=0;
		while ( leerFichero.hasNextLine() ){
			String info_need = leerFichero.next(),documento = leerFichero.next();
				int relevancia = leerFichero.nextInt();
			if ( info_need.equals(previousInfo)){
				cuenta++;
				necesidades.get(info_need).put(documento, relevancia);
			
			}
			else{
				previousInfo = info_need;
				cuenta=1;
				necesidades.put(info_need,new HashMap<String,Integer>());
				necesidades.get(info_need).put(documento, relevancia);
			}
			
			//Para añadir la posicion de los relevantes. 
			//En el guion dice que solo un maximo de 45 resultados por consulta
			if(relevancia == 1 && cuenta<=45){
				if(!posicionesRel.containsKey(info_need)){
					ArrayList<Integer> pos = new ArrayList<Integer>();
					pos.add(cuenta);
					posicionesRel.put(info_need,pos);
				}
				else{
					ArrayList<Integer> pos = posicionesRel.get(info_need);
					pos.add(cuenta);
					posicionesRel.put(info_need,pos);					
				}
			}
		}
		leerFichero.close();
	}
	
	public static HashMap<String, Necesidad> juicios(String resultados) throws FileNotFoundException{
		Scanner resultadosLeer = new Scanner(new File(resultados));
		HashMap<String, Necesidad> preguntas = new HashMap<String, Necesidad>();
		int cuenta =-1;
		while ( resultadosLeer.hasNextLine() ){
			String id = resultadosLeer.next(),
				documento = resultadosLeer.next();
			
			
			if ( !preguntas.containsKey(id) ){
				cuenta++;
				preguntas.put(id, new Necesidad(numeroDocumentos,id));
			}
			Necesidad necesidad = preguntas.get(id);
			
			if ( necesidades.get(id).containsKey(documento)){
				
				if ( necesidades.get(id).get(documento) == 1 ){
					//Documento recuperado y es relevante
					necesidad.setTruePositive(necesidad.getTruePositive()+1);
				}
				else if ( necesidades.get(id).get(documento) == 0){
					//Documento recuperado y no es relevante
					necesidad.setFalsePositive(necesidad.getFalsePositive()+1);
				}
				else{
					System.out.println("Valoracion " + necesidades.get(id).get(documento) + " no permitida.");
				}
				
				necesidades.get(id).remove(documento);
			}
			else{
				// recuperados  pero No relevantes
				necesidad.setFalsePositive(necesidad.getFalsePositive()+1);
			}
				
		}
		
		for (String key : necesidades.keySet()){
			
			for(String documento : necesidades.get(key).keySet()){
				if ( necesidades.get(key).get(documento) == 1){
					preguntas.get(key).setFalseNegative(preguntas.get(key).getFalseNegative()+1);
				}	
			}
		}
		resultadosLeer.close();
		return preguntas;
	}
	

}
