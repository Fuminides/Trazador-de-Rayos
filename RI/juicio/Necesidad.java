package juicio;

public class Necesidad {
	
	private int truePositive, falsePositive, falseNegative, numDoc;

	public Necesidad(int numeroDocumentos) {
		numDoc = numeroDocumentos;
	}

	public void setTruePositive(int truePositive) {
		this.truePositive = truePositive;
	}

	public int getTruePositive() {
		return truePositive;
	}


	public int getTrueNegative() {
		return numDoc - truePositive - truePositive - falseNegative;
	}

	public void setFalsePositive(int falsePositive) {
		this.falsePositive = falsePositive;
	}

	public int getFalsePositive() {
		return falsePositive;
	}

	public void setFalseNegative(int falseNegative) {
		this.falseNegative = falseNegative;
	}

	public int getFalseNegative() {
		return falseNegative;
	}
	
	public double precision(){
		return truePositive / (truePositive + falsePositive);
	}
	
	public double recall(){
		return truePositive / (truePositive + falseNegative);	
	}
	
	public double f1Score(){
		return 2 * precision() * recall() / (precision() + recall());
	}


}
