package semantico;

/**
 * Clase: Necesidad.
 *  
 * Javier Fumanal Idocin, 684229, Silvia Uson Fortanet 681721
 */
public class Necesidad {
	
	private int truePositive, falsePositive, falseNegative;
	private String id;

	public Necesidad(String id) {
		this.id=id;
	}
	/*
	 * Asigna el numero de truePositives que hay en la necesidad
	 */
	public void setTruePositive(int truePositive) {
		this.truePositive = truePositive;
	}
	/*
	 * Devuelve el numero de truePositives que hay en la necesidad
	 */
	public int getTruePositive() {
		return truePositive;
	}
	/*
	 * Asigna el numero de falsePositive que hay en la necesidad
	 */
	public void setFalsePositive(int falsePositive) {
		this.falsePositive = falsePositive;
	}
	/*
	 * Devuelve el numero de falsePositive que hay en la necesidad
	 */
	public int getFalsePositive() {
		return falsePositive;
	}
	/*
	 * Asigna el numero de falseNegative que hay en la necesidad
	 */
	public void setFalseNegative(int falseNegative) {
		this.falseNegative = falseNegative;
	}
	/*
	 * Devuelve el numero de falseNegative que hay en la necesidad
	 */
	public int getFalseNegative() {
		return falseNegative;
	}
	/*
	 * Devuelve el id de la necesidad
	 */
	public String getId(){
		return id;
	}
}
