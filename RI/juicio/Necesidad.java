package juicio;

import java.util.ArrayList;

public class Necesidad {
	
	private int truePositive, falsePositive, falseNegative, numDoc;
	private String id;

	public Necesidad(int numeroDocumentos,String id) {
		numDoc = numeroDocumentos;
		this.id=id;
	}
	
	public void setTruePositive(int truePositive) {
		this.truePositive = truePositive;
	}

	public int getTruePositive() {
		return truePositive;
	}


	public int getTrueNegative() {
		return numDoc - truePositive - falsePositive - falseNegative;
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
	public String getId(){
		return id;
	}
}
