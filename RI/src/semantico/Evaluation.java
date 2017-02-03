package semantico;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.NoSuchElementException;
import java.util.Scanner;
import java.text.DecimalFormat;
/**
 * Clase: Evaluation.
 * 
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
public class Evaluation {

	static HashMap<String,HashMap<String, Integer>> necesidades;
	static HashMap<String,ArrayList<Integer>> posicionesRel;
	static double[] relevantes = new double[5]; 
	
	static double[][] interpolate_total = new double[11][1];  
	static int interpolate_pointer = 0; 
	
	private static String qrelsName = null;
	private static String resultsFileName=null;
	private static String outputFileName=null;
	/**
	 * @param args
	 */
	// resultados = semanticResults08.txt

	public Evaluation(String qrels,String resultados){
		qrelsName =qrels;
		resultsFileName = resultados;
		outputFileName ="semanticEvaluationResults08.txt";

	}
	public void evaluar() {
	      
	    try {
	    	posicionesRel = new HashMap<String,ArrayList<Integer>>();
	    	posicionesRel.put("01-1",new ArrayList<Integer>());
	    	posicionesRel.put("04-3",new ArrayList<Integer>());
	    	posicionesRel.put("05-1",new ArrayList<Integer>());
	    	posicionesRel.put("09-4",new ArrayList<Integer>());
	    	posicionesRel.put("11-5",new ArrayList<Integer>());
			LeerNecesidades(qrelsName);
			HashMap<String, Necesidad> preguntas = juicios(resultsFileName);	
			escribirEvaluacion(outputFileName,preguntas);
			
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

	}
	
	
	/**
	 * metodo encargado de la parte de evaluacion , con todos los datos obtenidos anteriormente se calculan la
	 * precision , recall, prec@10 , average_precision , recall-precision y la interpolada para cada consulta y
	 * para el sistema en general
	 */
	private static void escribirEvaluacion(String outputFileName,HashMap<String, Necesidad> preguntas){
		FileWriter fichero = null;
	    PrintWriter pw = null;
	    try{
	        fichero = new FileWriter(outputFileName);
	        pw = new PrintWriter(fichero);
	        
	        DecimalFormat decimales = new DecimalFormat("0.000");
	        
	        double map =0.0;
	        double sumT=0.0;
	        double prec10total = 0.0;
	        double tp= 0.0,fp=0.0,fn=0.0,tpF= 0.0,fpF=0.0,fnF=0.0;
	        String [] v = new String [5];
	        v[0] ="01-1";
	        v[1]="04-3";
	        v[2]="05-1";
	        v[3]="09-4";
	        v[4]="11-5";
	        
	        for(int i=0;i<preguntas.size();i++){
	        	pw.println("INFORMATION_NEED	"+preguntas.get(v[i]).getId());
	        	
	        	
	        	tp = preguntas.get(v[i]).getTruePositive();
	        	fp = preguntas.get(v[i]).getFalsePositive();
	        	fn = preguntas.get(v[i]).getFalseNegative();
	        	
	        	tpF += tp;
	        	fpF += fp;
	        	fnF += fn;
	        	
	        	//PRECISION
	        	double precision = tp / (tp +fp);
	        	pw.println("precision	"+decimales.format(precision));
	        	
	        	// RECALL
	        	double recall = tp /(tp+fn);
	        	pw.println("recall	"+decimales.format(recall));
	        	
	        	// F1
	        	double f1=2*precision*recall / (precision + recall);
	        	pw.println("F1	"+decimales.format(f1));
	        	
	        	
	        	// PREC@10 -> Sum de los 10 primeros( 1 si rel 0 si no) entre 10
	        	int sum=0;
	        	System.out.println("El numero de relevantes de "+v[i]+" es: "+posicionesRel.get(v[i]).size());
	        	for(int x=0;x<posicionesRel.get(v[i]).size();x++){
	        		if(posicionesRel.get(v[i]).get(x) <= 10){
	        			sum+=1;
	        			sumT+=1;
	        		}
	        		else{
	        			break;
	        		}
	        	}
	        	double prec10 =sum/10.0;
	        	pw.println("prec@10	"+decimales.format(prec10));
	        	prec10total += prec10;
	      
	        	// AVERAGE_PRECISION -> Es la media de la precision de recall-precision
	        	double average_precision =0.0; 
	        	
	        	double p,r;	        	
	        	HashMap<Integer,Objeto> h = new HashMap<Integer,Objeto>();
	        	for(int x=0;x<posicionesRel.get(v[i]).size();x++){
        			p =(x+1)/(double) posicionesRel.get(v[i]).get(x);
        			r=(x+1)/(double) relevantes[i]; 
	        		h.put(x,new Objeto(p,r));
	        		average_precision +=p;
	        		
	        	}
	        	average_precision = average_precision /(double) posicionesRel.get(v[i]).size();
	        	pw.println("average_precision	"+decimales.format(average_precision));
	        	
	        	map += average_precision;
	        	
	        	// RECALL_PRECISION -> Es el recall y la precision en cada relevante encontrado 
	        	pw.println("recall_precision"); 
	        	for(int x=0;x<h.size();x++){
					pw.println(decimales.format(h.get(x).getR())+"	"+decimales.format(h.get(x).getP()));
	        	}
	        	
	        	// INTERPOLATED_RECALL_PRECISION ->por cada 10% de recall me quedo con la max(precision) a partir de ese recall. Interpolar la de arriba 
	        	interpolatePR(pw, decimales, h);    	        	
	        }
	        pw.println("TOTAL");
	        
	        //PRECISION
	        double ptotal = tpF/(tpF+fpF);
	        pw.println("precision	"+decimales.format(ptotal));
	        
	        //RECALL
	        double rtotal = tpF/(tpF+fnF);
	        pw.println("recall	"+decimales.format(rtotal));
	        
	        //f1
	        double f1total = 2*ptotal*rtotal / (ptotal + rtotal);
	        pw.println("F1	"+decimales.format(f1total));
	        
	        //PREC@10
	        prec10total = sumT / (double) (10.0*preguntas.size());
	        pw.println("prec@10	"+decimales.format(prec10total));
	        
	        //MAP
	        map = map /(double) preguntas.size();
	        pw.println("MAP	"+decimales.format(map));
	        
	        //INTERPOLATED_RECALL_PRECISION 
	        pw.println("interpolated_recall_precision"); 
	        for(double x=0.1;x<1.0;x+=0.1){
	        	pw.println(decimales.format(x)+"	"+decimales.format(interpolate_total[(int)(x*10)][0]/5));  
	        }
	        
	        fichero.close();
	    }
	    catch(Exception e) {
	    	e.printStackTrace();
	    }
	}

	/**
	 * INTERPOLATED_RECALL_PRECISION ->por cada 10% de recall me quedo con la max(precision) 
	 * a partir de ese recall. Interpolar la de arriba
	 * 
	 * @param pw
	 * @param decimales
	 * @param duplas
	 * @param i
	 * @param h
	 */
	private static void interpolatePR(PrintWriter pw, DecimalFormat decimales,
			HashMap<Integer, Objeto> h) { 
		pw.println("interpolated_recall_precision");
		double z=0.0;
		while(z<=1.0){
			ArrayList <Double> posiblesValores = new ArrayList<Double>();
			for(int x=0;x<h.size();x++){
				double valor=h.get(x).getR();
				if(valor>=z){
					posiblesValores.add(h.get(x).getP());
				}
			}		        
		    double max = 0.0;
			for (int t = 0; t < posiblesValores.size(); t++) {
		        if (posiblesValores.get(t) > max) {
		            max = posiblesValores.get(t);
		        }
			}
			pw.println(decimales.format(z)+"	"+decimales.format(max));
			z=z+0.1;
			interpolate_total[(int) (z*10)][0] += max;
		}
	}
	/**
	 * @throws FileNotFoundException 
	 * metodo encargado de leer el fichero de los juicios de valor guardando asi las posiciones de todos aquellos que 
	 * son relevantes para posteriormente poder aplicar la evaluacion
	 */
	private static void LeerNecesidades(String juiciosValor) throws FileNotFoundException{
		necesidades = new HashMap<String,HashMap<String, Integer>>();
		String previousInfo = "";
		Scanner leerFichero = new Scanner(new File(juiciosValor));
		int i = 0, index = -1; 
		try{
			while ( leerFichero.hasNextLine() ){
				String info_need = leerFichero.next(),documento = leerFichero.next();
				int relevancia = leerFichero.nextInt();
				if ( info_need.equals(previousInfo)){
					necesidades.get(info_need).put(documento, relevancia);
					if (relevancia == 1) { 
						i++;				
					}
				}
				else{
					previousInfo = info_need;
					if (index >= 0) {		
						relevantes[index] = i;
					}
					i=0; index++;		
					necesidades.put(info_need,new HashMap<String,Integer>());
					necesidades.get(info_need).put(documento, relevancia);
				}
			}
		} catch (NoSuchElementException e){;}
		relevantes[index] = i; 
		leerFichero.close();
	}
	/**
	 * @throws FileNotFoundException 
	 * metodo encargado de leer el fichero de los resultados obtenidos en nuestro sistema de recuperacion de manera que solo mire 45 resultados por
	 * cada necesidad y obtenga asi los truePositives, falsePositive y falseNegative de cada una de las consultas .
	 * Devuelve una HashMap en la que cada celda contiene una necesidad con los valores previamente citados
	 * 
	 */
	private static HashMap<String, Necesidad> juicios(String resultados) throws FileNotFoundException{
		Scanner resultadosLeer = new Scanner(new File(resultados));
		HashMap<String, Necesidad> preguntas = new HashMap<String, Necesidad>();
		int cuenta =1;
		try{
			while ( resultadosLeer.hasNextLine() ){
				String id = resultadosLeer.next(),
					documento = resultadosLeer.next();
				//Aqui se quita el recordsdc pero no hará falta porque en el fichero de resultados se esribe sin eso
				//String [] doc = documento.split("\\\\");
				//documento = doc[1];
				if ( !preguntas.containsKey(id) ){
					cuenta=1;
					preguntas.put(id, new Necesidad(id));
				}
				else{
					cuenta++;
				}
				//Dice que solo hay que coger 45 resultados por consulta por lo tanto nos quedamos con los 45 primeros
				if(cuenta <=45){
					Necesidad necesidad = preguntas.get(id);
					
					if ( necesidades.get(id).containsKey(documento)){
						
						if ( necesidades.get(id).get(documento) == 1 ){
							
							//Documento recuperado y es relevante
							//Si el documento recuperado es relevante me guardo la posicion del fichero de resultados en
							//la que se ha recuperado
							ArrayList<Integer> viejo =posicionesRel.get(id);
							
							viejo.add(cuenta);
							posicionesRel.put(id,viejo);
												
							
							
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
					
			}
			
			for (String key : necesidades.keySet()){
				
				for(String documento : necesidades.get(key).keySet()){
					if ( necesidades.get(key).get(documento) == 1){
						preguntas.get(key).setFalseNegative(preguntas.get(key).getFalseNegative()+1);
					}	
				}
			}
		} catch(NoSuchElementException e){;}
		resultadosLeer.close();
		return preguntas;
	}
	

}
