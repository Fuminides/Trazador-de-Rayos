package semantico;
/**
 * Clase: Objeto.
 *  
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
public class Objeto {
	private double precision;
	private double recall;
	/*
	 * Crea un objeto con su precision y su recall
	 */
	public Objeto(double p,double r){
		precision=p;
		recall=r;
	}
	/*
	 * Devuelve la precision del objeto
	 */
	public double getP(){
		return precision;
	}
	/*
	 * Devuelve el recall del objeto
	 */
	public double getR(){
		return recall;
	}
}
